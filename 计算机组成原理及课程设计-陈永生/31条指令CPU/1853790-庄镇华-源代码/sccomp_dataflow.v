`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/11 17:51:53
// Design Name: 
// Module Name: sccomp_dataflow
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

//顶层模块
module sccomp_dataflow(
    input clk_in,
	input reset,
	output [31 : 0] inst,
	output [31 : 0] pc
    );
	wire IM_R;
	wire DM_W;
	wire DM_R;
	wire DM_CS;
	wire [31 : 0] Out_ALU;
    wire [31 : 0] Out_DMEM;
    wire [31 : 0] Out_Rt;
	CPU  sccpu(clk_in, reset, inst, Out_DMEM, pc, Out_ALU, Out_Rt, IM_R, DM_CS, DM_R, DM_W);
	//im im0(pc[12 : 2], inst);
	imem imem(pc[12 : 2], inst);
	DMEM dmem0(clk_in, DM_CS, DM_W, DM_R, Out_ALU, Out_Rt, Out_DMEM);
endmodule
