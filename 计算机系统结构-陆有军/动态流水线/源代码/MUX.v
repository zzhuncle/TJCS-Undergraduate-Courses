
`timescale 1ns / 1ps

module Mux2_5 (
    input [4 : 0] a,
    input [4 : 0] b,
    input sign,
    output reg [4 : 0] z
    );
	
    always @ (*) begin
        case (sign)
            1'b0: z <= a;
            1'b1: z <= b;
        endcase
    end
	
endmodule

module Mux2_32 (
    input [31 : 0] a,
    input [31 : 0] b,
    input sign,
    output reg [31 : 0] z
    );
	
    always @ (*) begin
        case (sign)
            1'b0: z <= a;
            1'b1: z <= b;
        endcase
    end
	
endmodule

module Mux4_32 (
    input [31 : 0] a,
    input [31 : 0] b,
    input [31 : 0] c,
    input [31 : 0] d,
    input [1 : 0] sign,
    output reg [31:0] z
    );
	
    always @ (*) begin
        case (sign)
            2'b00: z <= a;
            2'b01: z <= b;
            2'b10: z <= c;
            2'b11: z <= d;
            default: z <= 31'bz;
        endcase
   end

endmodule

module Mux8_32 (
    input [31 : 0] a,
    input [31 : 0] b,
    input [31 : 0] c,
    input [31 : 0] d,
    input [31 : 0] e,
    input [31 : 0] f,
    input [31 : 0] g,
    input [31 : 0] h,
    input [2 : 0]  sign,
    output reg [31 : 0] z
    ); 
	
    always @ (*) begin
        case (sign)
            3'b000: z <= a;
            3'b001: z <= b;
            3'b010: z <= c;
            3'b011: z <= d;
            3'b100: z <= e;
            3'b101: z <= f;
            3'b110: z <= g;
            3'b111: z <= h;
            default: z <= 32'bz;
        endcase
    end
	
endmodule
