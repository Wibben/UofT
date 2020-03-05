module de1soc_top 
(
	// These are the board inputs/outputs required for all the ECE342 labs.
	// Each lab can use the subset it needs -- unused pins will be ignored.
	
    // Clock pins
    input                     CLOCK_50,

    // Seven Segment Displays
    output      [6:0]         HEX0,
    output      [6:0]         HEX1,
    output      [6:0]         HEX2,
    output      [6:0]         HEX3,
    output      [6:0]         HEX4,
    output      [6:0]         HEX5,

    // Pushbuttons
    input       [3:0]         KEY,

    // LEDs
    output      [9:0]         LEDR,

    // Slider Switches
    input       [9:0]         SW,

    // VGA
    output      [7:0]         VGA_B,
    output                    VGA_BLANK_N,
    output                    VGA_CLK,
    output      [7:0]         VGA_G,
    output                    VGA_HS,
    output      [7:0]         VGA_R,
    output                    VGA_SYNC_N,
    output                    VGA_VS
);
	logic [7:0] x,y,val;
	logic [15:0] out;
	logic enable_x;
	
	// Mapping switches
	assign enable_x = SW[9];
	assign val = SW[7:0];
	
	// Registers for holding x and y
	always_ff @(posedge CLOCK_50) begin
		if(enable_x) x <= val;
		else y <= val;
	end
	
	// Instantiate booth multiplier
	wallaceTree8x8 wallaceTreeMultiplier
	(
		.a(x),.b(y),
		.product(out)
	);
	
	// Hex Decoders
	hex_decoder hexdec_0
	(
		.hex_digit(out[3:0]),
		.segments(HEX0)
	);
	
	hex_decoder hexdec_1
	(
		.hex_digit(out[7:4]),
		.segments(HEX1)
	);
	
	hex_decoder hexdec_2
	(
		.hex_digit(out[11:8]),
		.segments(HEX2)
	);
	
	hex_decoder hexdec_3
	(
		.hex_digit(out[15:12]),
		.segments(HEX3)
	);
	
	// Turn off the other HEXes
	assign HEX4 = '1;
	assign HEX5 = '1;
endmodule
