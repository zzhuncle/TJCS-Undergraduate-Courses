`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 11:45:44
// Design Name: 
// Module Name: MUX
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

//32位选择器
module MUX(
    input [31 : 0] a,
	input [31 : 0] b,
	input choice,
	output reg [31 : 0] z
    );
	always @ (*) begin
	    case (choice)
		    1'b0: z <= a;
			1'b1: z <= b;
		endcase
	end
endmodule
