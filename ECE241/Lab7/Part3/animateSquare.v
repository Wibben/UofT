`timescale 1ns / 1ns

module animateSquare
	(
		CLOCK_50,						//	On Board 50 MHz
		// Your inputs and outputs here
		KEY,							// On Board Keys
		SW,
		// The ports below are for the VGA output.  Do not change.
		VGA_CLK,   						//	VGA Clock
		VGA_HS,							//	VGA H_SYNC
		VGA_VS,							//	VGA V_SYNC
		VGA_BLANK_N,						//	VGA BLANK
		VGA_SYNC_N,						//	VGA SYNC
		VGA_R,   						//	VGA Red[9:0]
		VGA_G,	 						//	VGA Green[9:0]
		VGA_B   						//	VGA Blue[9:0]
	);

	input			CLOCK_50;				//	50 MHz
	input	[3:0]	KEY;
	// Declare your inputs and outputs here
	input [9:0] SW;
	// Do not change the following outputs
	output			VGA_CLK;   				//	VGA Clock
	output			VGA_HS;					//	VGA H_SYNC
	output			VGA_VS;					//	VGA V_SYNC
	output			VGA_BLANK_N;				//	VGA BLANK
	output			VGA_SYNC_N;				//	VGA SYNC
	output	[7:0]	VGA_R;   				//	VGA Red[7:0] Changed from 10 to 8-bit DAC
	output	[7:0]	VGA_G;	 				//	VGA Green[7:0]
	output	[7:0]	VGA_B;   				//	VGA Blue[7:0]
	
	wire resetn;
	assign resetn = KEY[0]; // Active low, so don't invert
	
	// Create the colour, x, y and writeEn wires that are inputs to the controller.

	wire [2:0] colour;
	wire [7:0] x,y;
	wire writeEn;

	// Create an Instance of a VGA controller - there can be only one!
	// Define the number of colours as well as the initial background
	// image file (.MIF) for the controller.
	vga_adapter VGA(
			.resetn(resetn),
			.clock(CLOCK_50),
			.colour(colour),
			.x(x),
			.y(y),
			.plot(writeEn),
			/* Signals for the DAC to drive the monitor. */
			.VGA_R(VGA_R),
			.VGA_G(VGA_G),
			.VGA_B(VGA_B),
			.VGA_HS(VGA_HS),
			.VGA_VS(VGA_VS),
			.VGA_BLANK(VGA_BLANK_N),
			.VGA_SYNC(VGA_SYNC_N),
			.VGA_CLK(VGA_CLK));
		defparam VGA.RESOLUTION = "160x120";
		defparam VGA.MONOCHROME = "FALSE";
		defparam VGA.BITS_PER_COLOUR_CHANNEL = 1;
		defparam VGA.BACKGROUND_IMAGE = "black.mif";
			
	// Put your code here. Your code should produce signals x,y,colour and writeEn
	// for the VGA controller, in addition to any other functionality your design may require.
	
	// Control wires
	wire ld_xy,ld_colour,ld_pos;
	wire set_black,draw_pixel;
	wire[7:0] dx,dy;
	wire[7:0] X_IN,Y_IN;
	wire[2:0] C_DATA;
	
	// Assign inputs
	assign C_DATA 	= SW[2:0];
	
	// Control module
	control c0(
		.clock(CLOCK_50),.resetn(resetn),
		.ld_xy(ld_xy),.ld_colour(ld_colour),.ld_pos(ld_pos),
		.set_black(set_black),.draw_pixel(draw_pixel),
		.dx(dx),.dy(dy),.x(X_IN),.y(Y_IN)
	);
	
	// Controls plotting on VGA
	assign writeEn = draw_pixel;
	
	// Datapath module
	datapath d0(
		.clock(CLOCK_50),.resetn(resetn),
		.X_IN(X_IN),.Y_IN(Y_IN),.COLOUR_DATA(C_DATA),
		.ld_xy(ld_xy),.ld_colour(ld_colour),
		.ld_pos(ld_pos),.set_black(set_black),
		.dx(dx),.dy(dy),
		.xpos(x),.ypos(y),.colour(colour)
	);
endmodule

// Tracks state and datapath control signals depending on state
module control(
	input clock,resetn,
	output reg ld_xy,ld_colour,ld_pos,
	output reg set_black,draw_pixel,
	output reg[7:0] dx,dy,x,y // Counts up to X and Y shape size
	); // x,y is top left coord of shape
	
	// Parameters for counters
	localparam 	X_SHAPE 	= 8'd4,
					Y_SHAPE	= 8'd4,
					X_SCREEN = 8'd160,
					Y_SCREEN = 8'd120;
	
	// Keeps track of state
	reg[4:0] current_state,next_state;
	// Used to draw shapes and clear screen
	reg reset_dx,reset_dy,inc_dx,inc_dy;
	reg inc_xy,update_dir;
	reg full_reset;
	
	reg[1:0] direction; // direction[x,y]: 1 -> right/up, 0 -> left/down
	
	// Frame counter and control
	wire[3:0] frame;
	reg reset_frame,clear_shape,draw,erase;
	
	// Instantiates frame counter
	frameCounter fC0(
		.clock(clock),.resetn(resetn),
		.resetFrame(reset_frame),.frame(frame)
	);
	
	// Assigning state variables
	localparam	S_FULL_RESET 	= 5'd0,
					S_DRAW_SHAPE	= 5'd1,
					S_DRAW_0 		= 5'd2,
					S_DRAW_1 		= 5'd3,
					S_DRAW_2			= 5'd4,
					S_DRAW_3			= 5'd5,
					S_DRAW_4 		= 5'd6,
					S_RESET_FRAME 	= 5'd7,
					S_FRAME_COUNT	= 5'd8,
					S_CLEAR_SHAPE 	= 5'd9,
					S_MOVE 			= 5'd10,
					S_UPDATE_DIR	= 5'd11,
					S_RESET_0 		= 5'd12,
					S_RESET_1 		= 5'd13,
					S_RESET_2		= 5'd14,
					S_RESET_3		= 5'd15,
					S_RESET_4 		= 5'd16,
					S_RESET_DONE	= 5'd17;

	// Register for whether it is drawing or erasing
	always @(posedge clock)
	begin
		if(!resetn) // Active low reset
			clear_shape <= 0;
		else if(draw) // Draw cycle
			clear_shape <= 0;
		else if(erase) // Erase cycle
			clear_shape <= 1;
	end
	
	// Register for direction
	always @(posedge clock)
	begin
		if(!resetn) // Active low reset
			direction <= 2'b00;
		else if(update_dir)
			begin // Check for bounds
				if((x==X_SCREEN-4 & direction[1]) | (x==0 & !direction[1]))
					direction[1] <= !direction[1];
				if((y==Y_SCREEN-4 & !direction[0]) | (y==0 & direction[0]))
					direction[0] <= !direction[0];
			end
	end
	
	// Counter registers for dx,dy and x,y
	always @(posedge clock)
	begin
		if(!resetn) // Active low reset
		begin
			dx <= 8'b0;
			dy <= 8'b0;
			x <= 8'b0;
			y <= 8'b0;
		end
		else if(full_reset)
		begin
			dx <= 8'b0;
			dy <= 8'b0;
			x <= X_SCREEN-4;
			y <= 8'b0;
		end
		else
		begin
			// dx counter
			if(reset_dx)
				dx <= 8'b0;
			else if(inc_dx)
				dx <= dx+1;
			
			// dy counter
			if(reset_dy)
				dy <= 8'b0;
			else if(inc_dy)
				dy <= dy+1;
				
			// x and y counter
			// direction[x,y]: 1 -> right/up, 0 -> left/down
			if(inc_xy)
			begin
				x <= direction[1] ? (x+1):(x-1);
				y <= direction[0] ? (y-1):(y+1);
			end
		end
	end
	
	// State table
	always @(*)
	begin
		case(current_state)
			S_FULL_RESET:
				next_state = S_RESET_0;
			S_DRAW_SHAPE: // Prepares to draw shape (colour<-C, reset dy)
				next_state = S_DRAW_0;
			S_DRAW_0: // Reset dx
				next_state = S_DRAW_1;
			S_DRAW_1: // Set pixel fill position
				next_state = S_DRAW_2;
			S_DRAW_2: // Fill pixel
				next_state = S_DRAW_3;
			S_DRAW_3: // Increment dx
				next_state = (dx==X_SHAPE-1) ? S_DRAW_4:S_DRAW_1;
			S_DRAW_4: // Increment dy
			begin
				if(dy==Y_SHAPE-1) // See if this round drew or erased
					next_state = (clear_shape) ? S_MOVE:S_RESET_FRAME;
				else next_state = S_DRAW_0;
			end
			S_RESET_FRAME: // Resets frame count
				next_state = S_FRAME_COUNT;
			S_FRAME_COUNT: // Counts frame to 0
				next_state = (frame==0) ? S_CLEAR_SHAPE:S_FRAME_COUNT;
			S_CLEAR_SHAPE: // Prepares to erase shape (colour<-0, reset dy)
				next_state = S_DRAW_0;
			S_MOVE: // Move top-left corner of shape
				next_state = S_UPDATE_DIR;
			S_UPDATE_DIR: // Updates direction
				next_state = S_DRAW_SHAPE;
			S_RESET_0: // Reset dx
				next_state = S_RESET_1;
			S_RESET_1: // Set pixel fill position
				next_state = S_RESET_2;
			S_RESET_2: // Fill pixel
				next_state = S_RESET_3;
			S_RESET_3: // Increment dx
				next_state = (dx==X_SCREEN-1) ? S_RESET_4:S_RESET_1;
			S_RESET_4: // Increment dy
				next_state = (dy==Y_SCREEN-1) ? S_RESET_DONE:S_RESET_0;
			S_RESET_DONE:
				next_state = S_DRAW_SHAPE;
			default: next_state = S_FULL_RESET;
		endcase
	end
	
	// Changing data control signals
	always @(*)
	begin
		// Initializing signals to 0 to avoid latches
		// Internal controls
		reset_dx = 0; reset_dy = 0; inc_dx = 0; inc_dy = 0;
		reset_frame = 0; draw = 0; erase = 0; 
		inc_xy = 0; update_dir = 0; full_reset = 0;
		// External controls
		ld_xy = 0; ld_colour = 0; ld_pos = 0; 
		set_black = 0; draw_pixel = 0;
		
		case(current_state)
			S_FULL_RESET: // Clears the screen
			begin
				ld_xy = 1;
				set_black = 1;
				reset_dy = 1;
			end
			S_DRAW_SHAPE: // Prepares to draw shape (colour<-C, reset dy)
			begin
				ld_xy = 1;
				ld_colour = 1;
				draw = 1;
				reset_dy = 1;
			end
			S_DRAW_0: // Reset dx
			begin
				reset_dx = 1;
			end
			S_DRAW_1: // Set pixel fill position
			begin
				ld_pos = 1;
			end
			S_DRAW_2: // Fill pixel
			begin
				draw_pixel = 1;
			end
			S_DRAW_3: // Increment dx
			begin
				inc_dx = 1;
			end
			S_DRAW_4: // Increment dy
			begin
				inc_dy = 1;
			end
			S_RESET_FRAME: // Resets frame count
			begin
				reset_frame = 1;
			end
			S_CLEAR_SHAPE: // Prepares to erase shape (colour<-0, reset dy)
			begin
				set_black = 1;
				erase = 1;
				reset_dy = 1;
			end
			S_MOVE: // Move top-left corner of shape
			begin
				inc_xy = 1;
			end
			S_UPDATE_DIR: // Updates direction
			begin
				update_dir = 1;
			end
			S_RESET_0: // Reset dx
				reset_dx = 1;
			S_RESET_1: // Set pixel fill position
				ld_pos = 1;
			S_RESET_2: // Fill pixel
				draw_pixel = 1;
			S_RESET_3: // Increment dx
				inc_dx = 1;
			S_RESET_4: // Increment dy
				inc_dy = 1;
			S_RESET_DONE:
				full_reset = 1;
		endcase
	end
	
	// Register for current state
	always @(posedge clock)
	begin
		if(!resetn) // Reset to value input, active low
			current_state <= S_FULL_RESET;
		else // Load next state
			current_state <= next_state;
	end
endmodule

// Modifies data and outputs depending on control signals
module datapath(
	input clock,resetn,
	input[7:0] X_IN,Y_IN,
	input[2:0] COLOUR_DATA,
	input ld_xy,ld_colour,ld_pos,
	input set_black,
	input[7:0] dx,dy,
	output reg[7:0] xpos,
	output reg[6:0] ypos,
	output reg[2:0] colour
	);
	
	// Internal registers
	reg[7:0] x,y;
	
	// Registers x,y,xpos,ypos and colour with input logic
	always @(posedge clock)
	begin
		if(!resetn) // Active low reset
		begin
			x <= 8'b0;
			y <= 8'b0;
			xpos <= 8'b0;
			ypos <= 8'b0;
			colour <= 3'b0;
		end
		else
		begin
			// x and register
			if(ld_xy)
			begin
				x <= X_IN;
				y <= Y_IN;
			end
			// xpos and ypos registers (for drawing shape)
			if(ld_pos)
			begin
				xpos <= x+dx;
				ypos <= y+dy;
			end
			// colour register
			if(ld_colour)
				colour <= COLOUR_DATA;
			else if(set_black)
				colour <= 3'b000;
		end
	end
endmodule

module frameCounter(input clock,resetn,resetFrame, output reg[3:0] frame);
	wire enable;
	
	reg[19:0] rateDivider;
	// Change to 100000 for simulation
	localparam divisor = 1;
	
	// Rate divider
	always @(posedge clock)
	begin
		if(!resetn) // Active low reset
			rateDivider <= 833333/divisor-1;
		else if(resetFrame | rateDivider==0) // Reset at command and 1 cycle
			rateDivider <= 833333/divisor-1;
		else // Count down
			rateDivider <= rateDivider-1;
	end
	
	assign enable = (rateDivider==0);
	
	// Frame counter
	always @(posedge clock)
	begin
		if(!resetn) // Active low reset
			frame <= 15-1;
		else if(resetFrame | frame==0) // Reset at command and 1 cycle
			frame <= 15-1;
		else if(enable) // Count down
			frame <= frame-1;
	end
endmodule
