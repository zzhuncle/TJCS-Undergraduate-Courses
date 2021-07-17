
`timescale 1ns / 1ps

module PipeIf(
    input [31 : 0] pc,
    input [31 : 0] cp0Pc,  // 中断跳转地址  
    input [31 : 0] bPc,    // beq bne bgez跳转地址
    input [31 : 0] rPc,    // jr jalr跳转地址
    input [31 : 0] jPc,    // j jal跳转地址
    input [2 : 0] pcMuxSel, 
    output [31 : 0] npc,   // next pc
    output [31 : 0] pc4,   // pc + 4
    output [31 : 0] instr 
    );

	imem instr_mem(pc[12 : 2], instr);
    Add32 pc_plus4_adder(pc, 32'h4, pc4);         
    Mux8_32 next_pc_mux(jPc, rPc, pc4, `EXCEPTION_ADDR, bPc, cp0Pc, 32'b0, 32'b0, pcMuxSel, npc);
	
endmodule

module PipeIfIdReg(
    input clk,
    input rst,
    input [31 : 0] ifPc4,
    input [31 : 0] ifInstr,
    input stall,
    input isBranch,
    output reg [31 : 0] idPc4,
    output reg [31 : 0] idInstr 
    );

    always @ (posedge clk or posedge rst)
    begin
		if (rst == `RST_ENABLED) begin
		    idPc4 <= 32'h0;
		    idInstr <= 32'h0;       
		end else if (isBranch == 1'b1) begin
            idPc4 <= 32'h0;
            idInstr <= 32'h0;
        end else if (stall == `RUN) begin
		    idPc4 <= ifPc4;
		    idInstr <= ifInstr;
		end
	end
	
endmodule

module PipeId(
	input clk,
    input rst,
    input [31 : 0] pc4,  
    input [31 : 0] instruction,
    input [31 : 0] rfWdata,   //写入寄存器堆的数据
    input [31 : 0] hiWdata,
    input [31 : 0] loWdata,
    input [4 : 0] rfWaddr,    // 写入寄存器堆的地址
    input rfWena,             // 寄存器堆写信号
    input hiWena,             // HI寄存器写信号
    input loWena,             // LO寄存器写信号
    //EXE数据  
	input [4 : 0] exeRfWaddr,  
    input exeRfWena,
    input exeHiWena,
    input exeLoWena,
    input [31 : 0] exeMulHi,  //乘积高位
    input [31 : 0] exeMulLo,  //乘积低位
    input [31 : 0] exeDivR,   //余数
    input [31 : 0] exeDivQ,   //商
    input [31 : 0] exeRsDataOut,
    input [31 : 0] exeLoOut,
    input [31 : 0] exePc4,
    input [31 : 0] exeClzOut,
    input [31 : 0] exeAluOut,
    input [31 : 0] exeHiOut,
    input [1 : 0]  exeHiMuxSel,
    input [1 : 0] exeLoMuxSel,
    input [2 : 0] exeRfMuxSel,
    input [5 : 0] exeOp,
    input [5 : 0] exeFunc,
	//MEM数据
	input [4 : 0] memRfWaddr,  
    input memRfWena,
    input memHiWena,
    input memLoWena,
    input [31 : 0] memMulHi,
    input [31 : 0] memMulLo,
    input [31 : 0] memDivR,
    input [31 : 0] memDivQ,
    input [31 : 0] memRsDataOut,
    input [31 : 0] memDmemOut,
    input [31 : 0] memLoOut,
    input [31 : 0] memPc4,
    input [31 : 0] memClzOut,
    input [31 : 0] memAluOut,
    input [31 : 0] memHiOut,
    input [1 : 0] memHiMuxSel,
    input [1 : 0] memLoMuxSel,
    input [2 : 0] memRfMuxSel,
    output [31 : 0] idCp0Pc,  //中断地址
    output [31 : 0] idRPc,
    output [31 : 0] idBPc,    //无条件跳转地址
    output [31 : 0] idJPc,    //有条件跳转地址
    output [31 : 0] idRsDataOut,  
    output [31 : 0] idRtDataOut,
    output [31 : 0] idImm,    //立即数值
    output [31 : 0] idShamt,  
    output [31 : 0] idPc4,  
    output [31 : 0] idCp0Out, //CP0传出数据
    output [31 : 0] idHiOut,  //HI寄存器数据
    output [31 : 0] idLoOut,  //LO寄存器数据
    output [4 : 0] idRfWaddr,
    output [3 : 0] idAluc,
    output idMulSign,
    output idDivSign,
    output idCutterSign,
    output idClzEna,
    output idMulEna,
    output idDivEna,
    output idDmemEna,
    output idHiWena,
    output idLoWena,
    output idRfWena,
    output idDmemWena,
    output [1 : 0] idDmemWCs,
    output [1 : 0] idDmemRCs,
    output idCutterMuxSel,
    output idAluMux1Sel, 
    output [1 : 0] idAluMux2Sel,
    output [1 : 0] idHiMuxSel,
    output [1 : 0] idLoMuxSel,
    output [2 : 0] idCutterSel,
    output [2 : 0] idRfMuxSel,
    output [2 : 0] idPcMuxSel,
    output [5 : 0] idOp,
    output [5 : 0] idFunc,
    output stall,
    output isBranch,
    output [31 : 0] reg28,
    output [31 : 0] reg29
    );

    wire [4 : 0] rs;
    wire [4 : 0] rt;
    wire [4 : 0] rd;
    wire [5 : 0] op;
    wire [5 : 0] func;
    wire rfRena1;
    wire rfRena2;

    wire [15 : 0] ext16DataIn;
    wire [17 : 0] ext18DataIn;
    wire [31 : 0] ext16DataOut;
    wire [31 : 0] ext18DataOut;
    wire ext16Sign;
    wire mfc0;
    wire mtc0;
    wire eret;
    
    wire ifDf;
    wire isRs;
    wire isRt;
    wire [31 : 0] dfHiTemp;
    wire [31 : 0] dfLoTemp;
    wire [31 : 0] dfRsTemp;
    wire [31 : 0] dfRtTemp;
    wire [31 : 0] exeDfHiOut;
    wire [31 : 0] exeDfLoOut;
    wire [31 : 0] exeDfRfWdata;
    wire [31 : 0] memDfHiOut;
    wire [31 : 0] memDfLoOut;
    wire [31 : 0] memDfRfWdata;
    
    wire cp0Exception;
    wire [4 : 0] cp0Cause;
    wire [4 : 0] cp0Addr;
    wire [31 : 0] cp0Status;

    wire ext5MuxSel;
    wire [4 : 0] ext5MuxOut;
    
    wire [31 : 0] hiTemp;
    wire [31 : 0] loTemp;
    wire [31 : 0] rsTemp;
    wire [31 : 0] rtTemp;
    
    assign op = instruction[31 : 26];
    assign rs = instruction[25 : 21];
    assign rt = instruction[20 : 16];
    assign func = instruction[5 : 0];
    assign ext16DataIn = instruction[15 : 0];
    assign ext18DataIn = {instruction[15 : 0], 2'b00};

    assign idImm = ext16DataOut;
    assign idJPc = {pc4[31 : 28], instruction[25 : 0], 2'b00};
    assign idRPc = idRsDataOut;
    assign idPc4 = pc4;
    assign idRfWaddr = rd;
    assign idRsDataOut = ifDf && isRs ? dfRsTemp  :  rsTemp;
    assign idRtDataOut = ifDf && isRt ? dfRtTemp  :  rtTemp;
    assign idHiOut = ifDf ? dfHiTemp  :  hiTemp;
    assign idLoOut = ifDf ? dfLoTemp  :  loTemp;
    assign idOp = op;
    assign idFunc = func;


    IdForwarding dataForwarding(clk, rst, op, func, rs, rt, rfRena1, rfRena2, exeRfWaddr, exeRfWena, exeHiWena, exeLoWena, exeDfHiOut,
        exeDfLoOut, exeDfRfWdata, exeOp, exeFunc, memRfWaddr, memRfWena, memHiWena, memLoWena, memDfHiOut, memDfLoOut, memDfRfWdata,
        dfRsTemp, dfRtTemp, dfHiTemp, dfLoTemp, stall, ifDf, isRs, isRt);
		
    Mux4_32 exeDfMuxHi(exeDivR, exeMulHi, exeRsDataOut, 32'h0, exeHiMuxSel, exeDfHiOut);
	
    Mux4_32 memDfMuxHi(memDivR, memMulHi, memRsDataOut, 32'h0, memHiMuxSel, memDfHiOut);
	
    Mux4_32 exeDfMuxLo(exeDivQ, exeMulLo, exeRsDataOut, 32'b0, exeLoMuxSel, exeDfLoOut);
	
    Mux4_32 memDfMuxLo(memDivQ, memMulLo, memRsDataOut, 32'b0, memLoMuxSel, memDfLoOut);

    Mux8_32 exeDfMuxRf(exeLoOut, exePc4, exeClzOut, 32'b0, 32'b0, exeAluOut, exeHiOut, exeMulLo, exeRfMuxSel, exeDfRfWdata);
	
    Mux8_32 memDfMuxRf(memLoOut, memPc4, memClzOut, 32'b0, memDmemOut, memAluOut, memHiOut, memMulLo, memRfMuxSel, memDfRfWdata);

    Mux2_5 extend5_mux(instruction[10  :  6], idRsDataOut[4  :  0], ext5MuxSel, ext5MuxOut);

    Add32 b_pc_adder(pc4, ext18DataOut, idBPc);

    Extend #(.WIDTH(5)) sa_ext(ext5MuxOut, 1'b0, idShamt);

	Extend #(.WIDTH(16)) imm_ext(ext16DataIn, ext16Sign, ext16DataOut);

	Extend #(.WIDTH(18)) ext18_b_pc(ext18DataIn, 1'b1, ext18DataOut);

    RegFile cpuRegfile(clk, rst, rfWena, rs, rt, rfRena1, rfRena2, rfWaddr, rfWdata, rsTemp, rtTemp, reg28, reg29);

    Cp0 cpuCp0(clk, rst, mfc0, mtc0, pc4 - 4, cp0Addr, idRtDataOut, cp0Exception, eret, cp0Cause, idCp0Out, cp0Status, idCp0Pc);

    Reg hiReg(clk, rst, hiWena, hiWdata, hiTemp);

    Reg loReg(clk, rst, loWena, loWdata, loTemp);

    BranchCompare compare(clk, rst, idRsDataOut, idRtDataOut, op, func, cp0Exception, isBranch);

    Operation operationUnit(isBranch, instruction, op, func, cp0Status, idRfWena, idHiWena, idLoWena, idDmemWena, rfRena1, rfRena2, idClzEna, idMulEna,
        idDivEna,  idDmemEna, idDmemWCs, idDmemRCs, ext16Sign, idCutterSign, idMulSign, idDivSign, idAluc, rd, mfc0, mtc0, eret, cp0Exception, cp0Addr,
        cp0Cause, ext5MuxSel, idCutterMuxSel, idAluMux1Sel, idAluMux2Sel, idHiMuxSel, idLoMuxSel, idCutterSel, idRfMuxSel, idPcMuxSel);

endmodule

module PipeIdExeReg(
    input clk,
    input rst,
    input wena,
    input [31 : 0] idPc4,
    input [31 : 0] idRsDataOut,
    input [31 : 0] idRtDataOut,
    input [31 : 0] idImm, 
    input [31 : 0] idShamt,
    input [31 : 0] idCp0Out,
    input [31 : 0] idHiOut, 
    input [31 : 0] idLoOut, 
    input [4 : 0] idRfWaddr,
    input idClzEna,
    input idMulEna,
    input idDivEna,
    input idDmemEna,
    input idMulSign,
    input idDivSign,
    input idCutterSign,
    input [3 : 0] idAluc, 
    input idRfWena,  
    input idHiWena,
    input idLoWena,
    input idDmemWena,
    input [1 : 0] idDmemWCs,
    input [1 : 0] idDmemRCs,
    input stall,
    input idCutterMuxSel,
    input idAluMux1Sel,
    input [1 : 0] idAluMux2Sel,
    input [1 : 0] idHiMuxSel,
    input [1 : 0] idLoMuxSel,
    input [2 : 0] idCutterSel,
    input [2 : 0] idRfMuxSel, 
    input [5 : 0] idOp,
    input [5 : 0] idFunc,
    output reg [31 : 0] exePc4,
    output reg [31 : 0] exeRsDataOut,
    output reg [31 : 0] exeRtDataOut,
    output reg [31 : 0] exeImm,
    output reg [31 : 0] exeShamt,
    output reg [31 : 0] exeCp0Out,
    output reg [31 : 0] exeHiOut,
    output reg [31 : 0] exeLoOut,
    output reg [4 : 0] exeRfWaddr,
    output reg exeClzEna,
    output reg exeMulEna,
    output reg exeDivEna,
    output reg exeDmemEna,
    output reg exeMulSign,
    output reg exeDivSign,
    output reg exeCutterSign,
    output reg [3 : 0] exeAluc,
    output reg exeRfWena,  
    output reg exeHiWena,
    output reg exeLoWena,
    output reg exeDmemWena,
    output reg [1 : 0] exeDmemWCs,
    output reg [1 : 0] exeDmemRCs,
    output reg exeAluMux1Sel,
    output reg [1 : 0] exeAluMux2Sel,
    output reg exeCutterMuxSel,
    output reg [1 : 0] exeHiMuxSel,
    output reg [1 : 0] exeLoMuxSel,
    output reg [2 : 0] exeCutterSel,
    output reg [2 : 0] exeRfMuxSel,
    output reg [5 : 0] exeOp,
    output reg [5 : 0] exeFunc
    );

    always @ (posedge clk or posedge rst) begin
        if(rst == 1'b1 || stall == 1'b1) begin
		    exeDmemEna <= 1'b0;
            exeCutterSign <= 1'b0;
            exeAluc <= 4'b0;
            exeRfWena <= 1'b0;
            exeHiWena <= 1'b0;
            exeLoWena <= 1'b0;
            exeDmemWena <= 1'b0;
            exeDmemWCs <= 2'b0;
            exeDmemRCs <= 2'b0;
            exeAluMux1Sel <= 1'b0;
            exeAluMux2Sel <= 1'b0;
            exeHiMuxSel <= 2'b0;
            exeLoMuxSel <= 2'b0;
            exeCutterSel <= 3'b0;
            exeRfMuxSel <= 3'b0;
            exeOp <= 6'b0;
            exeFunc <= 6'b0;
            exePc4 <= 32'b0;
            exeRsDataOut <= 32'b0;
            exeRtDataOut <= 32'b0;
            exeImm <= 32'b0;
            exeShamt <= 32'b0;
            exeCp0Out <= 32'b0;
            exeHiOut <= 32'b0;
            exeLoOut <= 32'b0;
            exeRfWaddr <= 5'b0;
            exeClzEna <= 1'b0;
            exeMulEna <= 1'b0;
            exeDivEna <= 1'b0;
			exeCutterMuxSel <= 1'b0;
			exeMulSign <= 1'b0;
            exeDivSign <= 1'b0;
        end else if(wena == 1'b1) begin
            exeDmemWena <= idDmemWena;
            exeDmemWCs <= idDmemWCs;
            exeDmemRCs <= idDmemRCs;
            exeAluMux1Sel <= idAluMux1Sel;
            exeAluMux2Sel <= idAluMux2Sel;
			exePc4 <= idPc4;
            exeRsDataOut <= idRsDataOut;
            exeRtDataOut <= idRtDataOut;
            exeImm <= idImm;
            exeShamt <= idShamt;
            exeCp0Out <= idCp0Out;
            exeHiOut <= idHiOut;
            exeLoOut <= idLoOut;
            exeCutterMuxSel <= idCutterMuxSel;
            exeHiMuxSel <= idHiMuxSel;
            exeLoMuxSel <= idLoMuxSel;
            exeCutterSel <= idCutterSel;
            exeRfMuxSel <= idRfMuxSel;
            exeOp <= idOp;
            exeFunc <= idFunc;
			exeRfWaddr <= idRfWaddr;
            exeClzEna <= idClzEna;
            exeMulEna <= idMulEna;
            exeDivEna <= idDivEna;
            exeDmemEna <= idDmemEna;
            exeMulSign <= idMulSign;
            exeDivSign <= idDivSign;
            exeCutterSign <= idCutterSign;
            exeAluc <= idAluc;
            exeRfWena <= idRfWena;
            exeHiWena <= idHiWena;
            exeLoWena <= idLoWena;
        end
    end 

endmodule

module PipeExe(
    input rst,
    input [31 : 0] pc4,
    input [31 : 0] rsDataOut,
    input [31 : 0] rtDataOut,
    input [31 : 0] imm,
    input [31 : 0] shamt,
    input [31 : 0] cp0Out,
    input [31 : 0] hiOut,
    input [31 : 0] loOut,
    input [4 : 0] rfWaddr,
    input [3 : 0] aluc,  
    input mulEna,
    input divEna,
    input clzEna,
    input dmemEna,
    input mulSign,
    input divSign,
    input cutterSign,
    input rfWena,       
    input hiWena,     
    input loWena,    
    input dmemWena,
    input [1 : 0] dmemWCs,
    input [1 : 0] dmemRCs,
    input cutterMuxSel,
    input aluMux1Sel,
    input [1 : 0] aluMux2Sel,
    input [1 : 0] hiMuxSel,
    input [1 : 0] loMuxSel,
    input [2 : 0] cutterSel,
    input [2 : 0] rfMuxSel, 
    output [31 : 0] exeMulHi,  
    output [31 : 0] exeMulLo,  
    output [31 : 0] exeDivR,  
    output [31 : 0] exeDivQ,  
    output [31 : 0] exeClzOut,
    output [31 : 0] exeAluOut,
    output [31 : 0] exePc4,
    output [31 : 0] exeRsDataOut,
    output [31 : 0] exeRtDataOut,
    output [31 : 0] exeCp0Out,
    output [31 : 0] exeHiOut,
    output [31 : 0] exeLoOut,
    output [4 : 0] exeRfWaddr,
    output exeDmemEna,
    output exeRfWena,       
    output exeHiWena,     
    output exeLoWena,    
    output exeDmemWena,
    output [1 : 0] exeDmemWCs,
    output [1 : 0] exeDmemRCs,
    output exeCutterSign,
    output exeCutterMuxSel,
    output [2 : 0] exeCutterSel,
    output [1 : 0] exeHiMuxSel,
    output [1 : 0] exeLoMuxSel,
    output [2 : 0] exeRfMuxSel
    );

    wire zero;
    wire carry;
    wire negative;
    wire overflow;
    wire [31 : 0] aluIn1;
    wire [31 : 0] aluIn2;

    assign exeDmemRCs = dmemRCs;
    assign exeDmemWCs = dmemWCs;
    assign exePc4 = pc4;
    assign exeRsDataOut = rsDataOut;
    assign exeRtDataOut = rtDataOut;
    assign exeHiOut = hiOut;
    assign exeLoOut = loOut;
	assign exeHiMuxSel = hiMuxSel;
    assign exeLoMuxSel = loMuxSel;
    assign exeRfMuxSel = rfMuxSel;
    assign exeRfWaddr = rfWaddr;
    assign exeCp0Out = cp0Out;
    assign exeDmemEna = dmemEna;
    assign exeRfWena = rfWena;
    assign exeHiWena = hiWena;
    assign exeLoWena = loWena;
    assign exeDmemWena = dmemWena;
    assign exeCutterSign = cutterSign;
    assign exeCutterMuxSel = cutterMuxSel;
    assign exeCutterSel = cutterSel;
	
	Div cpu_div(rst, divEna, divSign, rsDataOut, rtDataOut, exeDivQ, exeDivR);
    Mult cpu_mult(rst, mulEna, mulSign, rsDataOut, rtDataOut, exeMulHi, exeMulLo);
    Counter cpu_counter(rsDataOut, clzEna, exeClzOut);
    Mux2_32 alu_mux1(shamt, rsDataOut, aluMux1Sel, aluIn1);
    Mux4_32 alu_mux2(rtDataOut, imm, 32'b0, 32'b0, aluMux2Sel, aluIn2);
    Alu cpu_alu(aluIn1, aluIn2, aluc, exeAluOut, zero, carry, negative, overflow);

endmodule

module PipeExeMemReg(
    input clk,
    input rst,
    input wena,
    input [31 : 0] exeMulHi,
    input [31 : 0] exeMulLo,  
    input [31 : 0] exeDivR,
    input [31 : 0] exeDivQ,  
    input [31 : 0] exeClzOut,
    input [31 : 0] exeAluOut, 
    input [31 : 0] exePc4,
    input [31 : 0] exeRsDataOut,
    input [31 : 0] exeRtDataOut,
    input [31 : 0] exeCp0Out,
    input [31 : 0] exeHiOut,
    input [31 : 0] exeLoOut,
    input [4 : 0] exeRfWaddr,
    input exeDmemEna,
    input exeCutterSign,
    input exeRfWena,       
    input exeHiWena,     
    input exeLoWena,
    input exeDmemWena,
    input [1 : 0] exeDmemWCs,
    input [1 : 0] exeDmemRCs,
    input exeCutterMuxSel,
    input [1 : 0] exeHiMuxSel,
    input [1 : 0] exeLoMuxSel,
    input [2 : 0] exeCutterSel,
    input [2 : 0] exeRfMuxSel, 
    output reg [31 : 0] memMulHi,
    output reg [31 : 0] memMulLo,  
    output reg [31 : 0] memDivR,
    output reg [31 : 0] memDivQ,  
    output reg [31 : 0] memClzOut,
    output reg [31 : 0] memAluOut, 
    output reg [31 : 0] memPc4,
    output reg [31 : 0] memRsDataOut,
    output reg [31 : 0] memRtDataOut,
    output reg [31 : 0] memCp0Out,
    output reg [31 : 0] memHiOut,
    output reg [31 : 0] memLoOut,
    output reg [4 : 0] memRfWaddr,
    output reg memDmemEna,
    output reg memCutterSign,
    output reg memRfWena,       
    output reg memHiWena,     
    output reg memLoWena,
    output reg memDmemWena,
    output reg [1 : 0] memDmemWCs,
    output reg [1 : 0] memDmemRCs,
    output reg memCutterMuxSel,
    output reg [1 : 0] memHiMuxSel,
    output reg [1 : 0] memLoMuxSel,
    output reg [2 : 0] memCutterSel,
    output reg [2 : 0] memRfMuxSel
    );

    always @ (posedge clk or posedge rst) 
    begin
        if (rst == `RST_ENABLED) begin
		    memHiMuxSel <= 0;
            memLoMuxSel <= 0;
            memCutterSel <= 0;
            memRfMuxSel <= 0;
			memHiOut <= 0;
            memLoOut <= 0;
            memRfWaddr <= 0;
            memDmemEna <= 0;
            memCutterSign <= 0;
            memMulHi <= 0;
            memMulLo <= 0;
            memDivR <= 0;
            memDivQ <= 0;
            memClzOut <= 0;
            memAluOut <= 0;
            memPc4 <= 0;
            memRsDataOut <= 0;
            memRtDataOut <= 0;
            memCp0Out <= 0;
            memRfWena <= 0;
            memHiWena <= 0;
            memLoWena <= 0;
            memDmemWena <= 0;
            memDmemWCs <= 0;
            memDmemRCs <= 0;
            memCutterMuxSel <= 0;
        end else if (wena == `WRITE_ENABLED) begin
		    memHiOut <= exeHiOut;
            memLoOut <= exeLoOut;
            memRfWaddr <= exeRfWaddr;
            memMulHi <= exeMulHi;
            memMulLo <= exeMulLo;
            memDivR <= exeDivR;
            memDivQ <= exeDivQ;
            memClzOut <= exeClzOut;
            memAluOut <= exeAluOut;
            memPc4 <= exePc4;
            memRsDataOut <= exeRsDataOut;
            memRtDataOut <= exeRtDataOut;
            memCp0Out <= exeCp0Out;
            memHiMuxSel <= exeHiMuxSel;
            memLoMuxSel <= exeLoMuxSel;
            memCutterSel <= exeCutterSel;
			memDmemWCs <= exeDmemWCs;
            memDmemRCs <= exeDmemRCs;
            memCutterMuxSel <= exeCutterMuxSel;
            memRfMuxSel <= exeRfMuxSel;
            memDmemEna <= exeDmemEna;
            memCutterSign <= exeCutterSign;
            memRfWena <= exeRfWena;
            memHiWena <= exeHiWena;
            memLoWena <= exeLoWena;
            memDmemWena <= exeDmemWena;
        end
    end 

endmodule

module PipeMem(
    input clk,
    input [31 : 0] mulHi,
    input [31 : 0] mulLo,  
    input [31 : 0] divR,
    input [31 : 0] divQ,  
    input [31 : 0] clzOut,
    input [31 : 0] aluOut, 
    input [31 : 0] pc4,
    input [31 : 0] rsDataOut,
    input [31 : 0] rtDataOut,
    input [31 : 0] cp0Out,
    input [31 : 0] hiOut,
    input [31 : 0] loOut, 
    input [4 : 0] rfWaddr,
    input dmemEna,
    input rfWena,       
    input hiWena,     
    input loWena,    
    input dmemWena,
    input cutterSign,
    input [1 : 0] dmemWCs,
    input [1 : 0] dmemRCs,
    input cutterMuxSel,
    input [2 : 0] cutterSel,
    input [1 : 0] hiMuxSel,
    input [1 : 0] loMuxSel,
    input [2 : 0] rfMuxSel, 
    output [31 : 0] memMulHi,  
    output [31 : 0] memMulLo,  
    output [31 : 0] memDivR,  
    output [31 : 0] memDivQ,  
    output [31 : 0] memClzOut,
    output [31 : 0] memAluOut,  
    output [31 : 0] memDmemOut,
    output [31 : 0] memPc4,
    output [31 : 0] memRsDataOut,
    output [31 : 0] memCp0Out,
    output [31 : 0] memHiOut,
    output [31 : 0] memLoOut,
    output [4 : 0] memRfWaddr,
    output memRfWena,       
    output memHiWena,     
    output memLoWena,    
    output [1 : 0] memHiMuxSel,
    output [1 : 0] memLoMuxSel,
    output [2 : 0] memRfMuxSel
    );

    wire [31 : 0] cutterMuxOut;
	wire [31 : 0] dmemOut;

    assign memDivQ = divQ;
    assign memDivR = divR;
    assign memClzOut = clzOut;
    assign memAluOut = aluOut;
    assign memPc4 = pc4;
    assign memRsDataOut = rsDataOut;
    assign memHiOut = hiOut;
    assign memLoOut = loOut;
    assign memCp0Out = cp0Out;
    assign memRfWaddr = rfWaddr;
    assign memRfWena = rfWena;
    assign memHiWena = hiWena;
    assign memLoWena = loWena;
    assign memHiMuxSel = hiMuxSel;
    assign memLoMuxSel = loMuxSel;
    assign memRfMuxSel = rfMuxSel;
	assign memMulHi = mulHi;
    assign memMulLo = mulLo;

    Cut cpuCutter(cutterMuxOut, cutterSel, cutterSign, memDmemOut);
    Mux2_32 cutterMux(rtDataOut, dmemOut, cutterMuxSel, cutterMuxOut);
    Dmem cpuDmem(clk, dmemEna, dmemWena, dmemWCs, dmemRCs, memDmemOut, aluOut, dmemOut);

endmodule

module PipeMemWbReg (
    input clk,
    input rst,
    input wena,
    input [31 : 0] memMulHi,
    input [31 : 0] memMulLo,  
    input [31 : 0] memDivR,
    input [31 : 0] memDivQ,  
    input [31 : 0] memClzOut,
    input [31 : 0] memAluOut,
    input [31 : 0] memDmemOut, 
    input [31 : 0] memPc4,
    input [31 : 0] memRsDataOut,
    input [31 : 0] memCp0Out,
    input [31 : 0] memHiOut,
    input [31 : 0] memLoOut,
    input [4 : 0] memRfWaddr,
    input memRfWena,       
    input memHiWena,     
    input memLoWena,
    input [1 : 0] memHiMuxSel,
    input [1 : 0] memLoMuxSel,
    input [2 : 0] memRfMuxSel, 
    output reg [31 : 0] wbMulHi,
    output reg [31 : 0] wbMulLo,  
    output reg [31 : 0] wbDivR,
    output reg [31 : 0] wbDivQ, 
    output reg [31 : 0] wbClzOut, 
    output reg [31 : 0] wbAluOut,
    output reg [31 : 0] wbDmemOut, 
    output reg [31 : 0] wbPc4,
    output reg [31 : 0] wbRsDataOut,
    output reg [31 : 0] wbCp0Out,
    output reg [31 : 0] wbHiOut,
    output reg [31 : 0] wbLoOut,
    output reg [4 : 0] wbRfWaddr,
    output reg wbRfWena,       
    output reg wbHiWena,     
    output reg wbLoWena,
    output reg [1 : 0] wbHiMuxSel,
    output reg [1 : 0] wbLoMuxSel,
    output reg [2 : 0] wbRfMuxSel
    );

    always @ (posedge clk or posedge rst) 
    begin
        if(rst == `RST_ENABLED) begin
		    wbPc4 <= 0;
            wbRsDataOut <= 0;
            wbCp0Out <= 0;
            wbHiOut <= 0;
            wbLoOut <= 0;
            wbRfWaddr <= 0;
            wbRfWena <= 0;
            wbHiWena <= 0;
            wbLoWena <= 0;
            wbHiMuxSel <= 0;
            wbLoMuxSel <= 0;
            wbRfMuxSel <= 0;
			wbMulHi <= 0;
            wbMulLo <= 0;
			wbDivR <= 0;
            wbDivQ <= 0;
            wbClzOut <= 0;
            wbAluOut <= 0;
            wbDmemOut <= 0;
        end else if(wena == `WRITE_ENABLED) begin
			wbDivR <= memDivR;
            wbDivQ <= memDivQ;
            wbPc4 <= memPc4;
            wbRsDataOut <= memRsDataOut;
            wbCp0Out <= memCp0Out;
            wbHiOut <= memHiOut;
            wbLoOut <= memLoOut;
            wbRfWaddr <= memRfWaddr;
            wbRfWena <= memRfWena;
            wbHiWena <= memHiWena;
            wbLoWena <= memLoWena;
			wbMulHi <= memMulHi;
            wbMulLo <= memMulLo;
            wbClzOut <= memClzOut;
            wbAluOut <= memAluOut;
            wbDmemOut <= memDmemOut;
			wbHiMuxSel <= memHiMuxSel;
            wbLoMuxSel <= memLoMuxSel;
            wbRfMuxSel <= memRfMuxSel;
        end
    end 

endmodule

module PipeWb(
    input [31 : 0] mulHi,
    input [31 : 0] mulLo,  
    input [31 : 0] divR,
    input [31 : 0] divQ,  
    input [31 : 0] clzOut,
    input [31 : 0] aluOut,
    input [31 : 0] dmemOut, 
    input [31 : 0] pc4,
    input [31 : 0] rsDataOut,
    input [31 : 0] cp0Out,
    input [31 : 0] hiOut,
    input [31 : 0] loOut, 
    input [4 : 0] rfWaddr,
    input rfWena,       
    input hiWena,     
    input loWena,    
    input [1 : 0] hiMuxSel,
    input [1 : 0] loMuxSel,
    input [2 : 0] rfMuxSel, 
    output [31 : 0] hiWdata,
    output [31 : 0] loWdata,
    output [31 : 0] rfWdata,
    output [4 : 0] wbRfWaddr,
    output wbRfWena,       
    output wbHiWena,     
    output wbLoWena 
    );
	
	assign wbRfWaddr = rfWaddr;
    assign wbLoWena = loWena;
	assign wbRfWena = rfWena;
    assign wbHiWena = hiWena;

    Mux4_32 mux_lo(divQ, mulLo, rsDataOut, 32'b0, loMuxSel, loWdata);
    Mux4_32 mux_hi(divR, mulHi, rsDataOut, 32'h0, hiMuxSel, hiWdata);
    Mux8_32 mux_rf(loOut, pc4, clzOut, cp0Out, dmemOut, aluOut, hiOut, mulLo, rfMuxSel, rfWdata);

endmodule
