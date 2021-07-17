
`timescale 1ns / 1ps

module Counter(
    input [31 : 0] in,
    input ena,
    output reg [31 : 0] out
    );
	
	always @ (*) begin
		if (ena == `ENABLED) begin
			if (in[31])
				out <= 32'd0;
			else if (in[30])
				out <= 32'd1;
			else if (in[29])
				out <= 32'd2;
			else if (in[28])
				out <= 32'd3;
			else if (in[27])
				out <= 32'd4;
			else if (in[26])
				out <= 32'd5;
			else if (in[25])
				out <= 32'd6;
			else if (in[24])
				out <= 32'd7;
			else if (in[23])
				out <= 32'd8;
			else if (in[22])
				out <= 32'd9;
			else if (in[21])
				out <= 32'd10;
			else if (in[20])
				out <= 32'd11;
			else if (in[19])
				out <= 32'd12;
			else if (in[18])
				out <= 32'd13;
			else if (in[17])
				out <= 32'd14;
			else if (in[16])
				out <= 32'd15;
			else if (in[15])
				out <= 32'd16;
			else if (in[14])
				out <= 32'd17;
			else if (in[13])
				out <= 32'd18;
			else if (in[12])
				out <= 32'd19;
			else if (in[11])
				out <= 32'd20;
			else if (in[10])
				out <= 32'd21;
			else if (in[9])
				out <= 32'd22;
			else if (in[8])
				out <= 32'd23;
			else if (in[7])
				out <= 32'd24;
			else if (in[6])
				out <= 32'd25;
			else if (in[5])
				out <= 32'd26;
			else if (in[4])
				out <= 32'd27;
			else if (in[3])
				out <= 32'd28;
			else if (in[2])
				out <= 32'd29;
			else if (in[1])
				out <= 32'd30;
			else if (in[0])
				out <= 32'd31;
			else if (in[0] == 1'b0)
				out <= 32'd32;
			else 
				out <= 32'bx;
		end 
    end
	
endmodule


