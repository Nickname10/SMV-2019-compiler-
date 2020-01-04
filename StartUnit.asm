.586

.model flat, stdcall
includelib kernel32.lib
includelib libucrt.lib
ExitProcess PROTO :DWORD
GetStdHandle Proto :DWORD
SetConsoleTitleA PROTO :DWORD
WriteConsoleA PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD 
printConsole PROTO pstr: dword, _size: dword
intToString PROTO number: sdword, pstr: dword 
.stack 4096 
.const
consoletitle db 'SMV-2019', 0
ERRMESS db 'Error zero division',0 
wordTrue db 'True' 
wordFalse db'False' 
emptstr db 0 
C1 sdword 5 
C2 sdword 1 
C3 sdword 1 
C4 sdword 1 
C5 sdword 1 
C6 db 1, 32
C7 db 9, 32, 45, 32, 224, 160, 162, 173, 235, 10
C8 db 1, 32
C9 db 12, 32, 45, 32, 173, 165, 32, 224, 160, 162, 173, 235, 10
C10 sdword 0 
C11 sdword 5 
C12 sdword 25 
C13 sdword 0 
C14 sdword 2 
C15 db 34, 144, 165, 167, 227, 171, 236, 226, 160, 226, 32, 162, 235, 224, 160, 166, 165, 173, 168, 239, 32, 50, 53, 32, 43, 32, 40, 45, 50, 53, 42, 50, 41, 58, 32
C16 sdword 5 
C17 sdword 449 
C18 sdword 255 
C19 db 48, 10, 135, 173, 160, 231, 165, 173, 168, 165, 32, 175, 165, 224, 165, 172, 165, 173, 173, 174, 169, 32, 98, 105, 110, 97, 114, 121, 44, 32, 170, 174, 226, 174, 224, 174, 165, 32, 224, 160, 162, 173, 174, 32, 49, 48, 49
 	db 58, 32
C20 db 45, 10, 135, 173, 160, 231, 165, 173, 168, 165, 32, 175, 165, 224, 165, 172, 165, 173, 173, 174, 169, 32, 111, 99, 116, 44, 32, 170, 174, 226, 174, 224, 174, 165, 32, 224, 160, 162, 173, 174, 32, 55, 48, 49, 58, 32
C21 db 43, 10, 135, 173, 231, 165, 173, 168, 165, 32, 175, 165, 224, 165, 172, 165, 173, 173, 174, 169, 32, 104, 101, 120, 44, 32, 170, 174, 226, 174, 224, 174, 165, 32, 224, 160, 162, 173, 174, 32, 102, 102, 58, 32
C22 sdword 5 
C23 sdword 25 
C24 sdword 10 
C25 sdword 3 
C26 sdword 2 
C27 sdword 100 
C28 db 53, 10, 135, 173, 160, 231, 165, 173, 168, 165, 32, 162, 235, 224, 160, 166, 165, 173, 168, 239, 32, 40, 40, 53, 42, 50, 53, 32, 43, 32, 49, 48, 41, 32, 100, 105, 118, 32, 51, 32, 42, 32, 50, 41, 32, 109, 111
 	db 100, 32, 49, 48, 48, 58, 32
C29 db 9, 49, 50, 51, 52, 53, 54, 55, 56, 57
C30 db 15, 10, 132, 171, 168, 173, 160, 32, 225, 226, 224, 174, 170, 168, 58, 32
C31 db 3, 32, 61, 32
C32 db 4, 115, 116, 114, 49
C33 db 4, 115, 116, 114, 50
C34 db 36, 10, 145, 224, 160, 162, 173, 168, 172, 32, 175, 174, 161, 160, 169, 226, 174, 162, 174, 32, 173, 165, 225, 170, 174, 171, 236, 170, 174, 32, 225, 226, 224, 174, 170, 58, 10
C35 db 4, 115, 116, 114, 49
C36 db 14, 55, 32, 228, 160, 170, 226, 174, 224, 168, 160, 171, 32, 58, 32
C37 sdword 7 
.DATA 
strResult byte 11 dup(?)
numberSize dword 0
D0 sdword 0 
D1 sdword 0 
D2 sdword 0 
D3 sdword 0 
D4 sdword 0 
D5 sdword 0 
D6 sdword 0 
D7 dword 0 
D8 sdword 0 
D9 dword 0 
D10 dword 0 
D11 sdword 0 
.code
OPERATOR_MORE PROC, PARM1: SDWORD, PARM2: SDWORD
mov eax, PARM1 
 mov ebx, PARM2 
CMP eax, ebx 
jg isGreater
mov eax, 0
jmp markAfterGreater 
isGreater: mov eax, 1 
markAfterGreater: 
ret
 OPERATOR_MORE ENDP
OPERATOR_LESS PROC, PARM3: SDWORD, PARM4: SDWORD 
mov eax, PARM3 
 mov ebx, PARM4 
CMP eax, ebx 
jg isGreater2
mov eax, 1
jmp markAfterGreater2 
isGreater2: mov eax, 0 
markAfterGreater2: 
ret
 OPERATOR_LESS ENDP
printConsole PROC  pstr : dword, _size : dword
push eax

INVOKE GetStdHandle, -11
INVOKE WriteConsoleA, eax, pstr, _size, 0, 0

pop eax

ret 

printConsole ENDP
intToString PROC number: sdword, pstr: dword
push eax
push ebx
push ecx
push edi
push esi
mov esi, offset strResult
mov ecx, sizeof strResult
mov al, ' '
CLEARSTR:
mov [esi], al
inc esi
LOOP CLEARSTR
mov edi, pstr
mov esi, 0
mov eax, number
cdq
mov ebx, 10
test eax, 80000000h
idiv ebx
jz plus
neg eax
neg edx
mov cl, '-'
mov[edi], cl
inc edi
plus :
push dx
inc esi
test eax, eax
jz fin
cdq
idiv ebx
jmp plus
fin :
mov ecx, esi
write :
pop bx 
add bl, '0'
mov[edi], bl
inc edi
loop write
mov ecx, sizeof strResult
mov esi, offset strResult
COUNTER :
mov al, [esi]
cmp al, 32
jz IC
inc numberSize
IC :
inc esi
LOOP COUNTER
pop esi
pop edi
pop ecx
pop ebx
pop eax
ret
intToString ENDP
B1 PROC str1 : DWORD
mov edi, str1
xor eax, eax
mov al, [edi]
ret
B1 ENDP
B0 PROC str1 : DWORD, str2 : DWORD
mov edi, str1
mov esi, str2
xor eax, eax
xor ebx, ebx
mov al, [edi]
mov bl, [esi]
cmp al, bl
jne false
mov ecx, eax
inc ecx
CYCLE :
mov al, [edi]
mov bl, [esi]
cmp al, bl
jne false
inc edi
inc esi
LOOP CYCLE
jmp true 
false:
xor eax, eax
ret
true :
mov eax, 1
ret
B0 ENDP
F0 PROC  P0 : SDWORD
PUSH P0
PUSH C2
CALL OPERATOR_LESS
PUSH eax 
pop eax 
cmp eax, 0
jz ZI0
PUSH P0
PUSH C3
POP ebx 
POP eax
SUB eax, ebx 
PUSH eax 
POP P0
PUSH P0
PUSH C4
POP ebx 
POP eax 
ADD eax, ebx
PUSH eax
PUSH P0
CALL F0
PUSH eax 
POP ebx 
POP eax 
IMUL ebx
PUSH eax
POP eax
ret
jmp AE0
ZI0: 
PUSH C5
POP eax
ret
AE0:
jmp MarkAfterError
ERRORZERODIVISION:
INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS
 push 0
 call ExitProcess
MarkAfterError:
xor eax, eax
ret 
F0 ENDP 
F1 PROC  P1 : DWORD, P2 : DWORD
PUSH P2
PUSH P1
CALL B0
PUSH eax 
pop eax 
cmp eax, 0
jz ZI1
mov edi,P1
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi, offset C6
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi,P2
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi, offset C7
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
jmp AE1
ZI1: 
mov edi,P1
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi, offset C8
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi,P2
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi, offset C9
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
AE1:
PUSH C10
POP eax
ret
jmp MarkAfterError
ERRORZERODIVISION:
INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS
 push 0
 call ExitProcess
MarkAfterError:
xor eax, eax
ret 
F1 ENDP 
FP0 PROC 
PUSH D0
PUSH C11
POP ebx 
POP eax 
ADD eax, ebx
PUSH eax
POP D0
ret
FP0 ENDP
main PROC
INVOKE SetConsoleTitleA, ADDR consoletitle
PUSH C1
POP D0
PUSH C12
POP D1
PUSH C13
PUSH D1
PUSH C14
POP ebx 
POP eax 
IMUL ebx
PUSH eax
POP ebx 
POP eax
SUB eax, ebx 
PUSH eax 
POP D2
PUSH D1
PUSH D2
POP ebx 
POP eax 
ADD eax, ebx
PUSH eax
POP D1
mov edi, offset C15
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
INVOKE intToString,D1, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
PUSH C16
POP D3
PUSH C17
POP D4
PUSH C18
POP D5
mov edi, offset C19
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
INVOKE intToString,D3, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
mov edi, offset C20
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
INVOKE intToString,D4, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
mov edi, offset C21
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
INVOKE intToString,D5, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
PUSH C22
PUSH C23
POP ebx 
POP eax 
IMUL ebx
PUSH eax
PUSH C24
POP ebx 
POP eax 
ADD eax, ebx
PUSH eax
PUSH C25
POP ecx 
POP eax 
 CDQ 
cmp ecx, 0
jz ERRORZERODIVISION
IDIV ecx 
PUSH eax 
PUSH C26
POP ebx 
POP eax 
IMUL ebx
PUSH eax
PUSH C27
POP ecx 
POP eax 
 CDQ 
cmp ecx, 0
jz ERRORZERODIVISION
IDIV ecx 
PUSH edx 
POP D6
mov edi, offset C28
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
INVOKE intToString,D6, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
push offset C29
POP D7
PUSH D7
CALL B1
PUSH eax 
POP D8
mov edi, offset C30
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi,D7
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
mov edi, offset C31
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
INVOKE intToString,D8, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
push offset C32
POP D9
push offset C33
POP D10
mov edi, offset C34
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
PUSH D10
PUSH D9
CALL F1
push offset C35
POP D10
PUSH D10
PUSH D9
CALL F1
mov edi, offset C36
mov al, [edi]
inc edi
INVOKE printConsole, edi, al
PUSH C37
CALL F0
PUSH eax 
POP D11
INVOKE intToString,D11, ADDR strResult
INVOKE printConsole, ADDR strResult, numberSize
mov numberSize, 0
jmp MarkAfterError
ERRORZERODIVISION:
INVOKE printConsole, ADDR ERRMESS, sizeof ERRMESS
MarkAfterError:
 push 0
 call ExitProcess 
 main ENDP 
 end main 
