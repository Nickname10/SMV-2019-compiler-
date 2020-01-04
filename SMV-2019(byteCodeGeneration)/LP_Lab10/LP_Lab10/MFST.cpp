#include "pch.h"
#include "MFST.h"
#include "Debug_MFST.h"

#define EMPTY_SYMBOL TS('#')

namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	}

    MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	}

	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	}

	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	}

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	}

	Mfst::Mfst(Tables plex, GRB::Greibah pgrebach)
	{
		grebach = pgrebach;
		lex = plex;
		
		lenta = new short[lenta_size = lex.LEXTABLE->size+lex.LEXTABLE->countEnd];
		int indLenta = 0;
		for (int k = 0; k < lex.LEXTABLE->size; k++)
		{
			if (lex.LEXTABLE->table[k].lexema == LEX_END)
			{

				lenta[indLenta++] = TS(EMPTY_SYMBOL);
				lenta[indLenta++] = lex.LEXTABLE->table[k].lexema;
			}
			else
			{
				lenta[indLenta++] = lex.LEXTABLE->table[k].lexema;

			}
		}
		lenta_position = 0;
		st.push(grebach.stbottomT);
		st.push(grebach.startN);
		nrulechain = -1;

	}

	char * Mfst::getCSt(char * buf)
	{
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			short p = st._Get_container()[k];

			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[st.size()] = 0x00;
		return buf;
	}

	char * Mfst::getCLenta(char * buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	}

	char * Mfst::getDiagnosis(short n, char * buf)
	{
		char *rc = new char;
		*rc = '\0';
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s", err.id, lex.LEXTABLE->table[lpos].stringNumber, err.message);
			delete rc;
			rc = buf;
		}
		return rc;
	}

	bool Mfst::savestate()
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		TREE.push(MfstState(lenta_position, st, nrule, nrulechain));

		MFST_TRACE6("SAVESTATE:", storestate.size());
		return true;
	}

	bool Mfst::reststate()
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			
			MFST_TRACE5("RESSTATE")
				MFST_TRACE2
			
		}
		return rc;
	}

	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
		return true;
	}
	
	Mfst::RC_STEP MFST::Mfst::step()
	{
		
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top()))
			{
	
				GRB::Rule rule;
		
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain* chain = new GRB::Rule::Chain;

					nrulechain = rule.GetNextChain(lenta[lenta_position], chain, nrulechain + 1);
					
					if (nrulechain >= 0)
					{
						MFST_TRACE1
							savestate(); st.pop(); push_chain(*chain); rc = NS_OK;
								
							
						MFST_TRACE2
					}
					else
					{	
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
						savediagnosis(NS_NORULECHAIN); rc = reststate() ? NS_NORULECHAIN : NS_NORULE;
						
					}
				}
				else rc = NS_ERROR;
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				
				
					lenta_position++; st.pop(); nrulechain = -1; rc = TS_OK;
				
				MFST_TRACE3
			}
			else {
				
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN") rc = reststate() ? TS_NOK : NS_NORULECHAIN;
				
				
			}

		}
		else { 
				MFST_TRACE4("LENTA_END")
				if (st.size() > 2) rc = NS_NORULE;
				else rc = LENTA_END;
				 
		}
		return rc;
	}

	void Mfst::start(Parm::PARM parms, Log::LOG lg)
	{
		
		fout = fstream(parms.out, ios_base::app); fout << "\n\n\n\n\n\t\t\tТрассировка синтаксического разбора:\n\n\n\n ";
		MFST_TRACE_START
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step();
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK) rc_step = step();
		switch (rc_step)
		{
		case LENTA_END: {
			MFST_TRACE4("----->LENTA_END");
			fout << "------------------------------------------------------------------- ----" << endl;
			//sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			fout << setw(4) << left << 0 << ": всего строк " << lenta_size << ". синтаксический анализ выполнен без ошибок" << endl;
			rc = true;
			printrules();
			break;
		}
		case NS_NORULE: {
			MFST_TRACE4("------>NS_NORULE")
				fout << "--------------------------------------------------------------------- ----" << endl;
			
			cout << getDiagnosis(0, buf) << endl;
			cout << getDiagnosis(1, buf) << endl;
			cout << getDiagnosis(2, buf) << endl;
			*lg.stream << getDiagnosis(0, buf) << endl;
			*lg.stream << getDiagnosis(1, buf) << endl;
			*lg.stream << getDiagnosis(2, buf) << endl;
			exit(0);
			
			break;
		}
		case NS_NORULECHAIN: MFST_TRACE4("------>NS_NORULECHAIN") break;
		case NS_ERROR: MFST_TRACE4("------NS_ERROR") break;
		case SURPRISE: MFST_TRACE4("------SURPRISE") break;
		}

	}

	bool Mfst::savediagnosis(RC_STEP pprc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER&&lenta_position <= diagnosis[k].lenta_position) k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, pprc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
				diagnosis[j].lenta_position = -1;
		};
		return rc;
	}

	void Mfst::printrules()
	{
		MfstState state;
		GRB::Rule rule;
		fout << endl << endl <<"----------------------Правила::-----------------------" << endl << endl;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = grebach.getRule(state.nrule);
			fout << state.lenta_position << ": " << rule.getCRule(rbuf, state.nrulechain) << endl;

		}
		
	}

	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		}
		return true;
	}
}