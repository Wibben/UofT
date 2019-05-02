`timescale 1ns / 1ns // `timescale time_unit/time_precision

// Morse code encoder with LED flash output
module MorseEncoder(input[9:0] SW, input[3:0] KEY, input CLOCK_50, output[9:0] LEDR);
	wire[2:0] letterSelect;
	wire PLoad,reset;
	
	wire[12:0] MorseCode;
	wire enableClock;
	wire[24:0] rDivider;
	wire flash;
	
	// Input assignment
	assign letterSelect = SW[2:0];
	assign PLoad = ~KEY[1];
	assign reset = ~KEY[0];
	
	// Letter to Morse Code LUT
	MorseTable MT0(.letterSelect(letterSelect),.MorseCode(MorseCode));
	
	// Clock ever 0.5 seconds
	rateDivider rD0(.clock(CLOCK_50),.reset(reset),.counter(rDivider));
	assign enableClock = (rDivider==0);
	
	// Shift register to display the morse code
	shiftRegister reg0(.clock(CLOCK_50),.reset(reset),.PL(PLoad),
							 .enable(enableClock),.DATA_IN(MorseCode),.leftBit(flash));
	
	// Output
	assign LEDR[0] = flash;
endmodule

// Left 13-bit shift register, inserts a 0 onto the right
// Active high async reset, active high async parallel load
module shiftRegister(input [12:0] DATA_IN, input reset,clock,PL,enable, output reg leftBit);
	reg[12:0] Q;
	
	always @(posedge clock)
	begin
		if(reset) // Async reset
		begin
			Q <= 0;
			leftBit <= 0;
		end
		else if(PL) // Async parallel load
			Q <= DATA_IN;
		else if(enable) // 0.5s clock pulse
		begin // Do the shifting
			leftBit <= Q[12];
			Q[12] <= Q[11];
			Q[11] <= Q[10];
			Q[10] <= Q[9];
			Q[9] <= Q[8];
			Q[8] <= Q[7];
			Q[7] <= Q[6];
			Q[6] <= Q[5];
			Q[5] <= Q[4];
			Q[4] <= Q[3];
			Q[3] <= Q[2];
			Q[2] <= Q[1];
			Q[1] <= Q[0];
			Q[0] <= 0;
		end
	end
endmodule

// LUT for the Morse code of the selected letter (S-Z)
module MorseTable(input[2:0] letterSelect, output reg[12:0] MorseCode);
	always @(*)
	begin
		case(letterSelect)
			0: // S
				MorseCode = 13'b1010100000000;
			1: // T
				MorseCode = 13'b1110000000000;
			2: // U
				MorseCode = 13'b1010111000000;
			3: // V
				MorseCode = 13'b1010101110000;
			4: // W
				MorseCode = 13'b1011101110000;
			5: // X
				MorseCode = 13'b1110101011100;
			6: // Y
				MorseCode = 13'b1110101110111;
			7: // Z
				MorseCode = 13'b1110111010100;
			default: // Latch prevention
				MorseCode = 0;
		endcase
	end
endmodule

// Active high, synchronous reset positive edge triggered counter with parallel load
// Counts down from 25M-1 to 0, 25M is 25 bits
module rateDivider(input clock,reset, output reg[24:0] counter);
	// When simulating use divisor = 1000000
	// Uploading to FPGA use divisor = 1
	// Used because simulating a 50 Mhz clock means a very long run time
	parameter divisor = 1; 

	always @(posedge clock)
	begin
		if(reset) // Active high reset to loadIn
			counter <= 25000000/divisor-1;
		else if(counter==0) // Reset to initial value (1 cycle)
			counter <= 25000000/divisor-1;
		else // Count down
			counter <= counter-1;
	end
endmodule
