#include "pch.h"
#include "Parm.h"
#include "Error.h"
using namespace std;
namespace Parm
{
	
	PARM getparm(int argc, _TCHAR* argv[])
	{
		if (argc<1)  ERROR_THROW(100);
		PARM newParm;
		wstring wParm(argv[1]);
		wstring parm;
		if (wParm.length() < 5) throw ERROR_THROW(100);
		if ((wParm.length() - 4) > PARM_MAX_SIZE) throw ERROR_THROW(104);
		for (int i = 0; i < 4; i++)
		{
			parm += wParm[i];
		}
		if (parm != PARM_IN) throw ERROR_THROW(100);
		int ind = 0;
		for (int i = 4; i < wParm.length(); i++)
		{
			newParm.in[ind++] = wParm[i];
		}
		newParm.in[ind] = '\0';
		wParm.clear();
		bool isOut = false, isLog = false;
		for (int i = 2; i < argc; i++)
		{
			parm.clear();
			ind = 0;
			wstring wParm_(argv[i]);
			for (int j = 0; j < 5; j++)
			{
				parm += wParm_[j];
			}
			if (parm == PARM_OUT)
			{
				isOut = true;
				for (int j = 5; j < wParm_.length(); j++)
				{
					newParm.out[ind++] = wParm_[j];
				}
				newParm.out[ind] = '\0';
			}
			if (parm == PARM_LOG)
			{
				isLog = true;
				for (int j = 5; j < wParm_.length(); j++)
				{
					newParm.log[ind++] = wParm_[j];
				}
				newParm.log[ind] = '\0';
			}
		}
		
		if (!isOut)
		{
			
			wstring newString(newParm.in);
			newString = newString + L".out";
			ind = 0;
			for (int i = 0; i < newString.length(); i++)
				newParm.out[ind++] = newString[i];
			newParm.out[ind] = '\0';
		}
		if (!isLog)
		{
			wstring newString(newParm.in);
			newString = newString + L".log";
			ind = 0;
			for (int i = 0; i < newString.length(); i++)
				newParm.log[ind++] = newString[i];
			newParm.log[ind] = '\0';
		}

		return newParm;
	}
}
