#pragma once
#include "pch.h"
#include "IT.h"
#include "LT.h"
#include "CreatingTables.h" 
namespace polishNotation
{
	bool PolishNotation(int lexTablePos, LT::LexTable* lexTable, IT::IdTable* idTable);
}