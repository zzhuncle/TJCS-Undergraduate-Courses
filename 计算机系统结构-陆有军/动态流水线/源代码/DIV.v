
`timescale 1ns / 1ps

module Div(
    input reset,
    input ena,
    input sign,
    input [31 : 0] dividend,
    input [31 : 0] divisor,
    output [31 : 0] q,
    output [31 : 0] r
    );

    reg isMinus;
    reg isDivMinus;
    reg [63 : 0] tempDividend;
    reg [63 : 0] tempDivisor;
    integer counter;
    
	assign q = (ena == `CHIP_ENABLED) ? tempDividend[31 : 0] : 32'b0;
    assign r = (ena == `CHIP_ENABLED) ? tempDividend[63 : 32]: 32'b0;
	
    always @ (*) 
    begin
        if (reset == `RST_ENABLED) begin
            tempDividend <= 0;
            tempDivisor <= 0;
            isMinus <= 0;
            isDivMinus <= 0;
        end else if(ena == `ENABLED) begin
            if (sign == `UNSIGNED) begin
                tempDividend = dividend;
                tempDivisor = {divisor, 32'b0}; 
                for (counter = 0; counter < 32; counter = counter + 1) begin
                    tempDividend = tempDividend << 1;
                    if (tempDividend >= tempDivisor) begin
                        tempDividend = tempDividend - tempDivisor;
                        tempDividend = tempDividend + 1;
                    end
                end
                counter = 0;
            end else begin
                tempDividend = dividend;
                tempDivisor = {divisor, 32'b0};
                isMinus = dividend[31] ^ divisor[31];
                isDivMinus = dividend[31];
                if (dividend[31] == 1) begin
					tempDividend = dividend ^ 32'hffffffff;
					tempDividend = tempDividend + 1;
                end
                if (divisor[31] == 1) begin
                    tempDivisor = {divisor ^ 32'hffffffff, 32'b0};
                    tempDivisor = tempDivisor + 64'h0000000100000000;
                end 
                for (counter = 0; counter < 32; counter = counter + 1) begin
                    tempDividend = tempDividend << 1;
                    if (tempDividend >= tempDivisor) begin
                        tempDividend = tempDividend - tempDivisor;
                        tempDividend = tempDividend + 1;
                    end
                end
                if (isDivMinus == 1) begin
                    tempDividend = tempDividend ^ 64'hffffffff00000000;
                    tempDividend = tempDividend + 64'h0000000100000000;
                end          
                if (isMinus == 1) begin
                    tempDividend = tempDividend ^ 64'h00000000ffffffff;
                    tempDividend = tempDividend + 64'h0000000000000001;
                    if (tempDividend[31:0] == 32'b0) begin
                        tempDividend = tempDividend - 64'h0000000100000000;
                    end
                end
            end
        end
    end
    
endmodule
