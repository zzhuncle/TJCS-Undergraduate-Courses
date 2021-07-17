`timescale 1ns / 1ps

//PC寄存器
module PCReg(
    input clk,
	input rst,
	input ena,
	input [31 : 0] data_in,
	output reg [31 : 0] data_out
    );
	
	always @ (negedge clk or posedge rst) begin
	    if (rst) 
		    data_out <= 32'h00400000;//指令存储起始地址
		else begin 
		    if (ena)
                data_out <= data_in;
        end
	end
endmodule

//HI LO寄存器
module HILOReg(
    input clk,
    input rst,
    input ena,
    input [31 : 0] data_in,
    output reg [31 : 0] data_out
);

    always @ (negedge clk or posedge rst) begin
        if (rst)
            data_out <= 32'b0;
        else begin
            if (ena)
                data_out <= data_in;
        end
    end
endmodule
