#include "pch.h"
#include "AssemblerCodeGeneration.h"

using namespace std;
void AssemblerCodeGeneration::GenerateAsmFile(Parm::PARM parm)
{

	ifstream fin(parm.in);
	ofstream fout(parm.out);
	fout << ".586\n\n" << ".model flat, stdcall\n" << "includelib kernel32.lib\n"
		<< "includelib libucrt.lib\n" << "ExitProcess PROTO :DWORD\n"
		<< "GetStdHandle Proto :DWORD\n" << "SetConsoleTitleA PROTO :DWORD\n"
		<< "WriteConsoleA PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD \n"
		<< "printConsole PROTO pstr: dword, _size: dword\n"
		<< "intToString PROTO number: sdword, pstr: dword \n"
		<< ".stack 4096 \n" << ".const\n" << "consoletitle db 'SMV-2019', 0\n"<<"ERRMESS db 'Error zero division',0 \n"
		<< "wordTrue db 'True' \n" << "wordFalse db'False' \n" << "emptstr db 0 \n";
	int markNumber = 0;
	string typeSegment;
	int iterator = 0;
	int currentTypeSegment = -1;
	int numberBoolZeroMark = 0;
	int numberIfZeroMARK = 0;
	bool isWordDataOuted = false;
	bool isWordCodeOuted = false;
	bool isWordMainOuted = false;
	int typeDataLastFunction = -1;
	int lastCodeSegment = 0;
	string lastFunctionName;
	string lastProcedureName;
	while (!fin.eof())
	{
		char symb;
		string typeSegment = GetNameSegment(fin);
		int iTypeSegment = atoi(typeSegment.c_str());
		bool isMainOuted = false;
		switch (iTypeSegment)
		{
		case CODE_DATA_SEGMENT:

			if (!isWordDataOuted)
			{
				fout << ".DATA \n";
				fout << "strResult byte 11 dup(?)\n";
				fout << "numberSize dword 0\n";
				isWordDataOuted = true;
			}
			{
				string name = "D";
				string number = "";
				for (int i = 0; i < 32; i++)
				{
					fin >> symb;
					number += symb;
				}
				name += convertString2ToString10(number);
				fout << name << " ";
				string type;
				for (int i = 0; i < 2; i++)
				{
					fin >> symb;
					type += symb;
				}
				int iTypeData = atoi((convertString2ToString10(type)).c_str());
				switch (iTypeData)
				{
				case CODE_TYPE_BOOLEAN:
				case CODE_TYPE_INT:
				{
					fout << "sdword 0 \n";
				}
				break;
				case CODE_TYPE_STRING:
				{
					fout << "dword 0 \n";
				}
				break;
				}

			}
			break;
		case CODE_CONST_SEGMENT:
		{
			string name = "C";
			name = name + Get32BitName(fin);
			fout << name << " ";
			int iTypeData = atoi(Get2BitData(fin).c_str());
			switch (iTypeData)
			{
			case CODE_TYPE_BOOLEAN:
			case CODE_TYPE_INT:
			{
				fout << "sdword ";
				fout << Get32BitName(fin) << " \n";
			}
			break;
			case CODE_TYPE_STRING:
			{
				fout << "db ";
				string length = "";
				length = Get32BitName(fin);
				string out;
				if (atoi(length.c_str()) != 0)
				{
					fout << length << ", ";
					string ASCII;
					for (int i = 0; i < atoi(length.c_str()); i++)
					{
						for (int j = 0; j < 8; j++)
						{
							fin >> symb;
							ASCII += symb;
						}

						unsigned int buffer = convertWin1251symbTo866(atoi(convertString2ToString10(ASCII).c_str()));
						if (buffer == 35)
						{
							char str[10];
							_itoa_s((int)buffer, str, 10);
							if (((i + 1) % 47) == 0)
							{
								out[out.length() - 2] = '\n';
								out = out + '\t' + "db " + "10" + ", ";
							}

							else  out = out + "10, ";
							ASCII.clear();
						}
						else
						{
							char str[10];
							_itoa_s((int)buffer, str, 10);
							if (((i + 1) % 47) == 0)
							{
								out[out.length() - 2] = '\n';
								out = out + "\tdb " + str + ", ";
							}
							else out = out + str + ", ";
							ASCII.clear();
						}
					}
					
					for (int i = 0; i < out.length() - 2; i++)
					{
						fout << out[i];
					}
					fout << '\n';
				}
				else fout << length << '\n';
			}
			break;
			default:
				break;
			}

		}
		break;
		case CODE_FUNCTION_SEGMENT:
		{

			if (!isWordDataOuted)
			{
				fout << ".DATA \n";
				fout << "strResult byte 11 dup(?)\n";
				fout << "numberSize dword 0\n";
				isWordDataOuted = true;
			}
			if (!isWordCodeOuted)
			{
				fout << ".code\n";
				fout << "OPERATOR_MORE PROC, PARM1: SDWORD, PARM2: SDWORD\n";
				fout << "mov eax, PARM1 \n mov ebx, PARM2 \n";
				fout << "CMP eax, ebx \n";
				fout << "jg isGreater\n";
				fout << "mov eax, 0\n";
				fout << "jmp markAfterGreater \n";
				fout << "isGreater: mov eax, 1 \n";
				fout << "markAfterGreater: \n";
				fout << "ret\n OPERATOR_MORE ENDP\n";

				fout << "OPERATOR_LESS PROC, PARM3: SDWORD, PARM4: SDWORD \n";
				fout << "mov eax, PARM3 \n mov ebx, PARM4 \n";
				fout << "CMP eax, ebx \n";
				fout << "jg isGreater2\n";
				fout << "mov eax, 1\n";
				fout << "jmp markAfterGreater2 \n";
				fout << "isGreater2: mov eax, 0 \n";
				fout << "markAfterGreater2: \n";
				fout << "ret\n OPERATOR_LESS ENDP\n";

				fout << "printConsole PROC  pstr : dword, _size : dword"
					<< "\npush eax\n"
					<< "\nINVOKE GetStdHandle, -11"
					<< "\nINVOKE WriteConsoleA, eax, pstr, _size, 0, 0\n"
					<< "\npop eax\n"
					<< "\nret \n"
					<< "\nprintConsole ENDP\n";
				fout << "intToString PROC number: sdword, pstr: dword\n"
					<< "push eax\n"
					<< "push ebx\n"
					<< "push ecx\n"
					<< "push edi\n"
					<< "push esi\n"
					<< "mov esi, offset strResult\n"
					<< "mov ecx, sizeof strResult\n"
					<< "mov al, ' '\n"
					<< "CLEARSTR:\n"
					<< "mov [esi], al\n"
					<< "inc esi\n"
					<< "LOOP CLEARSTR\n"
					<< "mov edi, pstr\n"
					<< "mov esi, 0\n"
					<< "mov eax, number\n"
					<< "cdq\n"
					<< "mov ebx, 10\n"
					<< "test eax, 80000000h\n"
					<< "idiv ebx\n"
					<< "jz plus\n"
					<< "neg eax\n"
					<< "neg edx\n"
					<< "mov cl, '-'\n"
					<< "mov[edi], cl\n"
					<< "inc edi\n"
					<< "plus :\n"
					<< "push dx\n"
					<< "inc esi\n"
					<< "test eax, eax\n"
					<< "jz fin\n"
					<< "cdq\n"
					<< "idiv ebx\n"
					<< "jmp plus\n"
					<< "fin :\n"
					<< "mov ecx, esi\n"
					<< "write :\n"
					<< "pop bx \n"
					<< "add bl, '0'\n"
					<< "mov[edi], bl\n"
					<< "inc edi\n"
					<< "loop write\n"
					<< "mov ecx, sizeof strResult\n"
					<< "mov esi, offset strResult\n"
					<< "COUNTER :\n"
					<< "mov al, [esi]\n"
					<< "cmp al, 32\n"
					<< "jz IC\n"
					<< "inc numberSize\n"
					<< "IC :\n"
					<< "inc esi\n"
					<< "LOOP COUNTER\n"
					<< "pop esi\n"
					<< "pop edi\n"
					<< "pop ecx\n"
					<< "pop ebx\n"
					<< "pop eax\n"
					<< "ret\n"
					<< "intToString ENDP\n";
				fout << "B1 PROC str1 : DWORD\n"
					<< "mov edi, str1\n"
					<< "xor eax, eax\n"
					<< "mov al, [edi]\n"
					<< "ret\n"
					<< "B1 ENDP\n";
				fout << "B0 PROC str1 : DWORD, str2 : DWORD\n"
					<< "mov edi, str1\n"
					<< "mov esi, str2\n"
					<< "xor eax, eax\n"
					<< "xor ebx, ebx\n"
					<< "mov al, [edi]\n"
					<< "mov bl, [esi]\n"
					<< "cmp al, bl\n"
					<< "jne false\n"
					<< "mov ecx, eax\n"
					<< "inc ecx\n"
					<< "CYCLE :\n"
					<< "mov al, [edi]\n"
					<< "mov bl, [esi]\n"
					<< "cmp al, bl\n"
					<< "jne false\n"
					<< "inc edi\n"
					<< "inc esi\n"
					<< "LOOP CYCLE\n"
					<< "jmp true \n"
					<< "false:\n"
					<< "xor eax, eax\n"
					<< "ret\n"
					<< "true :\n"
					<< "mov eax, 1\n"
					<< "ret\n"
					<< "B0 ENDP\n";
				isWordCodeOuted = true;
			}
			if (lastFunctionName != "")
			{
				switch (typeDataLastFunction)
				{
				case CODE_TYPE_INT:
				case CODE_TYPE_BOOLEAN:
				{
					fout << "jmp MarkAfterError\n";
					fout << "ERRORZERODIVISION:\n";
					fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
					fout << " push 0\n call ExitProcess\n";
					fout << "MarkAfterError:\n";
					fout << "xor eax, eax\n";

				}
				break;
				case CODE_TYPE_STRING:
				{
					fout << "jmp MarkAfterError\n";
					fout << "ERRORZERODIVISION:\n";
					fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
					fout << " push 0\n call ExitProcess\n";
					fout << "MarkAfterError:\n";
					fout << "mov eax, offset emptstr\n";
				}
				break;
				}
				fout << "ret \n" << lastFunctionName << " ENDP \n";
				lastFunctionName = "";
			}
			if (lastProcedureName != "")
			{
				fout << "ret\n" << lastProcedureName << " ENDP\n";
				lastProcedureName = "";
			}
			string nameFunc = "F" + Get32BitName(fin);
			lastFunctionName = nameFunc;
			typeDataLastFunction = atoi(Get2BitData(fin).c_str());
			fout << nameFunc << " PROC ";
			int countParm = atoi(Get32BitName(fin).c_str());
			for (int i = 0; i < countParm; i++)
			{
				int TypeSegment = atoi(GetNameSegment(fin).c_str());
				string nameParm = " P" + Get32BitName(fin);
				int TypeData = atoi(Get2BitData(fin).c_str());
				fout << nameParm << " :";
				switch (TypeData)
				{
				case CODE_TYPE_BOOLEAN:
				case CODE_TYPE_INT:
					fout << " SDWORD";
					break;
				case CODE_TYPE_STRING:
					fout << " DWORD";
				default:
					break;
				}
				if (i != countParm - 1) fout << ',';
			}
			fout << '\n';
		}
		break;
		case CODE_MAIN_SEGMENT:
		{

			if (!isWordDataOuted)
			{
				fout << ".DATA \n";
				fout << "strResult byte 11 dup(?)\n";
				fout << "numberSize dword 0\n";
				isWordDataOuted = true;
			}
			if (lastFunctionName != "")
			{
				switch (typeDataLastFunction)
				{
				case CODE_TYPE_INT:
				case CODE_TYPE_BOOLEAN:
				{
					fout << "jmp MarkAfterError\n";
					fout << "ERRORZERODIVISION:\n";
					fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
					fout << " push 0\n call ExitProcess\n";
					fout << "MarkAfterError:\n";
					fout << "xor eax, eax\n";
				}
				break;
				case CODE_TYPE_STRING:
				{
					fout << "jmp MarkAfterError\n";
					fout << "ERRORZERODIVISION:\n";
					fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
					fout << " push 0\n call ExitProcess\n";
					fout << "MarkAfterError:\n";
					fout << "mov eax, offset emptstr\n";
				}
				break;
				}
				fout << "ret\n" << lastFunctionName << " ENDP\n";
				lastFunctionName = "";
			}
			if (lastProcedureName != "")
			{
				fout << "ret\n" << lastProcedureName << " ENDP\n";
				lastProcedureName = "";
			}
			if (!isWordCodeOuted)
			{
				fout << ".code\n";
				fout << "OPERATOR_MORE PROC, PARM1: SDWORD, PARM2: SDWORD\n";
				fout << "mov eax, PARM1 \n mov ebx, PARM2 \n";
				fout << "CMP eax, ebx \n";
				fout << "jg isGreater\n";
				fout << "mov eax, 0\n";
				fout << "jmp markAfterGreater \n";
				fout << "isGreater: mov eax, 1 \n";
				fout << "markAfterGreater: \n";
				fout << "ret\n OPERATOR_MORE ENDP\n";

				fout << "OPERATOR_LESS PROC, PARM3: SDWORD, PARM4: SDWORD \n";
				fout << "mov eax, PARM3 \n mov ebx, PARM4 \n";
				fout << "CMP eax, ebx \n";
				fout << "jg isGreater2\n";
				fout << "mov eax, 1\n";
				fout << "jmp markAfterGreater2 \n";
				fout << "isGreater2: mov eax, 0 \n";
				fout << "markAfterGreater2: \n";
				fout << "ret\n OPERATOR_LESS ENDP\n";

				fout << "printConsole PROC  pstr : dword, _size : dword"
					<< "\npush eax\n"
					<< "\nINVOKE GetStdHandle, -11"
					<< "\nINVOKE WriteConsoleA, eax, pstr, _size, 0, 0\n"
					<< "\npop eax\n"
					<< "\nret \n"
					<< "\nprintConsole ENDP\n";
				fout << "intToString PROC number: sdword, pstr: dword\n"
					<< "push eax\n"
					<< "push ebx\n"
					<< "push ecx\n"
					<< "push edi\n"
					<< "push esi\n"
					<< "mov esi, offset strResult\n"
					<< "mov ecx, sizeof strResult\n"
					<< "mov al, ' '\n"
					<< "CLEARSTR:\n"
					<< "mov [esi], al\n"
					<< "inc esi\n"
					<< "LOOP CLEARSTR\n"
					<< "mov edi, pstr\n"
					<< "mov esi, 0\n"
					<< "mov eax, number\n"
					<< "cdq\n"
					<< "mov ebx, 10\n"
					<< "test eax, 80000000h\n"
					<< "idiv ebx\n"	
					<< "jz plus\n"
					<< "neg eax\n"
					<< "neg edx\n"
					<< "mov cl, '-'\n"
					<< "mov[edi], cl\n"
					<< "inc edi\n"
					<< "plus :\n"
					<< "push dx\n"
					<< "inc esi\n"
					<< "test eax, eax\n"
					<< "jz fin\n"
					<< "cdq\n"
					<< "idiv ebx\n"
					<< "jmp plus\n"
					<< "fin :\n"
					<< "mov ecx, esi\n"
					<< "write :\n"
					<< "pop bx \n"
					<< "add bl, '0'\n"
					<< "mov[edi], bl\n"
					<< "inc edi\n"
					<< "loop write\n"
					<< "mov ecx, sizeof strResult\n"
					<< "mov esi, offset strResult\n"
					<< "COUNTER :\n"
					<< "mov al, [esi]\n"
					<< "cmp al, 32\n"
					<< "jz IC\n"
					<< "inc numberSize\n"
					<< "IC :\n"
					<< "inc esi\n"
					<< "LOOP COUNTER\n"
					<< "pop esi\n"
					<< "pop edi\n"
					<< "pop ecx\n"
					<< "pop ebx\n"
					<< "pop eax\n"
					<< "ret\n"
					<< "intToString ENDP\n";
				fout << "B1 PROC str1 : DWORD\n"
					<< "mov edi, str1\n"
					<< "xor eax, eax\n"
					<< "mov al, [edi]\n"
					<< "ret\n"
					<< "B1 ENDP\n";
				fout << "B0 PROC str1 : DWORD, str2 : DWORD\n"
					<< "mov edi, str1\n"
					<< "mov esi, str2\n"
					<< "xor eax, eax\n"
					<< "xor ebx, ebx\n"
					<< "mov al, [edi]\n"
					<< "mov bl, [esi]\n"
					<< "cmp al, bl\n"
					<< "jne false\n"
					<< "mov ecx, eax\n"
					<<"inc ecx\n"
					<< "CYCLE :\n"
					<< "mov al, [edi]\n"
					<< "mov bl, [esi]\n"
					<< "cmp al, bl\n"
					<< "jne false\n"
					<< "inc edi\n"
					<< "inc esi\n"
					<< "LOOP CYCLE\n"
					<< "jmp true \n"
					<< "false:\n"
					<< "xor eax, eax\n"
					<< "ret\n"
					<< "true :\n"
					<< "mov eax, 1\n"
					<< "ret\n"
					<< "B0 ENDP\n";
				isWordCodeOuted = true;
			}
			fout << "main PROC\nINVOKE SetConsoleTitleA, ADDR consoletitle\n";
			isWordMainOuted = true;
		}
		break;
		case CODE_COMMAND_SEGMENT:
		{
			string sCommand = Get6BitCommand(fin);
			int iCommand = atoi(sCommand.c_str());
			switch (iCommand)
			{
			case opcode::OP_POP:
			{
				fout << "POP ";
				int iNameSegment = atoi(GetNameSegment(fin).c_str());
				string nameValue = "";
				switch (iNameSegment)
				{
				case CODE_CONST_SEGMENT:
				{
					nameValue = "C";
				}
				break;
				case CODE_DATA_SEGMENT:
				{
					nameValue = "D";
				}
				break;
				case CODE_PARM_SEGMENT:
				{
					nameValue = "P";
				}
				default:
					break;
				}
				nameValue += Get32BitName(fin);
				fout << nameValue << '\n';
			}
			break;
			case opcode::OP_PUSH:
			{
				fout << "PUSH ";
				string name;
				switch (atoi(GetNameSegment(fin).c_str()))
				{
				case CODE_CONST_SEGMENT:
					name = "C";
					break;
				case CODE_DATA_SEGMENT:
					name = "D";
					break;
				case CODE_PARM_SEGMENT:
					name = "P";
					break;
				default:
					break;
				}
				name += Get32BitName(fin);
				fout << name << '\n';
			}
			break;
			case opcode::OP_PUSH_OFFSET:
			{
				fout << "push offset ";
				string name;
				switch (atoi(GetNameSegment(fin).c_str()))
				{
				case CODE_CONST_SEGMENT:
					name = "C";
					break;
				case CODE_DATA_SEGMENT:
					name = "D";
					break;
				case CODE_PARM_SEGMENT:
					name = "C";
					break;
				default:
					break;
				}
				name += Get32BitName(fin);
				fout << name << '\n';
			}
			break;
			case opcode::OP_ADD:
			{
				fout << "POP ebx \n" << "POP eax \n";
				fout << "ADD eax, ebx\n";
				fout << "PUSH eax\n";
			}
			break;
			case opcode::OP_SUB:
			{
				fout << "POP ebx \n" << "POP eax\n";
				fout << "SUB eax, ebx \n";
				fout << "PUSH eax \n";
			}
			break;
			case opcode::OP_DIV:
			{
				fout << "POP ecx \n" << "POP eax \n CDQ \n";
				fout << "cmp ecx, 0\n";
				fout << "jz ERRORZERODIVISION\n";
				fout << "IDIV ecx \n";
				fout << "PUSH eax \n";
			}
			break;
			case opcode::OP_MOD:
			{
				fout << "POP ecx \n" << "POP eax \n CDQ \n";
				fout << "cmp ecx, 0\n";
				fout << "jz ERRORZERODIVISION\n";
				fout << "IDIV ecx \n";
				fout << "PUSH edx \n";
			}
			break;
			case opcode::OP_MUL:
			{
				fout << "POP ebx \n" << "POP eax \n";
				fout << "IMUL ebx\n";
				fout << "PUSH eax\n";
			}
			break;
			case opcode::OP_CALL:
			{
				fout << "CALL ";
				string NameSegment = GetNameSegment(fin);
				int iNameSegment = atoi(NameSegment.c_str());
				string nameFunc;
				switch (iNameSegment)
				{
				case CODE_FUNCTION_SEGMENT: nameFunc = "F";
					break;
				case CODE_BASE_LIBRARY_SIGMENT: nameFunc = "B";
					break;
				case CODE_PROCEDURE_SEGMENT: nameFunc = "FB";
					break;
				}

				nameFunc += Get32BitName(fin);
				fout << nameFunc << "\n";
				fout << "PUSH eax \n";
			}
			break;
			case opcode::OP_CALL_PROC:
			{
				fout << "CALL ";
				string NameSegment = GetNameSegment(fin);
				int iNameSegment = atoi(NameSegment.c_str());
				string nameFunc;
				switch (iNameSegment)
				{
				case CODE_FUNCTION_SEGMENT: nameFunc = "F";
					break;
				case CODE_BASE_LIBRARY_SIGMENT: nameFunc = "B";
					break;
				case CODE_PROCEDURE_SEGMENT: nameFunc = "FP";
					break;
				}

				nameFunc += Get32BitName(fin);
				fout << nameFunc << "\n";
			}
			break;
			case opcode::OP_CMP_LESS:
			{

				fout << "CALL OPERATOR_MORE\n";
				fout << "PUSH eax \n";
			}
			break;
			case opcode::OP_CMP_MORE:
			{
				fout << "CALL OPERATOR_LESS\n";
				fout << "PUSH eax \n";
			}
			break;
			case opcode::OP_RET:
			{
				fout << "POP eax\nret\n";
			}
			break;
			case opcode::OP_WRITE:
			{
				int iTypeData = atoi(Get2BitData(fin).c_str());
				switch (iTypeData)
				{
				case CODE_TYPE_BOOLEAN:
				{
					string nameOperand;
					switch (atoi(GetNameSegment(fin).c_str()))
					{
					case CODE_CONST_SEGMENT:
					{
						nameOperand = "C";
					}
					break;
					case CODE_DATA_SEGMENT:
					{
						nameOperand = "D";
					}
					break;
					case CODE_PARM_SEGMENT:
					{
						nameOperand = "P";
					}
					default:
						break;
					}
					nameOperand += Get32BitName(fin);
					char cnumberZeroMark[33];
					_itoa_s(numberBoolZeroMark++, cnumberZeroMark, 10);
					string nameZeroMark = "Z" + string(cnumberZeroMark);
					string nameAfterZeroMark = "A" + nameZeroMark;
					fout << "cmp " << nameOperand << ", 0\n" << "jz " << nameZeroMark << '\n';
					fout << "INVOKE printConsole, ADDR wordTrue, sizeof wordTrue\n";
					fout << "jmp " << nameAfterZeroMark << '\n';
					fout << nameZeroMark << ": ";
					fout << "INVOKE printConsole, ADDR wordFalse, sizeof wordFalse\n";
					fout << nameAfterZeroMark << ": \n";


				}
				break;
				case CODE_TYPE_INT:
				{
					string nameOperand;
					switch (atoi(GetNameSegment(fin).c_str()))
					{
					case CODE_CONST_SEGMENT:
					{
						nameOperand = "C";
					}
					break;
					case CODE_DATA_SEGMENT:
					{
						nameOperand = "D";
					}
					break;
					case CODE_PARM_SEGMENT:
					{
						nameOperand = "P";
					}
					default:
						break;
					}
					nameOperand += Get32BitName(fin);

					fout << "INVOKE intToString," << nameOperand << ", ADDR strResult\n";
					fout << "INVOKE printConsole, ADDR strResult, numberSize\n";
					fout << "mov numberSize, 0\n";
				}
				break;
				case CODE_TYPE_STRING:
				{
					string nameOperand;
					switch (atoi(GetNameSegment(fin).c_str()))
					{
					case CODE_CONST_SEGMENT:
					{
						nameOperand = "C";
						nameOperand += Get32BitName(fin);
						fout << "mov edi, offset " << nameOperand << "\n";
						fout << "mov al, [edi]\n";
						fout << "inc edi\n";
						fout << "INVOKE printConsole, edi, al\n";
					}
					break;
					case CODE_DATA_SEGMENT:
					{
						nameOperand = "D";
						nameOperand += Get32BitName(fin);
						fout << "mov edi," << nameOperand << "\n";
						fout << "mov al, [edi]\n";
						fout << "inc edi\n";
						fout << "INVOKE printConsole, edi, al\n";
					}
					break;
					case CODE_PARM_SEGMENT:
					{
						nameOperand = "P";
						nameOperand += Get32BitName(fin);
						fout << "mov edi," << nameOperand << "\n";
						fout << "mov al, [edi]\n";
						fout << "inc edi\n";
						fout << "INVOKE printConsole, edi, al\n";
					}
					default:
						break;
					}

				}
				default:
					break;
				}
			}
			break;
			case opcode::OP_IF:
			{
				fout << "pop eax \n";
				fout << "cmp eax, 0\n";
				fout << "jz ZI" << numberIfZeroMARK << '\n';
			}
			break;
			case opcode::OP_CREATE_MARK_FALSE:
			{
				fout << "ZI" << numberIfZeroMARK++ << ": \n";
			}
			break;
			case opcode::OP_GO_AFTER_ELSE:
			{
				fout << "jmp " << "AE" << markNumber << '\n';
			}
			break;
			case opcode::OP_CREATE_MARK_AFTER_FALSE:
			{
				fout << "AE" << markNumber++ << ":\n";
			}
			break;
			default:
				break;
			}



		}
		break;
		case CODE_PROCEDURE_SEGMENT:
		{

			if (!isWordDataOuted)
			{
				fout << ".DATA \n";
				fout << "strResult byte 11 dup(?)\n";
				fout << "numberSize dword 0\n";
				isWordDataOuted = true;
			}
			if (!isWordCodeOuted)
			{
				fout << ".code\n";
				fout << "OPERATOR_MORE PROC, PARM1: SDWORD, PARM2: SDWORD\n";
				fout << "mov eax, PARM1 \n mov ebx, PARM2 \n";
				fout << "CMP eax, ebx \n";
				fout << "jg isGreater\n";
				fout << "mov eax, 0\n";
				fout << "jmp markAfterGreater \n";
				fout << "isGreater: mov eax, 1 \n";
				fout << "markAfterGreater: \n";
				fout << "ret\n OPERATOR_MORE ENDP\n";

				fout << "OPERATOR_LESS PROC, PARM3: SDWORD, PARM4: SDWORD \n";
				fout << "mov eax, PARM3 \n mov ebx, PARM4 \n";
				fout << "CMP eax, ebx \n";
				fout << "jg isGreater2\n";
				fout << "mov eax, 1\n";
				fout << "jmp markAfterGreater2 \n";
				fout << "isGreater2: mov eax, 0 \n";
				fout << "markAfterGreater2: \n";
				fout << "ret\n OPERATOR_LESS ENDP\n";

				fout << "printConsole PROC  pstr : dword, _size : dword"
					<< "\npush eax\n"
					<< "\nINVOKE GetStdHandle, -11"
					<< "\nINVOKE WriteConsoleA, eax, pstr, _size, 0, 0\n"
					<< "\npop eax\n"
					<< "\nret \n"
					<< "\nprintConsole ENDP\n";
				fout << "intToString PROC number: sdword, pstr: dword\n"
					<< "push eax\n"
					<< "push ebx\n"
					<< "push ecx\n"
					<< "push edi\n"
					<< "push esi\n"
					<< "mov esi, offset strResult\n"
					<< "mov ecx, sizeof strResult\n"
					<< "mov al, ' '\n"
					<< "CLEARSTR:\n"
					<< "mov [esi], al\n"
					<< "inc esi\n"
					<< "LOOP CLEARSTR\n"
					<< "mov edi, pstr\n"
					<< "mov esi, 0\n"
					<< "mov eax, number\n"
					<< "cdq\n"
					<< "mov ebx, 10\n"
					<< "test eax, 80000000h\n"
					<< "idiv ebx\n"		
					<< "jz plus\n"
					<< "neg eax\n"
					<< "neg edx\n"
					<< "mov cl, '-'\n"
					<< "mov[edi], cl\n"
					<< "inc edi\n"
					<< "plus :\n"
					<< "push dx\n"
					<< "inc esi\n"
					<< "test eax, eax\n"
					<< "jz fin\n"
					<< "cdq\n"
					<< "idiv ebx\n"
					<< "jmp plus\n"
					<< "fin :\n"
					<< "mov ecx, esi\n"
					<< "write :\n"
					<< "pop bx \n"
					<< "add bl, '0'\n"
					<< "mov[edi], bl\n"
					<< "inc edi\n"
					<< "loop write\n"
					<< "mov ecx, sizeof strResult\n"
					<< "mov esi, offset strResult\n"
					<< "COUNTER :\n"
					<< "mov al, [esi]\n"
					<< "cmp al, 32\n"
					<< "jz IC\n"
					<< "inc numberSize\n"
					<< "IC :\n"
					<< "inc esi\n"
					<< "LOOP COUNTER\n"
					<< "pop esi\n"
					<< "pop edi\n"
					<< "pop ecx\n"
					<< "pop ebx\n"
					<< "pop eax\n"
					<< "ret\n"
					<< "intToString ENDP\n";
				fout << "B1 PROC str1 : DWORD\n"
					<< "mov edi, str1\n"
					<< "xor eax, eax\n"
					<< "mov al, [edi]\n"
					<< "ret\n"
					<< "B1 ENDP\n";
				fout << "B0 PROC str1 : DWORD, str2 : DWORD\n"
					<< "mov edi, str1\n"
					<< "mov esi, str2\n"
					<< "xor eax, eax\n"
					<< "xor ebx, ebx\n"
					<< "mov al, [edi]\n"
					<< "mov bl, [esi]\n"
					<< "cmp al, bl\n"
					<< "jne false\n"
					<< "mov ecx, eax\n"
					<<"inc ecx \n"
					<< "CYCLE :\n"
					<< "mov al, [edi]\n"
					<< "mov bl, [esi]\n"
					<< "cmp al, bl\n"
					<< "jne false\n"
					<< "inc edi\n"
					<< "inc esi\n"
					<< "LOOP CYCLE\n"
					<< "jmp true \n"
					<< "false:\n"
					<< "xor eax, eax\n"
					<< "ret\n"
					<< "true :\n"
					<< "mov eax, 1\n"
					<< "ret\n"
					<< "B0 ENDP\n";
				isWordCodeOuted = true;
			}
			if (lastFunctionName != "")
			{
				switch (typeDataLastFunction)
				{
				case CODE_TYPE_INT:
				case CODE_TYPE_BOOLEAN:
				{
					fout << "jmp MarkAfterError\n";
					fout << "ERRORZERODIVISION:\n";
					fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
					fout << " push 0\n call ExitProcess\n";
					fout << "MarkAfterError:\n";
					fout << "xor eax, eax\n";
				}
				break;
				case CODE_TYPE_STRING:
				{
					fout << "jmp MarkAfterError\n";
					fout << "ERRORZERODIVISION:\n";
					fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
					fout << " push 0\n call ExitProcess\n";
					fout << "MarkAfterError:\n";
					fout << "mov eax, offset emptstr\n";
				}
				break;
				}
				fout << "ret \n" << lastFunctionName << " ENDP \n";
				lastFunctionName = "";
			}
			if (lastProcedureName != "")
			{
				fout << "ret\n" << lastProcedureName << " ENDP\n";
				lastProcedureName = "";
			}
			lastProcedureName = "FP" + Get32BitName(fin);
			fout << lastProcedureName << " PROC \n";
		}
		default:
			break;

			typeSegment.clear();
		}

	}
	fout << "jmp MarkAfterError\n";
	fout << "ERRORZERODIVISION:\n";
	fout << "INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS\n";
	fout << "MarkAfterError:\n";
	fout << " push 0\n call ExitProcess \n main ENDP \n end main \n";
}

string AssemblerCodeGeneration::convertString2ToString10(string str)
{
	int itog = 0;
	int _razr = 0;
	//конвертирование числа в 10 систему счисления
	for (int i = str.length() - 1; i >= 0; i--)
	{
		if (str[i] == '1')
		{
			itog += pow(2, _razr);
		}
		_razr++;
	}
	char buf[100];
	_itoa_s(itog, buf, 10);
	string ret(buf);
	return ret;

}

string AssemblerCodeGeneration::Get32BitName(ifstream & fin)
{
	string outStr;
	char symb;
	for (int i = 0; i < 32; i++)
	{
		fin >> symb;
		outStr += symb;
	}
	outStr = convertString2ToString10(outStr);
	return outStr;
}

string AssemblerCodeGeneration::GetNameSegment(ifstream & fin)
{
	char symb;
	string outType;
	for (int i = 0; i < 4; i++)
	{
		fin >> symb;
		outType += symb;
	}
	outType = convertString2ToString10(outType);
	return outType;
}

string AssemblerCodeGeneration::Get2BitData(ifstream & fin)
{
	char symb;
	string outData;
	for (int i = 0; i < 2; i++)
	{
		fin >> symb;
		outData += symb;
	}
	outData = convertString2ToString10(outData);
	return outData;
}

string AssemblerCodeGeneration::Get6BitCommand(ifstream & fin)
{
	char symb;
	string outStr;
	for (int i = 0; i < 6; i++)
	{
		fin >> symb;
		outStr += symb;

	}
	outStr = convertString2ToString10(outStr);
	return outStr;
}
std::string AssemblerCodeGeneration::StringReplacer(const std::string& inputStr, const std::string& src, const std::string& dst)
{
	std::string result(inputStr);

	size_t pos = result.find(src);
	while (pos != std::string::npos) {
		result.replace(pos, src.size(), dst);
		pos = result.find(src, pos);
	}

	return result;
}

unsigned int AssemblerCodeGeneration::convertWin1251symbTo866(unsigned char symb)
{
	unsigned int code = (unsigned int)symb;
	if (240 <= code && code <= 255)
	{
		return code - 16;
	}
	if (192 <= code && code <= 239)
	{
		return code - 64;
	}


	return code;
}
