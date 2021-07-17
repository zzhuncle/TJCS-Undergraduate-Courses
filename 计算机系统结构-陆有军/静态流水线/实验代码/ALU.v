
`timescale 1ns / 1ps

module Alu (
    input [31:0] a,    
    input [31:0] b, 
    input [3:0] aluc,
    output [31:0] r,
    output zero,
    output carry, 
    output negative, 
    output overflow
    );

    reg [32 : 0] result;
    reg same_signal;
    reg flag;

    always @ (*) begin
        case(aluc)
            4'b0000: result = a + b;
            4'b0010: 
			begin
                result = $signed(a) + $signed(b);
                same_signal = ~(a[31] ^ b[31]);
                flag = (same_signal && result[31] != a[31]) ? 1 : 0;
            end
            4'b0001: result = a - b;
            4'b0011: 
			begin
                result = $signed(a) - $signed(b);
                same_signal = ~(a[31] ^ b[31]);
                flag = (~same_signal && result[31] != a[31]) ? 1 : 0;
            end

            4'b0100: result = a & b;
            4'b0101: result = a | b;
            4'b0110: result = a ^ b;
            4'b0111: result = ~(a | b);

            4'b1000: result = {b[15 : 0], 16'b0};
            4'b1001: result = {b[15 : 0], 16'b0};

            4'b1011: result = ($signed(a) < $signed(b)) ? 1 : 0;
            4'b1010: result =  (a < b) ? 1 : 0;

            4'b1100: result = $signed(b) >>> a;
            4'b1110: result = b << a;
            4'b1111: result = b << a;
            4'b1101: result = b >> a;
        endcase
    end
    
    assign r = result[31 : 0];
    assign carry = (aluc == 4'b0000 | aluc == 4'b0001 | aluc == 4'b1010 | aluc == 4'b1100 | aluc == 4'b1101 | aluc == 4'b1110) ? result[32] : 1'bz; 
    assign zero = (r == 32'b0) ? 1 : 0; 
    assign negative = result[31];
    assign overflow = (aluc == 4'b0010 | aluc == 4'b0011) ? flag : 1'bz; 
    
endmodule