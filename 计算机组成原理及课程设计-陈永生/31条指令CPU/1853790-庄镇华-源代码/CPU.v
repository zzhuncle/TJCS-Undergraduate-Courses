`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 20:56:10
// Design Name: 
// Module Name: CPU
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

//CPU31
module CPU(
    input clk,
	input reset,
	input [31 : 0] instr_code,
	input [31 : 0] rdata,
	output [31 : 0] pc,
	output [31 : 0] addr,
	output [31 : 0] wdata,
	output IM_R,
	output DM_CS,
	output DM_R,
	output DM_W
    );
	 //控制信号(除有关存储器）
    wire PC_CLK;                          
    wire M1;                         
    wire M2;                         
    wire M3;                         
    wire M4;                         
    wire M5;                         
    wire M6;                         
    wire M7;                         
    wire M8;                        
    wire M9;                                                
    wire [3:0] ALUC;                 
    wire RF_W;                       
    wire RF_CLK;                     
    wire S_EXT16;                    
    //运算标志位
	wire zero;
	wire carry;
	wire negative;
	wire overflow;
	//译码后指令
	wire [31 : 0] instr_index;
	//数据通路(除有关存储器）（从部件输出的通路）
	wire [31:0] D_ALU;               
    wire [31:0] D_PC;                
    wire [31:0] D_RF;                
    wire [31:0] D_Rs;                
    wire [31:0] D_Rt;                
    wire [31:0] D_IM;                
    wire [31:0] D_DM;                
    wire [31:0] D_Mux1;              
    wire [31:0] D_Mux2;              
    wire [31:0] D_Mux3;              
    wire [4:0]  D_Mux4;              
    wire [4:0]  D_Mux5;              
    wire [31:0] D_Mux6;              
    wire [31:0] D_Mux7;              
    wire [31:0] D_Mux8;              
    wire [31:0] D_Mux9;                        
                                      
    wire [31:0] D_EXT1;              
    wire [31:0] D_EXT5;              
    wire [31:0] D_EXT16;             
    wire [31:0] D_EXT18;             
    wire [31:0] D_ADD;               
    wire [31:0] D_ADD8;              
    wire [31:0] D_NPC;               
    wire [31:0] D_ii;         
    assign PC_CLK = clk;
	assign RF_CLK = ~clk;
	//外部通路连接
    assign pc = D_PC;
    assign addr = D_ALU;
    assign wdata = D_Rt;
	//指令译码
    instr_dec cpu_ins(instr_code, instr_index);
    operation cpu_opcode(zero, instr_index, IM_R, M1, M2, M3, M4, M5, M6, M7, M8, M9, ALUC, RF_W, DM_W, DM_R, DM_CS, S_EXT16);//IM_R
	//部件
    PCReg   pc_out      (PC_CLK, reset, D_Mux1, D_PC);//有时钟信号
    ALU     cpu_alu     (D_Mux8, D_Mux9, ALUC[3:0], D_ALU, zero, carry, negative, overflow);
    RegFile cpu_ref     (RF_CLK, reset, RF_W, instr_code[25:21], instr_code[20:16], D_Mux5, D_Mux6, D_Rs, D_Rt);
    MUX     cpu_mux1    (D_Mux2, D_Mux3, M1, D_Mux1);
    
    MUX     cpu_mux2    (D_ii, D_Rs, M2, D_Mux2);
	MUX     cpu_mux3    (D_NPC, D_ADD, M3, D_Mux3);
    MUX5    cpu_mux4    (instr_code[10:6], D_Rs[4:0], {instr_index[30], M4}, D_Mux4);
    MUX5    cpu_mux5    (instr_code[15:11], instr_code[20:16], {instr_index[30], M5}, D_Mux5);
    MUX     cpu_mux6    (D_Mux7, D_ADD8, M6, D_Mux6);
    MUX     cpu_mux7    (rdata, D_ALU, M7, D_Mux7);
    MUX     cpu_mux8    (D_EXT5, D_Rs, M8, D_Mux8);
    MUX     cpu_mux9    (D_EXT16, D_Rt, M9, D_Mux9);
    extend5  cpu_ext5   (D_Mux4, D_EXT5);
    extend16 cpu_ext16  (instr_code[15:0], S_EXT16, D_EXT16);
    extend18 cpu_ext18  (instr_code[15:0], D_EXT18);
    add     cpu_add     (D_EXT18, D_NPC, D_ADD, overflow);
    add8    cpu_add8    (D_PC, D_ADD8);
    NPC     cpu_npc     (D_PC, reset, D_NPC);
    II      cpu_ii      (D_PC[31:28], instr_code[25:0], D_ii);
endmodule
