;---------------------------------
;数据段
DATA   SEGMENT
       PROMPT   DB  "ATTENTION: The score of absent students is supposed to be ~. ",0DH,0AH,'$'
       PROMPT1  DB  "Please input the number of students in this class(10~60): $",0DH,0AH
       PROMPT21 DB  "Please input the score of the $"
       PROMPT22 DB  "th student: $",0DH,0AH
       PROMPT3  DB  "The max score is: $"
       PROMPT4  DB  "The min score is: $"
       PROMPT5  DB  "The average score is: $"
       PROMPT6  DB  "The total students num is: $"
       PROMPT7  DB  "The valid students num is: $"
       PROMPT8  DB  "Excellent-num: $"
       PROMPT9  DB  "G  o  o d-num: $"
       PROMPTA  DB  "M e d i u-num: $"
       PROMPTB  DB  "P  a  s s-num: $"
       PROMPTC  DB  "F  a  i l-num: $"
       ADDR     DW  PROMPTC,PROMPTB,PROMPTA,PROMPT9,PROMPT8
       PROMPTD  DB  "  percentage: $"
       ENDLINE  DB  0DH,0AH,'$'
       ERR      DB  "Error!!! There is no valid score to display!!!$",0DH,0AH
       ERRO     DB  0DH,0AH,"Error!!! The num is out of scope!!!$"
       NUM      DW  0H              ;用于存储学生的总数目
       NUMR     DW  0H              ;用于存储未缺席学生的数目
       BUFFER1  DB  4,?,4 DUP(?)    ;用于存储输入的数目字符  
       SCORE    DW  61 DUP(?)       ;用于存储学生的成绩
       BUFFER2  DB  10,?,10 DUP(?)  ;用于存储输入的成绩字符
       FAIL     DB   0H             ;数目不会超过一个字节
       PASS     DB   0H
       MEDIUM   DB   0H
       GOOD     DB   0H
       EXCE     DB   0H             ;ascil的'0'便于输出
       FPER     DW   0H
       PPER     DW   0H
       MPER     DW   0H
       GPER     DW   0H
       EPER     DW   0H             ;各个等级的百分比
       DIGIT    DB   5 DUP(?)       ;存储要保留的数的各个位数
       FLAG     DW   0H            
       PER      DW   0H  
DATA   ENDS

;---------------------------------
;代码段
CODE   SEGMENT
MAIN   PROC    FAR
       ASSUME  DS:DATA,CS:CODE
START:    
       PUSH   DS
       XOR    AX,AX
       PUSH   AX
       MOV    AX,DATA
       MOV    DS,AX                 ;准备
    
       CALL   INPUT                 ;输入信息
       
       LEA    DX,PROMPT6            
       MOV    AH,09H
       INT    21H
       MOV    AX,NUM
       MOV    DI,AX
       CALL   INTODEC               ;显示总学生数
       
       LEA    DX,ENDLINE
       MOV    AH,09H
       INT    21H                   ;换行，为了好看
 
       LEA    DX,PROMPT7
       MOV    AH,09H
       INT    21H
       MOV    AX,NUMR
       MOV    DI,AX
       CALL   INTODEC               ;显示未缺席学生数
       
       LEA    DX,ENDLINE
       MOV    AH,09H
       INT    21H                   ;换行，为了好看
       
       MOV    AX,NUMR
       MOV    CX,AX
       CALL   MAX                   ;显示最高分
       CALL   MIN                   ;显示最低分
       CALL   AVER                  ;显示平均分
       
       CMP    NUMR,0
       JE     ERROR
       
       CALL   STAT                  ;分析计算等级比例数据
       CALL   COUTNUM               ;显示等级以及比例
       JMP    FINAL
       
ERROR:
       LEA    DX,ERR
       MOV    AH,09H
       INT    21H
FINAL:
       RET
MAIN   ENDP

;---------------------------------
;子程序名：AVER
;功能：显示学生平均成绩
;入口参数：SCORE数组
;出口参数：显示学生成绩
;所占用寄存器：AX,DX,CX,BX,DI,SI
;注意：保留到小数点后一位
AVER   PROC   NEAR
       PUSH    AX
       PUSH    BX
       PUSH    CX
       PUSH    DX
       LEA     DX,PROMPT5
       MOV     AH,09H
       INT     21H
        
       MOV     CX,NUMR
       CMP     CX,0
       JE      QV
       JMP     AGAINN0
       
QV:
       PUSH    AX
       PUSH    DX
       MOV     DL,'~'
       MOV     AH,02H
       INT     21H
       POP     DX
       POP     AX
       JMP     QVF
       
AGAINN0:
       MOV     BX,OFFSET SCORE
       MOV     AX,0H
AGAINN: 
       ADD     BX,2
       PUSH    AX
       MOV     AX,'~'
       CMP     [BX-2],AX
       POP     AX
       JE      AGAINN
       ADD     AX,[BX-2]
       LOOP    AGAINN

       MOV     DX,0
       MOV     BX,100
       MUL     BX
       DIV     NUMR
       
       MOV     PER,AX
       CALL    COUTPER
       
QVF:
       LEA     DX,ENDLINE
       MOV     AH,09H
       INT     21H
       POP     DX
       POP     CX
       POP     BX
       POP     AX      
       RET
AVER   ENDP

;---------------------------------
;子程序名：INTODEC
;功能：两字节转化并输出10进制的子程序
;入口参数：DI
;出口参数：显示10进制的数字
;所占用寄存器：CX,BX,DX,AX
;示例：输入DI=64H，显示"100D"

INTODEC   PROC   NEAR     
       PUSH AX
       PUSH BX
       PUSH CX
       PUSH DX
          
       MOV  CX,0
       MOV  BX,10
       MOV  AX,DI
M:     MOV  DX,0
       CMP  AX,0
       JZ   DISP
       DIV  BX
       PUSH DX
       INC  CX
       JMP  M
DISP:
       CMP  CX,0
       JNZ  DISP0
       MOV  DL,'0'
       MOV  AH,02H
       INT  21H          
DISP0:
       CMP  CX,0
       JZ   RE
       POP  DX
       ADD  DX,30H
       MOV  DH,0
       MOV  AH,02H
       INT  21H
       DEC  CX
       JMP  DISP0
RE:       
       POP  DX
       POP  CX
       POP  BX
       POP  AX
       RET
INTODEC   ENDP

;---------------------------------
;子程序名：INPUT
;功能：输入学生数目以及学生成绩的子程序
;入口参数：无
;出口参数：NUM:总学生数
;          NUMR:未缺席学生数
;          SCORE[2*i-2][2*i-1]:第i个学生的成绩，若缺席，则用‘~’的ASCII码表示
;所占用寄存器：AX,DX,CX,BX,DI,SI
;注意：学生数目指所有的学生数目（包含缺席的学生）
;      输入学生成绩时，“~”代表该学生缺席考试

INPUT  PROC    NEAR
       JMP     NEXTT
NEXTT0:
       PUSH    AX
       PUSH    DX
       LEA     DX,ERRO
       MOV     AH,09H
       INT     21H
       POP     DX
       POP     AX        
NEXTT:
       LEA     DX,ENDLINE
       MOV     AH,09H
       INT     21H            ;换行
       LEA     DX,PROMPT1
       INT     21H            ;输入提示
       LEA     DX,BUFFER1
       MOV     AH,0AH
       INT     21H            ;输入学生总数
       CMP     [BUFFER1+1],2H  
       JNZ     NEXTT0         ;总数一定是两位数
       CMP     [BUFFER1+2],36H
       JG      NEXTT0         ;先讨论是60的情况
       JE      L3             
       JL      L4             
L3:    
       CMP     [BUFFER1+3],30H
       JNZ     NEXTT0         ;十位是6，个位只能是0
L4:   
       SUB     [BUFFER1+2],30H
       SUB     [BUFFER1+3],30H 
       CMP     [BUFFER1+2],1
       JL      NEXTT0         ;十位小于一
       CMP     [BUFFER1+3],0
       JL      NEXTT0         ;个位小于零
       CMP     [BUFFER1+3],9
       JG      NEXTT0         ;个位大于九      
       MOV     AL,BUFFER1+2
       MOV     BL,10          
       MUL     BL             
       ADD     AL,[BUFFER1+3]
       CBW
       MOV     NUM,AX
       MOV     NUMR,AX
       CBW
       MOV     CX,AX       ;循环次数
       
       LEA     DX,ENDLINE
       MOV     AH,09H
       INT     21H         ;换行，为了好看
       
;      以上均为输入学生总数目的错误处理以及最终的合法输入
       PUSH    DX
       PUSH    AX
       LEA     DX,PROMPT
       MOV     AH,09H
       INT     21H
       POP     AX
       POP     DX    
       
       MOV     SI,-2
       MOV     DI,0
L5:
       CMP     CX,0
       JE      L7
       
       INC     DI
       ADD     SI,2
       DEC     CX
L6:
       CALL    PROM        ;输入
       
       MOV     FLAG,0      ;初始化
       CALL    JUDGEIN     ;检查
       
       LEA     DX,ENDLINE
       MOV     AH,09H
       INT     21H         ;换行
       
       CMP     FLAG,1
       
       JE      L5
       JMP     L6
       
;      以上均为输入各个学生的成绩的错误处理以及最终的合法输入

L7:       
       RET
       INPUT   ENDP
       
;-----------------------------------
;子程序名：STAT
;功能：统计成绩等级以及计算比例
;入口参数：SCORE数组
;出口参数：FAIL人数数组，FPER比例数组
;所占用寄存器：AX,BX,CX,DX,SI,DI
;示例：

STAT   PROC    NEAR
       PUSH    AX
       PUSH    BX
       PUSH    CX
       PUSH    DX
       PUSH    SI
       PUSH    DI   
       MOV     SI,0
G1:    
       MOV     AX,SCORE[SI]        ;把分数给AX
       CMP     AX,'~'
       JE      G4
       MOV     BX,10               ;被除数
       DIV     BL                  ;无符号除法[(AL)=(AX)/SRC,(AH)=(AX)%SRC]被除数为8位
                                   ;[这里获得分数的十位数]
       MOV     BL,AL               ;把分数的十位数字放在BL中
       SUB     BX,5                ;把十位数字减5
       CMP     BX,0
       JLE     G2
       CMP     BX,5
       JE      G6
       JMP     G3
G2:
       MOV     BX,0
       JMP     G3
G6:
       MOV     BX,4
       JMP     G3
G3:
       ADD     FAIL[BX],1          ;对应的等加的人数加一
G4:
       ADD     SI,2                ;因为是DW型要加2才会取出GOALS的下一个
       LOOP    G1                  ;LOOP终止条件CX=0,所以上面CX先赋值10,每次循环自减
       
;以上统计成绩等级人数
       
       MOV     SI,0
       MOV     DI,0
       MOV     CX,5
G5:
       MOV     DX,0
       MOV     AL,FAIL[SI]
       CBW
       MOV     BX,10000            
       MUL     BX                  ;乘以10000精确到百分号前面的一位小数
       DIV     NUMR                ;除以有效人数保存在AX中
       
       MOV     FPER[DI],AX
       INC     SI
       ADD     DI,2
       LOOP    G5
      
;以上计算成绩等级比例
       POP     DI
       POP     SI
       POP     DX
       POP     CX
       POP     BX
       POP     AX
       
       RET
       STAT    ENDP
       
;----------------------------------
;子程序名：COUTNUM
;功能：输出成绩等级以及各个成绩等级比例的子程序
;入口参数：
;出口参数：
;所占用寄存器：AX,CX,DX,SI,DI
;示例：
   
COUTNUM   PROC    NEAR    
       PUSH    AX
       PUSH    CX
       PUSH    DX
       PUSH    SI
       PUSH    DI  
       MOV     SI,0
       MOV     DI,0
       MOV     CX,5
CONTINUE:      
       
       MOV     DX,ADDR[DI]
       MOV     AH,09H
       INT     21H
       
       MOV     AL,FAIL[SI];用AX转成字，注意保护AX
       CBW
       PUSH    DI
       MOV     DI,AX
       CALL    INTODEC
       POP     DI
       
       LEA     DX,PROMPTD
       MOV     AH,09H
       INT     21H
       
       PUSH    AX
       MOV     AX,FPER[DI]
       MOV     PER,AX
       POP     AX
       CALL    COUTPER    ;显示各个等级的比例
       MOV  DL,'%'
       MOV  AH,02H
       INT  21H  
       
       LEA     DX,ENDLINE
       MOV     AH,09H
       INT     21H
       
       ADD     DI,2
       INC     SI
       LOOP    CONTINUE
       
       POP    DI
       POP    SI
       POP    DX
       POP    CX
       POP    AX

       RET
COUTNUM  ENDP
       
;---------------------------------
;子程序名：MAX
;功能：显示最大值
;入口参数：SCORE数组
;出口参数：显示最大值
;所占用寄存器：DX,AX,CX,BX,
;示例：

MAX     PROC    NEAR
        PUSH    AX
        PUSH    BX
        PUSH    CX
        PUSH    DX
        LEA     DX,PROMPT3
        MOV     AH,09H
        INT     21H
        
        MOV     CX,NUMR
        CMP     CX,0
        JE      QA
        JMP     AGAIN0
QA:
        PUSH    AX
        PUSH    DX
        MOV     DL,'~'
        MOV     AH,02H
        INT     21H
        POP     DX
        POP     AX
        JMP     QAF

AGAIN0:
        MOV     BX,OFFSET SCORE
        MOV     AX,0H;最小的无符号数
AGAIN:  
        ADD     BX,2
        PUSH    AX
        MOV     AX,'~'
        CMP     [BX-2],AX
        POP     AX
        JE      AGAIN
        CMP     AX,[BX-2]
        JAE     NEXT
        MOV     AX,[BX-2]
NEXT:   
        LOOP    AGAIN

        MOV     DI,AX
        CALL    INTODEC
QAF:
        LEA     DX,ENDLINE
        MOV     AH,09H
        INT     21H
        POP     DX
        POP     CX
        POP     BX
        POP     AX      
        RET
MAX     ENDP

;---------------------------------
;子程序名：MIN
;功能：显示最小值
;入口参数：SCORE数组
;出口参数：显示最小值
;所占用寄存器：DX,AX,CX,BX,DI
;示例：

MIN     PROC    NEAR
        PUSH    AX
        PUSH    BX
        PUSH    CX
        PUSH    DX
        PUSH    DI
        LEA     DX,PROMPT4
        MOV     AH,09H
        INT     21H
        
        MOV     CX,NUMR
        CMP     CX,0
        JE      QI
        JMP     AAGAIN0
QI:
        PUSH    AX
        PUSH    DX
        MOV     DL,'~'
        MOV     AH,02H
        INT     21H
        POP     DX
        POP     AX
        JMP     QIF
        
AAGAIN0:        
        MOV     BX,OFFSET SCORE
        MOV     AX,0FFFFH;最大的无符号数
AAGAIN: 
        ADD     BX,2
        PUSH    AX
        MOV     AX,'~'
        CMP     [BX-2],AX
        POP     AX
        JE      AAGAIN
        CMP     AX,[BX-2]
        JBE     NNEXT
        MOV     AX,[BX-2]
NNEXT:  
        LOOP    AAGAIN
        MOV     DI,AX
        CALL    INTODEC
QIF:
        LEA     DX,ENDLINE
        MOV     AH,09H
        INT     21H
        POP     DI
        POP     DX
        POP     CX
        POP     BX
        POP     AX      
        RET
MIN     ENDP

;---------------------------------
;子程序名：PROM
;功能：一些输入提示
;入口参数：
;出口参数：
;所占用寄存器：DX,AX
;示例：

PROM   PROC    NEAR
       PUSH    AX
       PUSH    DX
       LEA     DX,PROMPT21
       MOV     AH,09H
       INT     21H
       
       CALL    INTODEC
       
       LEA     DX,PROMPT22
       INT     21H
       
       LEA     DX,BUFFER2
       MOV     AH,0AH
       INT     21H
       POP     DX
       POP     AX
       RET 
PROM   ENDP

;---------------------------------
;子程序名：JUDGEIN
;功能：判断每一次输入学生成绩的合法性
;入口参数：无
;出口参数：FLAG为0表示不合法，为1表示合法
;所占用寄存器：AX,BX,CX,DX,DI,SI
;注意：必须保证输入的每一个字符都是数字
;      必须保证输入的数字在0~100范围
;      特殊处理缺席的学生成绩，以‘~’代替

JUDGEIN PROC NEAR 
       PUSH    AX
       PUSH    BX                    
       PUSH    CX
       PUSH    DX
       PUSH    SI
       PUSH    DI
       
       MOV     CH,0
       MOV     CL,BUFFER2+1
       MOV     DI,-1
N:
       INC     DI
       CMP     [BUFFER2+2+DI],'0'
       JL      N5
       CMP     [BUFFER2+2+DI],'9'
       JG      N5
       LOOP    N
       
       JMP     N7
N5:
       CMP     [BUFFER2+2+DI],'~'
       JNE     FINA
       CMP     [BUFFER2+1],1
       JNE     FINA
       JE      N6
N6:
       DEC     NUMR
       MOV     FLAG,1
       MOV     AX,'~'
       MOV     SCORE[SI],AX
       JMP     FINA
N7:    
       MOV     CH,0
       MOV     CL,BUFFER2+1
       MOV     DI,-1
       MOV     AX,0
       MOV     BX,10
NN:
       INC     DI
       MUL     BX
       MOV     DX,0
       MOV     DL,[BUFFER2+2+DI]
       SUB     DL,'0'
       ADD     AX,DX
       LOOP    NN
       
       CMP     AX,0
       JL      FINA
       CMP     AX,100
       JG      FINA
      
       MOV     SCORE[SI],AX
       MOV     FLAG,1
FINA:
       CMP     FLAG,0
       JNE     FFINA
       LEA     DX,ERRO
       MOV     AH,09H
       INT     21H
FFINA:
       POP     DI
       POP     SI
       POP     DX
       POP     CX
       POP     BX
       POP     AX
       RET
JUDGEIN ENDP

;---------------------------------
;子程序名：COUTPER
;功能：显示比例（精确到小数点后一位）
;入口参数：
;出口参数：
;所占用寄存器：
;示例：
COUTPER  PROC  NEAR
       PUSH    SI
       PUSH    CX
       MOV     CX,5
       MOV     SI,-1
Q0:
       INC     SI
       MOV     DIGIT[SI],0
       LOOP    Q0         ;清零DIGIT元素
       POP     CX
       POP     SI
       
       PUSH AX
       PUSH BX
       PUSH SI
       PUSH DX
       MOV  SI,0
       MOV  BX,10
       MOV  AX,PER
Q1:    
       MOV  DX,0
       CMP  AX,0
       JZ   QDISP0
       DIV  BX
       MOV  DIGIT[SI],DL  ;由最低位开始输入
       INC  SI            ;SI的数目代表非零位数
       JMP  Q1
QDISP0:
       CMP  SI,0
       JNZ  QDISP
       MOV  DL,'0'
       MOV  AH,02H
       INT  21H
       JMP  QRE           
QDISP: 
       PUSH    AX
       MOV     AL,DIGIT
       CBW
       CMP     AX,5H
       POP     AX
       JNL     Q2
       JMP     Q3
Q2:
       ADD     [DIGIT+1],1H
Q3:
       CMP  SI,2
       JLE  Q4
       JMP  Q5
Q4:    
       MOV  SI,3
Q5:
       DEC  SI
       CMP  SI,0
       JZ   QRE
       CMP  SI,1
       JNZ  Q6
       MOV  DL,'.'
       MOV  AH,02H
       INT  21H
Q6:
       MOV  DL,DIGIT[SI]
       ADD  DL,30H
       MOV  AH,02H
       INT  21H
       JMP  Q5
QRE:       
       POP  DX
       POP  SI
       POP  BX
       POP  AX
 
       RET
COUTPER  ENDP

CODE   ENDS
       END     START