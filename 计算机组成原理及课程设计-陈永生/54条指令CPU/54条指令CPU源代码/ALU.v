`timescale 1ns / 1ps

`define ALU_ADDU    4'b0000
`define ALU_ADD     4'b0010
`define ALU_SUBU    4'b0001
`define ALU_SUB     4'b0011
`define ALU_AND     4'b0100
`define ALU_OR      4'b0101
`define ALU_XOR     4'b0110
`define ALU_NOR     4'b0111
`define ALU_LUI     4'b100x
`define ALU_SLT     4'b1011
`define ALU_SLTU    4'b1010
`define ALU_SRA     4'b1100
`define ALU_SLL_A   4'b111x
`define ALU_SRL     4'b1101

//运算器
module ALU(    
    input [31 : 0] a,    
    input [31 : 0] b,    
    input [3 : 0] aluc,   
    output [31 : 0] r,    
    output zero,        //零
    output carry,       //位
    output negative,    //负
    output overflow     //溢
    );
	reg [32 : 0] temp;

    wire Equal;
    wire isZero;

    assign Equal = (a == b);
    assign isZero = (temp[31 : 0] == 32'd0);

    assign r = temp[31 : 0];
    //有符号加减法
    assign overflow = (aluc == `ALU_ADD | aluc == `ALU_SUB) ? ((temp[32] != temp[31]) ? 1'b1 : 1'b0) : 1'b0;
    assign carry = (aluc == `ALU_ADDU | aluc == `ALU_SUBU | aluc == `ALU_SLTU | aluc === `ALU_SLL_A | aluc == `ALU_SRA | aluc == `ALU_SRL) ? temp[32] : 1'b0;
    assign negative = (aluc == `ALU_SLT) ? r[0] : r[31];
    assign zero = (aluc == `ALU_SLT | aluc == `ALU_SLTU) ? Equal : isZero;

    always @ (*) begin 
        casex(aluc)
            `ALU_ADDU : temp = {1'b0, a} + {1'b0, b};
            `ALU_ADD : temp = {a[31], a} + {b[31], b};
            `ALU_SUBU : temp = {1'b0, a} - {1'b0, b};
            `ALU_SUB : temp = {a[31], a} - {b[31], b};
            `ALU_AND : 
				begin
					temp[32] = 1'b0;
					temp[31 : 0] = a & b;
				end
            `ALU_OR : 
				begin
					temp[32] = 1'b0;
					temp[31 : 0] = a | b;
				end
            `ALU_XOR : 
				begin
					temp[32] = 1'b0;
					temp[31 : 0] = a ^ b;
				end
            `ALU_NOR : 
				begin
					temp[32] = 1'b0;
					temp[31 : 0] = ~(a | b);
				end
            `ALU_LUI : 
				begin
					temp[32] = 1'b0;
					temp[31 : 0] = {b[15 : 0], 16'b0};
				end
            `ALU_SLT : temp = ($signed(a) < $signed(b)) ? 33'd1 : 33'd0;
            `ALU_SLTU : temp = (a < b) ? {1'b1, 32'd1} : {1'b0, 32'd0};
            `ALU_SRA : 
				begin
					if (a == 32'd0)
						{temp[31 : 0], temp[32]} = {b, 1'b0};
					else 
						{temp[31 : 0], temp[32]} = ($signed(b) >>> (a - 1'b1));
				end
            `ALU_SLL_A : temp = (b << a[4 : 0]);
            `ALU_SRL : 
				begin
					if (a == 32'd0)
						{temp[31 : 0], temp[32]} = {b, 1'b0};
					else 
						{temp[31 : 0], temp[32]} = (b >> (a - 1'b1));
				end
            default : ;
        endcase 
    end
endmodule
