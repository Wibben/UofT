`timescale 1ns / 1ns // `timescale time_unit/time_precision

// 7 to 1 multiplexer
module mux7to1(input [9:0] SW, output [9:0] LEDR);
	wire [6:0] data;
	wire [2:0] sel;
	
	// Input assignment
	assign data = SW[6:0];
	assign sel = SW[9:7];
	
	// Multiplexer code to select data
	reg out;
	always @(*)
	begin
		case(sel[2:0])
			3'b000: out = data[0];
			3'b001: out = data[1];
			3'b010: out = data[2];
			3'b011: out = data[3];
			3'b100: out = data[4];
			3'b101: out = data[5];
			3'b110: out = data[6];
			default:;
		endcase
	end
	
	// Output
	assign LEDR[0] = out;
endmodule