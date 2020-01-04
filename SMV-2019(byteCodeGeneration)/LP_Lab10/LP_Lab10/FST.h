#pragma once
#include "pch.h"
namespace fst {

	struct RELATION //�����:������->������� ����� ��������� ��
	{
		char symbol;  //������ ��������
		short nnode; // ����� ��������� 
		RELATION(
			char c = 0x00,
			short ns = NULL);
	};


	struct NODE // ������� ����� ��������
	{
		short n_relation;	//������� ����� �������� 
		RELATION *relations;	//����������� ����� 
		NODE();
		NODE(
			short n,	//���������� ����������� ����� 
			RELATION rel, ...	//������ �����
		);
	};




	struct FST	//������������������� �������� �������
	{
		const char* string;	//�������(������, ����������� 0�00)
		short position;		// ������� ������� � ������� 
		short nstates;		// ���������� �������� �������� 
		NODE* nodes;	// ���� ���������: [0] - ��������� ���������, [nstate - 1] - ��������
		short* rstates;		// ��������� ��������� �������� �� ������ ������� 
		FST(
			const char* s,	// �������(������, ����������� 0�00)
			short ns,	// ���������� ��������� �������� 
			NODE n, ...		// ������ ��������� (���� ���������)
		);
	};



	bool step(FST& fst, short*& rstates);

	bool execute(FST& fst);
};

