#include "pch.h"
#include "IT.h"
#include "Warning.h"
namespace Warning
{

	Table* WarningTable;

	void printConsole()
	{

		for (int i = 0; i <WarningTable->indLastWarning; i++)//����� ��������������
		{
			std::cout << "������:" << Warning::WarningTable->Warnigs[i].line << "." << Warning::WarningTable->Warnigs[i].message << std::endl;
		}
	}


	Note::Note(int idError, int line, char id[ID_MAXSIZE])
	{
		this->id = idError;
		this->line = line;

		switch (idError)
		{
		case 5:  message = "�������������� 70: �������������������� �������������: " + std::string(id) + " (��������� ������������� �� ���������)";
			break;
		case 6:  message = "�������������� 72: ��� �������������� ��������� 20 ��������, ��������� �������� ��: " + std::string(id);
			break;
		case 7:  message = "�������������� 71: �� ������� �� ������ ��������� return � �������: " + std::string(id) + " (�� ��������� ������� ���������� : integer - 0, string - "", boolean - false)";
			break;
			break;
		default: message = "�������������� ��������������"; 
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