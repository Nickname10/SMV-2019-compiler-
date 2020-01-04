#pragma once
#include "pch.h"
#include "MFST.h"
#include "Greibah.h"

#define GRB_ERROR_SERIES 600
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)


#define MFST_TRACE_START fout<<std::setw(4)<<std::left<<"Шаг"<<":"\
						<<std::setw(20)<<std::left<<" Правило" \
						<<std::setw(30)<<std::left<<" Входная лента"\
						<<std::setw(20)<<std::left<<" Стек"\
						<<std::endl;

#define ISNS(n) GRB::Rule::Chain::isN(n)
#define MFST_TRACE1 fout<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
					<<std::setw(20)<<std::left<<rule.getCRule(rbuf,nrulechain)\
					<<std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
					<<std::setw(20)<<std::left<<getCSt(sbuf)\
					<<std::endl;

#define MFST_TRACE2 fout<<std::setw(4)<<std::left<<FST_TRACE_n<<": "\
						 <<std::setw(20)<<std::left<<" "\
						 <<std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
						<<std::setw(20)<<std::left<<getCSt(sbuf)\
						<<std::endl;
#define MFST_TRACE3 fout<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
						 <<std::setw(20)<<std::left<<" "\
						 <<std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
						<<std::setw(20)<<std::left<<getCSt(sbuf)\
						<<std::endl;
#define MFST_TRACE4(c) fout<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
						 <<std::setw(20)<<std::left<<c\
						<<std::endl;
#define MFST_TRACE5(c) fout<<std::setw(4)<<std::left<<FST_TRACE_n<<": "\
						 <<std::setw(20)<<std::left<<c\
						<<std::endl;

#define MFST_TRACE6(c,k) fout<<std::setw(4)<<std::left<<FST_TRACE_n<<": "\
						 <<std::setw(20)<<std::left<<c<<k\
						<<std::endl;
#define MFST_TRACE7 fout<<std::setw(4)<<std::left<<state.lenta_position<": "\
						 <<std::setw(20)<<std::left<<rule.getCRule(rbuf,state.nrulechain)\
						<<std::endl;
static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];//печать