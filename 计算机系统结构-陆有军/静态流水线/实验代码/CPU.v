
`timescale 1ns / 1ps

module Cpu(
    input clk,
    input rst,
    output [31 : 0] instr,
    output [31 : 0] pc,
    output [31 : 0] reg28,
    output [31 : 0] reg29
    );

    // IF输入
    wire [31 : 0] ifPcI;  
    wire [31 : 0] ifCp0PcI;  
    wire [31 : 0] ifBPcI;  
    wire [31 : 0] ifRPcI;  
    wire [31 : 0] ifJPcI;  
    wire [2 : 0] ifPcMuxSelI;  

    //IF输出
    wire [31 : 0] ifNpcO; 
    wire [31 : 0] ifPc4O;
    wire [31 : 0] ifInstructionO; 

    wire stall;
    wire isBranch;
    wire pcWena;

    //ID输入
    wire [31 : 0] idPc4I;  
    wire [31 : 0] idInstructionI;
    wire [31 : 0] idRfWdataI;    
    wire [31 : 0] idHiWdataI;
    wire [31 : 0] idLoWdataI;
    wire [4 : 0] idRfWaddrI; 
    wire idRfWenaI;        
    wire idHiWenaI;        
    wire idLoWenaI;       

    // ID输出
    wire [31 : 0] idCp0PcO;  
    wire [31 : 0] idRPcO;
    wire [31 : 0] idBPcO;  
    wire [31 : 0] idJPcO;  
    wire [31 : 0] idRsDataOutO;  
    wire [31 : 0] idRtDataOutO;
    wire [31 : 0] idImmO; 
    wire [31 : 0] idShamtO;
    wire [31 : 0] idPc4O;  
    wire [31 : 0] idCp0OutO;   
    wire [31 : 0] idHiOutO;     
    wire [31 : 0] idLoOutO;      
    wire [4 : 0]  idRfWaddrO;
    wire [3 : 0] idAlucO;
    wire idMulSignO;
    wire idDivSignO;
    wire idCutterSignO;
    wire idClzEnaO;
    wire idMulEnaO;
    wire idDivEnaO;
    wire idDmemEnaO;
    wire idHiWenaO;
    wire idLoWenaO;
    wire idRfWenaO;
    wire idDmemWenaO;
    wire [1 : 0] idDmemWCsO;
    wire [1 : 0] idDmemRCsO;
    wire idCutterMuxSelO;
    wire idAluMux1SelO;
    wire [1 : 0] idAluMux2SelO;
    wire [1 : 0] idHiMuxSelO;
    wire [1 : 0] idLoMuxSelO;
    wire [2 : 0] idCutterSelO;
    wire [2 : 0] idRfMuxSelO;
    wire [2 : 0] idPcMuxSelO;

    // EXE输入
    wire [31 : 0] exePc4I;
    wire [31 : 0] exeImmI;
    wire [31 : 0] exeShamtI;
    wire [31 : 0] exeRsDataOutI;
    wire [31 : 0] exeRtDataOutI;
    wire [31 : 0] exeCp0OutI;
    wire [31 : 0] exeHiOutI;
    wire [31 : 0] exeLoOutI;
    wire [4 : 0] exeRfWaddrI;
    wire [3 : 0] exeAlucI;  
    wire exeMulEnaI;
    wire exeDivEnaI;
    wire exeClzEnaI;
    wire exeDmemEnaI;
    wire [1 : 0] exeDmemWCsI;
    wire [1 : 0] exeDmemRCsI;
    wire exeMulSignI;
    wire exeDivSignI;
    wire exeCutterSignI;
    wire exeRfWenaI;       
    wire exeHiWenaI;     
    wire exeLoWenaI;    
    wire exeDmemWenaI;
    wire exeCutterMuxSelI;
    wire exeAluMux1SelI;
    wire [1 : 0] exeAluMux2SelI;
    wire [2 : 0] exeCutterSelI;
    wire [1 : 0] exeHiMuxSelI;
    wire [1 : 0] exeLoMuxSelI;
    wire [2 : 0] exeRfMuxSelI; 

    // EXE输出
    wire [31 : 0] exeMulHiO;
    wire [31 : 0] exeMulLoO;
    wire [31 : 0] exeDivRO;
    wire [31 : 0] exeDivQO;
    wire [31 : 0] exeClzOutO;
    wire [31 : 0] exeAluOutO;
    wire [31 : 0] exePc4O;
    wire [31 : 0] exeRsDataOutO;
    wire [31 : 0] exeRtDataOutO;
    wire [31 : 0] exeCp0OutO;
    wire [31 : 0] exeHiOutO;
    wire [31 : 0] exeLoOutO;
    wire [4 : 0] exeRfWaddrO;
    wire exeDmemEnaO;
    wire exeCutterSignO;
    wire exeRfWenaO;       
    wire exeHiWenaO;     
    wire exeLoWenaO;    
    wire exeDmemWenaO;
    wire [1 : 0] exeDmemWCsO;
    wire [1 : 0] exeDmemRCsO;
    wire exeCutterMuxSelO;
    wire [2 : 0] exeCutterSelO;
    wire [1 : 0] exeHiMuxSelO;
    wire [1 : 0] exeLoMuxSelO;
    wire [2 : 0] exeRfMuxSelO;

    // MEM输入
    wire [31 : 0] memMulHiI;
    wire [31 : 0] memMulLoI;  
    wire [31 : 0] memDivRI;
    wire [31 : 0] memDivQI;  
    wire [31 : 0] memClzOutI;
    wire [31 : 0] memAluOutI; 
    wire [31 : 0] memPc4I;
    wire [31 : 0] memRsDataOutI;
    wire [31 : 0] memRtDataOutI;
    wire [31 : 0] memCp0OutI;
    wire [31 : 0] memHiOutI;
    wire [31 : 0] memLoOutI; 
    wire [4 : 0] memRfWaddrI;
    wire memDmemEnaI;
    wire memRfWenaI;       
    wire memHiWenaI;     
    wire memLoWenaI;    
    wire memDmemWenaI;
    wire memCutterSignI;
    wire [1 : 0] memDmemWCsI;
    wire [1 : 0] memDmemRCsI;
    wire memCutterMuxSelI;
    wire [2 : 0] memCutterSelI;
    wire [1 : 0] memHiMuxSelI;
    wire [1 : 0] memLoMuxSelI;
    wire [2 : 0] memRfMuxSelI; 

    // MEM输出
    wire [31 : 0] memMulHiO;  
    wire [31 : 0] memMulLoO;  
    wire [31 : 0] memDivRO;  
    wire [31 : 0] memDivQO;  
    wire [31 : 0] memClzOutO;
    wire [31 : 0] memAluOutO;  
    wire [31 : 0] memDmemOutO;
    wire [31 : 0] memPc4O;
    wire [31 : 0] memRsDataOutO;
    wire [31 : 0] memCp0OutO;
    wire [31 : 0] memHiOutO;
    wire [31 : 0] memLoOutO;
    wire [4 : 0] memRfWaddrO;
    wire memRfWenaO;       
    wire memHiWenaO;     
    wire memLoWenaO;    
    wire [1 : 0] memHiMuxSelO;
    wire [1 : 0] memLoMuxSelO;
    wire [2 : 0] memRfMuxSelO;

    // WB输入
    wire [31 : 0] wbMulHiI;
    wire [31 : 0] wbMulLoI;  
    wire [31 : 0] wbDivRI;
    wire [31 : 0] wbDivQI;  
    wire [31 : 0] wbClzOutI;
    wire [31 : 0] wbAluOutI; 
    wire [31 : 0] wbDmemOutI;
    wire [31 : 0] wbPc4I;
    wire [31 : 0] wbRsDataOutI;
    wire [31 : 0] wbCp0OutI;
    wire [31 : 0] wbHiOutI;
    wire [31 : 0] wbLoOutI; 
    wire [4 : 0] wbRfWaddrI;
    wire wbRfWenaI;       
    wire wbHiWenaI;     
    wire wbLoWenaI;    
    wire [1 :0] wbHiMuxSelI;
    wire [1 : 0] wbLoMuxSelI;
    wire [2 : 0] wbRfMuxSelI; 

    // WB输出
    wire [31 : 0] wbHiWdataO;
    wire [31 : 0] wbLoWdataO;
    wire [31 : 0] wbRfWdataO;
    wire [4 : 0] wbRfWaddrO;
    wire wbRfWenaO;       
    wire wbHiWenaO;     
    wire wbLoWenaO;    

    wire idExeWena;
    wire exeMemWena;
    wire memWbWena;

    assign instr = ifInstructionO;
    assign pc = ifPcI;
    assign pcWena = 1'b1;
    assign idExeWena = 1'b1;
    assign exeMemWena = 1'b1;
    assign memWbWena = 1'b1;
    
    assign ifPcMuxSelI = idPcMuxSelO;
    assign ifCp0PcI = idCp0PcO;
    assign ifBPcI = idBPcO;
    assign ifRPcI = idRPcO;
    assign ifJPcI = idJPcO;
    
    assign idRfWdataI = wbRfWdataO;
    assign idHiWdataI = wbHiWdataO;
    assign idLoWdataI = wbLoWdataO;
    assign idRfWaddrI = wbRfWaddrO;
    assign idRfWenaI = wbRfWenaO;
    assign idHiWenaI = wbHiWenaO;
    assign idLoWenaI = wbLoWenaO;
	
    PcReg programCounter(clk, rst, pcWena, stall, ifNpcO, ifPcI);
	
    PipeIf instrFetch(ifPcI, ifCp0PcI, ifBPcI, ifRPcI, ifJPcI, ifPcMuxSelI, ifNpcO, ifPc4O, ifInstructionO);
	
	PipeIfIdReg ifIdReg(clk, rst, ifPc4O, ifInstructionO, stall, isBranch, idPc4I, idInstructionI);
	
    PipeId instruction_decoder(clk, rst, idPc4I, idInstructionI, idRfWdataI, idHiWdataI, idLoWdataI, idRfWaddrI, idRfWenaI, idHiWenaI, idLoWenaI, 
		exeRfWaddrO, exeRfWenaO, exeHiWenaO, exeLoWenaO, 
		memRfWaddrO, memRfWenaO, memHiWenaO, memLoWenaO, 
		idCp0PcO, idRPcO, idBPcO, idJPcO, idRsDataOutO, idRtDataOutO, idImmO, idShamtO, idPc4O,  
        idCp0OutO, idHiOutO, idLoOutO, idRfWaddrO, idAlucO, idMulSignO, idDivSignO, idCutterSignO, idClzEnaO,
        idMulEnaO, idDivEnaO, idDmemEnaO, idHiWenaO, idLoWenaO, idRfWenaO, idDmemWenaO, idDmemWCsO, idDmemRCsO,
        idCutterMuxSelO, idAluMux1SelO, idAluMux2SelO, idHiMuxSelO, idLoMuxSelO, idCutterSelO, idRfMuxSelO, idPcMuxSelO,
        stall, isBranch, reg28, reg29);
		
    PipeIdExeReg idExeReg(clk, rst, idExeWena, idPc4O, idRsDataOutO, idRtDataOutO, idImmO, idShamtO, idCp0OutO, idHiOutO, 
        idLoOutO, idRfWaddrO, idClzEnaO, idMulEnaO, idDivEnaO, idDmemEnaO, idMulSignO, idDivSignO, idCutterSignO,
        idAlucO, idRfWenaO, idHiWenaO, idLoWenaO, idDmemWenaO, idDmemWCsO, idDmemRCsO, stall,
        idCutterMuxSelO, idAluMux1SelO, idAluMux2SelO, idHiMuxSelO, idLoMuxSelO, idCutterSelO, idRfMuxSelO, 
        exePc4I, exeRsDataOutI, exeRtDataOutI, exeImmI, exeShamtI, exeCp0OutI, exeHiOutI, exeLoOutI, exeRfWaddrI,
        exeClzEnaI, exeMulEnaI, exeDivEnaI, exeDmemEnaI, exeMulSignI, exeDivSignI, exeCutterSignI, exeAlucI,
        exeRfWenaI, exeHiWenaI, exeLoWenaI, exeDmemWenaI, exeDmemWCsI, exeDmemRCsI, exeAluMux1SelI, exeAluMux2SelI,
        exeCutterMuxSelI, exeHiMuxSelI, exeLoMuxSelI, exeCutterSelI, exeRfMuxSelI);
		
	//57
    PipeExe executor(rst, exePc4I, exeRsDataOutI, exeRtDataOutI, exeImmI, exeShamtI, exeCp0OutI, exeHiOutI, exeLoOutI,
        exeRfWaddrI, exeAlucI, exeMulEnaI, exeDivEnaI, exeClzEnaI, exeDmemEnaI, exeMulSignI, exeDivSignI, exeCutterSignI,
        exeRfWenaI, exeHiWenaI, exeLoWenaI, exeDmemWenaI, exeDmemWCsI, exeDmemRCsI, exeCutterMuxSelI, exeAluMux1SelI, 
		exeAluMux2SelI, exeHiMuxSelI, exeLoMuxSelI, exeCutterSelI, exeRfMuxSelI, 
        exeMulHiO, exeMulLoO, exeDivRO, exeDivQO, exeClzOutO, exeAluOutO, exePc4O, exeRsDataOutO, exeRtDataOutO,
        exeCp0OutO, exeHiOutO, exeLoOutO, exeRfWaddrO,
        exeDmemEnaO, exeRfWenaO, exeHiWenaO, exeLoWenaO, exeDmemWenaO, exeDmemWCsO, exeDmemRCsO, exeCutterSignO,
        exeCutterMuxSelO, exeCutterSelO, exeHiMuxSelO, exeLoMuxSelO, exeRfMuxSelO);

    //55
    PipeExeMemReg exeMemReg(clk, rst, exeMemWena, exeMulHiO, exeMulLoO, exeDivRO, exeDivQO, exeClzOutO, exeAluOutO, 
        exePc4O, exeRsDataOutO, exeRtDataOutO, exeCp0OutO, exeHiOutO, exeLoOutO, exeRfWaddrO, exeDmemEnaO, exeCutterSignO,
        exeRfWenaO, exeHiWenaO, exeLoWenaO, exeDmemWenaO, exeDmemWCsO, exeDmemRCsO, exeCutterMuxSelO, exeHiMuxSelO,
        exeLoMuxSelO, exeCutterSelO, exeRfMuxSelO, memMulHiI, memMulLoI, memDivRI, memDivQI, memClzOutI, memAluOutI, 
        memPc4I, memRsDataOutI, memRtDataOutI, memCp0OutI, memHiOutI, memLoOutI, memRfWaddrI, memDmemEnaI, memCutterSignI,
        memRfWenaI, memHiWenaI, memLoWenaI, memDmemWenaI, memDmemWCsI, memDmemRCsI, memCutterMuxSelI, memHiMuxSelI,
        memLoMuxSelI, memCutterSelI, memRfMuxSelI);

	//46
    PipeMem memory(clk, memMulHiI, memMulLoI, memDivRI, memDivQI, memClzOutI, memAluOutI, memPc4I, memRsDataOutI,
        memRtDataOutI, memCp0OutI, memHiOutI, memLoOutI, memRfWaddrI, memDmemEnaI, memRfWenaI, memHiWenaI, memLoWenaI,    
        memDmemWenaI, memCutterSignI, memDmemWCsI, memDmemRCsI, memCutterMuxSelI, memCutterSelI, memHiMuxSelI,
        memLoMuxSelI, memRfMuxSelI, memMulHiO, memMulLoO, memDivRO, memDivQO, memClzOutO, memAluOutO, memDmemOutO,
        memPc4O, memRsDataOutO, memCp0OutO, memHiOutO, memLoOutO, memRfWaddrO, memRfWenaO, memHiWenaO, memLoWenaO,    
        memHiMuxSelO, memLoMuxSelO, memRfMuxSelO);

	//41
    PipeMemWbReg memWbReg(clk, rst, memWbWena, memMulHiO, memMulLoO, memDivRO, memDivQO, memClzOutO,
        memAluOutO, memDmemOutO, memPc4O, memRsDataOutO, memCp0OutO, memHiOutO, memLoOutO, memRfWaddrO,
        memRfWenaO, memHiWenaO, memLoWenaO, memHiMuxSelO, memLoMuxSelO, memRfMuxSelO, wbMulHiI, wbMulLoI,  
        wbDivRI, wbDivQI, wbClzOutI, wbAluOutI, wbDmemOutI, wbPc4I, wbRsDataOutI, wbCp0OutI, wbHiOutI,
        wbLoOutI, wbRfWaddrI, wbRfWenaI, wbHiWenaI, wbLoWenaI, wbHiMuxSelI, wbLoMuxSelI, wbRfMuxSelI);

	//26
    PipeWb writeBack(wbMulHiI, wbMulLoI, wbDivQI, wbDivQI, wbClzOutI, wbAluOutI, wbDmemOutI, wbPc4I,
        wbRsDataOutI, wbCp0OutI, wbHiOutI, wbLoOutI, wbRfWaddrI, wbRfWenaI, wbHiWenaI, wbLoWenaI, wbHiMuxSelI,
        wbLoMuxSelI, wbRfMuxSelI, wbHiWdataO, wbLoWdataO, wbRfWdataO, wbRfWaddrO, wbRfWenaO, wbHiWenaO, wbLoWenaO);    

endmodule