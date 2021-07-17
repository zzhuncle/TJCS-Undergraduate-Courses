`timescale 1ns / 1ps
module divu_tb;
	reg [31 : 0] a;
    reg [31 : 0] b;
    reg start;
    reg clk;
    reg rst;
    wire [31 : 0] q;
    wire [31 : 0] r;
    wire busy;
	
	wire [31 : 0] rc1;
	wire [31 : 0] rc2;
	
    integer intevel = 70;
	integer short = 1;
	
	DIVU uut(a, b, start, clk, rst, q, r, busy);
	assign rc1 = a / b;
    assign rc2 = a % b;
	
	always  #1 clk = ~clk;
    
    initial begin
        clk = 0;
        rst = 1;
        #5 rst = 0;
        
        a = 32'hfffffff0;
        b = 32'h00000005;
        start = 1'b1; #short start = 1'b0;
        #(intevel - short);
        a = 32'b0;
        b = 32'hffffffff;
        start = 1'b1; #short start = 1'b0;
        #(intevel - short);
        a = 32'd26;
        b = 32'd5;
        start = 1'b1; #short start = 1'b0;
        #(intevel - short);
        a = 32'hffffffff;
        b = 32'hffffffff;
        start = 1'b1; #short start = 1'b0;        
        #(intevel - short);
        a = 32'h0000ffff;
        b = 32'hffff0000;
        start = 1'b1; #short start = 1'b0;        
        #(intevel-short);
        a = 32'hffff0000;
        b = 32'h0000ffff;
        start = 1'b1; #short start = 1'b0;
        #(intevel - short);
        a = 32'hffff0000;
        b = 32'hfffffffe;
        start = 1'b1; #short start = 1'b0;
        #(intevel - short);
        a = 32'h00000001;
        b = 32'h0000000f;
        #50 $stop;
    end  
endmodule