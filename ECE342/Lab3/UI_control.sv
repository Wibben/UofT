// Control for UI module
module UI_control
(
	input clk,reset,
	input go,done,
	output start,update
);
	enum int unsigned
	{
		S_INPUT,
		S_GO_WAIT,
		S_DRAW,
		S_DRAW_WAIT,
		S_UPDATE
	} current_state,next_state;
	
	// State table
	always_comb begin
		case(current_state)
			S_INPUT: 
				next_state = go ? S_GO_WAIT:S_INPUT;
			S_GO_WAIT:
				next_state = go ? S_GO_WAIT:S_DRAW;
			// Drawing states
			S_DRAW: // Start drawing(LDA FSM)
				next_state = S_DRAW_WAIT;
			S_DRAW_WAIT: // Wait for drwaing to complete
				next_state = done ? S_UPDATE:S_DRAW_WAIT;
			S_UPDATE: // Update x0,y0 with x1,y1
				next_state = S_INPUT;
			default: next_state = S_INPUT; // On error go to input
		endcase
	end
	
	// Control signals
	always_comb begin
		start = 1'b0; update = 1'b0;
		
		case(current_state)
			S_DRAW:
				start = 1'b1;
			S_UPDATE:
				update = 1'b1;
		endcase
	end
	
	// State FFs
	always_ff@(posedge clk) begin
		if(reset) current_state <= S_INPUT;
		else current_state <= next_state;
	end
endmodule
