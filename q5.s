	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R5,=n1
	LDR R9,=result
	MOV R1,#0x0A
	LDR R0,[R5]
	
UP	CMP R0,#00
	BEQ last
	BL DIV
	
	CMP R8,#29
	STRHS R7,[R9],#4
	MOVHS R8,#00
	MOVHS R7,#00
	
	LSL R5,R5,R8
	ORR R7,R5
	ADD R8,#04
	MOV R0,R4
	B UP
DIV MOV R3,#00
DW	CMP R0,R1
	BLO exit
	SUB R0,R1
	ADD R3,#01
	B DW
	
exit MOV R4,R3   ;q
	MOV R5,R0    ;r
	BX LR
	
last STR R7,[R9]
	
STOP B STOP
n1 DCD 0xffffffff;
	AREA mydata, DATA, READWRITE
result DCD 0
	END