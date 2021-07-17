
`timescale 1ns / 1ps

module RegFile(
    input clk, 
    input rst, 
    input wena, 
    input [4 : 0] raddr1, 
    input [4 : 0] raddr2, 
    input rena1,
    input rena2,
    input [4 : 0] waddr, 
    input [31 : 0] wdata, 
    output reg [31 : 0] rdata1, 
    output reg [31 : 0] rdata2,
    output [31 : 0] reg28,
    output [31 : 0] reg29
    );
    
    reg [31 : 0] array_reg[0 : 31];

	always @ (posedge clk or posedge rst) 
    begin
        if(rst == `RST_ENABLED) begin
		    array_reg[0] <= 32'b0;
			array_reg[1] <= 32'b0;
			array_reg[2] <= 32'b0;
			array_reg[3] <= 32'b0;
			array_reg[4] <= 32'b0;
			array_reg[5] <= 32'b0;
			array_reg[6] <= 32'b0;
			array_reg[7] <= 32'b0;
			array_reg[8] <= 32'b0;
			array_reg[9] <= 32'b0;
			array_reg[10] <= 32'b0;
			array_reg[11] <= 32'b0;
			array_reg[12] <= 32'b0;
			array_reg[13] <= 32'b0;
			array_reg[14] <= 32'b0;
			array_reg[15] <= 32'b0;
			array_reg[16] <= 32'b0;
			array_reg[17] <= 32'b0;
			array_reg[18] <= 32'b0;
			array_reg[19] <= 32'b0;
			array_reg[20] <= 32'b0;
			array_reg[21] <= 32'b0;
			array_reg[22] <= 32'b0;
			array_reg[23] <= 32'b0;
			array_reg[24] <= 32'b0;
			array_reg[25] <= 32'b0;
			array_reg[26] <= 32'b0;
			array_reg[27] <= 32'b0;
			array_reg[28] <= 32'b0;
			array_reg[29] <= 32'b0;
			array_reg[30] <= 32'b0;
			array_reg[31] <= 32'b0;
        end else begin
            if((wena == `WRITE_ENABLED) && (waddr != 0))
                array_reg[waddr] = wdata;
        end
	end

	always @ (*) begin
	    if (rst == `RST_ENABLED) 
			  rdata1 <= 32'b0;
	    else if (raddr1 == 5'b0) 
	  		rdata1 <= 32'b0;
	    else if((raddr1 == waddr) && (wena == `WRITE_ENABLED) && (rena1 == `READ_ENABLED)) 
	  	    rdata1 <= wdata;
	    else if(rena1 == `READ_ENABLED) 
	        rdata1 <= array_reg[raddr1];
	    else 
	        rdata1 <= 32'b0;
	end

	always @ (*) 
    begin
	    if(rst == `RST_ENABLED) 
			  rdata2 <= 32'b0;
	    else if(raddr2 == 5'b0) 
	  		rdata2 <= 32'b0;
        else if((raddr2 == waddr) && (wena == `WRITE_ENABLED) && (rena2 == `READ_ENABLED)) 
            rdata2 <= wdata;
	    else if(rena2 == `READ_ENABLED) 
	        rdata2 <= array_reg[raddr2];
	    else 
	        rdata2 <= 32'b0;
	end

    assign reg29 = array_reg[29];
	assign reg28 = array_reg[28];

endmodule

