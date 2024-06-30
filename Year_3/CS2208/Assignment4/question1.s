        AREA question1, CODE, READONLY
        ENTRY

Zero    EQU  0                  ;to check if last digit is zero
MulCon  EQU  1                  ;multiplication constant
FirstI  EQU  0                  ;first index for first digit
SecondI EQU  1                  ;second index for second digit
Diff    EQU  2                  ;to increment counter to go to next digit
MaxOne  EQU  12                 ;when to stop counting for first group of digits
MaxTwo  EQU  11                 ;when to stop counting for second group of digits
Divisor EQU  10                 ;for repeated subtraction to see if answer is divisible by 10
RemAsci EQU  0x30               ;to subtract from ascii code to get decimal value
LastDig EQU  11                 ;memory spaces after first digit in a string to find last digit
FinalS  EQU  1                  ;success value to put into r0
FinalF  EQU  2                  ;fail value to put into r0
	
        MOV  r0, #FirstI        ;initialize r0 as an accumulator
        ADR  r1, UPC            ;point r1 to UPC's address to access later
		
LoopOne LDRB r2, [r1, r0]       ;move to the new digit
        SUB  r2, r2, #RemAsci
        ADD  r3, r3, r2         ;add current digit to first sum at r3
        ADD  r0, r0, #Diff      ;increment counter by 2 to count the second next digit
        CMP  r0, #MaxOne        ;check if all second group of 5 digits are counted
        BNE  LoopOne            ;loop again if not all first group of digits have been counted
		
        MOV  r0, #SecondI       ;initialize to 1 to use odd digits to count the second group of 5 digits
		
LoopTwo LDRB r2, [r1, r0]       ;move to the new digit
        SUB  r2, r2, #RemAsci   ;subtract to transform from ASCII to digit
        ADD  r4, r4, r2         ;add current digit to second sum at r4
        ADD  r0, r0, #Diff      ;increment counter by 2 to count the second next digit
        CMP  r0, #MaxTwo        ;check if all first group of 6 digits are counted
        BNE  LoopTwo            ;loop again if not all second group of digits have been counted
		
        ADD  r3, r3, LSL #MulCon;multiply first group by 3
        ADD  r3, r4             ;add 3x the first group and the second group and store sum in r3
		
        LDRB r4, [r1, #LastDig] ;store check digit in r4
        SUB  r4, r4, #RemAsci   ;subtract to transform from ASCII to digit
        ADD  r3, r4             ;add check digit to r3
		
        CMP  r3, #Zero          ;compare to see if all already 0s
        BEQ  Success            ;succeed if already all 0s
		
LoopThr SUBS r3, r3, #Divisor   ;divide by 10 until 0 or negative
        BEQ  Success            ;branch to Success if zero
        BPL  LoopThr            ;keep looping until reach 0 or a negative number
        MOV r0, #FinalF         ;number went into negatives - then not divisible by zero, not a valid UPC Code. failure message
        b Endless               ;not valid UPC code, skip success and go to endless loop
		
Success MOV r0, #FinalS         ;success message
		
Endless B    Endless

UPC     DCB "013800150738" ;correct UPC string
UPC2    DCB "060383755577" ;correct UPC string
UPC3    DCB "065633454712" ;correct UPC string 
UPC4    DCB "000000000000" ;correct UPC string
UPC5    DCB "013800150739" ;incorrect UPC string

        END