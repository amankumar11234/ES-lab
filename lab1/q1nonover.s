	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0, =src
	LDR R1, =src + (len*4)
	
	MOV R2, #len
BACK	LDR R3, [R0], #4
		STR R3, [R1], #4
		SUB R2, #1
		TEQ R2, #0
		BNE BACK
STOP B STOP
len equ 10

	AREA mydata, DATA, READWRITE
src DCD 0x00000008
	END