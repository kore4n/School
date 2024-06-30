     AREA quizseven, CODE, READONLY
     ENTRY
          MOV   R0, #9
          MOV   R1, #3
repeat    ADD   R1,R1,R0,LSL#3
          TST   R1,#1
          SUBNE R0,#3
          SUBEQ R0,#9
          CMP   R0,#0
          BGT   repeat
halt      B     halt
     END