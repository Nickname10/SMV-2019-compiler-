#pragma once
#include "pch.h"
#include "LT.h"
#define ID_MAXSIZE 21
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffff
#define TI_STR_MAXSIZE 255

namespace IT
{
	enum IDDATATYPE
	{
		Undef = 0, INT = 1, STR = 2, BOOL = 3
	};
	enum IDTYPE
	{
		Value = 1,
		Function = 2,
		Parm = 3,
		Lexema = 4,
		Procedure = 5
	};
	struct Entry
	{
		
		int idxfirstLE;//номер первой строки
		char id[ID_MAXSIZE];
		IDDATATYPE idDataType;
		IDTYPE idType;
		int numberSystem;
		bool isInitialized;
		std::string view;
		union uValue
		{
			int vint;
			struct 
			{
				unsigned char len;
				char str[TI_STR_MAXSIZE - 1];

			}vstr[TI_STR_MAXSIZE];
		}value;
		int countParm;
		std::string nameInByteCode;
		std::list<IDDATATYPE> parms;
		Entry(int numberString, char id[ID_MAXSIZE], IDDATATYPE idDataType, IDTYPE idType, uValue value, std::string view = NULL, int numberSystem = 10);
		Entry();
	};
	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};
	IdTable* Create(int size);
	int Add(IdTable* idtable,Entry);
	int GetIndxLastElement(IdTable* idtable);
	void Delete(IdTable* idtable);
	int isInIdTable(IdTable idTable, Entry entry);
	bool searchCopy(IdTable idTable, Entry entry);
	bool operator ==(Entry e1, Entry e2);
	
}