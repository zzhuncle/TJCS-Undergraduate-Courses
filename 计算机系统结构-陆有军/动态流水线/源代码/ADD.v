
`timescale 1ns / 1ps

module Add32 (
    input [31 : 0] a,
    input [31 : 0] b,
    output [31 : 0] z
    );
    assign z = a + b;
endmodule