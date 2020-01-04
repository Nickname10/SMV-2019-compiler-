#include "pch.h"
#include "FST.h"


fst::RELATION::RELATION(char c, short nn) {
	symbol = c;
	nnode = nn;
};

fst::NODE::NODE() {
	n_relation = 0;
	RELATION* relations = NULL;
};

fst::NODE::NODE(short n, RELATION rel, ...) {
	n_relation = n;
	RELATION* p = &rel;
	relations = new RELATION[n];
	for (short i = 0; i < n; i++) relations[i] = p[i];
};

fst::FST::FST(const char* s, short ns, NODE n, ...) {
	string = s;
	nstates = ns;
	nodes = new NODE[ns];
	NODE* p = &n;
	for (int k = 0; k < ns; k++) nodes[k] = p[k];
	rstates = new short[nstates];
	memset(rstates, 0xff, sizeof(short) * nstates);
	rstates[0] = 0;
	position = -1;
};

bool fst::step(FST& fst, short*& rstates) {
	bool rc = false;
	std::swap(rstates, fst.rstates);
	for (short i = 0; i < fst.nstates; i++)
	{
		if (rstates[i] == fst.position)
			for (short j = 0; j < fst.nodes[i].n_relation; j++)
				if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {
					fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
					rc = true;
				};
	};
	return rc;
};
bool fst::execute(FST& fst)
{
	short* rstates = new short[fst.nstates];
	memset(rstates, 0xff, sizeof(short) * fst.nstates);
	short lstring = strlen(fst.string);
	bool rc = true;
	for (short i = 0; i < lstring && rc; i++)
	{
		fst.position++;
		rc = step(fst, rstates);
	};
	delete[] rstates;
	return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
};
