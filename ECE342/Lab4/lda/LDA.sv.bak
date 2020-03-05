// Line Drawing Algorithm Module
module LDA
(
	input clk,reset,
	input start,
	input[8:0] x0,x1,
	input[7:0] y0,y1,
	input[2:0] i_colour,
	output plot,done,
	output[8:0] x,
	output[7:0] y,
	output[2:0] o_colour
);
	// Datapath signals
	logic signed[9:0] dp_error;
	logic setup;
	logic inc_x,inc_y,end_of_loop;
	logic inc_error,dec_error;
	
	// Datapath module
	LDA_datapath LDA_dp
	(
		.clk(clk), .reset(reset),
		.i_x0(x0), .i_x1(x1),
		.i_y0(y0), .i_y1(y1),
		.i_colour(i_colour),
		.setup(setup),
		.inc_x(inc_x), .inc_y(inc_y),
		.inc_error(inc_error), .dec_error(dec_error),
		.error(dp_error),
		.o_x(x), .o_y(y),
		.o_colour(o_colour),
		.end_of_loop(end_of_loop)
	);
	
	// Control module
	LDA_control LDA_ctrl
	(
		.clk(clk), .reset(reset),
		.start(start), .end_of_loop(end_of_loop),
		.error(dp_error),
		.setup(setup), .plot(plot), .done(done),
		.inc_x(inc_x), .inc_y(inc_y), 
		.inc_error(inc_error), .dec_error(dec_error)
	);
endmodule
