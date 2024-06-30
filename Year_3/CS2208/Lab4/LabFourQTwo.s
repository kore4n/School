     AREA prog, CODE, READWRITE
     ENTRY
	 mov r0, #100
	 
     LDR r2, =6554
	 MUL r1, r2, r0
	 ASR r1, #16
Loop B Loop
     END