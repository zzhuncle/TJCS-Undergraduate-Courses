
`timescale 1ns / 1ps

module pipe_top(
    input clk,
    input rst,
    input switchRes,
    output [7:0] oSeg,
    output [7:0] oSel
    );

    reg [30:0] count;
    reg clk1s;

    wire [31:0] instruction;
    wire [31:0] pc;
    wire [31:0] reg28;
    wire [31:0] reg29;
    wire [31:0] seg7In;

    always @ (posedge clk or posedge rst) 
    begin
        if(rst) begin
            clk1s <= 0;
            count <= 0;
        end else if(count == 99999) begin
            count <= 0;
            clk1s <= ~clk1s;
        end else
            count <= count + 1;
    end

    Cpu cpu(clk1s, rst, instruction, pc, reg28, reg29);
    Mux2_32 resMux(reg28, reg29, switchRes, seg7In);
    Seg7x16 seg7(clk, rst, 1, seg7In, oSeg, oSel);

endmodule
