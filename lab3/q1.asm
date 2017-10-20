	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=VALUE1
	LDR R1,=VALUE2
	LDR R5,=result
	LDR R4, [R0]
	LDR R1,[R1]
		
BACK	MOV R9,#0x00000000
		MSR XPSR, R9
		
		ADDS R2, R4    ;r8+r7      v2+v1
		ADC R3,#0
		SUB R1,#1		
		
		TEQ R1,#0
		BNE BACK
		
	STR R2, [R5], #4
	
	STR R3, [R5]
STOP B STOP
VALUE1 DCD 0xFFFFFFFF;
VALUE2 DCD 0x000000FF
	AREA mydata, DATA, READWRITE
result DCD 0, 0; 0, 0, 0, 0, 0, 0, 0 ,0 ;ram
	END