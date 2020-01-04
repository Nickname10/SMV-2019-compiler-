#include "pch.h"
#include "LT.h"
#include "FST.h"
#include "Error.h"
using namespace std;
namespace LT {
	LexTable* Create(int size)
	{
		LexTable* newTable = new LexTable;
		newTable->table = new Entry[size];
		newTable->size = 0;
		newTable->maxsize = size;
		
		return newTable;
	}

	void Add(LexTable* lextable, Entry entry)
	{
		if (lextable->size > lextable->maxsize) throw ERROR_THROW(2);
		lextable->table[lextable->size] = entry;
		lextable->size++;
	}

	Entry* GetEntry(LexTable* lextable, int n)
	{
		return &(lextable->table[n-1]);
	}

	void Delete(LexTable* lextable)
	{
		delete lextable->LexemText;
		delete lextable->table;
		delete lextable;
	}

	Entry::Entry(char lexem, int stringNumber, int numberInIdTable, char symbolOperation, string view)
	{
		this->lexema = lexem;
		this->stringNumber = stringNumber;
		this->numberInIdTable = numberInIdTable;
		this->symbolOperation = symbolOperation;
		this->semanticChecked = false;
		this->view = view;
	}

	Entry::Entry()
	{
		
		lexema = UNDEF_SYMBOL;
		numberInIdTable = UNDEF_NUMBER;
		stringNumber = UNDEF_NUMBER;
		symbolOperation = UNDEF_SYMBOL;
		semanticChecked = false;
	}

}
