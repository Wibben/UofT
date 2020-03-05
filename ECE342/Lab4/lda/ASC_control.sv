// Control module for ASC
module ASC_control
(
	input clk,reset,
	input write,
	output logic wait_request,
	input[2:0] address,
	
	input mode,status,done,
	output logic start,
	output logic set_mode,
	output logic set_start,set_end,set_colour
);
	enum int unsigned
	{
		S_IDLE,
		S_DRAW,
		S_DRAW_WAIT
	} current_state,next_state;

	// State table
	always_comb begin
		case(current_state)
			S_IDLE: begin
				if(mode == 1'b0) 
					next_state = (write & address==3'b010) ? S_DRAW:S_IDLE;
				else
					next_state = (status & address==3'b010) ? S_IDLE:S_DRAW;
			end
			S_DRAW:
				next_state = S_DRAW_WAIT;
			S_DRAW_WAIT:
				next_state = done ? S_IDLE:S_DRAW_WAIT;
		endcase
	end
	
	// Control signals
	always_comb begin
		start = 1'b0; 
		wait_request = 1'b0;
		set_mode = 1'b0;
		set_start = 1'b0; set_end = 1'b0;
		set_colour = 1'b0;
		
		case(current_state)
			S_IDLE: begin
				if(write & address==3'b000) set_mode = 1'b1;
				else if(write & address==3'b011) set_start = 1'b1;
				else if(write & address==3'b100) set_end = 1'b1;
				else if(write & address==3'b101) set_colour = 1'b1;
			end
			S_DRAW: begin
				start = 1'b1;
			end
			S_DRAW_WAIT: begin
				if(mode==1'b0) wait_request = 1'b1;
				else begin // Poll mode
					//if(write & address==3'b000) set_mode = 1'b1;
					//else 
					if(write & address==3'b011) set_start = 1'b1;
					else if(write & address==3'b100) set_end = 1'b1;
					else if(write & address==3'b101) set_colour = 1'b1;
				end
			end
		endcase
	end
	
	// State FFs
	always_ff@(posedge clk) begin
		if(reset) current_state <= S_IDLE;
		else current_state <= next_state;
	end
endmodule
