`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 14:27:11
// Design Name: 
// Module Name: add
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

//加法器
module add(
    input [31 : 0] a,
	input [31 : 0] b,
	output [31 : 0] z,
	output overflow
    );
	assign z = a + b;
	assign overflow = (a[31] == b[31] && a[31] != z[31]) ? 1 : 0;
endmodule
