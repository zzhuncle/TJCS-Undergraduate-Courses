
`timescale 1ns / 1ps

module Cp0(
    input clk,
    input rst,
    input mfc0,
    input mtc0,
    input [31 : 0] pc,
    input [4 : 0] addr,
    input [31 : 0] wdata,
    input exception,
    input eret,
    input [4 : 0] cause,
    output [31 : 0] rdata,
    output [31 : 0] status,
    output [31 : 0] epc_out
    );

    reg [31 : 0] cp0_register [31 : 0];
    reg [31 : 0] temp;
    integer i;

    always @ (posedge clk or posedge rst) 
    begin
        if (rst == `RST_ENABLED) begin
            for(i = 0; i < 32; i = i + 1) begin
                if(i == `STATUS)
                    cp0_register[i] <= 32'h0000000f;
                else
                    cp0_register[i] <= 0;
            end
            temp <= 0;
        end else begin if(mtc0)
                cp0_register[addr] <= wdata;
			else if(exception) begin
                cp0_register[`EPC] = pc;
                temp = cp0_register[`STATUS];
                cp0_register[`STATUS] = cp0_register[`STATUS] << 5;
                cp0_register[`CAUSE] = {25'b0, cause, 2'b0};
            end else if(eret) 
                cp0_register[`STATUS] = temp;
        end 
    end

    assign status = cp0_register[`STATUS];
    assign epc_out = eret ? cp0_register[`EPC] : `EXCEPTION_ADDR;
    assign rdata = mfc0 ? cp0_register[addr] : 32'h0;

endmodule
