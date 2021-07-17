`timescale 1ns / 1ps

//ADD
module add(
    input [31 : 0] a,
    input [31 : 0] b,
    output [31 : 0] z
    );
    assign z = a + b;
endmodule

//ADD8
module add8(
    input [31 : 0] a,
    output [31 : 0] z
    );
    assign z = a + 4;
endmodule

//NPC
module npc(
    input [31 : 0] a,
    input rst,  //高电平有效
    output [31 : 0] z
    );
    assign z = (rst == 1'b1) ? a : a + 4;
endmodule
