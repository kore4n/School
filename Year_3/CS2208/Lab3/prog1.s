     AREA prog, CODE, READONLY
     ENTRY
	 mov	r7, #0
	 mov	r10, #0
     LDR r7, [r10, #8]
	 
Loop B    Loop   

A    DCD  0x100
	
     END