`timescale 1ns / 1ps

//计算前导零个数
module counter(
	input [31 : 0] data,
    output reg [31 : 0] count
    );
	always @ (*) begin
		if (data[31])
			count <= 32'd0;
		else if (data[30])
			count <= 32'd1;
		else if (data[29])
			count <= 32'd2;
		else if (data[28])
			count <= 32'd3;
		else if (data[27])
			count <= 32'd4;
		else if (data[26])
			count <= 32'd5;
		else if (data[25])
			count <= 32'd6;
		else if (data[24])
			count <= 32'd7;
		else if (data[23])
			count <= 32'd8;
		else if (data[22])
			count <= 32'd9;
		else if (data[21])
			count <= 32'd10;
		else if (data[20])
			count <= 32'd11;
		else if (data[19])
			count <= 32'd12;
		else if (data[18])
			count <= 32'd13;
		else if (data[17])
			count <= 32'd14;
		else if (data[16])
			count <= 32'd15;
		else if (data[15])
			count <= 32'd16;
		else if (data[14])
			count <= 32'd17;
		else if (data[13])
			count <= 32'd18;
		else if (data[12])
			count <= 32'd19;
		else if (data[11])
			count <= 32'd20;
		else if (data[10])
			count <= 32'd21;
		else if (data[9])
			count <= 32'd22;
		else if (data[8])
			count <= 32'd23;
		else if (data[7])
			count <= 32'd24;
		else if (data[6])
			count <= 32'd25;
		else if (data[5])
			count <= 32'd26;
		else if (data[4])
			count <= 32'd27;
		else if (data[3])
			count <= 32'd28;
		else if (data[2])
			count <= 32'd29;
		else if (data[1])
			count <= 32'd30;
		else if (data[0])
			count <= 32'd31;
		else if (data[0] == 1'b0)
			count <= 32'd32;
		else 
			count <= 32'bx;
    end
endmodule
