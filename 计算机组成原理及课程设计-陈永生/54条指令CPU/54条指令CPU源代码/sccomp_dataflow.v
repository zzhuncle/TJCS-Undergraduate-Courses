`timescale 1ns / 1ps

module sccomp_dataflow(
    input clk_in,
    input reset,
    output [31 : 0] inst,
    output [31 : 0] pc
    );
	wire [31 : 0] _inst;
    wire [31 : 0] rdata;//DMEM
    wire [31 : 0] addr;//DMEM
    wire [31 : 0] wdata;//DMEM
    wire [1 : 0] DMS_mux;
    wire [2 : 0] DML_mux;
    wire [31 : 0] _pc;
    wire DM_W;
    
    assign inst = _inst;
    assign pc = _pc;
    
    cpu sccpu(clk_in, reset, _inst, rdata, _pc, addr, wdata, DMS_mux, DML_mux, DM_W);
    imem scimem(_pc[12 : 2], _inst);
    DMEM scdmem(clk_in, DM_W, DMS_mux, DML_mux, addr, wdata, rdata);
endmodule
