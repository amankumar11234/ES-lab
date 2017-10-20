	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R5, =src
	LDR R0, =src + (len-1)*4
	LDR R1, =src + (2*len-lap)*4
	
	MOV R2, #len
	
BACK	LDR R3, [R0],#-4
		STR R3, [R1],#-4
		SUB R2, #1
		TEQ R2, #0
		BNE BACK
STOP B STOP
len equ 10
lap equ 4
	
	AREA mycod, DATA, READWRITE
src DCD 0x10001008
	END
	