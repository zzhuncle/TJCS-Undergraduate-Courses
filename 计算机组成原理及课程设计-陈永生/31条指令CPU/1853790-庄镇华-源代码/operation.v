`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/06/10 09:15:19
// Design Name: 
// Module Name: operation
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

//控制器
module operation(
	input z,//ALU的零标志位
	input [31 : 0] instr_index,
	output IM_R,
	output M1,
	output M2,
	output M3,
	output M4,
	output M5,
	output M6,
	output M7,
	output M8,
	output M9,
	output [3 : 0] ALUC,
	output RF_W,
	output DM_W,
	output DM_R,
	output DM_CS,
	output S_EXT16//16位有符号扩展
    );
	
	assign IM_R = 1;
	assign M1 = ~(instr_index[16] | instr_index[29] | instr_index[30]);
	assign M2 = instr_index[16];
	assign M3 = (instr_index[24] & z) | (instr_index[25] & ~z);
 	assign M4 = instr_index[13] | instr_index[14] | instr_index[15];
	assign M5 = instr_index[17] | instr_index[18] | instr_index[19] | instr_index[20] | instr_index[21] | 
	            instr_index[22] | instr_index[23] | instr_index[26] | instr_index[27] | instr_index[28];
	assign M6 = instr_index[30];
	assign M7 = ~instr_index[22];
	assign M8 = ~(instr_index[10] | instr_index[11] | instr_index[12] | instr_index[13] | instr_index[14] | instr_index[15]);
	assign M9 = ~(instr_index[17] | instr_index[18] | instr_index[19] | instr_index[20] | instr_index[21] | 
	              instr_index[22] | instr_index[23] | instr_index[26] | instr_index[27] | instr_index[28]);
	assign RF_W = ~(instr_index[16] | instr_index[23] | instr_index[24] | instr_index[25] | instr_index[29]);
	
	assign DM_W = instr_index[23];
	assign DM_R = instr_index[22];
	assign DM_CS = instr_index[22] | instr_index[23];
	assign ALUC[0] = instr_index[2] | instr_index[3] | instr_index[5] | instr_index[7] | instr_index[8] | instr_index[11]|
		             instr_index[14] | instr_index[20] | instr_index[24] | instr_index[25] | instr_index[26];
	assign ALUC[1] = instr_index[0] | instr_index[2] | instr_index[6] | instr_index[7] | instr_index[8] | instr_index[9] |
		             instr_index[10] | instr_index[13] | instr_index[17] | instr_index[21] | instr_index[22] | 
					 instr_index[23] | instr_index[24] | instr_index[25] | instr_index[26] | instr_index[27];
	assign ALUC[2] = instr_index[4] | instr_index[5] | instr_index[6] | instr_index[7] | instr_index[10] | instr_index[11] | 
	                 instr_index[12] | instr_index[13] | instr_index[14] | instr_index[15] | 
					 instr_index[19] | instr_index[20] | instr_index[21];
	assign ALUC[3] = instr_index[8] | instr_index[9] | instr_index[10] | instr_index[11] | instr_index[12] | instr_index[13] | 
	                 instr_index[14] | instr_index[15] | instr_index[26] | instr_index[27] | instr_index[28];
    assign S_EXT16 = ~(instr_index[19] | instr_index[20] | instr_index[21]);//除逻辑运算外均为符号扩展
endmodule
