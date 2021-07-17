DATA  SEGMENT
      A DW 70H
      B DW 60H
      C DW 40H
      D DW 20H
      S DW  ?
      X DW  5H
      Y DW  2H
      ERR DB 0DH,0AH,"ERROR:DIV 0!!!$"
DATA  ENDS
CODE  SEGMENT
      ASSUME  CS:CODE,DS:DATA
START:MOV    AX,DATA
      MOV    DS,AX
      MOV    AX,A
      IMUL   B                    ;乘积存在DX:AX
      MOV    BX,C
      ADD    BX,D                 ;c+d存在BX
      CMP    BX,0H
      JE     ERRO
      IDIV   BX                 ;商存在AX，余数存在DX
      
      MOV    BX,AX                ;用BX存取第一步结果
      MOV    AX,A
      SUB    AX,B
      IMUL   X
      CMP    Y,0H
      JE     ERRO
      IDIV   Y  
                     ;商在AX，余数在DX
      ADD    BX,AX
      MOV    AX,C
      MOV    DX,0H
      CMP    D,0H
      JE     ERRO
      IDIV   D                  ;商在AX,余数在DX
      
      ADD    BX,AX
      MOV    S,BX
      JMP    RIG
      
ERRO:  MOV    DX,OFFSET ERR
      MOV    AH,9H
      INT    21H 
RIG:  MOV   AX,4C00H
      INT    21H
CODE  ENDS
      END  START