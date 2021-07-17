`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 12:36:16
// Design Name: 
// Module Name: extend5
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

//5位零扩展为16位
module extend5 #(parameter WIDTH = 5)(
    input [WIDTH - 1 : 0] a,
	output [31 : 0] b
    );
	assign b = {{(32 - WIDTH){1'b0}}, a};
endmodule
