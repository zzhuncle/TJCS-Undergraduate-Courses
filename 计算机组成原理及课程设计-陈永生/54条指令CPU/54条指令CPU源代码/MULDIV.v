`timescale 1ns / 1ps

`define MUL     2'b00
`define MULTU   2'b01
`define DIV     2'b10
`define DIVU    2'b11

//乘除法器
module MULDIV(
    input [1 : 0] mdx,
    input [31 : 0] a,
    input [31 : 0] b,
    output reg [31 : 0] r1,
    output reg [31 : 0] r2
    );

    always @ (*) begin
        case(mdx)
            `MUL : {r2, r1} = $signed(a) * $signed(b);
            `MULTU : {r2, r1} = a * b;
            `DIV :  
				begin
					r1 <= $signed(a) / $signed(b);
					r2 <= $signed(a) % $signed(b);
				end
            `DIVU : 
				begin
					r1 <= a / b;
					r2 <= a % b;
				end
            default : {r2, r1} = 64'bx;
        endcase
    end
endmodule
