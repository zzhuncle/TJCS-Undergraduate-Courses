
`timescale 1ns / 1ps

//扩展
module Extend #(parameter WIDTH = 5)(
    input [WIDTH - 1 : 0] a,
    input sign, //1有符号 0无符号
    output [31 : 0] z
    );
    assign z = sign ? ({{(32 - WIDTH){a[WIDTH - 1]}}, a}) : ({{(32 - WIDTH){1'b0}}, a});
endmodule
