DATA SEGMENT
    BUFFER DB 81,?,81 DUP(?)
    MESS DB 0DH,0AH,"Please input a string:$"
    MESS0 DB 0DH,0AH,"number:$"
    MESS1 DB 0DH,0AH,"letter:$"
    MESS2 DB 0DH,0AH,"else:$"
    COUNT DB 3 DUP(0)
    ADDD DB 30H
DATA ENDS
CODE SEGMENT
    ASSUME CS:CODE,DS:DATA
START :MOV AX,DATA
	     MOV DS,AX
	     LEA DX,MESS
	     MOV AH,09H
	     INT 21H
	     MOV AH,0AH
	     LEA DX,BUFFER
	     INT 21H
       LEA BX,BUFFER
       MOV CL,BUFFER+1
       MOV CH,0
       MOV SI,1
NUMBER:CMP SI,CX
       JA LETTER0
       INC SI
       CMP BUFFER[SI],'0'
       JB NUMBER
       CMP BUFFER[SI],'9'
       JA NUMBER
       INC COUNT
       JMP NUMBER
LETTER0:MOV SI,1
LETTER :CMP SI,CX
        JA ELSE0
        INC SI
        CMP BUFFER[SI],'A'
        JB LETTER
        CMP BUFFER[SI],'z'
        JA LETTER
        CMP BUFFER[SI],'a'
        JNB FADD0
        CMP BUFFER[SI],'Z'
        JNA FADD0
        JMP LETTER
FADD0:  INC COUNT+1
        JMP LETTER
ELSE0:  ADD COUNT+2,CL
        MOV AL,COUNT
        SUB COUNT+2,AL
        MOV AL,COUNT+1
        SUB COUNT+2,AL
        
        LEA DX,MESS0
	      MOV AH,09H
	      INT 21H
	      MOV AH,0
	      MOV AL,COUNT
	      PUSH AX
	      CALL OUTPUT_HEX
	      MOV  DL,' '
	      MOV  AH,2
	      INT  21H
	      POP  AX
	      CALL OUTPUT_DEC
	      
	      LEA DX,MESS1
	      MOV AH,09H
	      INT 21H
	      MOV AH,0
	      MOV AL,COUNT+1
        PUSH AX
	      CALL OUTPUT_HEX
	      MOV  DL,' '
	      MOV  AH,2
	      INT  21H
	      POP  AX
	      CALL OUTPUT_DEC
	      
	      LEA DX,MESS2
	      MOV AH,09H
	      INT 21H
	      MOV AH,0
	      MOV AL,COUNT+2
	      PUSH AX
	      CALL OUTPUT_HEX
	      MOV  DL,' '
	      MOV  AH,2
	      INT  21H
	      POP  AX
	      CALL OUTPUT_DEC
	      
	      MOV	AX,4C00H
	      INT  21H
	      
OUTPUT_HEX    PROC
        PUSH AX
        AND AL,0F0H
        MOV CL,4
        SHR AL,CL
        CMP AL,10
        JL B1
        ADD AL,7
B1:     ADD AL,30H
        MOV DL,AL
        MOV AH,2
        INT 21H
        POP AX

        AND AL,0FH
        CMP AL,10
        JL B2
        ADD AL,7
B2:     ADD AL,30H
        MOV DL,AL
        MOV AH,2
        INT 21H
        
        MOV DL,'H'
        MOV AH,2
        INT 21H
        RET
OUTPUT_HEX    ENDP

OUTPUT_DEC    PROC
        PUSH AX
        PUSH BX
        PUSH CX
        PUSH DX
        MOV CX,0
        MOV BX,10
L:      MOV DX,0
        CMP AX,0
        JZ  DISP0
        DIV BX
        PUSH DX
        INC  CX
        JMP  L
DISP0:  CMP  CX,0
        JZ   RE
        POP  DX
        ADD  DX,30H
        MOV  AH,2
        INT  21H
        DEC  CX
        JMP  DISP0
RE:     MOV  DL,'D'
        MOV  AH,2
        INT  21H
        POP  DX
        POP  CX
        POP  BX
        POP  AX
        RET
OUTPUT_DEC    ENDP

CODE ENDS
	      END	START
	    
	      
      
	      
        