// Avalon Slave Controller Module
module ASC
(
	input clk,reset,
	input read,write,
	output wait_request,
	input[2:0] address,
	input[31:0] write_data,
	output[31:0] read_data,
	
	input done,
	output start,
	output[8:0] x0,x1,
	output[7:0] y0,y1,
	output[2:0] colour
);
	// Datapath signals
	logic set_mode;
	logic set_start,set_end,set_colour;
	logic dp_mode,dp_status;
	
	// Datapath module
	ASC_datapath ASC_dp
	(
		.clk(clk), .reset(reset),
		.read(read),
		.address(address),
		.write_data(write_data),
		.read_data(read_data),
		.start(start), .done(done),
		.set_mode(set_mode),
		.set_start(set_start), .set_end(set_end), 
		.set_colour(set_colour),
		.mode(dp_mode), .status(dp_status),
		.x0(x0), .y0(y0),
		.x1(x1), .y1(y1),
		.colour(colour)
	);
	
	// Control module
	ASC_control ASC_ctrl
	(
		.clk(clk), .reset(reset),
		.write(write),
		.wait_request(wait_request),
		.address(address),
		.mode(dp_mode), .status(dp_status),
		.start(start), .done(done),
		.set_mode(set_mode),
		.set_start(set_start), .set_end(set_end), 
		.set_colour(set_colour)
	);
endmodule
