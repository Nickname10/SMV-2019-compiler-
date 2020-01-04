#include "pch.h"
#include "LT.h"
#include "IT.h"
#include "Error.h"
#include "CreatingTables.h"
#include "Semantic.h"
#include "Warning.h"
#include "PolishNotation.h"
static int iter;
#define FUNCTION_DECLARE "FUNCTION"
#define PROCEDURE_DECLARRE "PROCEDURE"
#define MAIN_DECLARE "MAIN"
namespace semantic 
{
	using namespace std;
	list<IT::Entry>  declareValueList;
	list<IT::Entry>  declareFunctionList;
	list<IT::Entry>  declareProcedureList;
	list<int> listnumberFunctionInIdTable; //Необходим для позднего связывания функции и её вызова
	list<int> listnumberProcedureInIdTable;
	string last_declare_global_block;
	char last_global_block_id[ID_MAXSIZE];
	int numberString_last_global_block;
	bool haveReturn = false;
	int ret = 0; //Необходим для возврата параметров из функции getFunction;
	int ret2 = 0; //Необходим для возврата параметров из функци getProcedure;
	IT::IDDATATYPE typeLastFunction = IT::IDDATATYPE::Undef;
	IT::Entry getProc(IT::Entry callProc)
	{
		ret2 = -1;
		int ind = 0;
		for (auto i :  declareProcedureList)
		{
			if (!strcmp(callProc.id, i.id))
			{
				int ind2 = 0;
				for (auto j : listnumberProcedureInIdTable)
				{
					if (ind2 == ind)
					{
						ret2 = j;
						break;
					}
					ind2++;
				}
				return i;
			}
			ind++;
		}
		return IT::Entry();
	}
	IT::Entry getFunction(IT::Entry callFunction)
	{
		ret = -1;
		int ind = 0;
		for (auto i : declareFunctionList)
		{
			if (!strcmp(callFunction.id, i.id))
			{
				int ind2 = 0;
				for (auto j : listnumberFunctionInIdTable)
				{
					if (ind2 == ind)
					{
						ret = j;
						break;
					}
					ind2++;
				}
				return i;
			}
			ind++;
		}
		return IT::Entry();
		
	}

	void check(Tables tables)// Семантический анализ
	{ 
		LT::LexTable* lexTable = tables.LEXTABLE;
		IT::IdTable*  idTable = tables.IDTABLE;
		
		for (int i = 0; i < lexTable->size; i++)
		{
		
			if (lexTable->table[i].lexema == LEX_FUNCTION)
			{
				//гарантированно послеследующей лексемой будет её идентификатор
				lexTable->table[i + 2].semanticChecked = true;
				LT::Entry lexEntryIdFunction = lexTable->table[i + 2];
				IT::Entry idEntryFunction = idTable->table[lexEntryIdFunction.numberInIdTable]; //Найдём запись в таблице идентификаторов
				//запишем информацию о количестве параметров функции и их типе, нужно идти до встречи правой скобки по таблицам
				int currentInd = i + 3; //индекс левой скобки
				while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
				{
					if (lexTable->table[currentInd].lexema == LEX_ID)
					{
						idEntryFunction.countParm++;
						idEntryFunction.parms.push_back(idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType);

						lexTable->table[currentInd].semanticChecked = true;
						
					}
					currentInd++;
				}
				
				declareFunctionList.push_back(idEntryFunction);//добавляем функцию в лист объявлений функций
				//пометим, что идентификатор обработан 
				listnumberFunctionInIdTable.push_back(lexEntryIdFunction.numberInIdTable);
				// ПРОВЕРКА НА НАЛИЧИЕ RETURN ПРЕДЫДУЩЕЙ ФУНКЦИИ
				if (!last_declare_global_block.empty())
				{
					if (!haveReturn) WARNING_HAVE_NO_RETURN(numberString_last_global_block, last_global_block_id);
				}
				typeLastFunction = idEntryFunction.idDataType;
				last_declare_global_block = FUNCTION_DECLARE;
				strcpy_s(last_global_block_id, idEntryFunction.id);
				numberString_last_global_block = lexEntryIdFunction.stringNumber;
				haveReturn = false;
				idTable->table[lexEntryIdFunction.numberInIdTable] = idEntryFunction;
				continue;
			}
			if (lexTable->table[i].lexema == LEX_RETURN)
			{
				int currentInd = i + 1;
				while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
				{
					switch (typeLastFunction)
					{
					case IT::IDDATATYPE::INT:	while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
					{
						if ((lexTable->table[currentInd].lexema == LEX_MORE) && ((lexTable->table[i].symbolOperation == OPERATION_MORE) || (lexTable->table[i].symbolOperation == OPERATION_LESS)))
						{
							if (typeLastFunction != IT::IDDATATYPE::BOOL)
							{
								throw ERROR_THROW_IN(706, lexTable->table[i].stringNumber,-1);
							}
						}
						else
							if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CMP)
							{
								if (typeLastFunction != IT::IDDATATYPE::BOOL) throw  ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
								while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
								{
									currentInd++;
								}
							}
							else
								if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CLENGTH)
								{
									if (typeLastFunction != IT::IDDATATYPE::INT) throw ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
									while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
									{
										currentInd++;
									}

								}
								else
									if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
									{
										if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
										{
											if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
												throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
											if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != typeLastFunction)
												throw ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
											while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
											{
												currentInd++;
											}
										}
										else
										{

											if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != typeLastFunction)
												throw ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
										}
									}
						currentInd++;
					}
												break;
					case IT::IDDATATYPE::STR:  while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
					{
						if (lexTable->table[currentInd].lexema == LEX_PLUS) throw ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
							if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
							{
								if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != IT::IDDATATYPE::STR)
								{
									throw ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
								}
								if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Procedure)
								{
									throw ERROR_THROW_SEMANTIC(711, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
								}
								if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
								{
									if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
									{
										throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
									}
									while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
									{
										currentInd++;
									}
								}
							}

						currentInd++;
					}
											   break;
					case IT::IDDATATYPE::BOOL: while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
					{
						if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CLENGTH || lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CMP)
						{
							while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
							{
								currentInd++;
							}
						}
						else
						if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
						{
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType == IT::IDDATATYPE::STR)
							{
								throw ERROR_THROW_IN(712, lexTable->table[i].stringNumber, -1);
							}
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Procedure)
							{
								throw ERROR_THROW_SEMANTIC(711, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
							}
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
							{
								if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
								{
									throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
								}
								while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
								{
									currentInd++;
								}
							}
						}
						currentInd++;
					}
											   break;
					case IT::IDDATATYPE::Undef: throw ERROR_THROW_SEMANTIC(709, lexTable->table[i].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
						break;
					}
					haveReturn = true;
					continue;
				}
			}
			if (lexTable->table[i].lexema == LEX_PROCEDURE)
			{
				//добавим процедуру в лист объявления процедур
				if (!last_declare_global_block.empty())
				{
					if (!haveReturn) WARNING_HAVE_NO_RETURN(numberString_last_global_block, last_global_block_id);
				}
				declareProcedureList.push_back(idTable->table[lexTable->table[i + 1].numberInIdTable]);
				listnumberProcedureInIdTable.push_back(lexTable->table[i + 1].numberInIdTable);
				lexTable->table[i + 1].semanticChecked = true;
				last_declare_global_block = PROCEDURE_DECLARRE;
				strcpy_s(last_global_block_id, idTable->table[lexTable->table[i + 1].numberInIdTable].id);
				numberString_last_global_block = lexTable->table[i + 1].stringNumber;
				haveReturn = true;
				continue;
			}
			if (lexTable->table[i].lexema == LEX_MAIN)
			{
				if (!last_declare_global_block.empty())
				{
					if (!haveReturn) WARNING_HAVE_NO_RETURN(numberString_last_global_block, last_global_block_id);
				}
				last_declare_global_block = MAIN_DECLARE;
				numberString_last_global_block = lexTable->table[i].stringNumber;
				haveReturn = false;
				continue;
			}
			if (lexTable->table[i].lexema == LEX_VALUE)
			{
				declareValueList.push_back(idTable->table[lexTable->table[i + 2].numberInIdTable]);
				lexTable->table[i + 2].semanticChecked = true;
				continue;
			}
			//Проверка вызова функции и связывание
			if ((lexTable->table[i-2].lexema!=LEX_VALUE && lexTable->table[i-1].lexema!=LEX_PROCEDURE )&& lexTable->table[i - 1].lexema != LEX_INTEGER &&
				lexTable->table[i].lexema == LEX_ID && lexTable->table[i+1].lexema == LEX_LEFT_SQUARE_BRACKET)
			{
				lexTable->table[i].semanticChecked = true;
			  //идём по списку аргументов при вызове 
				int currentInd = i + 1; // Индекс левой скобки
				list<IT::IDDATATYPE> callParmList;
				while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)//Заполняем аргументы вызова
				{
					if (lexTable->table[currentInd].lexema==LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
					{
						callParmList.push_back(idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType);
						lexTable->table[currentInd].semanticChecked = true;
					}
					currentInd++;
				}
				IT::Entry function = getFunction(idTable->table[lexTable->table[i].numberInIdTable]);

				if (ret == -1) 
					throw ERROR_THROW_SEMANTIC(707, lexTable->table[i].stringNumber, string(idTable->table[lexTable->table[i].numberInIdTable].id));//Еесли такая функция не найдена, то выдать ошибку
				lexTable->table[i].numberInIdTable = ret; //getFunction так же возвращает номер функции в таблице идентификаторов, т.к. лексический анализ, привязал её к переменной
				 
				if (function.countParm!=callParmList.size())
					throw ERROR_THROW_SEMANTIC(701, lexTable->table[i].stringNumber, string(idTable->table[lexTable->table[i].numberInIdTable].id))
				
				//сравнение списка параметров
				for (auto iter1 = callParmList.begin(), iter2 = function.parms.begin(); iter1 != callParmList.end(), iter2 != function.parms.end(); iter1++, iter2++)
				{
					if (*iter1 != *iter2)
						throw ERROR_THROW_SEMANTIC(701, lexTable->table[i].stringNumber, string(idTable->table[lexTable->table[i].numberInIdTable].id));
				}
				continue;
			}
			//Проверка вызова процедуры и связывание
			if ((lexTable->table[i - 1].lexema == LEX_SEMICOLON || lexTable->table[i-1].lexema == LEX_BEGIN || lexTable->table[i - 1].lexema == LEX_END) && lexTable->table[i].lexema == LEX_ID && lexTable->table[i + 1].lexema == LEX_SEMICOLON)
			{
				IT::Entry proc = getProc(idTable->table[lexTable->table[i].numberInIdTable]);
				if (ret2 == -1) throw ERROR_THROW_SEMANTIC(708, lexTable->table[i].stringNumber,string( idTable->table[lexTable->table[i].numberInIdTable].id));
				lexTable->table[i].numberInIdTable = ret2;
				continue;
			}
			//проверка левой и правой части выражения
			if (lexTable->table[i].lexema == LEX_EQUAL)
			{
				idTable->table[lexTable->table[i - 1].numberInIdTable].isInitialized = true;
				IT::IDDATATYPE typeLeftExpression = idTable->table[lexTable->table[i - 1].numberInIdTable].idDataType;
				int currentInd = i + 1;
				if (idTable->table[lexTable->table[i - 1].numberInIdTable].idType == IT::IDTYPE::Function || idTable->table[lexTable->table[i - 1].numberInIdTable].idType == IT::IDTYPE::Procedure)
				{
					throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
				}
				
					switch (typeLeftExpression)
					{
					case IT::IDDATATYPE::INT :	while (lexTable->table[currentInd].lexema!= LEX_SEMICOLON)
						{
							if ((lexTable->table[currentInd].lexema == LEX_MORE) && ((lexTable->table[currentInd].symbolOperation == OPERATION_MORE) || (lexTable->table[currentInd].symbolOperation == OPERATION_LESS)))
							{
								if (typeLeftExpression != IT::IDDATATYPE::BOOL)
								{
									throw ERROR_THROW_SEMANTIC(706, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id))
								}
							}
							else
								if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CMP)
								{
									if (typeLeftExpression != IT::IDDATATYPE::BOOL)
										throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
									while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
									{
										currentInd++;
									}
								}
								else
									if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CLENGTH)
									{
										if (typeLeftExpression != IT::IDDATATYPE::INT)
											throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
										while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
										{
											currentInd++;
										}

									}
									else
										if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
										{
											if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
											{
												if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
													throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
												if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != typeLeftExpression)
													throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
												while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
												{
													currentInd++;
												}
											}
											else
											{

												if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != typeLeftExpression)
													throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id))
											}
										}
							currentInd++;
						}
						break;
					case IT::IDDATATYPE::STR:  while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
					{
						if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CLENGTH || lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CMP)
						{
							throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
						}
						if (lexTable->table[currentInd].lexema == LEX_PLUS) throw ERROR_THROW_SEMANTIC(710, lexTable->table[i-1].stringNumber, string(idTable->table[lexTable->table[i-1].numberInIdTable].id))
						if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
						{
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != IT::IDDATATYPE::STR)
							{
								throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
							}
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Procedure)
							{
								throw ERROR_THROW_SEMANTIC(711, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
							}
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
							{
								if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
								{
									throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
								}
								while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
								{
									currentInd++;
								}
							}
						}

						currentInd++;
					}
						break;
					case IT::IDDATATYPE::BOOL: while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
					{
						if (lexTable->table [currentInd].lexema ==LEX_BASE_LIBRARY_CLENGTH || lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CMP)
						{ 
							while (lexTable->table[currentInd].lexema!=LEX_RIGHT_SQUARE_BRACKET)
							{
								currentInd++;
							}
						}else 
						if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
						{
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType == IT::IDDATATYPE::STR)
							{
								throw ERROR_THROW_SEMANTIC(705, lexTable->table[i - 1].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
							}
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Procedure)
							{
								throw ERROR_THROW_SEMANTIC(711, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
							}
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
							{
								if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
								{
									throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
								}
								while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
								{
									currentInd++;
								}
							}
						}
						currentInd++;
					}
						break;
					case IT::IDDATATYPE::Undef: throw ERROR_THROW_SEMANTIC(709, lexTable->table[i].stringNumber, string(idTable->table[lexTable->table[i - 1].numberInIdTable].id));
						break;
					}
					
					//polishNotation::PolishNotation(i + 1, tables.LEXTABLE, tables.IDTABLE);
				continue;
			}
			if (lexTable->table[i].lexema == LEX_ID && lexTable->table[i-1].lexema!= LEX_INTEGER && lexTable->table[i+1].lexema!=LEX_EQUAL && lexTable->table[i - 1].lexema != LEX_PROCEDURE)
			{
				if ((!idTable->table[lexTable->table[i].numberInIdTable].isInitialized) && idTable->table[lexTable->table[i].numberInIdTable].idType == IT::IDTYPE::Value )
				{
					WARNING_IND_UNDEF(lexTable->table[i].stringNumber, idTable->table[lexTable->table[i].numberInIdTable].id);
					idTable->table[lexTable->table[i].numberInIdTable].isInitialized = true;
				}
				continue;
			}
			if (lexTable->table[i].lexema == LEX_IF)
			{
				int currentInd = i + 2;
				while (lexTable->table[currentInd].lexema != LEX_THEN)
				{
					if (lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CLENGTH || lexTable->table[currentInd].lexema == LEX_BASE_LIBRARY_CMP)
					{
						while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
						{
							currentInd++;
						}
					}
					else
					if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
					{
						if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType == IT::IDDATATYPE::STR)
						{
							throw ERROR_THROW_IN(713,lexTable->table[i].stringNumber,-1);
						}
						if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Procedure)
						{
							throw ERROR_THROW_SEMANTIC(711, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
						}
						if (idTable->table[lexTable->table[currentInd].numberInIdTable].idType == IT::IDTYPE::Function)
						{
							if (lexTable->table[currentInd + 1].lexema != LEX_LEFT_SQUARE_BRACKET)
							{
								throw ERROR_THROW_SEMANTIC(701, lexTable->table[currentInd].stringNumber, string(idTable->table[lexTable->table[currentInd].numberInIdTable].id));
							}
							while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
							{
								currentInd++;
							}
						}
					}
					currentInd++;
				}
				continue;
			}
			if (lexTable->table[i].lexema == LEX_BASE_LIBRARY_CLENGTH)
			{
				int currentInd = i + 1;
				while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
				{
					if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL ||lexTable->table[currentInd].lexema == LEX_COMMA ||
						lexTable->table[currentInd].lexema == LEX_LEFT_SQUARE_BRACKET || lexTable->table[currentInd].lexema == LEX_RIGHT_SQUARE_BRACKET)
					{
						if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
						{
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != IT::IDDATATYPE::STR)
							{
								throw ERROR_THROW_SEMANTIC(607, lexTable->table[i].stringNumber, "");
							}
						}
					} else throw ERROR_THROW_SEMANTIC(607, lexTable->table[i].stringNumber, "");
						currentInd++;
				}
				continue;
			}
			if (lexTable->table[i].lexema == LEX_BASE_LIBRARY_CMP)
			{
				int currentInd = i + 1;
				while (lexTable->table[currentInd].lexema != LEX_RIGHT_SQUARE_BRACKET)
				{
					if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL ||  lexTable->table[currentInd].lexema == LEX_COMMA ||
						lexTable->table[currentInd].lexema == LEX_LEFT_SQUARE_BRACKET || lexTable->table[currentInd].lexema == LEX_RIGHT_SQUARE_BRACKET)
					{
						if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL)
						{
							if (idTable->table[lexTable->table[currentInd].numberInIdTable].idDataType != IT::IDDATATYPE::STR)
							{
								throw ERROR_THROW_SEMANTIC(606, lexTable->table[i].stringNumber, "");
							}
						}
					}
					else
						throw ERROR_THROW_SEMANTIC(606, lexTable->table[i].stringNumber, "");
					currentInd++;
				}
				continue;
			}
		
			if (lexTable->table[i].lexema == LEX_PRINT)
			{
				int counter = 0;
				int currentInd = i+1;
				while (lexTable->table[currentInd].lexema != LEX_SEMICOLON)
				{
					if (lexTable->table[currentInd].lexema == LEX_ID || lexTable->table[currentInd].lexema == LEX_LITERAL) counter++;
					currentInd++;
				}
				if (counter != 1)
					throw ERROR_THROW_IN(714, lexTable->table[i].stringNumber, UNDEF_NUMBER);
				i = currentInd - 1;
				continue;
			}
			

		}
	
	}
}
