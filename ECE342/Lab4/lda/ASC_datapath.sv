// Datapath module for ASC
module ASC_datapath
(
	input clk, reset,
	input read,
	input[2:0] address,
	input[31:0] write_data,
	output logic[31:0] read_data,
	
	input start,done,
	input set_mode,
	input set_start,set_end,set_colour,
	
	output logic mode,status,
	output logic[8:0] x0,x1,
	output logic[7:0] y0,y1,
	output logic[2:0] colour
);
	// Each clock cycle outputs determined by mode
	// FFs needed to store things like x,y,and colour
	always_ff@(posedge clk) begin
		if(reset) begin
			mode <= 1'b0;
			status <= 1'b0;
			x0 <= 9'd0;
			x1 <= 9'd0;
			y0 <= 8'd0;
			y1 <= 8'd0;
			colour <= 3'd0; // Make it black on default
		end
		else begin
			// Status register
			if(start) // Initiate drawing
				status <= 1'b1;
			else if(done) // Done drawing
				status <= 1'b0;
			
			// Mode register
			if(set_mode)
				mode <= write_data[0];
			
			// Set line starting point
			if(set_start) begin 
				x0 <= (write_data[8:0]>9'd335) ? 9'd335:write_data[8:0];
				y0 <= (write_data[16:9]>8'd209) ? 8'd209:write_data[16:9];
			end 
			
			// Set line ending point
			if(set_end) begin
				x1 <= (write_data[8:0]>9'd335) ? 9'd335:write_data[8:0];
				y1 <= (write_data[16:9]>8'd209) ? 8'd209:write_data[16:9];
			end 
			
			// Set line colour
			if(set_colour) 
				colour <= write_data[2:0];
		end
	end

	// Comb circuit to determine what to sent to read_data
	always_comb begin
		if(read) begin
			if(address == 3'b000) // Read mode
				read_data = {31'd0, mode};
			
			else if(address == 3'b001) // Read status
				read_data = {31'd0, status};
				
			else if(address == 3'b011) // Read line starting point
				read_data = {15'd0, y0[7:0], x0[8:0]}; 
				
			else if(address == 3'b100) // Read line ending point
				read_data = {15'd0, y0[7:0], x0[8:0]};
				
			else if(address == 3'b101) // Read line colour
				read_data = {29'd0, colour[2:0]};
				
			else read_data = 32'd0;
		end else // Deassert read_data
			read_data = 32'd0;
	end
endmodule
