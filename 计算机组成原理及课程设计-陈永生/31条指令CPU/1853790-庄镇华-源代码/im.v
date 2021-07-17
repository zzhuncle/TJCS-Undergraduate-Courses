`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/14 11:25:29
// Design Name: 
// Module Name: im
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

//指令存储器
module im(
    input [10 : 0] addr,
	output reg [31 : 0] inst
    );
	reg  [31 : 0] ram [4027 : 0];
	initial begin
	    $readmemh("D:/lout.txt", ram);
	end
	always @ (*) begin
		inst <= ram[addr];//数据存储起始地址
	end
endmodule
