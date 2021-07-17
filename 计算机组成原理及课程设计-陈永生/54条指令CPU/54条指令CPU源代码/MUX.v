`timescale 1ns / 1ps

//2路选择器 M4 M5 M6 M7 M8
module MUX #(parameter WIDTH = 32)(
    input [WIDTH - 1 : 0] a,
    input [WIDTH - 1 : 0] b,
    input M,
    output reg [WIDTH - 1 : 0] z
    );
    reg [WIDTH - 1 : 0] temp;
    always @ (*) begin
        if (M == 1'b0)
            z <= a;
        else if (M == 1'b1)
            z <= b;
        else 
            z <= {WIDTH{1'bx}};
    end
endmodule

//3路选择器 M3 
module MUX_JAL #(parameter WIDTH = 32)(
    input [WIDTH - 1 : 0] a,
    input [WIDTH - 1 : 0] b,
    input [1 : 0] M,
    output reg [WIDTH - 1 : 0] z
    );
    reg [WIDTH - 1 : 0] temp;
    always @ (*) begin
        casex(M) 
            2'b00 : z <= a;
            2'b01 : z <= b;
            2'b1x : z <= {WIDTH{1'b1}};
            default : z <= {WIDTH{1'bx}};
        endcase
    end
endmodule

//6路选择器 M1
module MUX_PC(
    input [31 : 0] npc,
    input [31 : 0] ii,
    input [31 : 0] rs,
    input [31 : 0] add,
    input [31 : 0] epc_out,
    input [2 : 0] M,
    output reg [31 : 0] res
);
    always @ (*) begin
        case(M)
            3'b000 : res = npc;
            3'b001 : res = ii;
            3'b010 : res = rs;
            3'b011 : res = add;
            3'b100 : res = epc_out;
            3'b101 : res = 32'h0040_0004;
			default : res = 32'bx;
        endcase
    end
endmodule

//8路选择器 M2
module MUX_Rd(
    input [31 : 0] alu,
    input [31 : 0] dmem,
    input [31 : 0] add8,
    input [31 : 0] cp0,
    input [31 : 0] lo,
    input [31 : 0] hi,
    input [31 : 0] mul_r1,
    input [31 : 0] clz,
    input [2 : 0] M,
    output reg [31:0] res
);
    always @ (*) begin
        case(M)
            3'b000 : res = alu;
            3'b001 : res = dmem;
            3'b010 : res = add8;
            3'b011 : res = cp0;
            3'b100 : res = lo;
            3'b101 : res = hi;
            3'b110 : res = mul_r1;
            3'b111 : res = clz;
            default : res = 32'bx;
        endcase
    end
endmodule
