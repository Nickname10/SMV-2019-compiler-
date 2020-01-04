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
	/* �������� ������� ������ � ���� ����������� 4��� �������+ 6 ��� + 36 ��� ��������� (4 ��� ������� + 32 ��� �������� )*/
	OP_PUSH = 0x1,
	/* ������� ������� �� ����� � �������� � �������� ���������� 6 ��� +36 ���*/
	OP_POP = 0x2,
	/* ������� ��� �������� �� ������� ����� ��������� ������� � ���� 6 ��� ����������� */
	OP_ADD = 0x3,
	/* ������� ��� �������� �� ������� ����� ��������� ������� � ���� 6 ��� ��� */
	OP_SUB = 0x4,
	/* �������� ��� �������� � ������� �����, ��������� �������� � ���� 6 ��� */
	OP_MUL = 0x5,
	/* ������ ������ �������� � ������� ����� ����� ����� ������� �� ������� ����� 6 ���*/
	OP_DIV = 0x6,
	/* ����� ������� �� ������� �� ��������� � ������� �����, ��������� ������� �� ������� ����� 6 ���*/
	OP_MOD = 0x7,
	/* �������� ��������� > ��� �������� � ������� �����, ������� �������, ���� ����� ������� ������ */
	OP_CMP_MORE = 0x8,
	/* �������� ��������� < ��� �������� � ������� �����, ������� �������, ���� ������ ������� ������*/
	OP_CMP_LESS = 0x9,
	/*������� �� ����� ������� ���������  6 ��� ��������, 36 ��� ������� (4 ���� ������� 32 ���) */
	OP_WRITE = 0x10,
	/* �������� ��������, �������� �� ������� ����� ������������ � �����
	���� ���� �� �� ����� ���, ���� ����� ���������, �� �� ����� ������
	��������������� ����� ������� if ����������� �������� � ������ ������
	����� ��� �������� ����� ������� ��� � ����������� ������� ���
	*/
	OP_IF = 0xa,
	/* ������� ����� ������� ������� 8���*/
	OP_CREATE_MARK_FALSE = 0xb,
	/*������� ����� ����� ������� �������*/
	OP_CREATE_MARK_AFTER_FALSE = 0xf,
	/*����� �������*/
	OP_CALL = 0xc,
	//������� ���������� �������, ������� � ������� ����� ������������
	OP_RET = 0xd,
	/*�������� � ���� ����� �������� ������*/
	OP_PUSH_OFFSET = 0xe,
	/*������� ����� �������� � ����� else*/
	OP_GO_AFTER_ELSE = 0x11,
	/*����� � ���� ����� ������ ������*/
	OP_PUSH_LENGTH_STRING = 0x12,
	/*���������� ��� ������ �� ������� �����, ��� ������ ���������, ������� �� ������� ����� ��������� �������� 1 - ���� ����� 0 ���� �� �����*/
	OP_CMP_STR = 0x13,
	/*�������� ��������� ��� �������, �� � ��������� ������������*/
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