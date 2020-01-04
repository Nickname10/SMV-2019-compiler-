#include "pch.h"
#include "IT.h"
#include "Warning.h"
namespace Warning
{

	Table* WarningTable;

	void printConsole()
	{

		for (int i = 0; i <WarningTable->indLastWarning; i++)//Вывод предупреждений
		{
			std::cout << "Строка:" << Warning::WarningTable->Warnigs[i].line << "." << Warning::WarningTable->Warnigs[i].message << std::endl;
		}
	}


	Note::Note(int idError, int line, char id[ID_MAXSIZE])
	{
		this->id = idError;
		this->line = line;

		switch (idError)
		{
		case 5:  message = "Предупреждение 70: Неинициализированный идентификатор: " + std::string(id) + " (Применена инициализация по умолчанию)";
			break;
		case 6:  message = "Предупреждение 72: Имя идентификатора превышает 20 символов, применено усечение до: " + std::string(id);
			break;
		case 7:  message = "Предупреждение 71: Не найдено ни одного оператора return у функции: " + std::string(id) + " (по умолчанию функции возвращают : integer - 0, string - "", boolean - false)";
			break;
			break;
		default: message = "Неопределенное предупреждение"; 
			break;
		}
	}

	Note::Note()
	{
		id = -1;
		line = -1;
		message = "";
	}
	Table * getWarningTable()
	{
		return WarningTable;
	}
	void Add(Note note)
	{

		if (WarningTable->indLastWarning < 100) WarningTable->Warnigs[WarningTable->indLastWarning] = note;
		WarningTable->indLastWarning++;
	}
	void CreateWarningTable()
	{
		WarningTable = new Table();
	}
}