        AREA prog, CODE, READWRITE
        ;ENTRY
        LDR SP, =STACK
        MOV r0, #0x11
        MOV r1, #0x22
PUSH_R0 STR r0, [SP, #4]!
PUSH_R1 STR r1, [SP, #4]!
POP_R0  LDR r0, [SP], #-4
POP_R1  LDR r1, [SP], #-4
Loop B  Loop
        space 32
STACK   DCD 0x0
        space 32
        END