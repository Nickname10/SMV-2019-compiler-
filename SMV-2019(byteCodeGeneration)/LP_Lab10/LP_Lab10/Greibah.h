#pragma once
#include "pch.h"

typedef short GRABLPHABET;// ������� �������� ���������� ��������� > 0, ����������� < 0
namespace GRB
{
	struct Rule //������� � ��������� �������
	{
		
		GRABLPHABET nn; //���������� (����� ������ �������) < 0 
		int iderror; //������������� ���������������� ���������
		short size; //���������� ������� - ������ ������ �������
		struct Chain
		{

			short size;//����� �������
			GRABLPHABET* nt;//������� ���������� (>0) � ������������ (<0)
			Chain() { size = 0; nt = 0; };
			Chain(short psize, GRABLPHABET s, ...);
			char* getChain(char*b); //�������� ������ ������� ���������
			static GRABLPHABET T(char t)//��������
			{
				return  GRABLPHABET(t);
			};
			static  GRABLPHABET N(char n)//����������
			{
				return (-GRABLPHABET(n));
			}
			static bool isT(GRABLPHABET s)//��������
			{
				return (s > 0);
			}
			static bool isN(GRABLPHABET s)// ����������
			{
				return (s < 0);
			}
			static char alphabet_to_char(GRABLPHABET s) //GRBALPHABET-> char
			{
				return (isT(s) ? char(s) : char(-s));
			}
		}*chains;//������ ������� - ������ ������ �������
		Rule() { nn = 0x00; size = 0;}
		Rule(GRABLPHABET pnn, int piderror, short psize, Chain c, ...);
		char* getCRule(char* b, short nchain); //�������� ������� � ���� N->������� (��� ����������)
		short GetNextChain(GRABLPHABET t, Rule::Chain* pchain, short j);// �������� ��������� �� j ���������� �������, ������� �� ����� ��� -1
		bool checkEmptyRule(Rule::Chain chain);//��������� ���� �� ������ �������
		
	};
	struct Greibah//���������� �������
	{
		short size;//���������� ������
		GRABLPHABET startN; //��������� ������
		GRABLPHABET stbottomT; //��� �����
		Rule* rules; //��������� ������
		Greibah()
		{
			short size = 0;
			startN = 0;
			stbottomT = 0; 
			rules = 0;
		}
		Greibah(GRABLPHABET pstartN, GRABLPHABET pstbottomT, short psize, Rule r, ...);
		short getRule(GRABLPHABET pnn, Rule& prule);//�������� �������, ������������ ����� ������� ��� -1
		Rule getRule(short n); //�������� ������� �� ������
	};
	Greibah getGreibach(); //�������� ����������
}