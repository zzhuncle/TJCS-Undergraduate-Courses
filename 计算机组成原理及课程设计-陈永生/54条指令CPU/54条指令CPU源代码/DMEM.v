`timescale 1ns / 1ps

//数据存储器
module DMEM(
    input clk,
	input wena,
	input [1 : 0] s_mux,//00 sw, 01 sh, 10 sb
    input [2 : 0] l_mux,//000 lw, 001 lh, 010 lhu, 011 lb, 100 lbu
	input [31 : 0] addr,
	input [31 : 0] data_in,
	output [31 : 0] data_out
    );
	reg [31 : 0] Buffer[2047 : 0];
	reg [31 : 0] temp;
	
	wire [1 : 0] low_addr;
    wire [9 : 0] high_addr;
	
	assign low_addr = addr[1 : 0];
    assign high_addr = addr[11 : 2];
	
	//写操作
	always @(negedge clk) begin
        if (wena) begin
            case(s_mux)
                2'b00 : Buffer[high_addr] <= data_in; 
                2'b01 : 
					begin
						if (low_addr == 2'b00)
							Buffer[high_addr][15 : 0] <= data_in[15 : 0];
						else if (low_addr == 2'b10)
							Buffer[high_addr][31 : 16] <= data_in[15 : 0];
					end
                2'b10 : 
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
	
	//读操作
	always @ (*) begin
		case(l_mux)
			3'b000 : temp <= Buffer[high_addr];
			3'b001 : 
				begin
					case(low_addr)
						2'b00 : temp <= {{16{Buffer[high_addr][15]}}, Buffer[high_addr][15 : 0]};
						2'b10 : temp <= {{16{Buffer[high_addr][31]}}, Buffer[high_addr][31 : 16]};
						default : temp <= 32'bx;
					endcase
				end
			3'b010 : 
				begin
					case(low_addr)
						2'b00 : temp <= {16'b0, Buffer[high_addr][15 : 0]};
						2'b10 : temp <= {16'b0, Buffer[high_addr][31 : 16]};
						default : temp <= 32'bx;
					endcase
				end
			3'b011 : 
				begin
					case(low_addr)
						2'b00 : temp <= {{24{Buffer[high_addr][7]}}, Buffer[high_addr][7 : 0]};
						2'b01 : temp <= {{24{Buffer[high_addr][15]}}, Buffer[high_addr][15 : 8]};
						2'b10 : temp <= {{24{Buffer[high_addr][23]}}, Buffer[high_addr][23 : 16]};
						2'b11 : temp <= {{24{Buffer[high_addr][31]}}, Buffer[high_addr][31 : 24]};
						default : temp <= 32'bx;
					endcase
				end
			3'b100 : 
				begin
					case(low_addr)
						2'b00 : temp <= {24'b0, Buffer[high_addr][7 : 0]};
						2'b01 : temp <= {24'b0, Buffer[high_addr][15 : 8]};
						2'b10 : temp <= {24'b0, Buffer[high_addr][23 : 16]};
						2'b11 : temp <= {24'b0, Buffer[high_addr][31 : 24]};
						default : temp <= 32'bx;
					endcase
				end
		endcase
	end
    assign data_out = temp;
endmodule
