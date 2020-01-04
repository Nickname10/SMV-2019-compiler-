#pragma once
#include "pch.h"
#include "IT.h"
#include "In.h"
#define WARNING_IND_UNDEF(line, identificator) Warning:: Add(Warning::Note(5,line,identificator));
#define WARNING_IND_LARGE(line, identificator) Warning:: Add(Warning::Note(6,line,identificator));
#define WARNING_HAVE_NO_RETURN(line, identificator) Warning::Add(Warning::Note(7,line,identificator));


namespace Warning
{
	void CreateWarningTable();
	struct Note
	{
		int id;
		int line; 
		std::string message;
		Note(int idError, int line, char id[ID_MAXSIZE]);
		Note();
	};
	struct Table
	{
		 Note Warnigs[100];
		 int indLastWarning = 0;
		 
	};
	Table* getWarningTable();

	void Add(Note note);
	void printConsole();
	
}
