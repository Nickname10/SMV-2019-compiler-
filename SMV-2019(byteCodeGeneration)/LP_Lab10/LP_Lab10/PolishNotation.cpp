#include "pch.h"
#include "PolishNotation.h"
using namespace std;
namespace polishNotation {
	int getPriorityOperation(LT::Entry entry)
	{
		int priority;
		switch (entry.symbolOperation)
		{
		case '<': priority = 1;
			break;
		case '>': priority = 1;
			break;
		case '(': priority = 0;
			break;
		case '+': priority = 2;
			break;
		case '-': priority = 2;
			break;
		case '*': priority = 3;
			break;
		case '/': priority = 3;
			break;
		case '%': priority = 3;
			break;
		default:
			priority = -1;
			break;
		}
		return priority;
	}
	bool PolishNotation(int lexTablePos, LT::LexTable* lexTable, IT::IdTable* idTable)
	{
		std::list<LT::Entry> outList;
		std::stack<LT::Entry> stack;
		std::stack<LT::Entry> stackParm;
		int indSemicolon;
		int i = 0;
		for (i = lexTablePos; ; i++)//|| (lexTable->table[i].lexema != LEX_THEN) || (lexTable->table[i].lexema!=LEX_ELSE))
		{
			if (lexTable->table[i].lexema == LEX_SEMICOLON||lexTable->table[i].lexema == LEX_THEN || lexTable->table[i].lexema == LEX_ELSE) break;
			if ((lexTable->table[i].lexema == LEX_ID && idTable->table[lexTable->table[i].numberInIdTable].idType == IT::IDTYPE::Function )
				||lexTable->table[i].lexema ==LEX_BASE_LIBRARY_CLENGTH || lexTable->table[i].lexema ==LEX_BASE_LIBRARY_CMP)
			{
				int tmp = i; //сохраняет запись функции
				while (lexTable->table[i].lexema != LEX_RIGHT_SQUARE_BRACKET)
				{
					i++;
					if (lexTable->table[i].lexema == LEX_ID || lexTable->table[i].lexema == LEX_LITERAL)
					{
						stackParm.push(lexTable->table[i]);

					}
				}
				while (!stackParm.empty())
				{
					outList.push_back(stackParm.top());
					stackParm.pop();
				}
				outList.push_back(LT::Entry('!', lexTable->table[i].stringNumber));
				outList.push_back(lexTable->table[tmp]);
				continue;
			}
			if (lexTable->table[i].lexema == LEX_ID || lexTable->table[i].lexema == LEX_LITERAL)
			{
				outList.push_back(lexTable->table[i]);
				continue;
			}
			if (lexTable->table[i].lexema == LEX_LEFTHESIS)
			{
				stack.push(lexTable->table[i]);
				continue;
			}
			if (lexTable->table[i].lexema == LEX_RIGHTHESIS)
			{
				while (stack.top().lexema != LEX_LEFTHESIS)
				{
					outList.push_back(stack.top());
					stack.pop();
				}
				stack.pop();
				continue;
			}
			if (stack.empty() || getPriorityOperation(stack.top()) < getPriorityOperation(lexTable->table[i]))
			{
				stack.push(lexTable->table[i]);
				continue;
			}
			if (getPriorityOperation(stack.top()) >= getPriorityOperation(lexTable->table[i]))
			{
				while (!stack.empty() && getPriorityOperation(stack.top()) >= getPriorityOperation(lexTable->table[i]))
				{
					outList.push_back(stack.top());
					stack.pop();
				}
				if (stack.empty() || getPriorityOperation(stack.top()) < getPriorityOperation(lexTable->table[i]))
				{
					stack.push(lexTable->table[i]);
					continue;
				}
				continue;
			}

		}
		indSemicolon = i;
		while (!stack.empty())
		{
			outList.push_back(stack.top());
			stack.pop();
		}
		int stringNumber = 0;
		for (i = lexTablePos; i < indSemicolon; i++)
		{
			if (!outList.empty())
			{
				lexTable->table[i] = outList.front();
				outList.pop_front();
				stringNumber = lexTable->table[i].stringNumber;
			}
			else
			{
				lexTable->table[i] = LT::Entry('@', stringNumber);
			}
		}
		return false;
	}


}