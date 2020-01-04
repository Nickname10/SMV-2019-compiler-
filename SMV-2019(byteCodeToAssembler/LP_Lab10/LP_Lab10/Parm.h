#pragma once
#include "pch.h"
namespace Parm
{
	struct PARM
	{
		std::wstring in;
		std::wstring out;


	};
	PARM getparm(int argc, _TCHAR* argv[]);
}