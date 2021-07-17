`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 12:25:39
// Design Name: 
// Module Name: PCReg
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

//PC寄存器
module PCReg(
    input clk,
	input rst,
	input [31 : 0] data_in,
	output reg [31 : 0] data_out
    );
	always @ (negedge clk or posedge rst) begin
	    if (rst) begin
		    data_out <= 32'h00400000;//指令存储起始地址
		end else begin
		    data_out <= data_in;
		end
	end
endmodule
