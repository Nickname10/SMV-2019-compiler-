#include "pch.h"
#include "FST.h"
#include "LT.h"
#include "IT.h"
#include "In.h"
#include "CreatingTables.h"
#include "Error.h"
#include "Warning.h"
#define EMPTY_SYMBOL '#'
#define VIEW_MAIN "MAIN"
#define VIEW_THEN "THEN" //THENNUMBERTHEN
#define VIEW_ELSE "ELSE" //ELSENUMBERELSE
#define BINARY 2
#define OCT 8
#define HEX 16
using namespace fst;
using namespace std;
using namespace LT;
string currentView;
stack<string> stackVisibilityToken;
int counterEnd;
int numberThen;
int numberElse;
int counterMain;
int numberCurrentLiteral;
int typeData;//1 - INT 2 - STR 3 - boolean
int typeId; // 1 - Value 2 - Function  3 - Parm  4 - Lexema 5 - procedure
void initTypeLexem(string currentLexem, int numberString, int poz, LexTable* lexTable, IT::IdTable* idTable)
{
	char* text = (char*)currentLexem.c_str();
	FST fstProcedure(text, 11,
		NODE(1, RELATION('p', 1)),
		NODE(1, RELATION('r', 2)),
		NODE(1, RELATION('o', 3)),
		NODE(1, RELATION('c', 4)),
		NODE(1, RELATION('e', 5)),
		NODE(1, RELATION('d', 6)),
		NODE(1, RELATION('u', 7)),
		NODE(1, RELATION('r', 8)),
		NODE(1, RELATION('e', 9)),
		NODE(1, RELATION(':', 10)),
		NODE());
	FST fstInteger(text, 8,
		NODE(1, RELATION('i', 1)),
		NODE(1, RELATION('n', 2)),
		NODE(1, RELATION('t', 3)),
		NODE(1, RELATION('e', 4)),
		NODE(1, RELATION('g', 5)),
		NODE(1, RELATION('e', 6)),
		NODE(1, RELATION('r', 7)),
		NODE());
	FST fstString(text, 4,
		NODE(1, RELATION('s', 1)),
		NODE(1, RELATION('t', 2)),
		NODE(1, RELATION('r', 3)),
		NODE());
	FST fstFunction(text, 10,
		NODE(1, RELATION('f', 1)),
		NODE(1, RELATION('u', 2)),
		NODE(1, RELATION('n', 3)),
		NODE(1, RELATION('c', 4)),
		NODE(1, RELATION('t', 5)),
		NODE(1, RELATION('i', 6)),
		NODE(1, RELATION('o', 7)),
		NODE(1, RELATION('n', 8)),
		NODE(1, RELATION(':', 9)),
		NODE()
	);
	FST fstValue(text, 7,
		NODE(1, RELATION('v', 1)),
		NODE(1, RELATION('a', 2)),
		NODE(1, RELATION('l', 3)),
		NODE(1, RELATION('u', 4)),
		NODE(1, RELATION('e', 5)),
		NODE(1, RELATION(':', 6)),
		NODE()
	);
	FST fstReturn(text, 7,
		NODE(1, RELATION('r', 1)),
		NODE(1, RELATION('e', 2)),
		NODE(1, RELATION('t', 3)),
		NODE(1, RELATION('u', 4)),
		NODE(1, RELATION('r', 5)),
		NODE(1, RELATION('n', 6)),
		NODE()
	);
	FST fstPrint(text, 6,
		NODE(1, RELATION('p', 1)),
		NODE(1, RELATION('r', 2)),
		NODE(1, RELATION('i', 3)),
		NODE(1, RELATION('n', 4)),
		NODE(1, RELATION('t', 5)),
		NODE()
	);
	FST fstMain(text, 5,
		NODE(1, RELATION('m', 1)),
		NODE(1, RELATION('a', 2)),
		NODE(1, RELATION('i', 3)),
		NODE(1, RELATION('n', 4)),
		NODE()
	);
	FST fstSemicolon(text, 2,
		NODE(1, RELATION(';', 1)),
		NODE()
	);
	FST fstComma(text, 2,
		NODE(1, RELATION(',', 1)),
		NODE()
	);
	FST fstBegin(text, 6,
		NODE(1, RELATION('b', 1)),
		NODE(1, RELATION('e', 2)),
		NODE(1, RELATION('g', 3)),
		NODE(1, RELATION('i', 4)),
		NODE(1, RELATION('n', 5)),
		NODE()
	);
	FST fstEnd(text, 4,
		NODE(1, RELATION('e', 1)),
		NODE(1, RELATION('n', 2)),
		NODE(1, RELATION('d', 3)),
		NODE()
	);
	FST fstLefthesis(text, 2,
		NODE(1, RELATION('(', 1)),
		NODE()
	);
	FST fstRighthesis(text, 2,
		NODE(1, RELATION(')', 1)),
		NODE()
	);
	FST fstPlus(text, 2,
		NODE(1, RELATION('+', 1)),
		NODE()
	);
	FST fstMinus(text, 2,
		NODE(1, RELATION('-', 1)),
		NODE()
	);
	FST fstStar(text, 2,
		NODE(1, RELATION('*', 1)),
		NODE()
	);
	FST fstDiv(text, 4,
		NODE(1, RELATION('d', 1)),
		NODE(1, RELATION('i', 2)),
		NODE(1, RELATION('v', 3)),
		NODE()
	);
	FST fstMod(text, 4,
		NODE(1, RELATION('m', 1)),
		NODE(1, RELATION('o', 2)),
		NODE(1, RELATION('d', 3)),
		NODE()
	);
	FST fstLiteralOfInteger(text, 2,
		NODE(20, RELATION('0', 0), RELATION('1', 0), RELATION('2', 0), RELATION('3', 0), RELATION('4', 0), RELATION('5', 0), RELATION('6', 0),
			RELATION('7', 0), RELATION('8', 0), RELATION('9', 0),
			RELATION('0', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1), RELATION('6', 1),
			RELATION('7', 1), RELATION('8', 1), RELATION('9', 1)),
		NODE()
	);
	FST fstId(text, 2,
		NODE(124, RELATION('a', 0), RELATION('b', 0), RELATION('c', 0), RELATION('d', 0), RELATION('e', 0),
			RELATION('f', 0), RELATION('g', 0), RELATION('h', 0), RELATION('i', 0), RELATION('j', 0), RELATION('k', 0),
			RELATION('l', 0), RELATION('m', 0), RELATION('n', 0), RELATION('o', 0), RELATION('p', 0), RELATION('q', 0),
			RELATION('r', 0), RELATION('s', 0), RELATION('t', 0), RELATION('u', 0), RELATION('v', 0), RELATION('w', 0),
			RELATION('x', 0), RELATION('y', 0), RELATION('z', 0),
			RELATION('A', 0), RELATION('B', 0), RELATION('C', 0), RELATION('D', 0), RELATION('E', 0),
			RELATION('F', 0), RELATION('G', 0), RELATION('H', 0), RELATION('I', 0), RELATION('J', 0), RELATION('K', 0),
			RELATION('L', 0), RELATION('M', 0), RELATION('N', 0), RELATION('O', 0), RELATION('P', 0), RELATION('Q', 0),
			RELATION('R', 0), RELATION('S', 0), RELATION('T', 0), RELATION('U', 0), RELATION('V', 0), RELATION('W', 0),
			RELATION('X', 0), RELATION('Y', 0), RELATION('Z', 0),
			RELATION('0', 0), RELATION('1', 0), RELATION('2', 0), RELATION('3', 0), RELATION('4', 0),
			RELATION('5', 0), RELATION('6', 0), RELATION('7', 0), RELATION('8', 0), RELATION('9', 0),
			RELATION('0', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1),
			RELATION('5', 1), RELATION('6', 1), RELATION('7', 1), RELATION('8', 1), RELATION('9', 1),
			RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION('e', 1),
			RELATION('f', 1), RELATION('g', 1), RELATION('h', 1), RELATION('i', 1), RELATION('j', 1), RELATION('k', 1),
			RELATION('l', 1), RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1), RELATION('q', 1),
			RELATION('r', 1), RELATION('s', 1), RELATION('t', 1), RELATION('u', 1), RELATION('v', 1), RELATION('w', 1),
			RELATION('x', 1), RELATION('y', 1), RELATION('z', 1),
			RELATION('A', 1), RELATION('B', 1), RELATION('C', 1), RELATION('D', 1), RELATION('E', 1),
			RELATION('F', 1), RELATION('G', 1), RELATION('H', 1), RELATION('I', 1), RELATION('J', 1), RELATION('K', 1),
			RELATION('L', 1), RELATION('M', 1), RELATION('N', 1), RELATION('O', 1), RELATION('P', 1), RELATION('Q', 1),
			RELATION('R', 1), RELATION('S', 1), RELATION('T', 1), RELATION('U', 1), RELATION('V', 1), RELATION('W', 1),
			RELATION('X', 1), RELATION('Y', 1), RELATION('Z', 1)

		),
		NODE()
	);
	FST fstEqual(text, 2,
		NODE(1, RELATION('=', 1)),
		NODE());

	FST fstLiteralOfString(text, 3,
		NODE(1, RELATION('"', 1)),
		NODE(142,
			RELATION('"', 2), RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION(':', 1),
			RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1), RELATION('i', 1), RELATION('j', 1),
			RELATION('k', 1), RELATION('l', 1), RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
			RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1), RELATION('u', 1), RELATION('v', 1),
			RELATION('w', 1), RELATION('x', 1), RELATION('y', 1), RELATION('z', 1), RELATION('0', 1), RELATION('1', 1),
			RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1), RELATION('6', 1), RELATION('7', 1),
			RELATION('8', 1), RELATION('9', 1), RELATION('а', 1), RELATION('б', 1), RELATION('в', 1), RELATION('г', 1),
			RELATION('д', 1), RELATION('е', 1), RELATION('ё', 1), RELATION('ж', 1), RELATION('з', 1), RELATION('и', 1),
			RELATION('й', 1), RELATION('к', 1), RELATION('л', 1), RELATION('м', 1), RELATION('н', 1), RELATION('о', 1),
			RELATION('п', 1), RELATION('р', 1), RELATION('с', 1), RELATION('т', 1), RELATION('у', 1), RELATION('ф', 1),
			RELATION('х', 1), RELATION('ц', 1), RELATION('ч', 1), RELATION('ш', 1), RELATION('щ', 1), RELATION('ъ', 1),
			RELATION('ы', 1), RELATION('ь', 1), RELATION('э', 1), RELATION('ю', 1), RELATION('я', 1), RELATION(' ', 1),
			RELATION('A', 1), RELATION('B', 1), RELATION('C', 1), RELATION('D', 1), RELATION('E', 1), RELATION('!', 1),
			RELATION('F', 1), RELATION('G', 1), RELATION('H', 1), RELATION('I', 1), RELATION('J', 1), RELATION('K', 1),
			RELATION('L', 1), RELATION('M', 1), RELATION('N', 1), RELATION('O', 1), RELATION('P', 1), RELATION('Q', 1),
			RELATION('R', 1), RELATION('S', 1), RELATION('T', 1), RELATION('U', 1), RELATION('V', 1), RELATION('W', 1),
			RELATION('X', 1), RELATION('Y', 1), RELATION('Z', 1), RELATION('\\', 1), RELATION('А', 1), RELATION('Б', 1),
			RELATION('В', 1), RELATION('Г', 1), RELATION('Д', 1), RELATION('Е', 1), RELATION('Ё', 1), RELATION('Ж', 1),
			RELATION('З', 1), RELATION('И', 1), RELATION('Й', 1), RELATION('К', 1), RELATION('Л', 1), RELATION('М', 1),
			RELATION('Н', 1), RELATION('О', 1), RELATION('П', 1), RELATION('Р', 1), RELATION('С', 1), RELATION('Т', 1),
			RELATION('У', 1), RELATION('Ф', 1), RELATION('Х', 1), RELATION('Ц', 1), RELATION('Ч', 1), RELATION('Ш', 1),
			RELATION('Щ', 1), RELATION('Ъ', 1), RELATION('Ы', 1), RELATION('Ь', 1), RELATION('Э', 1), RELATION('Ю', 1),
			RELATION('Я', 1), RELATION(',', 1), RELATION('[', 1), RELATION(']', 1), RELATION('(', 1), RELATION(')', 1),
			RELATION('=', 1), RELATION('-', 1), RELATION('+', 1), RELATION('*', 1)

			),
		NODE());
	FST fstLeftSquareBracket(text, 2,
		NODE(1, RELATION('[', 1)),
		NODE());
	FST fstRightSquareBracket(text, 2,
		NODE(1, RELATION(']', 1)),
		NODE());
	FST fstCMP(text, 4,
		NODE(1, RELATION('c', 1)),
		NODE(1, RELATION('m', 2)),
		NODE(1, RELATION('p', 3)),
		NODE());
	FST fstClength(text, 8,
		NODE(1, RELATION('c', 1)),
		NODE(1, RELATION('l', 2)),
		NODE(1, RELATION('e', 3)),
		NODE(1, RELATION('n', 4)),
		NODE(1, RELATION('g', 5)),
		NODE(1, RELATION('t', 6)),
		NODE(1, RELATION('h', 7)),
		NODE());
	FST fstIf(text, 3,
		NODE(1, RELATION('i', 1)),
		NODE(1, RELATION('f', 2)),
		NODE());
	FST fstThen(text, 5,
		NODE(1, RELATION('t', 1)),
		NODE(1, RELATION('h', 2)),
		NODE(1, RELATION('e', 3)),
		NODE(1, RELATION('n', 4)),
		NODE()
	);
	FST fstElse(text, 5,
		NODE(1, RELATION('e', 1)),
		NODE(1, RELATION('l', 2)),
		NODE(1, RELATION('s', 3)),
		NODE(1, RELATION('e', 4)),
		NODE()
	);
	FST fstMore(text, 2,
		NODE(1, RELATION('>', 1)),
		NODE()
	);
	FST fstLess(text, 2,
		NODE(1, RELATION('<', 1)),
		NODE()
	);

	FST fstBoolean(text, 8,
		NODE(1, RELATION('b', 1)),
		NODE(1, RELATION('o', 2)),
		NODE(1, RELATION('o', 3)),
		NODE(1, RELATION('l', 4)),
		NODE(1, RELATION('e', 5)),
		NODE(1, RELATION('a', 6)),
		NODE(1, RELATION('n', 7)),
		NODE());

	FST fstTrue(text, 5,
		NODE(1, RELATION('t', 1)),
		NODE(1, RELATION('r', 2)),
		NODE(1, RELATION('u', 3)),
		NODE(1, RELATION('e', 4)),
		NODE()
	);
	FST fstFalse(text, 6,
		NODE(1, RELATION('f', 1)),
		NODE(1, RELATION('a', 2)),
		NODE(1, RELATION('l', 3)),
		NODE(1, RELATION('s', 4)),
		NODE(1, RELATION('e', 5)),
		NODE()
	);
	FST fstLiteralOfInteger16(text, 3,
		NODE(16, RELATION('0', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1), RELATION('6', 1),
			RELATION('7', 1), RELATION('8', 1), RELATION('9', 1), RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION('e', 1),
			RELATION('f', 1)),
		NODE(17, RELATION('0', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1), RELATION('6', 1),
			RELATION('7', 1), RELATION('8', 1), RELATION('9', 1), RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION('e', 1),
			RELATION('f', 1), RELATION('h', 2)),
		NODE()
	);
	FST fstLiteralOfInteger8(text, 3,
		NODE(8, RELATION('0', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1), RELATION('6', 1),
			RELATION('7', 1)),
		NODE(9, RELATION('0', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1), RELATION('6', 1),
			RELATION('7', 1), RELATION('o', 2)),
		NODE()
	);
	FST fstLiteralOfInteger2(text, 3,
		NODE(2, RELATION('0', 1), RELATION('1', 1)),
		NODE(3, RELATION('0', 1), RELATION('1', 1), RELATION('b', 2)),
		NODE()
	);


	if (execute(fstMore))
	{
		Add(lexTable, Entry(LEX_MORE, numberString, UNDEF_NUMBER, OPERATION_MORE));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;

	}
	if (execute(fstLess))
	{
		Add(lexTable, Entry(LEX_LESS, numberString, UNDEF_NUMBER, OPERATION_LESS));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstInteger))
	{
		Add(lexTable, Entry(LEX_INTEGER, numberString));
		typeData = 1;
		return;

	}
	if (execute(fstString))
	{
		Add(lexTable, Entry(LEX_STRING, numberString));
		typeData = 2;
		return;
	}
	if (execute(fstBoolean))
	{
		Add(lexTable, Entry(LEX_BOOLEAN, numberString));
		typeData = 3;
		return;
	}
	if (execute(fstFunction))
	{
		Add(lexTable, Entry(LEX_FUNCTION, numberString));
		typeData = UNDEF_NUMBER;
		typeId = 2;

		return;
	}
	if (execute(fstProcedure))
	{
		Add(lexTable, Entry(LEX_PROCEDURE, numberString));
		typeId = 5;
		return;
	}
	if (execute(fstValue))
	{
		Add(lexTable, Entry(LEX_VALUE, numberString));
		typeId = 1;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstLeftSquareBracket))
	{
		Add(lexTable, Entry(LEX_LEFT_SQUARE_BRACKET, numberString));
		if (typeId == 2) typeId = 3;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstRightSquareBracket))
	{
		Add(lexTable, Entry(LEX_RIGHT_SQUARE_BRACKET, numberString));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;

		return;
	}
	if (execute(fstComma))
	{
		Add(lexTable, Entry(LEX_COMMA, numberString));
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstBegin))
	{
		Add(lexTable, Entry(LEX_BEGIN, numberString, UNDEF_NUMBER, UNDEF_SYMBOL, currentView));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstEnd))
	{
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		counterEnd++;
		Add(lexTable, Entry(LEX_END, numberString, UNDEF_NUMBER, UNDEF_SYMBOL, currentView));
		// удаление области видимости
		if (stackVisibilityToken.top() == VIEW_THEN || stackVisibilityToken.top() == VIEW_ELSE)// удаление области видимости для then/else
		{
			int counterSpace = 0;
			int indLastSpace = 0;
			for (int i = currentView.length() - 1; i >= 0; i--)
			{
				if (currentView[i] == '_')
				{
					counterSpace++;
					if (counterSpace == 3)
					{
						indLastSpace = i;
						break;
					}
				}

			}
			string tmp;
			for (int i = 0; i < indLastSpace + 1; i++)
			{
				tmp += currentView[i];
			}
			currentView.clear();
			currentView = tmp;
			tmp.clear();
		}
		else
		{
			int counterSpace = 0;
			int indLastSpace = 0;
			for (int i = currentView.length() - 1; i >= 0; i--)
			{
				if (currentView[i] == '_')
				{
					counterSpace++;
					if (counterSpace == 2)
					{
						indLastSpace = i;
						break;
					}
				}

			}
			string tmp;
			for (int i = 0; i < indLastSpace + 1; i++)
			{
				tmp += currentView[i];
			}
			currentView.clear();
			currentView = tmp;
			tmp.clear();
		}
		stackVisibilityToken.pop();
		return;
	}

	if (execute(fstDiv))
	{
		Add(lexTable, Entry(LEX_DIV, numberString, UNDEF_NUMBER, OPERATION_DIV));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstMod))
	{
		Add(lexTable, Entry(LEX_MOD, numberString, UNDEF_NUMBER, OPERATION_MOD));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstEqual))
	{
		Add(lexTable, Entry(LEX_EQUAL, numberString, UNDEF_NUMBER, UNDEF_SYMBOL, currentView));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstLefthesis))
	{
		Add(lexTable, Entry(LEX_LEFTHESIS, numberString, UNDEF_NUMBER, LEX_LEFTHESIS));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstRighthesis))
	{
		Add(lexTable, Entry(LEX_RIGHTHESIS, numberString, UNDEF_NUMBER, LEX_RIGHTHESIS));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstMain))
	{
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		Add(lexTable, Entry(LEX_MAIN, numberString));
		counterMain++;
		stackVisibilityToken.push(VIEW_MAIN);
		currentView = currentView + VIEW_MAIN + '_';
		return;
	}
	if (execute(fstMinus))
	{
		Add(lexTable, Entry(LEX_MINUS, numberString, UNDEF_NUMBER, OPERATION_MINUS));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstPlus))
	{
		Add(lexTable, Entry(LEX_PLUS, numberString, UNDEF_NUMBER, OPERATION_PLUS));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstStar))
	{
		Add(lexTable, Entry(LEX_STAR, numberString, UNDEF_NUMBER, OPERATION_MULTIPLICATION));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstPrint))
	{
		Add(lexTable, Entry(LEX_PRINT, numberString));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstCMP))
	{
		Add(lexTable, Entry(LEX_BASE_LIBRARY_CMP, numberString));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstClength))
	{
		Add(lexTable, Entry(LEX_BASE_LIBRARY_CLENGTH, numberString));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstReturn))
	{
		Add(lexTable, Entry(LEX_RETURN, numberString));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstSemicolon))
	{
		Add(lexTable, Entry(LEX_SEMICOLON, numberString));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstIf))
	{
		Add(lexTable, Entry(LEX_IF, numberString, UNDEF_NUMBER, UNDEF_SYMBOL, currentView));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstThen))
	{
		char buf[5];
		_itoa_s(numberThen, buf, 10);
		currentView = currentView + VIEW_THEN + '_' + buf + '_';
		stackVisibilityToken.push(VIEW_THEN);
		numberThen++;
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		Add(lexTable, Entry(LEX_THEN, numberString, UNDEF_NUMBER, UNDEF_SYMBOL, currentView));
		return;
	}
	if (execute(fstElse))
	{
		char buf[5];
		_itoa_s(numberElse, buf, 10);
		currentView = currentView + VIEW_ELSE + '_' + buf + '_';
		stackVisibilityToken.push(VIEW_ELSE);
		numberElse++;
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		Add(lexTable, Entry(LEX_ELSE, numberString, UNDEF_NUMBER, UNDEF_SYMBOL, currentView));
		return;
	}
	if (execute(fstTrue))
	{
		char buf[21];
		_itoa_s(++numberCurrentLiteral, buf, 10);
		IT::Entry::uValue value;
		value.vint = 1;
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, buf, IT::IDDATATYPE::BOOL, IT::IDTYPE::Lexema, value, currentView));
		Add(lexTable, Entry(LEX_TRUE, numberString, numberInIdTable));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}
	if (execute(fstFalse))
	{
		char buf[21];
		_itoa_s(++numberCurrentLiteral, buf, 10);
		IT::Entry::uValue value;
		value.vint = 0;
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, buf, IT::IDDATATYPE::BOOL, IT::IDTYPE::Lexema, value, currentView));
		Add(lexTable, Entry(LEX_TRUE, numberString, numberInIdTable));
		typeId = UNDEF_NUMBER;
		typeData = UNDEF_NUMBER;
		return;
	}

	if (execute(fstLiteralOfString))
	{
		numberCurrentLiteral++;
		char name[20];
		_itoa_s(numberCurrentLiteral, name, 10);
		if (currentLexem.length() > 256) throw ERROR_THROW_IN(9, numberString, poz);
		IT::Entry::uValue value;
		int ind = 0;

		currentLexem = StringReplacer(currentLexem, "\\n", "#");
		const char* text2 = currentLexem.c_str();
		for (int i = 1; i < strlen(text2) - 1; i++)
		{
			value.vstr->str[ind++] = text2[i];
		}
		value.vstr->str[ind] = '\0';
		value.vstr->len = strlen(text2) - 2;
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, name, IT::IDDATATYPE::STR, IT::IDTYPE::Lexema, value, currentView));
		Add(lexTable, Entry(LEX_LITERAL, numberString, numberInIdTable));
		return;

	}
	if (execute(fstLiteralOfInteger))
	{
		numberCurrentLiteral++;
		char name[20];
		_itoa_s(numberCurrentLiteral, name, 10);
		IT::Entry::uValue value;
		value.vint = atoi(text);
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, name, IT::IDDATATYPE::INT, IT::IDTYPE::Lexema, value, currentView));
		Add(lexTable, Entry(LEX_LITERAL, numberString, numberInIdTable));
		return;
	}
	if (execute(fstLiteralOfInteger2))
	{
		numberCurrentLiteral++;
		char name[20];
		_itoa_s(numberCurrentLiteral, name, 10);
		IT::Entry::uValue value;
		int itog = 0;
		int _razr = 0;
		//конвертирование числа в 10 систему счисления
		for (int i = strlen(text) - 2; i >= 0; i--)
		{
			if (text[i] == '1')
			{
				itog += pow(2, _razr);
			}
			_razr++;
		}
		_razr = 0;
		//конец
		value.vint = itog;
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, name, IT::IDDATATYPE::INT, IT::IDTYPE::Lexema, value, currentView, BINARY));
		Add(lexTable, Entry(LEX_LITERAL, numberString, numberInIdTable));
		_razr = 0;
		itog = 0;
		return;
	}
	if (execute(fstLiteralOfInteger8))
	{
		numberCurrentLiteral++;
		char name[20];
		_itoa_s(numberCurrentLiteral, name, 10);
		IT::Entry::uValue value;
		int itog = 0;
		int _razr = 0;
		//конвертирование числа в 10 систему счисления
		for (int i = strlen(text) - 2; i >= 0; i--)
		{
			if (text[i] == '1')
			{
				itog += pow(8, _razr);
			}
			if (text[i] == '2')
			{
				itog = itog + 2 * pow(8, _razr);
			}
			if (text[i] == '3')
			{
				itog = itog + 3 * pow(8, _razr);
			}
			if (text[i] == '4')
			{
				itog = itog + 4 * pow(8, _razr);
			}
			if (text[i] == '5')
			{
				itog = itog + 5 * pow(8, _razr);
			}
			if (text[i] == '6')
			{
				itog = itog + 6 * pow(8, _razr);
			}
			if (text[i] == '7')
			{
				itog = itog + 7 * pow(8, _razr);
			}
			_razr++;
		}
		_razr = 0;
		//конец
		value.vint = itog;
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, name, IT::IDDATATYPE::INT, IT::IDTYPE::Lexema, value, currentView, OCT));
		Add(lexTable, Entry(LEX_LITERAL, numberString, numberInIdTable));
		_razr = 0;
		itog = 0;
		return;
	}
	if (execute(fstLiteralOfInteger16))
	{
		numberCurrentLiteral++;
		char name[20];
		_itoa_s(numberCurrentLiteral, name, 10);
		IT::Entry::uValue value;
		int itog = 0;
		int _razr = 0;
		//конвертирование числа в 10 систему счисления
		for (int i = strlen(text) - 2; i >= 0; i--)
		{
			if (text[i] == '1')
			{
				itog += pow(16, _razr);
			}
			if (text[i] == '2')
			{
				itog = itog + 2 * pow(16, _razr);
			}
			if (text[i] == '3')
			{
				itog = itog + 3 * pow(16, _razr);
			}
			if (text[i] == '4')
			{
				itog = itog + 4 * pow(16, _razr);
			}
			if (text[i] == '5')
			{
				itog = itog + 5 * pow(16, _razr);
			}
			if (text[i] == '6')
			{
				itog = itog + 6 * pow(16, _razr);
			}
			if (text[i] == '7')
			{
				itog = itog + 7 * pow(16, _razr);
			}
			if (text[i] == '8')
			{
				itog = itog + 8 * pow(16, _razr);
			}
			if (text[i] == '9')
			{
				itog = itog + 9 * pow(16, _razr);
			}
			if (text[i] == 'a')
			{
				itog = itog + 10 * pow(16, _razr);
			}
			if (text[i] == 'b')
			{
				itog = itog + 11 * pow(16, _razr);
			}
			if (text[i] == 'c')
			{
				itog = itog + 12 * pow(16, _razr);
			}
			if (text[i] == 'd')
			{
				itog = itog + 13 * pow(16, _razr);
			}
			if (text[i] == 'e')
			{
				itog = itog + 14 * pow(16, _razr);
			}
			if (text[i] == 'f')
			{
				itog = itog + 15 * pow(16, _razr);
			}
			_razr++;
		}
		_razr = 0;
		//конец
		value.vint = itog;
		int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, name, IT::IDDATATYPE::INT, IT::IDTYPE::Lexema, value, currentView, HEX));
		Add(lexTable, Entry(LEX_LITERAL, numberString, numberInIdTable));
		_razr = 0;
		itog = 0;
		return;
	}
	if (execute(fstId))
	{
		//если длина лексемы больше 20 то применяем усечение до 20 символов и создаем предупреждение
		if (currentLexem.length() > 20)
		{
			currentLexem = currentLexem.substr(0, 20);
			text = (char*)currentLexem.c_str();
			WARNING_IND_LARGE(numberString, text);
		}
		IT::Entry::uValue value;
		value.vint = UNDEF_NUMBER;
		if (typeId == 2 || typeId == 5)
		{
			int numberInIdTable = isInIdTable(*idTable, IT::Entry(numberString, text, (IT::IDDATATYPE)typeData, (IT::IDTYPE) typeId, value, currentView));
			if (numberInIdTable == UNDEF_NUMBER)
			{
				numberInIdTable = IT::Add(idTable, IT::Entry(numberString, text, (IT::IDDATATYPE)typeData, (IT::IDTYPE) typeId, value, currentView));
				Add(lexTable, Entry(LEX_ID, numberString, numberInIdTable));
			}
			else
			{

				throw ERROR_THROW_SEMANTIC(702, numberString, string(text))

			}

			currentView = currentView + text + '_';
			stackVisibilityToken.push(text);
		}
		else
		{
			if (typeData != UNDEF_NUMBER && typeId != UNDEF_NUMBER)
			{
				//searchCopy ((если совпадает области видимости и идентификатор, выдать ошибку
				if (IT::searchCopy(*idTable, IT::Entry(numberString, text, (IT::IDDATATYPE)typeData, (IT::IDTYPE) typeId, value, currentView)))
					throw  ERROR_THROW_SEMANTIC(704, numberString, string(text));
				int numberInIdTable = IT::Add(idTable, IT::Entry(numberString, text, (IT::IDDATATYPE)typeData, (IT::IDTYPE) typeId, value, currentView));
				Add(lexTable, Entry(LEX_ID, numberString, numberInIdTable));
			}
			else
			{
				int numberInIdTable = isInIdTable(*idTable, IT::Entry(numberString, text, (IT::IDDATATYPE)typeData, (IT::IDTYPE) typeId, value, currentView));// НУЖНО ФИКСИТЬ ФУНКЦИЮ ЧТОБЫ ССЫЛКИ НОРМАЛЬНО ДЕЛАЛА ИНАЧЕ ЖОПА ПОЛНАЯ МНЕ

				if (numberInIdTable == UNDEF_NUMBER)
				{
					throw ERROR_THROW_SEMANTIC(700, numberString, string(text));
				}
				Add(lexTable, Entry(LEX_ID, numberString, numberInIdTable));
			}

		}

		return;
	}
	
	cout <<endl<< text<<"--->";
	throw ERROR_THROW_IN(9, numberString, poz);

}

Tables createTables(In::IN newIN) //создаем таблицу лексем
{
	currentView = "GLOBAL_";
	stackVisibilityToken.push(currentView);
	Tables newTables;
	LexTable* mainTable = Create(newIN.counterLexem);
	IT::IdTable* idMainTable = IT::Create(newIN.counterLexem);
	string currentLexem;
	int numberString = 1;
	int numberPosition = 1;
	bool quote = false;
	for (int i = 0; i < newIN.size; i++)
	{
		if (newIN.textFormated[i] == '"')
		{
			if (quote) quote = false;
			else quote = true;
			currentLexem += '"';
		}
		else
			if (newIN.textFormated[i] == ' ' && !quote)
			{
				if (!currentLexem.empty())
				{
					initTypeLexem(currentLexem, numberString, numberPosition, mainTable, idMainTable);
					currentLexem.clear();
				}
			}
			else
			{
				if (newIN.textFormated[i] == '\n')
				{
					numberString++;
					numberPosition = 1;
				}
				else currentLexem += newIN.textFormated[i];
			}
		numberPosition++;
	}
	if (!currentLexem.empty())
	{
		throw ERROR_THROW_IN(9, numberString, numberPosition);
	}
	if (counterMain > 1)
	{
		throw ERROR_THROW(6)
	}
	if (counterMain < 1)
	{
		throw ERROR_THROW(8)
	}
	mainTable->countEnd = counterEnd;
	newTables.LEXTABLE = mainTable;
	newTables.IDTABLE = idMainTable;
	return newTables;
}

std::string StringReplacer(const std::string& inputStr, const std::string& src, const std::string& dst)
{
	std::string result(inputStr);

	size_t pos = result.find(src);
	while (pos != std::string::npos) {
		result.replace(pos, src.size(), dst);
		pos = result.find(src, pos);
	}

	return result;
}
