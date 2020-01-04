#include "pch.h"
#include "Error.h"
#include "In.h"
using namespace std;
namespace In
{
	IN getin(wchar_t infile[])
	{
		ifstream fin(infile);
		IN newIn;
		if (!fin.is_open())
			throw ERROR_THROW(110);

		string STR;
		string text;

		while (!fin.eof())
		{
			getline(fin, STR);
			text += STR;
			text += '\n';
			newIn.lines++;
		}
		int line = 1;
		int column = 1;
		int length = text.length();
		newIn.textFormated = new unsigned char[length * 2];
		unsigned char* pText;
		pText = newIn.textFormated;
		string textAfterCheck;
		for (int i = 0; i < length; i++)
		{
			
			if (newIn.code[(int)((unsigned char)text[i])] == IN::T) textAfterCheck += text[i];
			else
			{
				if (newIn.code[(int)((unsigned char)text[i])] == IN::F)
				{
					throw ERROR_THROW_IN(113, line, column);
				}
				else if (newIn.code[(int)((unsigned char)text[i])] == IN::I)
					{
					newIn.ignor++;
					}
				else
			    textAfterCheck += (unsigned char)(newIn.code[(int)((unsigned char)text[i])]);
			}
			column++;
			if (text[i] == '\n')
			{
				line++;
				column = 1;
			}
		
		}
		
		bool quote = false;
		int indAfterDivOrMod = false;
		bool flagDiv = false;
		bool flagMod = false;
		pText = newIn.textFormated;
		for (int i = 0; i < length; i++)
		{
			
			if (textAfterCheck[i] == '"')
			{
				*pText = '"';
				pText++;
				if (quote) quote = false;
				else quote = true;
			}
			else
			{
				if (quote)
				{
					*pText = textAfterCheck[i];
					pText++;
				}
				else
				{
					
					if (i + 2 < length)
					{
						if ((textAfterCheck[i] == 'd'&&textAfterCheck[i + 1] == 'i'&&textAfterCheck[i + 2] == 'v'))
						{
							flagDiv = true;
							indAfterDivOrMod = i + 3;
						}
						else if ((textAfterCheck[i] == 'm'&&textAfterCheck[i + 1] == 'o'&&textAfterCheck[i + 2] == 'd'))
						{
							flagMod = true;
							indAfterDivOrMod = i + 3;
						}
					}
					

					if ((textAfterCheck[i] == ';') || (textAfterCheck[i] == ',') || 
						(textAfterCheck[i] == '+') || (textAfterCheck[i] == '-') || 
						(textAfterCheck[i] == '*') || (textAfterCheck[i] == '=') ||
						(textAfterCheck[i] == '(') || (textAfterCheck[i] == ')') || (textAfterCheck[i] == '[') ||
						(textAfterCheck[i] == ']') || (textAfterCheck[i] == '>') || (textAfterCheck[i] == '<') || (textAfterCheck[i] == '\n')||
						(flagDiv)||(flagMod)
						)
					{
						if (!flagDiv && !flagMod)
						{
							*pText = ' ';
							pText++;
							*pText = textAfterCheck[i];
							pText++;
							*pText = ' ';
							pText++;
						}
						else
						{ 
							if (flagDiv)
							{
								*pText = ' ';
								pText++;
								*pText = 'd';
								pText++;
								*pText = 'i';
								pText++;
								*pText = 'v';
								pText++;
								*pText = ' ';
								pText++;
								i = indAfterDivOrMod-1;
								flagDiv = false;
							}
							else
							{
								*pText = ' ';
								pText++;
								*pText = 'm';
								pText++;
								*pText = 'o';
								pText++;
								*pText = 'd';
								pText++;
								*pText = ' ';
								pText++;
								i = indAfterDivOrMod-1;
								flagMod = false;
							}
						}
						
					}
					else
					{
						if (textAfterCheck[i] == ':')
						{
							*pText = textAfterCheck[i];
							pText++;
							*pText = ' ';
							pText++;
						}
						else
						{
							*pText = textAfterCheck[i];
							pText++;
						}
					}

				}
			}	//*pText = text[i];
				
		}
		*pText = ' ';
		pText++;
		*pText = (unsigned char)'\0';
		pText = newIn.textFormated;
		newIn.textFormated = new unsigned char[length * 2];
		unsigned char* pText2 = newIn.textFormated;
		bool spaceAdded = false; 
		quote = false;
		for (int i = 0; i < strlen((char*)pText); i++)
		{
			if (pText[i]=='"') 
			{
				if (quote) quote = false;
				else quote = true;
			}
			if ((pText[i] == ' ') &&(!spaceAdded))
			{
				*pText2 = ' ';
				pText2++;
				if (!quote) spaceAdded = true;
			}
			else if (pText[i] != ' ')
			{
				spaceAdded = false;
				*pText2 = pText[i];
				pText2++;
			}
		}
		*pText2 = (unsigned char)'\0';
		int counterLexem = 1;
		pText2 = newIn.textFormated;
		for (int i = 0; i < strlen((char*)newIn.textFormated); i++)
		{
			if (*pText2 == ' ') counterLexem++;
			pText2++;
		}
		newIn.size = strlen((char*)newIn.textFormated);
		newIn.counterLexem = counterLexem;
	
		return newIn;
	}
}		
