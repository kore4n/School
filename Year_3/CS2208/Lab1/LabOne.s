		AREA LabOne, CODE, READWRITE
		ENTRY
			
		MOV r1,#2
		MOV r2,#4
		MOV r3,#8
		MOV r4,#5
		
		ADD r5,r1,r2
		MUL r6,r3,r4
		SUB r0,R5,r6
		
loop	b	loop
		END