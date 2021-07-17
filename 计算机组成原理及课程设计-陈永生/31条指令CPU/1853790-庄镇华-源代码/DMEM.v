`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 10:52:20
// Design Name: 
// Module Name: DMEM
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

//数据存储器
module DMEM(
    input clk,
	input wena,
	input [1 : 0] s_mux,//00 sw, 01 sh, 10 sb
    input [2 : 0] l_mux,//000 lw, 001 lh, 010 lhu, 011 lb, 100 lbu
	input [31:0] addr,
	input [31:0] data_in,
	output [31:0] data_out
    );
	reg [31:0] Buffer[2047:0];
	
	always @ (posedge clk) begin
		if (ena) begin
			if (wena) begin
				Buffer[(addr - 32'h10010000) >> 2] <= data_in;//数据存储起始地址
			end
		end
	end
    assign data_out = (ena & rena) ? (Buffer[(addr - 32'h10010000) >> 2]) : (32'b0);
endmodule
