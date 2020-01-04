#include "pch.h"
#include "IT.h"
#include "Error.h"
using namespace IT;
IdTable* IT::Create(int size) {
	IdTable* newIdTable = new IdTable;
	newIdTable->maxsize = size;
	newIdTable->table = new Entry[size];
	newIdTable->size = 0;
	return newIdTable;
}

int IT::Add(IdTable * idtable, Entry newEntry)
{
	if(idtable->size>idtable->maxsize) throw ERROR_THROW(3)
	idtable->table[idtable->size] = newEntry;
	idtable->size++;
	return(idtable->size - 1);
	
}

int IT::GetIndxLastElement(IdTable * idtable)
{
	return (idtable->size-1);
}

void IT::Delete(IdTable* idtable)
{
	delete idtable->table;
	delete idtable;
}

int IT::isInIdTable(IdTable idTable,Entry entry)
{
	int cuurentNumberSpace;		
	std::string compare;
	std::list<int> listRet;
	for (int i = 0; i < idTable.size; i++)//перебираем все области видимости
	{
		if (!strcmp(idTable.table[i].id, entry.id))
		{
			
			for (int j = 0; j < entry.view.length(); j++)
			{
				compare = compare + entry.view[j];
				if (entry.view[j] == '_')
				{
					if (compare == idTable.table[i].view) listRet.push_back(i);
				}
			}
			compare.clear();
		}
		
	}
	int maxLength = INT_MIN;
	int indViewMaxLength = UNDEF_NUMBER;
	if (listRet.empty()) return UNDEF_NUMBER;
	else
	{		
		for (auto i : listRet)
		{
			if (maxLength < (int)idTable.table[i].view.length())
			{
				indViewMaxLength = i;
				maxLength = idTable.table[i].view.length();
			}
		}
		
		
		return indViewMaxLength;
	}
}

bool IT::searchCopy(IdTable idTable, Entry entry)
{
	
	int cuurentNumberSpace;
	std::string compare;
	for (int i = 0; i < idTable.size; i++)
	{
		if (!strcmp(idTable.table[i].id, entry.id)&& idTable.table[i].view == entry.view)
		{
			return true;
		}

	}
	return false;
}

bool IT::operator==(Entry e1, Entry e2)//первым передвать операнд не из таблицы объявленных
{
	
	if (!strcmp(e1.id, e2.id))
	{
			
		std::string compare;
		for (int i = 0; i < e1.view.length(); i++)
		{
			compare = compare + e1.view[i];
			if (compare[i] == '_')
			{
				if (compare == e2.view)
				{
					return true;
				}
			}

		}
	}
	return false;
	
}

IT::Entry::Entry(int numberString, char id[ID_MAXSIZE], IDDATATYPE idDataType, IDTYPE idType, uValue value, std::string view, int numberSystem)
{
	strcpy_s(this->id, id);
	idxfirstLE = numberString;
	this->idDataType = idDataType;
	this->idType = idType;
	this->value = value;
	this->view = view;
	this->numberSystem = numberSystem;
	this->isInitialized = false;
	this->countParm = 0;

}
IT::Entry::Entry()
{
	idxfirstLE = UNDEF_NUMBER;
	this->idDataType = (IDDATATYPE)UNDEF_NUMBER;
	this->idType = (IDTYPE)UNDEF_NUMBER;
	uValue value;
	value.vint = UNDEF_NUMBER;
	this->value = value;
	this->view = "";
	this->numberSystem = 10;
	this->isInitialized = false;
}
