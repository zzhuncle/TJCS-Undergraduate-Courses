
`timescale 1ns / 1ps

module Seg7x16 (
    input clk,
	input reset,
	input cs,
	input [31 : 0] iData,
	output [7 : 0] oSeg,
	output [7 : 0] oSel
    );

    reg [14 : 0] cnt;
	always @ (posedge clk, posedge reset)
      if (reset)
        cnt <= 0;
      else
        cnt <= cnt + 1'b1;
    wire seg7Clk = cnt[14]; 
	
	reg [2 : 0] seg7Addr; 
	always @ (posedge seg7Clk, posedge reset)
	    if (reset)
		  seg7Addr <= 0;
		else
		  seg7Addr <= seg7Addr + 1'b1;
	reg [7 : 0] oSelR;
	always @ (*)
	case(seg7Addr)
		7 : oSelR = 8'b01111111;
		6 : oSelR = 8'b10111111;
		5 : oSelR = 8'b11011111;
		4 : oSelR = 8'b11101111;
		3 : oSelR = 8'b11110111;
		2 : oSelR = 8'b11111011;
		1 : oSelR = 8'b11111101;
		0 : oSelR = 8'b11111110;
	endcase

	reg [31 : 0] iDataStore;
	always @ (posedge clk, posedge reset)
	if (reset)
		iDataStore <= 0;
	else if(cs)
		iDataStore <= iData;
	  
	reg [7 : 0] segDataR;
	always @ (*)
	case(seg7Addr)
		0 : segDataR = iDataStore[3 : 0];
		1 : segDataR = iDataStore[7 : 4];
		2 : segDataR = iDataStore[11 : 8];
		3 : segDataR = iDataStore[15 : 12];
		4 : segDataR = iDataStore[19 : 16];
		5 : segDataR = iDataStore[23 : 20];
		6 : segDataR = iDataStore[27 : 24];
		7 : segDataR = iDataStore[31 : 28];
	endcase

	reg [7 : 0] oSegR;
	always @ (posedge clk, posedge reset)
	if (reset)
		oSegR <= 8'hff;
	else
		case(segDataR)
			4'h0 : oSegR <= 8'hC0;
			4'h1 : oSegR <= 8'hF9;
			4'h2 : oSegR <= 8'hA4;
			4'h3 : oSegR <= 8'hB0;
			4'h4 : oSegR <= 8'h99;
			4'h5 : oSegR <= 8'h92;
			4'h6 : oSegR <= 8'h82;
			4'h7 : oSegR <= 8'hF8;
			4'h8 : oSegR <= 8'h80;
			4'h9 : oSegR <= 8'h90;
			4'hA : oSegR <= 8'h88;
			4'hB : oSegR <= 8'h83;
			4'hC : oSegR <= 8'hC6;
			4'hD : oSegR <= 8'hA1;
			4'hE : oSegR <= 8'h86;
			4'hF : oSegR <= 8'h8E;
	  endcase
	  
	assign oSel = oSelR;
	assign oSeg = oSegR;
	 
endmodule