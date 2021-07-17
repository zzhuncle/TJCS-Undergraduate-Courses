
`timescale 1ns / 1ps

module IdForwarding(
    input clk,
    input rst,
    input [5 : 0] op,  
    input [5 : 0] func,
    input [4 : 0] rs,
    input [4 : 0] rt,
    input rfRena1,
    input rfRena2,
    //EXE数据  
    input [4 : 0] exeRfWaddr,  
    input exeRfWena,
    input exeHiWena,
    input exeLoWena,
    input [31 : 0] exeDfHiOut,
    input [31 : 0] exeDfLoOut,
    input [31 : 0] exeDfRfWdata,
    input [5 : 0] exeOp,
    input [5 : 0] exeFunc,
    //MEM数据
    input [4 : 0] memRfWaddr,  
    input memRfWena,
    input memHiWena,
    input memLoWena,
    input [31 : 0] memDfHiOut,
    input [31 : 0] memDfLoOut,
    input [31 : 0] memDfRfWdata,
    output reg [31 : 0] rsDataOut,  
    output reg [31 : 0] rtDataOut,
    output reg [31 : 0] hiOut,
    output reg [31 : 0] loOut,
    output reg stall,
    output reg ifDf,
    output reg isRs,
    output reg isRt
    );


    
    //ID、EXE和MEM阶段的数据冲突检测
    always @ (negedge clk or posedge rst) 
    begin
        if (rst == `RST_ENABLED) begin
            stall <= `RUN;
            rsDataOut <= 0;
            rtDataOut <= 0;
            hiOut <= 0;
            loOut <= 0;
            ifDf <= 0;
            isRs <= 0;
            isRt <= 0;
        end else if (stall == `STOP) begin
            stall <= `RUN;
            if (isRs == 1'b1) begin
                rsDataOut <= memDfRfWdata;
            end else begin
                rtDataOut <= memDfRfWdata;
            end
        end else if (stall == `RUN) begin
            ifDf = 0;
            isRs = 0;
            isRt = 0;
            if (op == `MFHI_OP && func == `MFHI_FUNC) begin
                if (exeHiWena == `WRITE_ENABLED) begin
                    hiOut <= exeDfHiOut;
                    ifDf <= 1;
                end else if (memHiWena == `WRITE_ENABLED) begin
                    hiOut <= memDfHiOut;
                    ifDf <= 1;
                end
            end else if (op == `MFLO_OP && func == `MFLO_FUNC) begin
                if (exeLoWena == `WRITE_ENABLED) begin
                    loOut <= exeDfLoOut;
                    ifDf <= 1;
                end else if (memLoWena == `WRITE_ENABLED) begin
                    loOut <= memDfLoOut;
                    ifDf <= 1;
                end
            end else begin
                if (exeRfWena == `WRITE_ENABLED && rfRena1 == `READ_ENABLED && exeRfWaddr == rs) begin
                    if (exeOp == `LW_OP || exeOp == `LH_OP || exeOp == `LHU_OP || exeOp == `LB_OP || exeOp == `LBU_OP) begin
                        stall <= `STOP;
                        isRs <= 1;
                        ifDf <= 1;
                    end
                    else begin
                        rsDataOut <= exeDfRfWdata;
                        ifDf <= 1;
                        isRs <= 1;
                    end
                end
                else if (memRfWena == `WRITE_ENABLED && rfRena1 == `READ_ENABLED && memRfWaddr == rs) begin
                    rsDataOut <= memDfRfWdata;
                    ifDf <= 1;
                    isRs <= 1;
                end
                if (exeRfWena == `WRITE_ENABLED && rfRena2 == `READ_ENABLED && exeRfWaddr == rt) begin
                    if (exeOp == `LW_OP || exeOp == `LH_OP || exeOp == `LHU_OP || exeOp == `LB_OP || exeOp == `LBU_OP) begin
                        stall <= `STOP;
                        isRt <= 1;
                        ifDf <= 1;
                    end else begin
                        rtDataOut <= exeDfRfWdata;
                        ifDf <= 1;
                        isRt <= 1;
                    end
                end else if (memRfWena == `WRITE_ENABLED && rfRena2 == `READ_ENABLED && memRfWaddr == rt) begin
                    rtDataOut <= memDfRfWdata;
                    ifDf <= 1;
                    isRt <= 1;
                end
            end
        end
	end      

endmodule