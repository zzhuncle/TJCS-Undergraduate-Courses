`timescale 1ns / 1ps

//单周期54条指令CPU
module cpu(
    input clk,          
    input rst,           
    input [31 : 0] instr_code,   
    input [31 : 0] rdata, 
	output [31 : 0] pc, 
    output [31 : 0] addr,  
    output [31 : 0] wdata,
    output [1 : 0] DMS_mux,
    output [2 : 0] DML_mux,
    output DM_W
    );
    //控制信号
    wire [2 : 0] m1;
    wire [2 : 0] m2;
    wire m3;
    wire m4;
    wire m5;
    wire m6;
    wire m7;
    wire m8;
    wire Hwe;
    wire Lwe;
    wire [1 : 0] mdx;//div divu mul multu
    wire exception;
    wire eret;
    wire mfc0;
    wire mtc0;
    wire [4 : 0] cause;
    wire [3 : 0] aluc;
    wire RF_W;
    wire ext_sign;// 1 符 0 零
    //选择器输出
	wire [31 : 0] out_m1;
	wire [31 : 0] out_m2;
	wire [4 : 0] out_m3;
    wire [4 : 0] out_m4;
    wire [31 : 0] out_m5;
    wire [31 : 0] out_m6;
    wire [31 : 0] out_m7;
    wire [31 : 0] out_m8;
    //ALU输出
    wire carry;
    wire negative;
    wire zero;
    wire overflow;
    wire [31 : 0] out_alu;
    //指令译码输出
    wire [63 : 0] instr_index;
    //pc_reg输出
    wire [31 : 0] out_pc;
    //RegFile输出端口
    wire [31 : 0] out_rf_rs;
    wire [31 : 0] out_rf_rt;
    //npc输出
    wire [31 : 0] out_npc;
    //add输出
    wire [31 : 0] out_add;
    //II输出
    wire [31 : 0] out_ii;
    //add8输出
    wire [31 : 0] out_add8;
    //ext16输出
    wire [31 : 0] out_ext16;
    //clz输出
    wire [31:0] out_clz;
    //muldiv输出端口
    wire [31 : 0] out_r1;
    wire [31 : 0] out_r2;
    //HI LO输出
    wire [31 : 0] out_hi;
    wire [31 : 0] out_lo;
    //cp0输出
    wire [31 : 0] out_rdata;
    wire [31 : 0] out_status;
    wire [31 : 0] out_epc;

    assign pc = out_pc;
    assign addr = out_alu;
    assign wdata = out_rf_rt;

    instr_dec cpu_instr_dec(instr_code, instr_index);
    PCReg cpu_pc(clk, rst, 1'b1, out_m1, out_pc);
    operation cpu_operation(zero, out_rf_rs[31], (out_rf_rs == out_rf_rt), instr_index, m1, m2, m3, m4, m5, m6, m7, m8, Hwe, Lwe, mdx,
	                        exception, eret, mfc0, mtc0, cause, aluc, RF_W, DM_W, DMS_mux, DML_mux, ext_sign);
    ALU cpu_alu(out_m5, out_m6, aluc, out_alu, zero, carry, negative, overflow); 
	RegFile cpu_ref(clk, rst, RF_W, instr_code[25 : 21], instr_code[20 : 16], out_m3, out_m2, out_rf_rs, out_rf_rt);
    npc cpu_npc(out_pc, rst, out_npc);
    add cpu_add({{14{instr_code[15]}}, instr_code[15 : 0], 2'b00}, out_npc, out_add);
    add8 cpu_add8(out_pc, out_add8);
    II cpu_ii(out_pc[31 : 28], instr_code[25 : 0], out_ii);
    counter cpu_clz(out_rf_rs, out_clz);
    HILOReg cpu_hi(clk, rst, Hwe, out_m7, out_hi);
    HILOReg cpu_lo(clk, rst, Lwe, out_m8, out_lo);
    MULDIV cpu_muldiv(mdx, out_rf_rs, out_rf_rt, out_r1, out_r2);
    cp0 cpu_cp0(clk, rst, mfc0, mtc0, out_pc, instr_code[15 : 11], out_rf_rt, exception, eret, cause, out_rdata, out_status, out_epc);
    extend #(.WIDTH(16)) cpu_ext16(instr_code[15 : 0], ext_sign, out_ext16);
	
    MUX_PC cpu_mux1(out_npc, out_ii, out_rf_rs, out_add, out_epc, m1, out_m1);
    MUX_Rd cpu_mux2(out_alu, rdata, out_add8, out_rdata, out_lo, out_hi, out_r1, out_clz, m2, out_m2);
	MUX_JAL #(.WIDTH(5)) cpu_mux3(instr_code[15 : 11], instr_code[20 : 16], {instr_index[30], m3}, out_m3);
	MUX #(.WIDTH(5)) cpu_mux4(instr_code[10 : 6], out_rf_rs[4 : 0], m4, out_m4);
	MUX #(.WIDTH(32)) cpu_mux5({27'b0, out_m4}, out_rf_rs, m5, out_m5);
    MUX #(.WIDTH(32)) cpu_mux6(out_rf_rt, out_ext16, m6, out_m6);
    MUX #(.WIDTH(32)) cpu_mux7(out_rf_rs, out_r2, m7, out_m7);
    MUX #(.WIDTH(32)) cpu_mux8(out_rf_rs, out_r1, m8, out_m8);
	
endmodule
