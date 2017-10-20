	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000
	DCD Reset_Handler
	
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0, =array
	LDR R1, =array + (len*4-4)
	MOV R2, #len/2

BACK	LDR R3, [R1]
		LDR R4, [R0]
		STR R3, [R0], #4
		STR R4, [R1], #-4
		
		SUB R2, #1
		TEQ R2, #0
		BNE BACK
STOP B STOP
len equ 10

	AREA mydata, DATA, READWRITE
array DCD 0x00000001
	END