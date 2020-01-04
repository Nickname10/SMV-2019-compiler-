#pragma once
#include "pch.h"
namespace fst {

	struct RELATION //ребро:символ->вершина графа переходов КА
	{
		char symbol;  //символ перехода
		short nnode; // новое состояние 
		RELATION(
			char c = 0x00,
			short ns = NULL);
	};


	struct NODE // вершина графа перехода
	{
		short n_relation;	//вершина графа перехода 
		RELATION *relations;	//инцидентные ребра 
		NODE();
		NODE(
			short n,	//количество инцидентных ребер 
			RELATION rel, ...	//список ребер
		);
	};




	struct FST	//недетерминированный конечный автомат
	{
		const char* string;	//цепочка(строка, завершается 0х00)
		short position;		// текущая позиция в цепочке 
		short nstates;		// количество состоний автомата 
		NODE* nodes;	// граф переходов: [0] - начальное состояние, [nstate - 1] - конечное
		short* rstates;		// возможные состояние автомата на данной позиции 
		FST(
			const char* s,	// цепочка(строка, завершается 0х00)
			short ns,	// количество состояний автомата 
			NODE n, ...		// список состояний (граф переходов)
		);
	};



	bool step(FST& fst, short*& rstates);

	bool execute(FST& fst);
};

