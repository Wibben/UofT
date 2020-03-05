// Datapath for LDA module
module LDA_datapath
(
	input clk,reset,
	input[8:0] i_x0,i_x1,
	input[8:0] i_y0,i_y1,
	input[2:0] i_colour,
	input setup,
	input inc_x,inc_y,
	input inc_error,dec_error,
	output logic[8:0] o_x,
	output logic[7:0] o_y,
	output logic[2:0] o_colour,
	output logic signed[9:0] error,
	output logic end_of_loop
);
	logic steep;
	logic[8:0] x0,x1,x,x0_steep,x1_steep;
	logic[8:0] y0,y1,y,y0_steep,y1_steep;
	logic[8:0] deltax,deltay;
	
	// Compute steepness and preprocessing inputs (swaps)
	always_comb begin
		deltax = (i_x1>i_x0) ? i_x1-i_x0:i_x0-i_x1;
		deltay = (i_y1>i_y0) ? i_y1-i_y0:i_y0-i_y1;
		if(deltay > deltax) steep = 1'b1;
		else steep = 1'b0;
		
		// if(steep)
		//		swap(x0,y0)
		//		swap(x1,y1)
		if(steep) begin
			x0_steep = i_y0;
			y0_steep = i_x0;
			x1_steep = i_y1;
			y1_steep = i_x1;
		end else begin
			x0_steep = i_x0;
			y0_steep = i_y0;
			x1_steep = i_x1;
			y1_steep = i_y1;
		end
		
		// if(x0>x1)
		//		swap(x0,x1)
		//		swap(y0,y1)
		if(x0_steep > x1_steep) begin
			x0 = x1_steep;
			x1 = x0_steep;
			y0 = y1_steep;
			y1 = y0_steep;
		end else begin
			x0 = x0_steep;
			x1 = x1_steep;
			y0 = y0_steep;
			y1 = y1_steep;
		end
	end
	
	// "local variables" that can be derived from comb circuits
	logic signed[9:0] dx,dy;
	logic signed[8:0] y_step;
	assign dx = x1-x0;
	assign dy = (y1>y0) ? (y1-y0):(y0-y1);
	assign y_step = (y1>y0) ? 9'd1:-9'd1;
	
	// FFs
	always_ff@(posedge clk) begin
		if(reset) begin // Reset on startup
			x <= 9'd0;
			y <= 9'd0;
			error <= 10'd0;
		end else if(setup) begin // Initialize
			x <= x0;
			y <= y0;
			error <= -(dx/10'd2);
		end else begin
			if(inc_y) y <= y+y_step;
			//else if(dec_y) y <= y-9'd1;
			
			if(inc_x) x <= x+9'd1;
			
			if(inc_error) error <= error+dy;
			else if(dec_error) error <= error-dx;
		end
	end
	
	// Tell control whether it is the end of the loop
	assign end_of_loop = (x==x1);
	// Coordinates is based on steepness of line
	assign o_x = steep ? y:x;
	assign o_y = steep ? x[7:0]:y[7:0];
	// Colour is passed through
	assign o_colour = i_colour;
endmodule
