
`timescale 1ns / 1ps

module Operation (
    input isBranch,
    input [31:0] instruction,
    input [5:0] Op,
    input [5:0] Func,
    input [31:0] status,   
    output rfWena,     
    output hiWena,
    output loWena,
    output dmemWena, 
    output rfRena1,
    output rfRena2,
    output clzEna,
    output mulEna,
    output divEna,
    output dmemEna,
    output [1:0] dmemWCs,
    output [1:0] dmemRCs,
    output ext16Sign,
    output cutterSign,
    output mulSign,
    output divSign,
    output [3:0] aluc,
    output [4:0] rd,
    output mfc0,
    output mtc0,
    output eret,
    output exception,
    output [4:0] cp0Addr,
    output [4:0] cause,
    output ext5MuxSel,
    output cutterMuxSel,
    output aluMux1Sel,       
    output [1:0] aluMux2Sel,
    output [1:0] hiMuxSel,
    output [1:0] loMuxSel,
    output [2:0] cutterSel,
    output [2:0] rfMuxSel,
    output [2:0] pcMuxSel
    );

    wire Addi = (Op == 6'b001000);
    wire Addiu = (Op == 6'b001001);
    wire Andi = (Op == 6'b001100);
    wire Ori = (Op == 6'b001101);
    wire Sltiu = (Op == 6'b001011);
	wire Div = (Op == 6'b000000 && Func == 6'b011010);
    wire Lui = (Op == 6'b001111);
    wire Xori = (Op == 6'b001110);
    wire Slti = (Op == 6'b001010);
    wire Addu = (Op == 6'b000000 && Func==6'b100001);
    wire And = (Op == 6'b000000 && Func == 6'b100100);
	wire Break = (Op == 6'b000000 && Func == 6'b001101);
    wire Beq = (Op == 6'b000100);
    wire Bne = (Op == 6'b000101);
    wire J = (Op == 6'b000010);
    wire Jal = (Op == 6'b000011);
	wire Bgez = (Op == 6'b000001);
    wire Jr = (Op == 6'b000000 && Func == 6'b001000);
    wire Lw = (Op == 6'b100011);
    wire Xor = (Op == 6'b000000 && Func == 6'b100110);
    wire Nor = (Op == 6'b000000 && Func == 6'b100111);
    wire Or = (Op == 6'b000000 && Func == 6'b100101);
	wire Teq = (Op == 6'b000000 && Func == 6'b110100);
    wire Sll = (Op == 6'b000000 && Func == 6'b000000);
    wire Sllv = (Op == 6'b000000 && Func == 6'b000100);
    wire Sltu = (Op == 6'b000000 && Func == 6'b101011);
    wire Sra = (Op == 6'b000000 && Func == 6'b000011);
    wire Srl = (Op == 6'b000000 && Func == 6'b000010);
    wire Subu = (Op == 6'b000000 && Func == 6'b100011);
	wire Syscall = (Op == 6'b000000 && Func== 6'b001100);
    wire Sw = (Op == 6'b101011);
    wire Add = (Op == 6'b000000 && Func == 6'b100000);
    wire Sub = (Op == 6'b000000 && Func == 6'b100010);
    wire Slt = (Op == 6'b000000 && Func == 6'b101010);
    wire Srlv = (Op == 6'b000000 && Func == 6'b000110);
    wire Srav = (Op == 6'b000000 && Func == 6'b000111);
	wire Multu = (Op == 6'b000000 && Func == 6'b011001);
    wire Clz = (Op == 6'b011100 && Func == 6'b100000);
    wire Divu = (Op == 6'b000000 && Func == 6'b011011);
    wire Eret = (Op == 6'b010000 && Func == 6'b011000);
    wire Jalr = (Op == 6'b000000 && Func == 6'b001001);
	wire Mul = (Op == 6'b011100 && Func == 6'b000010);
    wire Lb = (Op == 6'b100000);
    wire Lbu = (Op == 6'b100100);
    wire Lhu = (Op == 6'b100101);
    wire Sb = (Op == 6'b101000);
	wire Mtlo = (Op == 6'b000000 && Func == 6'b010011);
    wire Sh = (Op == 6'b101001);
    wire Lh = (Op == 6'b100001);
    wire Mfc0 = (instruction[31:21] == 11'b01000000000 && instruction[10:3]==8'b00000000);
    wire Mfhi = (Op == 6'b000000 && Func == 6'b010000);
	wire Mthi = (Op == 6'b000000 && Func == 6'b010001);
    wire Mflo = (Op == 6'b000000 && Func == 6'b010010);
    wire Mtc0 = (instruction[31:21] == 11'b01000000100 && instruction[10:3]==8'b00000000);
    
	// 使能信号
    assign rfRena1 = Addi + Addiu + Andi + Ori + Sltiu + Xori + Slti + Addu + And + Beq + Bne + Jr + Lw + Xor + Nor + Or + Sllv + Sltu + Subu + Sw + Add + Sub + Slt + Srlv + Srav + Clz + Divu + Jalr + Lb + Lbu + Lhu + Sb + Sh + Lh + Mul + Multu + Teq + Div;
    assign rfRena2 = Addu + And + Beq + Bne + Xor + Nor + Or + Sll + Sllv + Sltu + Sra + Srl + Subu + Sw + Add + Sub + Slt + Srlv + Srav + Divu + Sb + Sh + Mtc0 + Mul + Multu + Teq + Div;
    assign hiWena = Div + Divu + Multu + Mthi + Mul;
    assign loWena = Div + Divu + Multu + Mtlo + Mul;
    assign rfWena = Addi + Addiu + Andi + Ori + Sltiu + Lui + Xori + Slti + Addu + And + Xor + Nor + Or + Sll + Sllv + Sltu + Sra + Srl + Subu + Add + Sub + Slt + Srlv + Srav + Lb + Lbu + Lh + Lhu + Lw + Mfc0 + Clz + Jal + Jalr + Mfhi + Mflo + Mul;
    assign clzEna = Clz;
    assign mulEna = Mul + Multu;
    assign divEna = Div + Divu;                                     

	// 数据寄存器
    assign dmemWena = Sb + Sh + Sw;
    assign dmemEna = Lw + Sw + Sb + Sh + Lb + Lh + Lhu + Lbu;
    assign dmemWCs[1] = Sh + Sb;
    assign dmemWCs[0] = Sw + Sb;
    assign dmemRCs[1] = Lh + Lb + Lhu + Lbu;
    assign dmemRCs[0] = Lw + Lb + Lbu;     

	// 符号信号
    assign cutterSign = Lb + Lh;
    assign mulSign = Mul;
    assign divSign = Div;
    assign ext16Sign = Addi + Addiu + Sltiu + Slti;
    
	// ext5选择信号
    assign ext5MuxSel = Sllv + Srav + Srlv;

	// alu选择信号
    assign aluMux1Sel = ~(Sll + Srl + Sra + Div + Divu + Mul + Multu + J + Jr + Jal + Jalr + Mfc0 + Mtc0 + Mfhi + Mflo + Mthi + Mtlo + Clz + Eret + Syscall + Break);
    assign aluMux2Sel[1] = Bgez;
    assign aluMux2Sel[0] = Slti + Sltiu + Addi + Addiu + Andi + Ori + Xori + Lb + Lbu + Lh + Lhu + Lw + Sb + Sh + Sw + Lui;

	// 运算器
    assign aluc[3] = Slt + Sltu + Sllv + Srlv + Srav + Lui + Srl + Sra + Slti + Sltiu + Sll;
    assign aluc[2] = And + Or + Xor + Nor + Sll + Srl + Sra + Sllv + Srlv + Srav + Andi + Ori + Xori;
    assign aluc[1] = Add + Sub + Xor + Nor + Slt + Sltu + Sll + Sllv + Addi + Xori + Beq + Bne + Slti + Sltiu + Bgez + Teq;
    assign aluc[0] = Subu + Sub + Or + Nor + Slt + Sllv + Srlv + Sll + Srl + Slti + Ori + Beq + Bne + Bgez + Teq;
    
	// cutter选择信号
    assign cutterMuxSel = ~(Sb + Sh + Sw);
    assign cutterSel[2] = Sh;
    assign cutterSel[1] = Lb + Lbu + Sb;
    assign cutterSel[0] = Lh + Lhu + Sb;

	// regfiles选择信号
    assign rfMuxSel[2] = ~(Beq + Bne + Bgez + Div + Divu + Sb + Multu + Sh + Sw + J + Jr + Jal + Jalr + Mfc0 + Mtc0 + Mflo + Mthi + Mtlo + Clz + Eret + Syscall + Teq + Break);
    assign rfMuxSel[1] = Mul + Mfc0 + Mtc0 + Clz + Mfhi;
    assign rfMuxSel[0] = ~(Beq + Bne + Bgez + Div + Divu + Multu + Lb + Lbu + Lh + Lhu + Lw + Sb + Sh + Sw + J + Mtc0 + Mfhi + Mflo + Mthi + Mtlo + Clz + Eret + Syscall + Teq + Break);
    
	// pc选择信号
    assign pcMuxSel[2] = Eret + (Beq&&isBranch) + (Bne&&isBranch) + (Bgez&&isBranch);
    assign pcMuxSel[1] = ~(J + Jr + Jal + Jalr + pcMuxSel[2]);
    assign pcMuxSel[0] = Eret + exception + Jr + Jalr;

	// hi选择信号
    assign hiMuxSel[1] = Mthi;
    assign hiMuxSel[0] = Mul + Multu;

	// lo选择信号
    assign loMuxSel[1] = Mtlo;
    assign loMuxSel[0] = Mul + Multu;
	
	// rd信号
    assign rd = (Add + Addu + Sub + Subu + And + Or + Xor + Nor + Slt + Sltu + Sll + Srl + Sra + Sllv + Srlv + Srav + Clz + Jalr + Mfhi + Mflo + Mul) ? 
                   instruction[15:11] : (( Addi + Addiu + Andi + Ori + Xori + Lb + Lbu + Lh + Lhu + Lw + Slti + Sltiu + Lui + Mfc0) ? instruction[20:16] : (Jal ? 5'd31:5'b0));

    assign mfc0 = Mfc0;
    assign mtc0 = Mtc0;
    assign cp0Addr = instruction[15:11];
    assign exception = status[0] && ((Syscall && status[1]) || (Break && status[2]) || (Teq && status[3]));
    assign eret = Eret;
    assign cause = Break ? 5'b01001 : (Syscall ? 5'b01000 : (Teq ? 5'b01101 : 5'b00000));

endmodule
