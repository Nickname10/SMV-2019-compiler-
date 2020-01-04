#include "pch.h"
#include "Parm.h"
using namespace std;
namespace Parm
{
	
	PARM getparm(int argc, _TCHAR* argv[])
	{
		
		PARM newParm;
		if (argc < 2)
		{
			cout << "Ошибка путь к файлу не задан\n";
			exit(0);
		}
		if (argc > 2)
		{
			cout << "Слишком много параметров\n";
			exit(0);
		}
		newParm.in =argv[1];
		if (newParm.in.length() < 5)
		{
			cout << "Малая длина входного параметра\n";
			exit(0);
		}
		wstring expansion;
		for (int i = newParm.in.length() - 1; i >= newParm.in.length() - 4; i--)
		{
			expansion += newParm.in[i];
		}

		if (expansion != wstring(L"nib."))
		{
			cout << "Расширение исходного файла должно быть bin\n";
			exit(0);
		}
		for (int i = 0; i < newParm.in.length() - 4; i++)
		{
			newParm.out += newParm.in[i];
		}
		newParm.out += L".asm";

		return newParm;
	}
}
