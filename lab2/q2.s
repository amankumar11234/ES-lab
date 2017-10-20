	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=n1
	LDR R1,=n2
	
	LDR R6,[R0]
	LDR R7,[R1]
	
	SUBS R3, R7, R6      ;n2-n1   R7-R6
	SBC R4, #0
	LDR R5,=Dst
	STR R3, [R5], #4
	STR R4, [R5]
	
STOP B STOP
n1 DCD 0xFFFFFFFF
n2 DCD 0xFFFFFFFF
	AREA mycod, DATA, READWRITE
Dst DCD 0
	END
	