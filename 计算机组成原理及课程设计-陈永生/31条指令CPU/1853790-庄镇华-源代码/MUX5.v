`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 11:49:03
// Design Name: 
// Module Name: MUX5
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

//5位选择器
module MUX5(
    input [4 : 0] a,
	input [4 : 0] b,
	input [1 : 0] choice,
	output [4 : 0] z
    );
	reg [4 : 0] temp;
	always @ (*) begin
	    case (choice)
		    2'b00: temp <= a;
			2'b01: temp <= b;
			2'b10: temp <= 5'b11111;
			2'b11: temp <= 5'b11111;
		    default: temp <= 5'bzzzzz;
		endcase
	end		   
    assign z = temp;	
endmodule
