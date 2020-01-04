#include "pch.h"
#include "out.h"
#include "In.h"
#include "Parm.h"
#include "LT.h"
#include "IT.h"
#include "CreatingTables.h"
using namespace std;
void printOut(In::IN input, Parm::PARM parm,Tables tables)
{
	std::ofstream fout(parm.out);
	if (!fout.is_open()) throw ERROR_THROW(114);
	int currentNumberString=1;
	fout.fill(' ');
	fout << setw(5) << left<<"1";

	for (int i = 0; i < tables.LEXTABLE->size; i++)
	{
		
		if (currentNumberString != tables.LEXTABLE->table[i].stringNumber)
		{
			fout << endl;
			currentNumberString = tables.LEXTABLE->table[i].stringNumber;
			fout.fill(' ');
			fout << setw(4) << left << currentNumberString;
			fout << ' ';
		}
		fout << tables.LEXTABLE->table[i].lexema;
		
	}
	fout <<endl<< setw(14) << left << "Номер строки: " << setw(5) << left << " Лекс" << setw(25) << right << "Номер в таблице идентификаторов:" << setw(10) << left << "Символ мат. опер." << endl;
	for (int i = 0; i < tables.LEXTABLE->size; i++)
	{
		fout << endl << "--------------------------------------------------------------------------------------------------------------------------------------------------------------";
		fout << endl << setw(4) << left << tables.LEXTABLE->table[i].stringNumber << setw(15) << right << tables.LEXTABLE->table[i].lexema;
		if (tables.LEXTABLE->table[i].numberInIdTable != -1) fout << setw(25) << right << tables.LEXTABLE->table[i].numberInIdTable;
		if (tables.LEXTABLE->table[i].symbolOperation != '#') fout << setw(45) << right << tables.LEXTABLE->table[i].symbolOperation;
		
	}
	fout << endl << setw(20) << left << "ID" << setw(10) << left << "1ая встр" << setw(20) << left << "Тип данных" << setw(20) << left << "Тип идентификатора"<<setw(20)<<left<<"Видимость"<<setw(20)<<"Осн. сист. счисл." <<"Значение";
	for (int i = 0; i < tables.IDTABLE->size; i++)
	{
		fout << endl << setw(20) << left << tables.IDTABLE->table[i].id;
		fout << setw(10) << left << tables.IDTABLE->table[i].idxfirstLE;
		switch (tables.IDTABLE->table[i].idDataType)
		{
		case IT::INT:  fout << setw(20) << left <<"integer";
			break;
		case IT::STR: fout << setw(20) << left << "str";
			break;
		case IT::BOOL: fout << setw(20) << left << "boolean";
			break;
		default:fout << setw(20) << left << tables.IDTABLE->table[i].idDataType;
			break;
		}
		switch (tables.IDTABLE->table[i].idType)
		{
		case UNDEF_NUMBER: cout << "Warning: Error create out table";
			break;
		case IT::Value:  fout << setw(20) << left << "Value";
			break;
		case IT::Function: fout << setw(20) << left << "Function";
			break;
		case IT::Lexema: fout << setw(20) << left << "Lexema";
			break;
		case IT::Parm: fout << setw(20) << left << "Parm";
			break;
		case IT::Procedure: fout << setw(20) << left <<  "Procedure";
				break;
		default:fout << setw(20) << left << tables.IDTABLE->table[i].idType;
			break;
		}
		fout << setw(20) <<left << tables.IDTABLE->table[i].view;
		fout << setw(20)<<left<<tables.IDTABLE->table[i].numberSystem;
		if (tables.IDTABLE->table[i].idDataType == 1) fout << tables.IDTABLE->table[i].value.vint;
		if (tables.IDTABLE->table[i].idDataType == 2 && tables.IDTABLE->table[i].idType==IT::IDTYPE::Lexema)
		{

			for (int j = 0; j < tables.IDTABLE->table[i].value.vstr->len; j++)
				fout << left << tables.IDTABLE->table[i].value.vstr->str[j];
		}

	}
	fout.close();
}

		


