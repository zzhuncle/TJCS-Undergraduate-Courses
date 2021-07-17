`timescale 1ns / 1ps

`define ADD      0
`define ADDU     1
`define SUB      2
`define SUBU     3
`define AND      4
`define OR       5
`define XOR      6
`define NOR      7
`define SLT      8
`define SLTU     9
`define SLL      10
`define SRL      11
`define SRA      12
`define SLLV     13
`define SRLV     14
`define SRAV     15
`define JR       16
`define ADDI     17
`define ADDIU    18
`define ANDI     19
`define ORI      20
`define XORI     21
`define LW       22
`define SW       23
`define BEQ      24
`define BNE      25
`define SLTI     26
`define SLTIU    27
`define LUI      28
`define J        29
`define JAL      30
`define CLZ      31
`define DIVU     32
`define ERET     33
`define JALR     34
`define LB       35
`define LBU      36
`define LHU      37
`define SB       38
`define SH       39
`define LH       40
`define MFC0     41
`define MFHI     42
`define MFLO     43
`define MTC0     44
`define MTHI     45
`define MTLO     46
`define MUL      47
`define MULTU    48
`define SYSCALL  49
`define TEQ      50
`define BGEZ     51
`define BREAK    52
`define DIV      53

`define ALU_ADDU    4'b0000
`define ALU_ADD     4'b0010
`define ALU_SUBU    4'b0001
`define ALU_SUB     4'b0011
`define ALU_AND     4'b0100
`define ALU_OR      4'b0101
`define ALU_XOR     4'b0110
`define ALU_NOR     4'b0111
`define ALU_LUI     4'b100x
`define ALU_SLT     4'b1011
`define ALU_SLTU    4'b1010
`define ALU_SRA     4'b1100
`define ALU_SLL_A   4'b111x
`define ALU_SRL     4'b1101

//控制器
module operation(
    input alu_z,        //beq bne
    input alu_n,        //bgez
    input equal,        //teq
    input [63 : 0] instr_index,  
    output [2 : 0] M1,
	output reg [2 : 0] M2,
	output reg M3,
	output reg M4,
	output reg M5,
    output reg M6,
    output reg M7,
    output reg M8,
    output reg Hwe,
    output reg Lwe,
    output reg [1 : 0] MDX, //div divu mul multu
    output exception,
    output eret,
    output mfc0,
    output mtc0,
    output reg [4 : 0] cause,
    output reg [3 : 0] aluc,
    output RF_W,
    output DM_W,
    output reg [1 : 0] DMS_mux,
    output reg [2 : 0] DML_mux,
    output EXT_SIGN
    );
							
    assign exception = instr_index[`SYSCALL] | (instr_index[`TEQ] & equal) | instr_index[`BREAK];
    assign eret = instr_index[`ERET];
    assign mfc0 = instr_index[`MFC0];
    assign mtc0 = instr_index[`MTC0];
	
	//MDX
    always @ (*) begin
        if (instr_index[`MUL])
            MDX = 2'b00;
        else if (instr_index[`MULTU])
            MDX = 2'b01;
        else if (instr_index[`DIV])
            MDX = 2'b10;
        else if (instr_index[`DIVU])
            MDX = 2'b11;
        else 
            MDX = 2'bx;
    end

    //cause
    always @ (*) begin
        if (instr_index[`SYSCALL])
            cause = 5'b01000;
        else if ((instr_index[`TEQ] & equal))
            cause = 5'b01101;
        else if (instr_index[`BREAK])
            cause = 5'b01001;
        else 
            cause = 5'bx;
    end

    //DMS_mux
    always @ (*) begin
        if (instr_index[`SW])
            DMS_mux = 2'b00;
        else if (instr_index[`SH])
            DMS_mux = 2'b01;
        else if (instr_index[`SB])
            DMS_mux = 2'b10;
        else 
            DMS_mux = 2'bx;
    end
    
	//DML_mux
    always @ (*) begin
        if (instr_index[`LW])
            DML_mux = 3'b000;
        else if (instr_index[`LH])
            DML_mux = 3'b001;
        else if (instr_index[`LHU])
            DML_mux = 3'b010;
        else if (instr_index[`LB])
            DML_mux = 3'b011;
        else if (instr_index[`LBU])
            DML_mux = 3'b100;
        else 
            DML_mux = 3'bx;
    end

    //ALUC
    always @ (*) begin
        if (instr_index[`ADD] | instr_index[`ADDI] | instr_index[`LW] | instr_index[`SW] | instr_index[`LB] | instr_index[`LBU] | 
		    instr_index[`LHU] | instr_index[`LH] | instr_index[`SH] | instr_index[`SB])
            aluc = `ALU_ADD;
        else if (instr_index[`ADDU] | instr_index[`ADDIU])
            aluc = `ALU_ADDU;
        else if (instr_index[`SUB] | instr_index[`BEQ] | instr_index[`BNE])
            aluc = `ALU_SUB;
        else if (instr_index[`SUBU])
            aluc = `ALU_SUBU;
        else if (instr_index[`AND] | instr_index[`ANDI])
            aluc = `ALU_AND;
        else if (instr_index[`OR] | instr_index[`ORI])
            aluc = `ALU_OR;
        else if (instr_index[`XOR] | instr_index[`XORI])
            aluc = `ALU_XOR;
        else if (instr_index[`NOR])
            aluc = `ALU_NOR;
        else if (instr_index[`SLT] | instr_index[`SLTI])
            aluc = `ALU_SLT;
        else if (instr_index[`SLTU] | instr_index[`SLTIU])
            aluc = `ALU_SLTU;
        else if (instr_index[`SLL] | instr_index[`SLLV])
            aluc = `ALU_SLL_A;
        else if (instr_index[`SRL] | instr_index[`SRLV])
            aluc = `ALU_SRL;
        else if (instr_index[`SRA] | instr_index[`SRAV])
            aluc = `ALU_SRA;
        else if (instr_index[`LUI])
            aluc = `ALU_LUI;
        else 
            aluc = 4'bx;
    end
    
	//M1
	assign M1[0] = (instr_index[`BEQ] & alu_z) | (instr_index[`BNE] & ~alu_z) | (instr_index[`JAL : `J] != 0) | 
	                instr_index[`SYSCALL] | (instr_index[`TEQ] & equal) | instr_index[`BREAK] | (instr_index[`BGEZ] & ~alu_n);
    assign M1[1] = instr_index[`JR] | (instr_index[`BEQ] & alu_z) | (instr_index[`BNE] & ~alu_z) | instr_index[`JALR] | (instr_index[`BGEZ] & ~alu_n);
    assign M1[2] = instr_index[`ERET] | instr_index[`SYSCALL] | (instr_index[`TEQ] & equal) | instr_index[`BREAK];

    //M2
    always @ (*) begin
        if (instr_index[`SRAV : `ADD] != 0 | instr_index[`XORI : `ADDI] != 0 | instr_index[`LUI : `SLTI] != 0)
            M2 = 3'b000;
        else if (instr_index[`LW] | instr_index[`LB] | instr_index[`LBU] | instr_index[`LH] | instr_index[`LHU])
            M2 = 3'b001;
        else if (instr_index[`JAL] | instr_index[`JALR])
            M2 = 3'b010;
        else if (instr_index[`MFC0])
            M2 = 3'b011;
        else if (instr_index[`MFLO])
            M2 = 3'b100;
        else if (instr_index[`MFHI])
            M2 = 3'b101;
        else if (instr_index[`MUL])
            M2 = 3'b110;
        else if (instr_index[`CLZ])
            M2 = 3'b111;
        else 
            M2 = 3'bx;
    end
	
	//M3
	always @ (*) begin
        if (instr_index[`SRAV : `ADD] != 0 | instr_index[`CLZ] | instr_index[`JALR] | instr_index[`MTC0 : `MFHI] != 0 | instr_index[`MUL])
            M3 = 1'b0;
        else if (instr_index[`SW : `ADDI] != 0 | instr_index[`LUI : `SLTI] != 0 | instr_index[`MFC0 : `LB] != 0)
            M3 = 1'b1;
        else 
            M3 = 1'bx;
    end
	
	//M4
    always @ (*) begin
        if (instr_index[`SLLV] | instr_index[`SRLV] | instr_index[`SRAV])
            M4 = 1'b1;
        else if (instr_index[`SLL] | instr_index[`SRL] | instr_index[`SRA])
            M4 = 1'b0;
        else 
            M4 = 1'bx;
    end

    //M5
    always @ (*) begin
        if (instr_index[`SLTU : `ADD] != 0 | instr_index[`SLTIU : `ADDI] != 0 | instr_index[`LH : `LB] != 0 | instr_index[`BGEZ])
            M5 = 1'b1;
        else if (instr_index[`SRAV : `SLL] != 0)
            M5 = 1'b0;
        else 
            M5 = 1'bx;
    end

    //M6
    always @ (*) begin
        if (instr_index[`SRAV : `ADD] != 0 | instr_index[`BNE : `BEQ] != 0)
            M6 = 1'b0;
        else if (instr_index[`SW : `ADDI] != 0 | instr_index[`LUI : `SLTI] != 0 | instr_index[`LH : `LB] != 0)
            M6 = 1'b1;
        else 
            M6 = 1'bx;
    end
    
	//M7
    always @ (*) begin
        if (instr_index[`DIVU] | instr_index[`MULTU] | instr_index[`DIV]) begin
            M7 = 1'b1;
            Hwe = 1'b1;
        end else if (instr_index[`MTHI]) begin
            M7 = 1'b0;
            Hwe = 1'b1;
        end else begin
            M7 = 1'bx;
            Hwe = 1'b0;
        end
    end
    
	//M8
    always @ (*) begin
        if (instr_index[`DIVU] | instr_index[`MULTU] | instr_index[`DIV]) begin
            M8 = 1'b1;
            Lwe = 1'b1;
        end else if (instr_index[`MTLO]) begin
            M8 = 1'b0;
            Lwe = 1'b1;
        end else begin
            M8 = 1'bx;
            Lwe = 1'b0;
        end
    end

    assign DM_W = instr_index[`SW] | instr_index[`SB] | instr_index[`SH];
    assign RF_W = ~(instr_index[`JR] | instr_index[`SW] | instr_index[`BEQ] | instr_index[`BNE] | instr_index[`J] | instr_index[`DIVU] | instr_index[`ERET] |
                 	instr_index[`SB] | instr_index[`SH] | instr_index[`MTLO : `MTC0] != 0 | instr_index[`DIV : `MULTU] != 0);
    assign EXT_SIGN = ~(instr_index[`ANDI] | instr_index[`ORI] | instr_index[`XORI]);
endmodule