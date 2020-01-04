#include "pch.h"
#include "log.h"
#include "Error.h"
#include "Parm.h"
#include "In.h"
namespace Log
{
	LOG Log::getlog(wchar_t logfile[])
	{
		LOG res;
		res.stream = new std::ofstream;
		res.stream->open(logfile);
		if (!res.stream->is_open())
			throw ERROR_THROW(112);
		wcscpy_s(res.logfile, logfile);
		return res;
	}

	void WriteLine(LOG log, std:: string str)
	{
		*log.stream << str<<std::endl;
	}
	void WriteLog(LOG log)
	{
		time_t t = time(nullptr);
		tm now;
		localtime_s(&now, &t);
		char tmChars[PARM_MAX_SIZE];
		strftime(tmChars, PARM_MAX_SIZE, "%d.%m.%Y %H:%M:%S", &now);
		*log.stream << "---- �������� ------- ����: " << tmChars << std::endl;
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		*log.stream << "---- ��������� -------" << std::endl;
		char cnv[PARM_MAX_SIZE];
		size_t charsConverted(0);
		wcstombs_s(&charsConverted, cnv, parm.log, PARM_MAX_SIZE);
		*log.stream << "-log: " << cnv << std::endl;
		wcstombs_s(&charsConverted, cnv, parm.out, PARM_MAX_SIZE);
		*log.stream << "-out: " << cnv << std::endl;
		wcstombs_s(&charsConverted, cnv, parm.in, PARM_MAX_SIZE);
		*log.stream << "-in: " << cnv << std::endl;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "---- �������� ������ -----" << std::endl;
		*log.stream << "���������� ��������: " << in.size << std::endl;
		*log.stream << "���������������:     " << in.ignor << std::endl;
		*log.stream << "���������� �����:    " << in.lines << std::endl;
		*log.stream << "--------------------------" << std::endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream == nullptr || !log.stream->is_open())
		{


			if (error.inext.col!=UNDEF_NUMBER)
				std::cout << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
			else std::cout << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << std::endl;
		}
		else
		{
			if (error.inext.col != UNDEF_NUMBER)
			{
				std::cout << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
				*log.stream << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
			}
			else
			{
				std::cout<< "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << std::endl;
				*log.stream << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << std::endl;
			}
		}
	}
	void WriteWarnings(LOG log, Warning::Table* WarningTable)
	{
		*log.stream << "��������������: \n";
		bool hasWarnings = true;
		for (int i = 0; i < WarningTable->indLastWarning; i++)//����� ��������������
		{
			*log.stream << "������:" <<WarningTable->Warnigs[i].line << "." << WarningTable->Warnigs[i].message << std::endl;
			hasWarnings = false;
		}
		if (hasWarnings) *log.stream << "�������������� �����������\n";
	}
	void Close(LOG log)
	{
		if (log.stream != nullptr)
		{
			log.stream->close();
			delete log.stream;
		}
	}
};
