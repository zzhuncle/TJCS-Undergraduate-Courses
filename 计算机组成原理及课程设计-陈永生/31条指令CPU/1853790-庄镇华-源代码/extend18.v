`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 14:24:33
// Design Name: 
// Module Name: extend18
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

//18位零扩展为32位
module extend18 #(parameter WIDTH = 18)(
    input [15 : 0] a,
	output [31 : 0] b
    );
	assign b = {{(32 - WIDTH){a[15]}}, a, 2'b00};
endmodule
