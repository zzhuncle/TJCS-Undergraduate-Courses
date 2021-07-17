
`timescale 1ns / 1ps

module BranchCompare(
    input clk,
    input rst,
    input [31 : 0] in1, 
    input [31 : 0] in2,
    input [5 : 0] op,
    input [5 : 0] func,
    input exception,
    output reg isBranch 
    );
	
	always @ (*) begin
	    if (rst == `RST_ENABLED)
	        isBranch <= 1'b0;
		else if (op == `BGEZ_OP) 
			isBranch <= (in1 >= 0) ? 1'b1 : 1'b0;	
		else if (op == `BEQ_OP) 
			isBranch <= (in1 == in2) ? 1'b1 : 1'b0;
		else if (op == `TEQ_OP && func == `TEQ_FUNC)
			isBranch <= (in1 == in2) ? 1'b1 : 1'b0;
	    else if (op == `BNE_OP) 
			isBranch <= (in1 != in2) ? 1'b1 : 1'b0;
	    else if (op == `J_OP)
			isBranch <= 1'b1;
	    else if (op == `JR_OP && func == `JR_FUNC)
            isBranch <= 1'b1;
	    else if (op == `JAL_OP)
	        isBranch <= 1'b1;
        else if (op == `JALR_OP && func == `JALR_FUNC)
            isBranch <= 1'b1;
        else if (exception)
            isBranch <= 1'b1;
        else
            isBranch <= 1'b0;
	end      
	
endmodule
