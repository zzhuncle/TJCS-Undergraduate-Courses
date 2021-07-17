
`timescale 1ns / 1ps

`define STATUS_INDEX 12//status  
`define CAUSE_INDEX  13//cause 
`define EPC_INDEX    14//epc 

`define SYSCALL_CODE 5'b01000 // syscall excCode
`define BREAK_CODE   5'b01001 // break   excCode
`define TEQ_CODE     5'b01101 // teq     excCode

//协处理器
module cp0(
	input clk,
	input rst,
	input mfc0,//读CP0指令
	input mtc0,//写CP0指令
	input [31 : 0] pc,
	
	input [4 : 0] addr,//读、写CP0的寄存器地址
    input [31 : 0] wdata,//写入到CP0中的数据
	input exception,
	input eret,

	input [4 : 0] cause,
	output [31 : 0] rdata,//从CP0读出的数据
	output [31 : 0] status,
	output [31 : 0] epc_out
    );
	
	reg [31:0] cp0_regfile[31:0];
	
	assign rdata = (mfc0 ? cp0_regfile[addr] : rdata);
    assign status = cp0_regfile[`STATUS_INDEX];
    assign epc_out = eret ? cp0_regfile[`EPC_INDEX] : 32'h0040_0004;

    always @(negedge clk or posedge rst) begin
		if (rst) begin
			cp0_regfile[0] <= 0;
			cp0_regfile[1] <= 0;
			cp0_regfile[2] <= 0;
			cp0_regfile[3] <= 0;
			cp0_regfile[4] <= 0;
			cp0_regfile[5] <= 0;
			cp0_regfile[6] <= 0;
			cp0_regfile[7] <= 0;
			cp0_regfile[8] <= 0;
			cp0_regfile[9] <= 0;
			cp0_regfile[10] <= 0;
			cp0_regfile[11] <= 0;
			cp0_regfile[12] <= 32'h0000_0701;
			cp0_regfile[13] <= 0;
			cp0_regfile[14] <= 0;
			cp0_regfile[15] <= 0;
			cp0_regfile[16] <= 0;
			cp0_regfile[17] <= 0;
			cp0_regfile[18] <= 0;
			cp0_regfile[19] <= 0;
			cp0_regfile[20] <= 0;
			cp0_regfile[21] <= 0;
			cp0_regfile[22] <= 0;
			cp0_regfile[23] <= 0;
			cp0_regfile[24] <= 0;
			cp0_regfile[25] <= 0;
			cp0_regfile[26] <= 0;
			cp0_regfile[27] <= 0;
			cp0_regfile[28] <= 0;
			cp0_regfile[29] <= 0;
			cp0_regfile[30] <= 0;
			cp0_regfile[31] <= 0;
		end else begin
			if (mtc0) 
				cp0_regfile[addr] <= wdata;
			else if (exception) begin//处理异常
				//异常发生时的指令地址
				cp0_regfile[`EPC_INDEX] <= pc;
				//cause reg只用到 6-2 位
				cp0_regfile[`CAUSE_INDEX] <= {25'b0, cause, 2'b0};
			end 
		end
	end
endmodule
