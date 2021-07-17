MYDATA SEGMENT 
DATA   DW  1234H,5678H,9D4CH,0D7H,0,-1,7D2AH,8A0EH,10F5H,645DH,8000H,0FFFEH,10H,7FFFH,7FFEH 
N      EQU $-DATA 
MYDATA ENDS
 
CODE    SEGMENT
MAIN    PROC    FAR
        ASSUME  CS:CODE,DS:MYDATA
        PUSH    DS;保存原来的数据段地址
        MOV     AX,0H;0送AX
        PUSH    AX;0压栈
        MOV     AX,MYDATA
        MOV     DS,AX
        MOV     CX,N
        SAR     CX,1H
        MOV     BX,OFFSET DATA
        MOV     AX,8000H;最小的有符号偶数
AGAIN:  ADD     BX,2
        CMP     AX,[BX-2]
        JGE     NEXT
        TEST    WORD PTR [BX-2],01H
        JNZ     NEXT
        MOV     AX,[BX-2]
NEXT:   LOOP    AGAIN
        MOV     BX,AX
        CALL OUTPUT_HEX
        RET
MAIN    ENDP

OUTPUT_HEX PROC NEAR        
        MOV     CH,4
        MOV     CL,4
ROTATE: ROL     BX,CL
        MOV     AL,BL
        AND     AL,0FH
        ADD     AL,30H
        CMP     AL,3AH
        JL      PRINTIT
        ADD     AL,7
PRINTIT:MOV     DL,AL
        MOV     AH,2
        INT     21H
        DEC     CH
        JNZ     ROTATE
        MOV     AH,2
        MOV     DL,48H
        INT     21H
        RET
OUTPUT_HEX ENDP        

CODE    ENDS
       	END	MAIN