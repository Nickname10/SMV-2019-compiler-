#include "pch.h"
#include "Greibah.h"
#include "MFST.h"
#include "Greibach_rules.h"
#include "MFST.h"
using namespace std;
namespace GRB {
	
	Rule::Chain::Chain(short psize, GRABLPHABET s, ...)
	{
		nt = new GRABLPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i) nt[i] = (GRABLPHABET)p[i];
	};
	char * Rule::Chain::getChain(char * b)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};

	Rule::Rule(GRABLPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};
	char * Rule::getCRule(char * b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-'; b[2] = '>'; b[3] = 0x00;
		chains[nchain].getChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};
	short Rule::GetNextChain(GRABLPHABET t, Rule::Chain* pchain, short j)
	{
		
		short rc = -1;
		
		while (j < size &&  *(chains[j].nt) != t)
		{
			++j;	
		}

			rc = (j < size ? j : -1);

		if (rc >= 0) *pchain = chains[rc];
		return rc;
	}

	
	
	Greibah::Greibah(GRABLPHABET pstartN, GRABLPHABET pstbottomT, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottomT;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};


	short Greibah::getRule(GRABLPHABET pnn, Rule & prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];

		return rc;
	};
	Rule Greibah::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};
	Greibah getGreibach()
	{
		
		return greibach;
	}
}