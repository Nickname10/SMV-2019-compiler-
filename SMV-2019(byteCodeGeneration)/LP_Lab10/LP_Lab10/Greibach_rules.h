#pragma once
#include "Greibah.h"
#include "Debug_MFST.h"
#define EMPTY_SYMBOL TS('#')
#define EMPTY 1, EMPTY_SYMBOL
namespace GRB
{

	static Greibah greibach(NS('S'), TS('$'), 15,
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			4,
			Rule::Chain(4, TS('o'), TS('t'), TS('i'), NS('K')),
			Rule::Chain(10, TS('f'), TS('t'), TS('i'), TS('['), NS('P'), TS(']'), TS('b'), NS('B'), TS('e'), NS('S')),
			Rule::Chain(5, TS('m'), TS('b'), NS('Y'), TS('e'), NS('S')),
			Rule::Chain(6, TS('w'), TS('i'), TS('b'), NS('X'), TS('e'), NS('S'))

		),
		Rule(NS('B'), GRB_ERROR_SERIES + 1,
			10,
			Rule::Chain(EMPTY),
			Rule::Chain(4, TS('o'), TS('t'), TS('i'), NS('O')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('B')),
			Rule::Chain(6, TS('j'), TS('('), NS('E'), TS(')'), NS('T'), NS('B')),
			Rule::Chain(6, TS('p'), TS('['), NS('E'), TS(']'), TS(';'), NS('B')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('B')),
			Rule::Chain(6, TS('i'), TS('['), NS('L'), TS(']'), TS(';'), NS('B')),
			Rule::Chain(3, TS('z'), NS('Z'), TS(';')),
			Rule::Chain(3, TS('c'), NS('C'), TS(';')),
			Rule::Chain(3, TS('i'), TS(';'), NS('B'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,
			10,
			Rule::Chain(1, TS('i')),
			Rule::Chain(5, TS('i'), TS('['), NS('L'), TS(']'),NS('M')),	
			Rule::Chain(1, TS('l')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(2, TS('z'), NS('Z')),
			Rule::Chain(2, TS('c'), NS('C')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('['), NS('L'), TS(']'))
		),
		Rule(NS('P'), GRB_ERROR_SERIES + 3,
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('P'))
		),
		Rule(NS('L'), GRB_ERROR_SERIES + 4, 4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('L')),
			Rule::Chain(3, TS('l'), TS(','), NS('L'))
		),
		
		Rule(NS('M'), GRB_ERROR_SERIES + 5, 1,
			Rule::Chain(2, TS('v'), NS('E'))	
			),
		
		
		Rule(NS('C'), GRB_ERROR_SERIES + 6, // ПАРАМЕТРЫ ФУНКЦИИ CMP
			1,
			Rule::Chain(5, TS('['), NS('E'), TS(','), NS('E'), TS(']'))
		),
		Rule(NS('Z'), GRB_ERROR_SERIES + 7, 1, // ПАРАМЕТРЫ ФУНКЦИИ CLENGTH СТАНДАРТНОЙ БИБЛИОТЕКИ
		
			Rule::Chain(3, TS('['), NS('E'), TS(']') )
		),		
		Rule(NS('T'), GRB_ERROR_SERIES + 8,
			2,
		Rule::Chain(8, TS('q'), TS('b'), NS('B'), TS('e'), TS('n'), TS('b'), NS('B'), TS('e')),
		Rule::Chain(4, TS('q'), TS('b'), NS('B'), TS('e'))
			

		),
		Rule(NS('O'), GRB_ERROR_SERIES + 9, 2,
			Rule::Chain(2, TS(';'), NS('B')),
			Rule::Chain(4, TS('='), NS('E'), TS(';'), NS('B'))
		),
		Rule(NS('A'), GRB_ERROR_SERIES + 9, 2,
			Rule::Chain(2, TS(';'), NS('Y')),
			Rule::Chain(4, TS('='), NS('E'), TS(';'), NS('Y'))
		),
		Rule(NS('I'), GRB_ERROR_SERIES + 9, 2,
			Rule::Chain(2, TS(';'), NS('X')),
			Rule::Chain(4, TS('='), NS('E'), TS(';'), NS('X'))
		),
		Rule(NS('K'), GRB_ERROR_SERIES + 9, 2,
			Rule::Chain(2, TS(';'), NS('S')),
			Rule::Chain(4, TS('='), NS('E'), TS(';'), NS('S'))
		),
		Rule(NS('X'), GRB_ERROR_SERIES + 10,
			9,
			Rule::Chain(EMPTY),
			Rule::Chain(4, TS('o'), TS('t'), TS('i'), NS('O')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('X')),
			Rule::Chain(6, TS('j'), TS('('), NS('E'), TS(')'), NS('T'), NS('X')),
			Rule::Chain(6, TS('p'), TS('['), NS('E'), TS(']'), TS(';'), NS('X')),
			Rule::Chain(6, TS('i'), TS('['), NS('L'), TS(']'), TS(';'), NS('X')),
			Rule::Chain(3, TS('z'), NS('Z'), TS(';')),
			Rule::Chain(3, TS('c'), NS('C'), TS(';')),
			Rule::Chain(3, TS('i'), TS(';'), NS('X'))),
		Rule(NS('Y'), GRB_ERROR_SERIES + 11,
			9,
			Rule::Chain(EMPTY),
			Rule::Chain(4, TS('o'), TS('t'), TS('i'), NS('A')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('Y')),
			Rule::Chain(6, TS('j'), TS('('), NS('E'), TS(')'), NS('T'), NS('Y')),
			Rule::Chain(6, TS('p'), TS('['), NS('E'), TS(']'), TS(';'), NS('Y')),
			Rule::Chain(6, TS('i'), TS('['), NS('L'), TS(']'), TS(';'), NS('Y')),
			Rule::Chain(3, TS('z'), NS('Z'), TS(';')),
			Rule::Chain(3, TS('c'), NS('C'), TS(';')),
			Rule::Chain(3, TS('i'), TS(';'), NS('Y'))
		
		)

);
}