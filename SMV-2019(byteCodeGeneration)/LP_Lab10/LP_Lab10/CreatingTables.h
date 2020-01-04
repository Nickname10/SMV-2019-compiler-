#pragma once
#include "pch.h"
#include "LT.h"
#include "In.h"
#include "IT.h"
struct Tables {
	IT::IdTable* IDTABLE;
	LT::LexTable* LEXTABLE;
};
struct ListId
{
	IT::Entry id;
	IT::Entry* pNext = nullptr;
};
Tables createTables(In::IN newIN);
std::string StringReplacer(const std::string& inputStr, const std::string& src, const std::string& dst);

