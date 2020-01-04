#pragma once
#include "pch.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"
#include "Warning.h"
namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};
	static const LOG INITLOG = { L"", NULL };
	LOG getlog(wchar_t logfile[]);
	void WriteLine(LOG log, std::string);
	void WriteLog(LOG log);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);
	void WriteError(LOG log, Error::ERROR error);
	void WriteWarnings(LOG log, Warning::Table* WarningTable);
	void Close(LOG log);
	
};