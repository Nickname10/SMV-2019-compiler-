#pragma once
#include "pch.h"
#include "Error.h"
#define UNDEF_SYMBOL '#'
#define UNDEF_NUMBER -1
#define OPERATION_DIV '/'
#define OPERATION_MOD '%'
#define OPERATION_MINUS '-'
#define OPERATION_PLUS '+'
#define OPERATION_MULTIPLICATION '*'
#define OPERATION_MORE '>'
#define OPERATION_LESS '<'
#define LEX_FUNCTION 'f'
#define LEX_RETURN 'r'
#define LEX_VALUE 'o'
#define LEX_LEFT_SQUARE_BRACKET '['
#define LEX_RIGHT_SQUARE_BRACKET ']'
#define LEX_LEFTHESIS '('
#define LEX_RIGHTHESIS ')'
#define LEX_COMMA ','
#define LEX_BEGIN 'b'
#define LEX_END 'e'
#define LEX_DIV 'v'
#define LEX_MOD 'v'
#define LEX_MINUS 'v'
#define LEX_STAR 'v'
#define LEX_PLUS 'v'
#define LEX_EQUAL '='
#define LEX_MAIN 'm'
#define LEX_PRINT 'p'
#define LEX_BASE_LIBRARY_CMP 'c'
#define LEX_BASE_LIBRARY_CLENGTH 'z'
#define LEX_SEMICOLON ';'
#define LEX_IF 'j'
#define LEX_THEN 'q'
#define LEX_ELSE 'n'
#define LEX_LITERAL 'l'
#define LEX_ID 'i'
#define LEX_INTEGER 't'
#define LEX_STRING 't'
#define LEX_BOOLEAN 't'
#define LEX_MORE 'v'
#define LEX_LESS 'v'
#define LEX_TRUE 'l'
#define LEX_FALSE 'l'
#define LEX_PROCEDURE 'w'

namespace LT {
	struct Entry {
		std::string view;
		char lexema;
		int stringNumber;
		int numberInIdTable;
		char symbolOperation;
		bool semanticChecked;
		Entry(char lexem, int stringNumber, int numberInIdTable = UNDEF_NUMBER, char symbolOperation = UNDEF_SYMBOL, std::string view ="");
		Entry();
	};
	struct LexTable
	{
		int countEnd;
		int maxsize;
		int size;
		Entry* table;
		unsigned char* LexemText;
		int indEndConstSegment = 0;
		int indEndDataSegment = 0;
	};

	LexTable* Create(int size);
	void Add(LexTable* lextable, Entry entry);
	Entry* GetEntry(LexTable* lextable, int n);
	void Delete(LexTable* lextable);
	
}