
`timescale 1ns / 1ns

//寄存器
module Reg(
    input clk, 
    input rst, 
    input wena, 
    input [31 : 0] in, 
    output reg [31 : 0] out 
    );
	
    always @ (negedge clk or posedge rst)
    begin
        if(rst == 1) begin
            out <= 0;
        end else if(wena == 1) begin
            out <= in;
        end
    end

endmodule

module PcReg(
    input clk, 
    input rst, 
    input wena, 
    input stall,  // 暂停
    input [31:0] data_in,   // 下一个pc
    output [31:0] data_out   // 当前pc
    );

    reg [31:0] pc;

    always @ (posedge clk or posedge rst)
    begin
        if(rst == `RST_ENABLED) begin
            pc <= 32'h00400000;
        end

        else if(stall == `RUN) begin
            if(wena == `WRITE_ENABLED) begin
                pc <= data_in;
            end
        end
    end

    assign data_out = pc;

endmodule