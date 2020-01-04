#include "pch.h"
#include "Parm.h"
#include "AssemblerCodeGeneration.h"
int _tmain(int argc, _TCHAR *argv[])
{
	setlocale(LC_ALL, "rus");

	Parm::PARM parm = Parm::getparm(argc, argv);
	AssemblerCodeGeneration::GenerateAsmFile(parm);
	std::cout << "Трансляция успешно завершена\n";


	return 0;
}