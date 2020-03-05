// Datapath for UI module
module UI_datapath
(
	input clk,reset,
	input setX,setY,setCol,update,
	input[8:0] val,
	output logic[8:0] x0,x1,
	output logic[7:0] y0,y1,
	output logic[2:0] colour
);
	// FFs
	always_ff@(posedge clk) begin
		if(reset) begin // Reset on startup
			x0 <= 8'd0;
			x1 <= 9'd0;
			y0 <= 8'd0;
			y1 <= 8'd0;
		end 
		else begin
			if(setX) x1 <= (val>9'd335) ? 9'd335:val;
			else if(setY) y1 <= (val[7:0]>8'd209) ? 8'd209:val[7:0];
			else if(setCol) colour <= val[2:0];
			else if(update) begin
				x0 <= x1;
				y0 <= y1;
			end
		end
	end
endmodule
