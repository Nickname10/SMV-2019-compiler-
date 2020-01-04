#pragma once
#include "pch.h"
#include "CreatingTables.h"
#include "Greibah.h"
#include "Parm.h"
#include "Log.h"
#define MFST_DIAGN_NUMBER 3

#define MFST_DIAGN_MAXSIZE (2*ERROR_MAXSIZE_MESSAGE)

using namespace std;
typedef stack<short> MFSTSTACK;
static int currentLevel;
namespace MFST
{
	struct MfstState //состояние автомата(для сохранения)
	{
		short lenta_position;//позиция на ленте
		short nrule; //Номер правила 
		short nrulechain; //номер текущей цепочки, текущего правила
		MFSTSTACK st; //стек автомата
		MfstState();
		MfstState(short pposition, MFSTSTACK pst, short pnrulechain); //Номер текущей цепочки, текущего правила
		MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain);
	};
	//Магазинный автомат
	struct Mfst
	{
		enum RC_STEP//код возврата функции step
		{
			NS_OK, //найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,  //не найдено правило грамматики(ошибка в грамматике)
			NS_NORULECHAIN,//не найдена подходящая цепочка правила (Ошибка в исходном коде)
			NS_ERROR,//Неизвестный нетерминальный символ грамматики
			TS_OK,//тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,//Тек. символ ленты != вершине стека, восстановлено состояние
			LENTA_END,//текущая позиция ленты >= lenta_size
			SURPRISE //неожиданный код возврата (ошибка в step)
		};
		struct MfstDiagnosis
		{
			short lenta_position;//позиция на ленте
			RC_STEP rc_step;//код завершения шага
			short nrule; //Номер правила
			short nrule_chain; //номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position,//позиция на ленте 
				RC_STEP prc_step,//Код завршения шага
				short pnrule, // Номер правила
				short pnrule_chain //Номер цепочки правила
			);
		}diagnosis[MFST_DIAGN_NUMBER]; //Последние самые глубокие сообщения
		GRABLPHABET* lenta;// перекодированная (TS/NS) лента (из LEX)
		short lenta_position;//текущая позиция в ленте
		short nrule;//номер текущего правила
		short nrulechain;//номер текущей цепочки, текущего правила
		short lenta_size;//размер ленты
		GRB::Greibah grebach; //Грамматика грейбах
		Tables lex; //результат работы лексического анализатора
		MFSTSTACK st; //стек автомата
		stack<MfstState> storestate; //стек для сохранения состояний
		stack<MfstState> TREE; //стек для сохранения состояний успешного разбора
		
		Mfst() { lenta = 0; lenta_size = lenta_position = 0; };
		Mfst(Tables plex, GRB::Greibah pgrebach);
		char* getCSt(char* buf);// получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);//лента: n символов с pos
		char* getDiagnosis(short n, char* buf); //получить n-ую строку диагностики или 0х00
		bool savestate(); //сохранить состояние автомата
		bool reststate(); //восстановить состояние автомата
		bool push_chain(GRB::Rule::Chain chain); //поместить цепочку правила в стек
		RC_STEP step(); //выполнить шаг автомата
		void start(Parm::PARM parms, Log::LOG); //запустить автомат
		fstream fout;

		bool savediagnosis(RC_STEP pprc_step//Код завершения шага
		);
		void printrules(); //Вывести последовательность правил
		struct Deducation //Вывод
		{
			short size; //Количество шагов в выводе
			short* nrules; //номера правил грамматики
			short* nrulechains;//Номера цепочек правил грамматики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation(); //сохранить дерево вывода
		

	};
	
}