      AREA prog, CODE, READWRITE
      ENTRY
      ADR   r3,AAA
      MOV   r0,PC
      STMEA r3!,{PC}
      STR   PC,[r3]
      LDMFA r3,{r1,r2}
Loop  B     Loop
      DCD   1,2
AAA   DCD   3
      DCD   4,5
      END