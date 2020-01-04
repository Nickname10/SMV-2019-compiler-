
#include "pch.h"
#include "Log.h"
#include "Error.h"
#include "CreatingTables.h"
#include "FST.h"
#include "In.h"
#include "IT.h"
#include "LT.h"
#include "Parm.h"
#include "out.h"
#include "PolishNotation.h"
#include "MFST.h"
#include "Greibah.h"
#include "Semantic.h"
#include "Debug_MFST.h"
#include "Warning.h"
#include "ByteCodeGeneration.h"
int _tmain(int argc, _TCHAR *argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		
		Warning::CreateWarningTable();
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);
		Tables tables = createTables(in);
		
		printOut(in, parm, tables);
		MFST::Mfst mfst(tables, GRB::getGreibach());
		mfst.start(parm, log);
		semantic::check(tables);	
		ByteCode::CreateByteCode(tables,parm);
		Log::WriteWarnings(log, Warning::getWarningTable());
		Warning::printConsole();
		Log::WriteLine(log, "Трансляция успешно завершена\n" );
		std::cout << "Трансляция успешно завершена\n";
	
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	}
	return 0;
}