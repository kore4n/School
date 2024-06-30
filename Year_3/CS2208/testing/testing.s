        AREA testing, CODE, READONLY
        ENTRY
		MOV r0, #2
		MOV r1, #5
		MOV r2, #0
		LDR r0, =0xC9
		MVNS r1, r0
		
loop	b	loop
        END