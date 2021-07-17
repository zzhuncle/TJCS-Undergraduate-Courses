
`timescale 1ns / 1ps

//数据存储器
module Dmem (
    input clk,
    input ena,
    input wena,
    input [1 : 0] w_cs,
    input [1 : 0] r_cs, 
    input [31 : 0] data_in,
    input [31 : 0] addr,
    output reg [31 : 0] data_out
    );

	reg [31 : 0] Buffer[2047 : 0];
	
	wire [1 : 0] low_addr;
    wire [9 : 0] high_addr;
	
	assign low_addr = (addr - 32'h10010000) % 4;
    assign high_addr = (addr - 32'h10010000) / 4;

    //写操作
    always @ (posedge clk) begin
        if (ena == `ENABLED) begin
            if (wena == `WRITE_ENABLED) begin
			case (w_cs)
                2'b01 : Buffer[high_addr] <= data_in; 
                2'b10 : 
					begin
						if (low_addr == 2'b00)
							Buffer[high_addr][15 : 0] <= data_in[15 : 0];
						else if (low_addr == 2'b10)
							Buffer[high_addr][31 : 16] <= data_in[15 : 0];
					end
                2'b11 : 
					begin
						if (low_addr == 2'b00)
							Buffer[high_addr][7 : 0] <= data_in[7 : 0];
						else if (low_addr == 2'b01)
							Buffer[high_addr][15 : 8] <= data_in[7 : 0];
						else if (low_addr == 2'b10)
							Buffer[high_addr][23 : 16] <= data_in[7 : 0];
						else if (low_addr == 2'b11)
							Buffer[high_addr][31 : 24] <= data_in[7 : 0];
					end
                default : ; 
            endcase
            end
        end
    end
    
    //读操作
    always @ (*) begin
        if (ena == `ENABLED && wena == 1'b0) begin
		case (r_cs)
			2'b01 : data_out <= Buffer[high_addr];
			2'b10 : 
				begin
					case(low_addr)
						2'b00 : data_out <= Buffer[high_addr][15 : 0];
						2'b10 : data_out <= Buffer[high_addr][31 : 16];
						default : data_out <= 32'bx;
					endcase
				end
			2'b11 : 
				begin
					case(low_addr)
						2'b00 : data_out <= Buffer[high_addr][7 : 0];
						2'b01 : data_out <= Buffer[high_addr][15 : 8];
						2'b10 : data_out <= Buffer[high_addr][23 : 16];
						2'b11 : data_out <= Buffer[high_addr][31 : 24];
						default : data_out <= 32'bx;
					endcase
				end
		endcase
        end
    end
endmodule

//改变前缀
module Cut (
    input [31 : 0] in,
    input [2 : 0] sel,
    input sign,
    output reg [31 : 0] out
    );
	
    always @ (*) begin
        case (sel)
            3'b010: out <= {(sign && in[7]) ? 24'hffffff : 24'h000000, in[7 : 0]};
            3'b011: out <= {24'h000000, in[7 : 0]};
			3'b001: out <= {(sign && in[15]) ? 16'hffff : 16'h0000, in[15 : 0]};
            3'b100: out <= {16'h0000, in[15 : 0]};
            default: out <= in;
        endcase
    end

endmodule
