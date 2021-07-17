`timescale 1ns / 1ps

//指令译码器
module instr_dec(
    input [31 : 0] instr_code,
	output reg [64 : 0] instr_index
    );
	wire [11 : 0] temp;
	wire [4 : 0] rs;
	
	assign temp = {instr_code[31 : 26], instr_code[5 : 0]};
	assign rs = instr_code[25 : 21];
	
	always @ (*) begin
		casez(temp)
		    //R-type
	        12'b000000100000 : instr_index <= 64'h0000_0000_0000_0001;//0 add
            12'b000000100001 : instr_index <= 64'h0000_0000_0000_0002;//1 addu
            12'b000000100010 : instr_index <= 64'h0000_0000_0000_0004;//2 sub
            12'b000000100011 : instr_index <= 64'h0000_0000_0000_0008;//3 subu
            12'b000000100100 : instr_index <= 64'h0000_0000_0000_0010;//4 and
            12'b000000100101 : instr_index <= 64'h0000_0000_0000_0020;//5 or
            12'b000000100110 : instr_index <= 64'h0000_0000_0000_0040;//6 xor
            12'b000000100111 : instr_index <= 64'h0000_0000_0000_0080;//7 nor
            12'b000000101010 : instr_index <= 64'h0000_0000_0000_0100;//8 slt
            12'b000000101011 : instr_index <= 64'h0000_0000_0000_0200;//9 sltu
            12'b000000000000 : instr_index <= 64'h0000_0000_0000_0400;//10 sll
            12'b000000000010 : instr_index <= 64'h0000_0000_0000_0800;//11 srl
            12'b000000000011 : instr_index <= 64'h0000_0000_0000_1000;//12 sra
            12'b000000000100 : instr_index <= 64'h0000_0000_0000_2000;//13 sllv
            12'b000000000110 : instr_index <= 64'h0000_0000_0000_4000;//14 srlv
            12'b000000000111 : instr_index <= 64'h0000_0000_0000_8000;//15 srav
            12'b000000001000 : instr_index <= 64'h0000_0000_0001_0000;//16 jr
			//I-type
            12'b001000?????? : instr_index <= 64'h0000_0000_0002_0000;//17 addi
            12'b001001?????? : instr_index <= 64'h0000_0000_0004_0000;//18 addiu
            12'b001100?????? : instr_index <= 64'h0000_0000_0008_0000;//19 andi
            12'b001101?????? : instr_index <= 64'h0000_0000_0010_0000;//20 ori
            12'b001110?????? : instr_index <= 64'h0000_0000_0020_0000;//21 xori
            12'b100011?????? : instr_index <= 64'h0000_0000_0040_0000;//22 lw
            12'b101011?????? : instr_index <= 64'h0000_0000_0080_0000;//23 sw
            12'b000100?????? : instr_index <= 64'h0000_0000_0100_0000;//24 beq
            12'b000101?????? : instr_index <= 64'h0000_0000_0200_0000;//25 bne
            12'b001010?????? : instr_index <= 64'h0000_0000_0400_0000;//26 slti
            12'b001011?????? : instr_index <= 64'h0000_0000_0800_0000;//27 sltiu
            12'b001111?????? : instr_index <= 64'h0000_0000_1000_0000;//28 lui
			//J-type
            12'b000010?????? : instr_index <= 64'h0000_0000_2000_0000;//29 j
            12'b000011?????? : instr_index <= 64'h0000_0000_4000_0000;//30 jal
			//23条扩展指令
            12'b011100100000 : instr_index <= 64'h0000_0000_8000_0000;//31 clz
            12'b000000011011 : instr_index <= 64'h0000_0001_0000_0000;//32 divu
            12'b010000011000 : instr_index <= 64'h0000_0002_0000_0000;//33 eret
            12'b000000001001 : instr_index <= 64'h0000_0004_0000_0000;//34 jalr
            12'b100000?????? : instr_index <= 64'h0000_0008_0000_0000;//35 lb
            12'b100100?????? : instr_index <= 64'h0000_0010_0000_0000;//36 lbu
            12'b100101?????? : instr_index <= 64'h0000_0020_0000_0000;//37 lhu
            12'b101000?????? : instr_index <= 64'h0000_0040_0000_0000;//38 sb
            12'b101001?????? : instr_index <= 64'h0000_0080_0000_0000;//39 sh
            12'b100001?????? : instr_index <= 64'h0000_0100_0000_0000;//40 lh
            12'b010000000000 : 
				begin
					if (rs == 5'b00000)
						instr_index <= 64'h0000_0200_0000_0000;    //41 mfc0
					else if (rs == 5'b00100)
						instr_index <= 64'h0000_1000_0000_0000;    //44 mtc0
					else 
						instr_index <= 64'bx;
				end
            12'b000000010000 : instr_index <= 64'h0000_0400_0000_0000;//42 mfhi
            12'b000000010010 : instr_index <= 64'h0000_0800_0000_0000;//43 mflo
            12'b000000010001 : instr_index <= 64'h0000_2000_0000_0000;//45 mthi
            12'b000000010011 : instr_index <= 64'h0000_4000_0000_0000;//46 mtlo
            12'b011100000010 : instr_index <= 64'h0000_8000_0000_0000;//47 mul
            12'b000000011001 : instr_index <= 64'h0001_0000_0000_0000;//48 multu
            12'b000000001100 : instr_index <= 64'h0002_0000_0000_0000;//49 syscall
            12'b000000110100 : instr_index <= 64'h0004_0000_0000_0000;//50 teq
            12'b000001?????? : instr_index <= 64'h0008_0000_0000_0000;//51 bgez
            12'b000000001101 : instr_index <= 64'h0010_0000_0000_0000;//52 break
            12'b000000011010 : instr_index <= 64'h0020_0000_0000_0000;//53 div
            default : instr_index <= 64'bx;
        endcase
	end			
endmodule