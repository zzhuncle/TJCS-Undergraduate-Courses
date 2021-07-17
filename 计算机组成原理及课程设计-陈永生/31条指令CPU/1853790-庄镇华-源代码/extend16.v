`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 14:20:28
// Design Name: 
// Module Name: extend16
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

//16位扩展为32位：根据SIGN情况
module extend16 #(parameter WIDTH = 16)(
    input [WIDTH - 1 : 0] a,
	input sign,
	output [31 : 0] b
    );
	assign b = sign ? {{(32 - WIDTH){a[WIDTH - 1]}}, a} : {{(32 - WIDTH){1'b0}}, a};
endmodule
