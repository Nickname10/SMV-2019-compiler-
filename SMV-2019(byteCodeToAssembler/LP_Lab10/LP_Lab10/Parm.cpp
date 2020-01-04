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
			cout << "������ ���� � ����� �� �����\n";
			exit(0);
		}
		if (argc > 2)
		{
			cout << "������� ����� ����������\n";
			exit(0);
		}
		newParm.in =argv[1];
		if (newParm.in.length() < 5)
		{
			cout << "����� ����� �������� ���������\n";
			exit(0);
		}
		wstring expansion;
		for (int i = newParm.in.length() - 1; i >= newParm.in.length() - 4; i--)
		{
			expansion += newParm.in[i];
		}

		if (expansion != wstring(L"nib."))
		{
			cout << "���������� ��������� ����� ������ ���� bin\n";
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
