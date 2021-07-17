
`timescale 1ns / 1ps

module IdStall(
    input clk,
    input rst,
    input [5 : 0] op,  
    input [5 : 0] func,
    input [4 : 0] rs,
    input [4 : 0] rt,
    input rfRena1,
    input rfRena2,

    // EXE数据  
	input [4 : 0] exeRfWaddr,  
    input exeRfWena,
    input exeHiWena,
    input exeLoWena,

	// MEM数据
	input [4 : 0] memRfWaddr,  
    input memRfWena,
    input memHiWena,
    input memLoWena,

    output reg stall
    );

    reg count;
    
    // ID、EXE、MEM数据冲突检测
    always @ (negedge clk or posedge rst) 
    begin
        if (rst == `RST_ENABLED) begin
            stall <= `RUN;
            count <= 0;
        end else if (count >= 1) begin
            count <= count - 1;
        end else if (stall == `STOP) begin
            stall <= `RUN;            
        end else if (stall == `RUN) begin
            if (op == `MFHI_OP && func == `MFHI_FUNC) begin
                if (exeHiWena == `WRITE_ENABLED) begin
                    count <= 1;
                    stall <= `STOP;
                end else if (memHiWena == `WRITE_ENABLED) begin
                    stall <= `STOP;
                end
            end else if (op == `MFLO_OP && func == `MFLO_FUNC) begin
                if (exeLoWena == `WRITE_ENABLED) begin
                    count <= 1;
                    stall <= `STOP;
                end else if (memLoWena == `WRITE_ENABLED) begin
                    stall <= `STOP;
                end
            end else begin
                if (exeRfWena == `WRITE_ENABLED && rfRena1 == `READ_ENABLED && exeRfWaddr == rs) begin
                    count <= 1'b1;
                    stall <= `STOP;
                end else if (memRfWena == `WRITE_ENABLED && rfRena1 == `READ_ENABLED && memRfWaddr == rs) begin
                    stall <= `STOP;
                end
                if (exeRfWena == `WRITE_ENABLED && rfRena2 == `READ_ENABLED && exeRfWaddr == rt) begin
                    count <= 1'b1;
                    stall <= `STOP;
                end else if(memRfWena == `WRITE_ENABLED && rfRena2 == `READ_ENABLED && memRfWaddr == rt) begin
                    stall <= `STOP;
                end
            end
        end
	end      

endmodule