`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 14:37:06
// Design Name: 
// Module Name: ALU
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

//运算器
module ALU(
    input [31 : 0] a,  //32位输入，操作数1
    input [31 : 0] b,  //32位输入，操作数2
    input [3 : 0] aluc,//4位输入，控制alu的操作
    output [31 : 0] r, //32位输出，由a,b经过aluc指定的操作生成
    output zero,    //0标志位
    output carry,    //进位标志位//无符号和移位运算影响此运算符
    output negative, //负数标志位
    output overflow  //溢出标志位//只有有符号加减法影响此标记位
    );
	reg [32 : 0] result;

	wire signed [31 : 0] sa = a, sb = b;
	always @ (*) begin
		case(aluc)
			4'b0000: result = a + b;
			4'b0001: result = a - b;
			4'b0010: result = sa + sb;
			4'b0011: result = sa - sb;
			4'b0100: result = a & b;
			4'b0101: result = a | b;
			4'b0110: result = a ^ b;
			4'b0111: result = ~(a | b);
			4'b1000, 4'b1001: result = {b[15 : 0], 16'b0};
			4'b1010: result = a < b ? 1 : 0;
			4'b1011: result = sa < sb ? 1 : 0;
			4'b1100: {result[31 : 0], result[32]} = {sb >>> a, 1'b0};
			4'b1101: {result[31 : 0], result[32]} = {b >> a, 1'b0};
			4'b1110, 4'b1111: result = b << a;
			default: result = a + b;
		endcase
	end
	assign r = result[31 : 0];
	assign carry = result[32];
	assign zero = (r == 32'b0) ? 1 : 0;
	assign negative = result[31];
	assign overflow = result[32];
endmodule