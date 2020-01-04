#pragma once
#include "pch.h"

typedef short GRABLPHABET;// символы алфавита грамматики терминалы > 0, нетерминалы < 0
namespace GRB
{
	struct Rule //правило в граматике Грейбах
	{
		
		GRABLPHABET nn; //Нетерминал (левый символ правила) < 0 
		int iderror; //идентификатор диагностического сообщения
		short size; //количество цепочек - правых частей правила
		struct Chain
		{

			short size;//Длина цепочки
			GRABLPHABET* nt;//цепочка терминалов (>0) и нетерминалов (<0)
			Chain() { size = 0; nt = 0; };
			Chain(short psize, GRABLPHABET s, ...);
			char* getChain(char*b); //получить правую сторону терминала
			static GRABLPHABET T(char t)//терминал
			{
				return  GRABLPHABET(t);
			};
			static  GRABLPHABET N(char n)//нетерминал
			{
				return (-GRABLPHABET(n));
			}
			static bool isT(GRABLPHABET s)//терминал
			{
				return (s > 0);
			}
			static bool isN(GRABLPHABET s)// нетерминал
			{
				return (s < 0);
			}
			static char alphabet_to_char(GRABLPHABET s) //GRBALPHABET-> char
			{
				return (isT(s) ? char(s) : char(-s));
			}
		}*chains;//массив цепочек - правых частей правила
		Rule() { nn = 0x00; size = 0;}
		Rule(GRABLPHABET pnn, int piderror, short psize, Chain c, ...);
		char* getCRule(char* b, short nchain); //получить правило в виде N->цепочка (для распечатки)
		short GetNextChain(GRABLPHABET t, Rule::Chain* pchain, short j);// получить следующую за j подходящую цепочку, вернуть ее номер или -1
		bool checkEmptyRule(Rule::Chain chain);//проверить есть ли пустой переход
		
	};
	struct Greibah//грамматика Грейбах
	{
		short size;//количество правил
		GRABLPHABET startN; //стартовый символ
		GRABLPHABET stbottomT; //дно стека
		Rule* rules; //множество правил
		Greibah()
		{
			short size = 0;
			startN = 0;
			stbottomT = 0; 
			rules = 0;
		}
		Greibah(GRABLPHABET pstartN, GRABLPHABET pstbottomT, short psize, Rule r, ...);
		short getRule(GRABLPHABET pnn, Rule& prule);//получить правило, возвращается номер правила или -1
		Rule getRule(short n); //получить правило по номеру
	};
	Greibah getGreibach(); //получить грамматику
}