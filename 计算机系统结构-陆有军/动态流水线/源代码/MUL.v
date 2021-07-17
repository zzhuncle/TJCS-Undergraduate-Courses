
`timescale 1ns / 1ps

module Mult (  
    input reset,
    input ena,     
    input sign, 
    input [31 : 0] a,
    input [31 : 0] b,
    output [31 : 0] Hi,
    output [31 : 0] Lo
    );

    reg [63 : 0] stored;
    reg [63 : 0] temp;
	reg [31 : 0] tempB;
	reg [31 : 0] tempA;
    reg isMinus;
    integer i;

	assign Lo = (ena == `ENABLED) ? stored[31 : 0] : 32'b0;
    assign Hi = (ena == `ENABLED) ? stored[63 : 32] : 32'b0;
	
    always @ (*) 
    begin
        if (reset == `RST_ENABLED) begin
		    tempA <= 0;
            tempB <= 0;
            stored <= 0;
            isMinus <= 0;
        end else if(ena == `ENABLED) begin
            if (a == 0 || b == 0) begin
                stored <= 0;
            end else if(sign == `UNSIGNED) begin
                stored = 0;
                for (i = 0; i < 32; i = i + 1) begin
                    temp = b[i] ? ({32'b0, a} << i) : 64'b0;
                    stored = stored + temp;       
                end
            end else begin
                stored = 0;
                isMinus = a[31] ^ b[31];
                tempA = a;
                tempB = b;
                if (a[31] == 1) begin
                    tempA = a ^ 32'hffffffff;
                    tempA = tempA + 1;
                end
                if (b[31] == 1) begin
                    tempB = b ^ 32'hffffffff;
                    tempB = tempB + 1;
                end
                for (i = 0; i < 32; i = i + 1) begin
                    temp = tempB[i] ? ({32'b0, tempA} << i) : 64'b0;
                    stored = stored + temp;       
                end
                if (isMinus == 1) begin
                    stored = stored ^ 64'hffffffffffffffff;
                    stored = stored + 1;
                end
            end
        end
    end
    
endmodule
