      AREA prog, CODE, READWRITE
      ENTRY
      ADR  R0,C+1
      MOV  R1,#1
      LDRB R2,[R0,#5]
      LDRB R3,[R0],-R1,LSL#2
      LDRB R4,[R0,#2]!
Loop  B Loop

A     DCB 0x77
B     DCD 0xAABBCCDD
C     SPACE 1
D     DCB 0x88
E     SPACE 1
F     DCD 0x11223344
      END