
`timescale 1ns / 1ps
module DIVU(//32位无符号不恢复余数除法器
    input [31 : 0] dividend,//被除数
    input [31 : 0] divisor,//除数
    input start,//启动除法运算
    input clock,
    input reset,//高电平有效
    output [31 : 0] q,//商
    output [31 : 0] r,//余数
    output reg busy//除法器忙标志位
    );
	
	wire ready;
	reg [4 : 0] count;
	reg [31 : 0] reg_q;//商
	reg [31 : 0] reg_r;//余数
	reg [31 : 0] reg_b;//除数
	reg busy2, r_sign;
	assign ready = ~busy & busy2;
	//加、减法器
	//r_sign为0，说明相减结果为正，r_sign为1，说明相减结果为负
	wire [32 : 0] sub_add = r_sign ? ({reg_r, q[31]} + {1'b0, reg_b}) : ({reg_r, q[31]} - {1'b0, reg_b});
	assign r = r_sign ? reg_r + reg_b : reg_r;
	assign q = reg_q;
	always @ (posedge clock or posedge reset) begin
		if (reset) begin//重置
			count <= 5'b0;
			busy <= 0;
			busy2 <= 0;
		end else begin
			busy2 <= busy;
			if (start) begin//开始除法运算，初始化
				reg_r <= 32'b0;
				r_sign <= 0;
				reg_q <= dividend;//被除数赋值给商
				reg_b <= divisor;//除数赋值给除数
				count <= 5'b0;
				busy <= 1'b1;
			end else if (busy) begin//循环操作
				reg_r <= sub_add[31 : 0];//余数
				r_sign <= sub_add[32];//如果为负，下次相加
				reg_q <= {reg_q[30 : 0], ~sub_add[32]};//商
				count <= count + 5'b1;//计数器加1
				if (count == 5'b11111) 
					busy <= 0; 
			end//结束除法运算 
		end
	end
endmodule