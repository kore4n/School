
       AREA More_data_definitions, CODE, READONLY
       ENTRY
L1     SPACE 3          
L2     SPACE 3      
       ALIGN        
L3     SPACE 3   
L4     DCD   0x12345678
L5     DCD   +2_1111000011110000
L6     DCD   -2_1111000011110000
      
L7     DCW   255
L8     DCW  -255
  
L9     DCB   &0A
       ALIGN
  
LA     DCD  1,2,3,4
LB     DCB  5
LC     DCD  6       
 
       END
