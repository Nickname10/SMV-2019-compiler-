#include "pch.h"
#include "ByteCodeGeneration.h"
#include "Greibach_rules.h"
#include "Greibah.h"
#include "Parm.h"
#include "PolishNotation.h"

void ByteCode::CreateByteCode(Tables tables, Parm::PARM parm)
{
	wstring name;
	for (int i = 0; i < wcslen(parm.in)-4; i++)//получаем in без расширения, добавляем .bin
	{
		name += parm.in[i];
	}

	ofstream fout(name + wstring(L".bin"));
	if (!fout.is_open()) throw ERROR_THROW(115);
	SerializeIdTable(tables.IDTABLE, fout);
	LT::LexTable* lexTable = tables.LEXTABLE;
	IT::IdTable* idTable = tables.IDTABLE;
	int numberFunction = 0; //Номер функции, для генерирования имени
	int numberParm = 0; //Номер параметра, для генерирования имени
	int numberProcedure = 0; //Номер процедуры, для генерирования имени
	string bufCom = ""; //Хранит команды для глобальных переменных
	string viewLastThen = "";
	string viewLastElse = "";
	string viewLastIf = "";
	for (int i = 0; i < lexTable->size; i++)
	{
		if (lexTable->table[i].lexema == LEX_FUNCTION)
		{
			viewLastThen = "";
			viewLastElse = "";
			i = GenerateFunctionHeader(lexTable, idTable, i, numberFunction, numberParm, fout);

			continue;
		}
		if (lexTable->table[i].lexema == LEX_EQUAL)
		{
			IT::Entry leftOperand = idTable->table[lexTable->table[i - 1].numberInIdTable];
			polishNotation::PolishNotation(i + 1, lexTable, idTable);
			i++;
			if (lexTable->table[i-1].view == "GLOBAL_")//Подумать над условием! т.к. вывод иначе будет всегда в main
			{
				for (int j = i; lexTable->table[j].lexema != LEX_SEMICOLON; j++)
				{
					if (lexTable->table[j].lexema == '!')
					{
						bufCom += translateCommand(OP_CALL);
		
						switch (lexTable->table[j+1].lexema)
						{
						case  LEX_ID: bufCom += idTable->table[lexTable->table[j + 1].numberInIdTable].nameInByteCode;
							break;
						case  LEX_BASE_LIBRARY_CMP:
						{
							char buf[10];
							_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
							string sBuf(buf);
							while (sBuf.length() != 4)
							{
								sBuf = '0' + sBuf;
							}
							bufCom += sBuf;
							_itoa_s(BASE_LIBRARY_CMP_NAME, buf, 10);
							sBuf = string(buf);
							while (sBuf.length() != 32)
							{
								sBuf = '0' + sBuf;
							}
							bufCom += sBuf; 
							
						}
						break;
						case LEX_BASE_LIBRARY_CLENGTH:
						{
							char buf[10];
							_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
							string sBuf(buf);
							while (sBuf.length() != 4)
							{
								sBuf = '0' + sBuf;
							}
							bufCom += sBuf;
							_itoa_s(BASE_LIBRARY_CLENGTH_NAME, buf, 10);
							sBuf = string(buf);
							while (sBuf.length() != 32)
							{
								sBuf = '0' + sBuf;
							}
							bufCom += sBuf;
						
						}
						break;
							default:
								break;
						}
						
						j++;
						continue;
					}
					if (lexTable->table[j].lexema == LEX_PLUS)
					{
						switch (lexTable->table[j].symbolOperation)
						{
						case OPERATION_PLUS: bufCom+=translateCommand(OP_ADD);
							break;
						case OPERATION_MINUS: bufCom += translateCommand(OP_SUB);
							break;
						case OPERATION_MULTIPLICATION: bufCom += translateCommand(OP_MUL);
							break;
						case OPERATION_DIV: bufCom += translateCommand(OP_DIV);
							break;
						case OPERATION_MOD: bufCom += translateCommand(OP_MOD);
							break;
						case OPERATION_LESS: bufCom += translateCommand(OP_CMP_LESS);
							break;
						case OPERATION_MORE: bufCom += translateCommand(OP_CMP_MORE);
							break;
						}
					}
					if (lexTable->table[j].lexema == LEX_LITERAL || lexTable->table[j].lexema == LEX_ID)
					{
						switch (idTable->table[lexTable->table[j].numberInIdTable].idDataType)
						{
						case  IT::IDDATATYPE::INT:
						case  IT::IDDATATYPE::BOOL:
						{
							bufCom += translateCommand(OP_PUSH);
						}
						break;
						case IT::IDDATATYPE::STR:
						{
							if (idTable->table[lexTable->table[j].numberInIdTable].idType == IT::IDTYPE::Lexema)bufCom += translateCommand(OP_PUSH_OFFSET);
							else bufCom += translateCommand(OP_PUSH);
						}
						default:
							break;
						}

						bufCom+=idTable->table[lexTable->table[j].numberInIdTable].nameInByteCode;
					}
					i = j;
				}

				bufCom += translateCommand(OP_POP);
				bufCom += leftOperand.nameInByteCode;
			}
			else
			{
				for (int j = i; lexTable->table[j].lexema != LEX_SEMICOLON; j++)
				{
					if (lexTable->table[j].lexema == '!')
					{
						printCommand(OP_CALL,fout);

						switch (lexTable->table[j + 1].lexema)
						{
						case  LEX_ID: fout <<idTable->table[lexTable->table[j + 1].numberInIdTable].nameInByteCode;
							break;
						case  LEX_BASE_LIBRARY_CMP:
						{
							char buf[10];
							_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
							string sBuf(buf);
							while (sBuf.length() != 4)
							{
								sBuf = '0' + sBuf;
							}
							fout<< sBuf;
							_itoa_s(BASE_LIBRARY_CMP_NAME, buf, 10);
							sBuf = string(buf);
							while (sBuf.length() != 32)
							{
								sBuf = '0' + sBuf;
							}
							fout << sBuf;

						}
						break;
						case LEX_BASE_LIBRARY_CLENGTH:
						{
							char buf[10];
							_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
							string sBuf(buf);
							while (sBuf.length() != 4)
							{
								sBuf = '0' + sBuf;
							}
							fout<< sBuf;
							_itoa_s(BASE_LIBRARY_CLENGTH_NAME, buf, 10);
							sBuf = string(buf);
							while (sBuf.length() != 32)
							{
								sBuf = '0' + sBuf;
							}
							fout<< sBuf;

						}
						break;
						default:
							break;
						}

						j++;
						continue;
					}
					if (lexTable->table[j].lexema == LEX_PLUS)
					{
						switch (lexTable->table[j].symbolOperation)
						{
						case OPERATION_PLUS: printCommand(OP_ADD, fout);
							break;
						case OPERATION_MINUS: printCommand(OP_SUB, fout);
							break;
						case OPERATION_MULTIPLICATION: printCommand(OP_MUL, fout);
							break;
						case OPERATION_DIV: printCommand(OP_DIV, fout);
							break;
						case OPERATION_MOD: printCommand(OP_MOD, fout);
							break;
						case OPERATION_LESS: printCommand(OP_CMP_LESS, fout);
							break;
						case OPERATION_MORE: printCommand(OP_CMP_MORE, fout);
							break;
						}
					}
					if (lexTable->table[j].lexema == LEX_LITERAL || lexTable->table[j].lexema == LEX_ID)
					{
						switch (idTable->table[lexTable->table[j].numberInIdTable].idDataType)
						{
						case  IT::IDDATATYPE::INT:
						case  IT::IDDATATYPE::BOOL:
						{
							printCommand(OP_PUSH, fout);
						}
						break;
						case IT::IDDATATYPE::STR:
						{


							if (idTable->table[lexTable->table[j].numberInIdTable].idType == IT::IDTYPE::Lexema)printCommand(OP_PUSH_OFFSET,fout);
							else printCommand(OP_PUSH, fout);
						}
						default:
							break;
						}

						fout << idTable->table[lexTable->table[j].numberInIdTable].nameInByteCode;
					}
					i = j;
				}

				printCommand(opcode::OP_POP, fout);
				fout << leftOperand.nameInByteCode;
			}
			continue;
		}
		if (lexTable->table[i].lexema == LEX_RETURN)
		{
			polishNotation::PolishNotation(i + 1, lexTable, idTable);
			i++;
			for (int j = i; lexTable->table[j].lexema != LEX_SEMICOLON; j++)
			{
				if (lexTable->table[j].lexema == '!')
				{
					printCommand(OP_CALL, fout);

					switch (lexTable->table[j + 1].lexema)
					{
					case  LEX_ID: fout << idTable->table[lexTable->table[j + 1].numberInIdTable].nameInByteCode;
						break;
					case  LEX_BASE_LIBRARY_CMP:
					{
						char buf[10];
						_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
						string sBuf(buf);
						while (sBuf.length() != 4)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;
						_itoa_s(BASE_LIBRARY_CMP_NAME, buf, 10);
						sBuf = string(buf);
						while (sBuf.length() != 32)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;

					}
					break;
					case LEX_BASE_LIBRARY_CLENGTH:
					{
						char buf[10];
						_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
						string sBuf(buf);
						while (sBuf.length() != 4)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;
						_itoa_s(BASE_LIBRARY_CLENGTH_NAME, buf, 10);
						sBuf = string(buf);
						while (sBuf.length() != 32)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;

					}
					break;
					default:
						break;
					}

					j++;
					continue;
				}
				if (lexTable->table[j].lexema == LEX_PLUS)
				{
					switch (lexTable->table[j].symbolOperation)
					{
					case OPERATION_PLUS: printCommand(OP_ADD, fout);
						break;
					case OPERATION_MINUS: printCommand(OP_SUB, fout);
						break;
					case OPERATION_MULTIPLICATION: printCommand(OP_MUL, fout);
						break;
					case OPERATION_DIV: printCommand(OP_DIV, fout);
						break;
					case OPERATION_MOD: printCommand(OP_MOD, fout);
						break;
					case OPERATION_LESS: printCommand(OP_CMP_LESS, fout);
						break;
					case OPERATION_MORE: printCommand(OP_CMP_MORE, fout);
						break;
					}
				}
				if (lexTable->table[j].lexema == LEX_LITERAL || lexTable->table[j].lexema == LEX_ID)
				{
					switch (idTable->table[lexTable->table[j].numberInIdTable].idDataType)
					{
					case  IT::IDDATATYPE::INT:
					case  IT::IDDATATYPE::BOOL:
					{
						printCommand(OP_PUSH, fout);
					}
					break;
					case IT::IDDATATYPE::STR:
					{


						if (idTable->table[lexTable->table[j].numberInIdTable].idType == IT::IDTYPE::Lexema)printCommand(OP_PUSH_OFFSET, fout);
						else printCommand(OP_PUSH, fout);
					}
					default:
						break;
					}

					fout << idTable->table[lexTable->table[j].numberInIdTable].nameInByteCode;
				}
				i = j;
			}
			printCommand(opcode::OP_RET, fout);
			continue;
		}
		if (lexTable->table[i].lexema == LEX_MAIN)
		{
			viewLastElse = "";
			viewLastThen = "";
			char buf[5];

			_itoa_s(CODE_MAIN_SEGMENT, buf, 2);
			string sCodeMainSegment(buf);
			while (sCodeMainSegment.length() != 4)
			{
				sCodeMainSegment = '0' + sCodeMainSegment;
			}
			fout << sCodeMainSegment;
			char buffer;
			fout << bufCom;
			bufCom.clear();
			continue;
		}
		if (lexTable->table[i].lexema == LEX_PRINT)//У оператора принт будет спец кодировка, ещё 2 бита под тип данных параметра
		{
			//Сама комманда (6 бит) + (2бит тип данных печати) 
			printCommand(OP_WRITE, fout);
			switch (idTable->table[lexTable->table[i + 2].numberInIdTable].idDataType)
			{
			case IT::IDDATATYPE::BOOL:
			{
				fout << "11";
			}
			break;
			case IT::IDDATATYPE::INT:
			{
				fout << "01";
			}
			break;
			case IT::IDDATATYPE::STR:
			{
				fout << "10";

			}
			default:
				break;
			}
			fout << idTable->table[lexTable->table[i + 2].numberInIdTable].nameInByteCode;
			
			
		}
		if (lexTable->table[i].lexema == LEX_IF) 
		{	
			viewLastThen = "";
			viewLastElse = "";
			polishNotation::PolishNotation(i + 1, lexTable, idTable);
			i++;
			for (int j = i; ; j++)
			{
				if (lexTable->table[j].lexema == '!')
				{
					printCommand(OP_CALL, fout);

					switch (lexTable->table[j + 1].lexema)
					{
					case  LEX_ID: fout << idTable->table[lexTable->table[j + 1].numberInIdTable].nameInByteCode;
						break;
					case  LEX_BASE_LIBRARY_CMP:
					{
						char buf[10];
						_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
						string sBuf(buf);
						while (sBuf.length() != 4)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;
						_itoa_s(BASE_LIBRARY_CMP_NAME, buf, 10);
						sBuf = string(buf);
						while (sBuf.length() != 32)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;

					}
					break;
					case LEX_BASE_LIBRARY_CLENGTH:
					{
						char buf[10];
						_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
						string sBuf(buf);
						while (sBuf.length() != 4)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;
						_itoa_s(BASE_LIBRARY_CLENGTH_NAME, buf, 10);
						sBuf = string(buf);
						while (sBuf.length() != 32)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;

					}
					break;
					default:
						break;
					}

					j++;
					continue;
				}
				if (lexTable->table[j].lexema == LEX_PLUS)
				{
					switch (lexTable->table[j].symbolOperation)
					{
					case OPERATION_PLUS: printCommand(OP_ADD, fout);
						break;
					case OPERATION_MINUS: printCommand(OP_SUB, fout);
						break;
					case OPERATION_MULTIPLICATION: printCommand(OP_MUL, fout);
						break;
					case OPERATION_DIV: printCommand(OP_DIV, fout);
						break;
					case OPERATION_MOD: printCommand(OP_MOD, fout);
						break;
					case OPERATION_LESS: printCommand(OP_CMP_LESS, fout);
						break;
					case OPERATION_MORE: printCommand(OP_CMP_MORE, fout);
						break;
					}
				}
				if (lexTable->table[j].lexema == LEX_LITERAL || lexTable->table[j].lexema == LEX_ID)
				{
					switch (idTable->table[lexTable->table[j].numberInIdTable].idDataType)
					{
					case  IT::IDDATATYPE::INT:
					case  IT::IDDATATYPE::BOOL:
					{
						printCommand(OP_PUSH, fout);
					}
					break;
					case IT::IDDATATYPE::STR:
					{


						if (idTable->table[lexTable->table[j].numberInIdTable].idType == IT::IDTYPE::Lexema)printCommand(OP_PUSH_OFFSET, fout);
						else printCommand(OP_PUSH, fout);
					}
					default:
						break;
					}

					fout << idTable->table[lexTable->table[j].numberInIdTable].nameInByteCode;
				}
				i = j;
				if (lexTable->table[j-1].lexema == LEX_THEN || lexTable->table[j-1].lexema == LEX_ELSE) break;
			}
			
			printCommand(opcode::OP_IF, fout);
			i -= 2;
			continue;
		}
		if (lexTable->table[i].lexema == LEX_PROCEDURE)
		{
			i = GenerateProcedureHeader(lexTable, idTable, i, numberProcedure, fout);
			continue;
		}
		if (lexTable->table[i].lexema == LEX_THEN)
		{
			viewLastThen = lexTable->table[i].view;
			continue;
		}
		if (lexTable->table[i].lexema == LEX_ELSE)
		{
			viewLastElse = lexTable->table[i].view;
			printCommand(opcode::OP_CREATE_MARK_FALSE, fout);
			continue;
		}
		if (lexTable->table[i].lexema == LEX_END)
		{
			if (viewLastElse == lexTable->table[i].view)
			{
				printCommand(opcode::OP_CREATE_MARK_AFTER_FALSE, fout);
			}
			if (viewLastThen == lexTable->table[i].view)
			{
				printCommand(opcode::OP_GO_AFTER_ELSE,fout);
				if (lexTable->table[i + 1].lexema != LEX_ELSE)//В случае если использован не полный оператор if
				{
					printCommand(OP_CREATE_MARK_AFTER_FALSE, fout);
					printCommand(OP_CREATE_MARK_FALSE, fout);
				}
			}
			continue;
		}
		if ((lexTable->table[i - 1].lexema == LEX_SEMICOLON || lexTable->table[i - 1].lexema == LEX_BEGIN || lexTable->table[i - 1].lexema == LEX_END) && lexTable->table[i].lexema == LEX_ID && lexTable->table[i + 1].lexema == LEX_SEMICOLON)
		{
			printCommand(OP_CALL_PROC, fout);
			fout << idTable->table[lexTable->table[i].numberInIdTable].nameInByteCode;
			continue;
		}
		if ((lexTable->table[i - 1].lexema == LEX_SEMICOLON || lexTable->table[i - 1].lexema == LEX_BEGIN || lexTable->table[i - 1].lexema == LEX_END) && lexTable->table[i].lexema == LEX_ID && lexTable->table[i + 1].lexema == LEX_LEFT_SQUARE_BRACKET)
		{
			polishNotation::PolishNotation(i, lexTable, idTable);
			for (int j = i; lexTable->table[j].lexema != LEX_SEMICOLON; j++)
			{
				if (lexTable->table[j].lexema == '!')
				{
					printCommand(OP_CALL_PROC, fout);

					switch (lexTable->table[j + 1].lexema)
					{
					case  LEX_ID: fout << idTable->table[lexTable->table[j + 1].numberInIdTable].nameInByteCode;
						break;
					case  LEX_BASE_LIBRARY_CMP:
					{
						char buf[10];
						_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
						string sBuf(buf);
						while (sBuf.length() != 4)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;
						_itoa_s(BASE_LIBRARY_CMP_NAME, buf, 10);
						sBuf = string(buf);
						while (sBuf.length() != 32)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;

					}
					break;
					case LEX_BASE_LIBRARY_CLENGTH:
					{
						char buf[10];
						_itoa_s(CODE_BASE_LIBRARY_SIGMENT, buf, 2);
						string sBuf(buf);
						while (sBuf.length() != 4)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;
						_itoa_s(BASE_LIBRARY_CLENGTH_NAME, buf, 10);
						sBuf = string(buf);
						while (sBuf.length() != 32)
						{
							sBuf = '0' + sBuf;
						}
						fout << sBuf;

					}
					break;
					default:
						break;
					}

					j++;
					continue;
				}
				if (lexTable->table[j].lexema == LEX_PLUS)
				{
					switch (lexTable->table[j].symbolOperation)
					{
					case OPERATION_PLUS: printCommand(OP_ADD, fout);
						break;
					case OPERATION_MINUS: printCommand(OP_SUB, fout);
						break;
					case OPERATION_MULTIPLICATION: printCommand(OP_MUL, fout);
						break;
					case OPERATION_DIV: printCommand(OP_DIV, fout);
						break;
					case OPERATION_MOD: printCommand(OP_MOD, fout);
						break;
					case OPERATION_LESS: printCommand(OP_CMP_LESS, fout);
						break;
					case OPERATION_MORE: printCommand(OP_CMP_MORE, fout);
						break;
					}
				}
				if (lexTable->table[j].lexema == LEX_LITERAL || lexTable->table[j].lexema == LEX_ID)
				{
					switch (idTable->table[lexTable->table[j].numberInIdTable].idDataType)
					{
					case  IT::IDDATATYPE::INT:
					case  IT::IDDATATYPE::BOOL:
					{
						printCommand(OP_PUSH, fout);
					}
					break;
					case IT::IDDATATYPE::STR:
					{


						if (idTable->table[lexTable->table[j].numberInIdTable].idType == IT::IDTYPE::Lexema)printCommand(OP_PUSH_OFFSET, fout);
						else printCommand(OP_PUSH, fout);
					}
					default:
						break;
					}

					fout << idTable->table[lexTable->table[j].numberInIdTable].nameInByteCode;
				}
				i = j;
			}
			//Подумать насчет передачи строк в функцию
			continue;
		}
	}

}

void ByteCode::SerializeIdTable(IT::IdTable* idTable, ofstream& fout)
{
	for (int i = 0; i < idTable->size; i++)
	{
		if (idTable->table[i].idType == IT::IDTYPE::Lexema)
		{
			addNameSegmentToNameEntry(idTable->table[i], CODE_CONST_SEGMENT);
			addNameConstToNameEntry(idTable->table[i]);
			fout << idTable->table[i].nameInByteCode;
			printDataType(idTable->table[i], fout);
			printValueConst(idTable->table[i], fout);
		}
	}
	int nameValue = 0;
	for (int i = 0; i < idTable->size; i++)
	{
		if (idTable->table[i].idType == IT::IDTYPE::Value)
		{
			addNameSegmentToNameEntry(idTable->table[i], CODE_DATA_SEGMENT);
			addNameValueToNameEntry(idTable->table[i], nameValue++);
			fout << idTable->table[i].nameInByteCode;
			printDataType(idTable->table[i], fout);
		}
	}

}

void ByteCode::addNameSegmentToNameEntry(IT::Entry & entry, int codeSegment)
{
	char buf[5];
	_itoa_s(codeSegment, buf, 2);
	for (int i = 0; i < strlen(buf); i++)
	{
		entry.nameInByteCode += buf[i];
	}
	while (entry.nameInByteCode.length() != 4)
	{
		entry.nameInByteCode = '0' + entry.nameInByteCode;
	}

}

void ByteCode::addNameConstToNameEntry(IT::Entry & entry)
{
	char buf[33];
	_itoa_s(atoi(entry.id), buf, 2);
	string nameEntry(buf);
	while (nameEntry.length() != 32)
	{
		nameEntry = '0' + nameEntry;
	}
	entry.nameInByteCode = entry.nameInByteCode + nameEntry;

}

void ByteCode::printDataType(IT::Entry & entry, ofstream& fout)
{
	char buf[3];
	switch (entry.idDataType)
	{
	case IT::IDDATATYPE::INT:
		_itoa_s(CODE_TYPE_INT, buf, 2);
		break;
	case IT::IDDATATYPE::BOOL:
		_itoa_s(CODE_TYPE_BOOLEAN, buf, 2);
		break;
	case IT::IDDATATYPE::STR:
		_itoa_s(CODE_TYPE_STRING, buf, 2);
		break;
	}
	string type(buf);
	while (type.length() != 2)
	{
		type = '0' + type;
	}
	fout << type;
}

void ByteCode::printValueConst(IT::Entry & entry, ofstream & fout)
{
	char buf[33];
	switch (entry.idDataType)
	{
	case IT::IDDATATYPE::INT:
	case IT::IDDATATYPE::BOOL:
	{
		_itoa_s(entry.value.vint, buf, 2);
		string value(buf);
		while (value.length() != 32)
		{
			value = '0' + value;
		}
		fout << value;
	}
	break;
	case IT::IDDATATYPE::STR:
	{
		_itoa_s(entry.value.vstr->len, buf, 2);
		string value(buf);
		while (value.length() != 32)
		{
			value = '0' + value;
		}
		char buf2[9];
		for (int i = 0; i < entry.value.vstr->len; i++)
		{
			int CodeASCII = (unsigned int)(unsigned char)entry.value.vstr->str[i];
			_itoa_s(CodeASCII, buf2, 2);
			string binaryCodeASCII(buf2);
			while (binaryCodeASCII.length() != 8)
			{
				binaryCodeASCII = '0' + binaryCodeASCII;
			}
			value += binaryCodeASCII;
		}
		fout << value;

	}
	break;

	}
}

void ByteCode::addNameValueToNameEntry(IT::Entry & entry, int iName)
{
	char buf[33];
	_itoa_s(iName, buf, 2);
	string nameEntry(buf);
	while (nameEntry.length() != 32)
	{
		nameEntry = '0' + nameEntry;
	}
	entry.nameInByteCode = entry.nameInByteCode + nameEntry;
}

int ByteCode::GenerateFunctionHeader(LT::LexTable * lexTable, IT::IdTable * idTable, int currentPosition, int& numberFunction, int& numberParm, ofstream& fout)
{

	addNameSegmentToNameEntry(idTable->table[lexTable->table[currentPosition + 2].numberInIdTable], CODE_FUNCTION_SEGMENT);
	addNameValueToNameEntry(idTable->table[lexTable->table[currentPosition + 2].numberInIdTable], numberFunction++);
	fout << idTable->table[lexTable->table[currentPosition + 2].numberInIdTable].nameInByteCode;
	printDataType(idTable->table[lexTable->table[currentPosition + 2].numberInIdTable], fout);
	printCountParm(idTable->table[lexTable->table[currentPosition + 2].numberInIdTable], fout);
	int positionIdParm = currentPosition + 2;
	for (int i = 0; i < idTable->table[lexTable->table[currentPosition + 2].numberInIdTable].countParm; i++)
	{
		positionIdParm += 3;
		addNameSegmentToNameEntry(idTable->table[lexTable->table[positionIdParm].numberInIdTable], CODE_PARM_SEGMENT);
		addNameValueToNameEntry(idTable->table[lexTable->table[positionIdParm].numberInIdTable], numberParm++);
		fout << idTable->table[lexTable->table[positionIdParm].numberInIdTable].nameInByteCode;
		printDataType(idTable->table[lexTable->table[positionIdParm].numberInIdTable], fout);
	}
	return positionIdParm + 1;
}

void ByteCode::printCountParm(IT::Entry & entry, ofstream & fout)
{
	char buf[33];
	_itoa_s(entry.countParm, buf, 2);
	string value(buf);
	while (value.length() != 32)
	{
		value = '0' + value;
	}
	fout << value;
}

string ByteCode::translateCommand(int code)
{
	char buf[7];
	_itoa_s(code, buf, 2);
	std::string sCode(buf);
	while (sCode.length() != 6)
	{
		sCode = '0' + sCode;
	}
	char buf2[5];
	_itoa_s(CODE_COMMAND_SEGMENT, buf2, 2);
	std::string segment(buf2);
	while (segment.length() != 4)
	{
		segment = '0' + segment;
	}
	return (segment + sCode);
}

void ByteCode::printCommand(int code, ofstream & fout)
{
	char buf[7];
	_itoa_s(code, buf, 2);
	std::string sCode(buf);
	while (sCode.length() != 6)
	{
		sCode = '0' + sCode;
	}
	char buf2[5];
	_itoa_s(CODE_COMMAND_SEGMENT, buf2, 2);
	std::string segment(buf2);
	while (segment.length() != 4)
	{
		segment = '0' + segment;
	}
	fout << segment << sCode;

}
//Процедура представляется 4 бита сегмент 32 бита название
int ByteCode::GenerateProcedureHeader(LT::LexTable * lexTable, IT::IdTable * idTable, int currentPosition, int& numberProcedure, ofstream& fout)
{

	addNameSegmentToNameEntry(idTable->table[lexTable->table[currentPosition + 1].numberInIdTable], CODE_PROCEDURE_SEGMENT);
	addNameValueToNameEntry(idTable->table[lexTable->table[currentPosition + 1].numberInIdTable], numberProcedure++);
	fout << idTable->table[lexTable->table[currentPosition + 1].numberInIdTable].nameInByteCode;
	return currentPosition + 1;
}