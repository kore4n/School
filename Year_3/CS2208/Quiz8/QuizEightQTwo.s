      AREA prog, CODE, READWRITE
      ENTRY
;	  mov r3, #0
;	  ADR r4, albl
;	  mov r5, #7
;	  LDREQ r3, [r4, r5]!
;albl  DCB 32

	  ADR r4, albl
	  mov r6, #0x1C000
	  ADR r5, albl

	  LDRBPL r4, [r5, r6]
albl  DCB 32
      END