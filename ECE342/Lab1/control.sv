module control
(
	input clk,
	input reset,
	
	// Button input
	input i_enter,
	
	// Datapath
	output logic o_inc_actual,
	input i_over,
	input i_under,
	input i_equal,
	input i_out_of_guesses,
	
	// LED Control: Setting this to 1 will copy the current
	// values of over/under/equal to the 3 LEDs. Setting this to 0
	// will cause the LEDs to hold their current values.
	output logic o_update_leds,
	output logic o_dec_guesses
);

// Declare two objects, 'state' and 'nextstate'
// that are of enum type.
enum int unsigned
{
	S_STARTUP,
	S_STARTUP_WAIT,
	S_GUESS,
	S_GUESS_WAIT,
	S_CHECK,
	S_GAMEOVER
} state, nextstate;

// State table
always_comb begin
	case(state)
		S_STARTUP: 
			nextstate = i_enter ? S_STARTUP_WAIT:S_STARTUP;
		S_STARTUP_WAIT: 
			nextstate = i_enter ? S_STARTUP_WAIT:S_CHECK;
		S_GUESS: 
			if(i_out_of_guesses) nextstate = S_GAMEOVER;
			else nextstate = i_enter ? S_GUESS_WAIT:S_GUESS;
		S_GUESS_WAIT: 
			nextstate = i_enter ? S_GUESS_WAIT:S_CHECK;
		S_CHECK:
			nextstate = i_equal ? S_GAMEOVER:S_GUESS;
		S_GAMEOVER:
			nextstate = S_GAMEOVER; // Must reset to restart game
		default: nextstate = S_STARTUP; // If it get to here something went wrong, best to reset
	endcase
end

// Clocked always block for making state registers
always_ff @ (posedge clk or posedge reset) begin
	if (reset) state <= S_STARTUP; // Initial reset state is startup
	else state <= nextstate;
end

// Control variables
// always_comb replaces always @* and gives compile-time errors instead of warnings
// if you accidentally infer a latch
always_comb begin
	// Set default values for signals here, to avoid having to explicitly
	// set a value for every possible control path through this always block
	o_inc_actual = 1'b0;
	o_update_leds = 1'b0;
	o_dec_guesses = 1'b0;
	
	case (state)
		S_STARTUP: begin
			o_inc_actual = 1'b1;
		end
		S_CHECK: begin
			o_update_leds = 1'b1;
			o_dec_guesses = 1'b1;
		end
	endcase
end

endmodule
