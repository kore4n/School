		AREA test, CODE, READONLY
		ENTRY
		MOV r0, #0xFFFFFFFA
		MOV r1, #0xFFFFFFF1
		ADDS r2, r1, r0
loop	b	loop
		END