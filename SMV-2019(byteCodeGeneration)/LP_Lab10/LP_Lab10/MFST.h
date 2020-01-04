#pragma once
#include "pch.h"
#include "CreatingTables.h"
#include "Greibah.h"
#include "Parm.h"
#include "Log.h"
#define MFST_DIAGN_NUMBER 3

#define MFST_DIAGN_MAXSIZE (2*ERROR_MAXSIZE_MESSAGE)

using namespace std;
typedef stack<short> MFSTSTACK;
static int currentLevel;
namespace MFST
{
	struct MfstState //��������� ��������(��� ����������)
	{
		short lenta_position;//������� �� �����
		short nrule; //����� ������� 
		short nrulechain; //����� ������� �������, �������� �������
		MFSTSTACK st; //���� ��������
		MfstState();
		MfstState(short pposition, MFSTSTACK pst, short pnrulechain); //����� ������� �������, �������� �������
		MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain);
	};
	//���������� �������
	struct Mfst
	{
		enum RC_STEP//��� �������� ������� step
		{
			NS_OK, //������� ������� � �������, ������� �������� � ����
			NS_NORULE,  //�� ������� ������� ����������(������ � ����������)
			NS_NORULECHAIN,//�� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR,//����������� �������������� ������ ����������
			TS_OK,//���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK,//���. ������ ����� != ������� �����, ������������� ���������
			LENTA_END,//������� ������� ����� >= lenta_size
			SURPRISE //����������� ��� �������� (������ � step)
		};
		struct MfstDiagnosis
		{
			short lenta_position;//������� �� �����
			RC_STEP rc_step;//��� ���������� ����
			short nrule; //����� �������
			short nrule_chain; //����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position,//������� �� ����� 
				RC_STEP prc_step,//��� ��������� ����
				short pnrule, // ����� �������
				short pnrule_chain //����� ������� �������
			);
		}diagnosis[MFST_DIAGN_NUMBER]; //��������� ����� �������� ���������
		GRABLPHABET* lenta;// ���������������� (TS/NS) ����� (�� LEX)
		short lenta_position;//������� ������� � �����
		short nrule;//����� �������� �������
		short nrulechain;//����� ������� �������, �������� �������
		short lenta_size;//������ �����
		GRB::Greibah grebach; //���������� �������
		Tables lex; //��������� ������ ������������ �����������
		MFSTSTACK st; //���� ��������
		stack<MfstState> storestate; //���� ��� ���������� ���������
		stack<MfstState> TREE; //���� ��� ���������� ��������� ��������� �������
		
		Mfst() { lenta = 0; lenta_size = lenta_position = 0; };
		Mfst(Tables plex, GRB::Greibah pgrebach);
		char* getCSt(char* buf);// �������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);//�����: n �������� � pos
		char* getDiagnosis(short n, char* buf); //�������� n-�� ������ ����������� ��� 0�00
		bool savestate(); //��������� ��������� ��������
		bool reststate(); //������������ ��������� ��������
		bool push_chain(GRB::Rule::Chain chain); //��������� ������� ������� � ����
		RC_STEP step(); //��������� ��� ��������
		void start(Parm::PARM parms, Log::LOG); //��������� �������
		fstream fout;

		bool savediagnosis(RC_STEP pprc_step//��� ���������� ����
		);
		void printrules(); //������� ������������������ ������
		struct Deducation //�����
		{
			short size; //���������� ����� � ������
			short* nrules; //������ ������ ����������
			short* nrulechains;//������ ������� ������ ���������� (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation(); //��������� ������ ������
		

	};
	
}