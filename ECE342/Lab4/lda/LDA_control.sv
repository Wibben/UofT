// Control for LDA module
module LDA_control
(
	input clk,reset,
	input start,end_of_loop,
	input signed[9:0] error,
	output logic setup,plot,done,
	output logic inc_x,inc_y, save_coord,
	output logic inc_error,dec_error
);
	enum int unsigned
	{
		S_IDLE,
		S_SETUP,
		S_PLOT,
		S_UPDATE,
		S_DONE
	} current_state,next_state;
	
	// State table
	always_comb begin
		case(current_state)
			S_IDLE:
				next_state = start ? S_SETUP:S_IDLE;
			S_SETUP:
				next_state = S_PLOT;
			S_PLOT:
				next_state = S_UPDATE;
			S_UPDATE:
				next_state = (end_of_loop) ? S_DONE:S_PLOT;
			S_DONE:
				next_state = S_IDLE;
		endcase
	end
	
	// Control signals
	always_comb begin
		setup = 1'b0; plot = 1'b0; done = 1'b0;
		inc_x = 1'b0; inc_y = 1'b0; save_coord = 1'b0;
		inc_error = 1'b0; dec_error = 1'b0;
		
		case(current_state)
			S_IDLE:
				if(start) save_coord = 1'b1;
			S_SETUP:
				setup = 1'b1;
			S_PLOT: begin
				plot = 1'b1;
				inc_error = 1'b1;
			end
			S_UPDATE: begin
				inc_x = 1'b1; // Change x
				if(error > 0) begin
					inc_y = 1'b1; // Change y
					// Reset error
					dec_error = 1'b1;
				end
			end
			S_DONE:
				done = 1'b1;
		endcase
	end
	
	// State FFs
	always_ff@(posedge clk) begin
		if(reset) current_state <= S_IDLE;
		else current_state <= next_state;
	end
endmodule
