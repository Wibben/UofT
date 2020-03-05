// UI Module
module UI
(
	input clk,reset,
	input[3:0] KEY,
	input[8:0] SW,
	input done,
	output[8:0] x0,x1,
	output[7:0] y0,y1,
	output[2:0] colour,
	output start
);
	// Naming signals
	logic setX,setY,setCol,go;
	logic[8:0] val;
	
	assign setX = ~KEY[0];
	assign setY = ~KEY[1];
	assign setCol = ~KEY[2];
	assign go = ~KEY[3];
	assign val = SW;
	
	// Datapath signals
	logic dp_update;
	
	// Datapath module
	UI_datapath UI_dp
	(
		.clk(clk), .reset(reset),
		.setX(setX),
		.setY(setY),
		.setCol(setCol),
		.update(dp_update),
		.val(val),
		.x0(x0), .x1(x1),
		.y0(y0), .y1(y1),
		.colour(colour)
	);
	
	// Control module
	UI_control UI_ctrl
	(
		.clk(clk), .reset(reset),
		.go(go),
		.done(done),
		.update(dp_update),
		.start(start)
	);
endmodule

