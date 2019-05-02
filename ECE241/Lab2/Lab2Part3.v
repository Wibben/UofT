`timescale 1ns / 1ns // `timescale time_unit/time_precision

// 7 segment HEX decoder
// SW[3:0] data inputs
// HEX0[6:0] 7-segment output
// All equations derived in lab notebook
module displayHEX(input [9:0] SW,
				  output [6:0] HEX0);
	assign HEX0[0] = (SW[3]|SW[2]|SW[1]|~SW[0]) & (SW[3]|~SW[2]|SW[1]|SW[0]) & (~SW[3]|SW[2]|~SW[1]|~SW[0]) &
					 (~SW[3]|~SW[2]|SW[1]|~SW[0]);

	assign HEX0[1] = (SW[3]|~SW[2]|SW[1]|~SW[0]) & (SW[3]|~SW[2]|~SW[1]|SW[0]) & (~SW[3]|SW[2]|~SW[1]|~SW[0]) & 
					 (~SW[3]|~SW[2]|SW[1]|SW[0]) & (~SW[3]|~SW[2]|~SW[1]|SW[0]) & (~SW[3]|~SW[2]|~SW[1]|~SW[0]);
	
	assign HEX0[2] = (SW[3]|SW[2]|~SW[1]|SW[0]) & (~SW[3]|~SW[2]|SW[1]|SW[0]) & (~SW[3]|~SW[2]|~SW[1]|SW[0]) & 
					 (~SW[3]|~SW[2]|~SW[1]|~SW[0]);
	
	assign HEX0[3] = (SW[3]|SW[2]|SW[1]|~SW[0]) & (SW[3]|~SW[2]|SW[1]|SW[0]) & (SW[3]|~SW[2]|~SW[1]|~SW[0]) & 
					 (~SW[3]|SW[2]|~SW[1]|SW[0]) & (~SW[3]|~SW[2]|~SW[1]|~SW[0]);
	
	assign HEX0[4] = (SW[3]|SW[2]|SW[1]|~SW[0]) & (SW[3]|SW[2]|~SW[1]|~SW[0]) & (SW[3]|~SW[2]|SW[1]|SW[0]) & 
					 (SW[3]|~SW[2]|SW[1]|~SW[0]) & (SW[3]|~SW[2]|~SW[1]|~SW[0]) & (~SW[3]|SW[2]|SW[1]|~SW[0]);
	
	assign HEX0[5] = (SW[3]|SW[2]|SW[1]|~SW[0]) & (SW[3]|SW[2]|~SW[1]|SW[0]) & (SW[3]|SW[2]|~SW[1]|~SW[0]) & 
					 (SW[3]|~SW[2]|~SW[1]|~SW[0]) & (~SW[3]|~SW[2]|SW[1]|~SW[0]);
	
	assign HEX0[6] = (SW[3]|SW[2]|SW[1]|SW[0]) & (SW[3]|SW[2]|SW[1]|~SW[0]) & (SW[3]|~SW[2]|~SW[1]|~SW[0]) & 
					 (~SW[3]|~SW[2]|SW[1]|SW[0]);
endmodule;
