#pragma once
#include "pch.h"
#include "Parm.h"
#define CODE_TYPE_INT 0x1
#define CODE_TYPE_STRING 0x2
#define CODE_TYPE_BOOLEAN 0x3
#define CODE_CONST_SEGMENT 0x1
#define CODE_DATA_SEGMENT 0x2
#define CODE_FUNCTION_SEGMENT 0x3
#define CODE_PARM_SEGMENT 0x4
#define CODE_MAIN_SEGMENT 0x5
#define CODE_BASE_LIBRARY_SIGMENT 0x6
#define CODE_COMMAND_SEGMENT 0x7
#define CODE_PROCEDURE_SEGMENT 0x8
#define BASE_LIBRARY_CMP_NAME 0x0
#define BASE_LIBRARY_CLENGTH_NAME 0x1
typedef enum {
	/* Положить операнд справа в стек кодирование 4бит сегмент+ 6 бит + 36 бит аргумента (4 бит сегмент + 32 бит название )*/
	OP_PUSH = 0x1,
	/* Достать операнд из стека и положить в Заданную переменную 6 бит +36 бит*/
	OP_POP = 0x2,
	/* Сложить два операнда из вершина стека результат ложится в стек 6 бит кодирование */
	OP_ADD = 0x3,
	/* Вычесть два операнда из вершины стека результат ложится в стек 6 бит код */
	OP_SUB = 0x4,
	/* Умножить два операнда с вершины стека, результат кладется в стек 6 бит */
	OP_MUL = 0x5,
	/* Делить нацело операнды с вершины стека целая часть ложится на вершину стека 6 бит*/
	OP_DIV = 0x6,
	/* Взять остаток от деления от операндов с вершины стека, результат ложится на вершину стека 6 бит*/
	OP_MOD = 0x7,
	/* Операция сравнения > Два операнда с вершины стека, ложится единица, если левый операнд больше */
	OP_CMP_MORE = 0x8,
	/* Операция сравнения < Два операнда с вершины стека, ложится единица, если правый операнд больше*/
	OP_CMP_LESS = 0x9,
	/*Вывести на экран целевой платформы  6 бит операция, 36 бит операнд (4 бита сегмент 32 бит) */
	OP_WRITE = 0x10,
	/* Условный оператор, значение на вершине стека сравнивается с нулем
	Если ноль то по метке лжи, если любое ненулевое, то по метке истины
	Непосредственно после Команды if описываются действия в случае истины
	после них ставится метка условий лжи и описываются условия лжи
	*/
	OP_IF = 0xa,
	/* Создать метку ложного условия 8Бит*/
	OP_CREATE_MARK_FALSE = 0xb,
	/*Создать метку после ложного условия*/
	OP_CREATE_MARK_AFTER_FALSE = 0xf,
	/*Вызов функции*/
	OP_CALL = 0xc,
	//Вернуть управление функции, операнд с вершины стека возвращается
	OP_RET = 0xd,
	/*Положить в стек адрес операнда справа*/
	OP_PUSH_OFFSET = 0xe,
	/*Создать метку перехода к концу else*/
	OP_GO_AFTER_ELSE = 0x11,
	/*Ложит в стек длину строки справа*/
	OP_PUSH_LENGTH_STRING = 0x12,
	/*Сравнивает две строки на вершине стека, они оттуда убираются, ложится на вершину стека результат операции 1 - если равны 0 если не равны*/
	OP_CMP_STR = 0x13,
	/*Вызывает процедуру или функцию, но её результат игнорируется*/
	OP_CALL_PROC = 0x14
} opcode;
namespace AssemblerCodeGeneration
{
	using namespace std;
	void GenerateAsmFile(Parm::PARM parm);
	string convertString2ToString10(string str);
	string Get32BitName(ifstream&);
	string GetNameSegment(ifstream&);
	string Get2BitData(ifstream&);
	string Get6BitCommand(ifstream&);
	std::string StringReplacer(const std::string& inputStr, const std::string& src, const std::string& dst);
	unsigned int convertWin1251symbTo866(unsigned char symb);
}