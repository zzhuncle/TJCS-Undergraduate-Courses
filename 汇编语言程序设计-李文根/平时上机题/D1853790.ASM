_DATA   SEGMENT 'DATA'
MSG 	DB  	13, 10, "Please input a number: ", 13, 10, "$"
ADTABLE   DW   CASE0,CASE1,CASE2,CASE3,CASE4,CASE5,CASE6,CASE7,CASE_ERR
S1  	DB	  20H, "Monday", 13, 10, '$'
S2  	DB  	20H, "Tuesday", 13, 10, '$'
S3  	DB  	20H, "Wednesday", 13, 10, '$'
S4 	  DB  	20H, "Thursday", 13, 10, '$'
S5    DB    20H, "Friday", 13, 10, '$'
S6    DB    20H, "Saturday", 13, 10, '$'
S7    DB    20H, "Sunday", 13, 10, '$'
ERR   DB    20H, "Error!", 13, 10, '$'
_DATA   ENDS
_TEXT   SEGMENT 'CODE'
	ASSUME 	CS: _TEXT, DS: _DATA
START:  MOV 	AX, _DATA
	      MOV 	DS, AX
AGAIN:  MOV 	AH, 9H
	      MOV 	DX, OFFSET MSG
	      INT 	21H
	      MOV 	AH, 1H
	      INT 	21H
	      SUB 	AL, '0'
	      JL  	ERROR   ; 输入字符小于'0'的情况
	      CMP 	AL, 7H
	      JLE 	LOCL
	      JG    ERROR   ; 输入字符大于’7’的情况
ERROR:  MOV 	AL, 8H
LOCL:   MOV 	AH, 0
	      ADD 	AX, AX  ;AX×2
	      MOV 	BX, AX
	      JMP 	WORD PTR ADTABLE[BX]
CASE0:  MOV 	AX, 4C00H
	      INT 	21H
CASE1:  MOV 	DX, OFFSET S1
	      JMP 	ENDSWITCH
CASE2:  MOV 	DX, OFFSET S2
	      JMP 	ENDSWITCH
CASE3:  MOV 	DX, OFFSET S3
	      JMP 	ENDSWITCH
CASE4:  MOV 	DX, OFFSET S4
	      JMP 	ENDSWITCH
CASE5:  MOV 	DX, OFFSET S5
	      JMP 	ENDSWITCH
CASE6:  MOV 	DX, OFFSET S6
	      JMP 	ENDSWITCH
CASE7:  MOV 	DX, OFFSET S7
	      JMP 	ENDSWITCH
CASE_ERR:  MOV   DX, OFFSET ERR
	           JMP   ENDSWITCH
ENDSWITCH:  MOV AH, 9H
	          INT 	21H
	          JMP 	AGAIN
_TEXT   ENDS
	      END 	START
