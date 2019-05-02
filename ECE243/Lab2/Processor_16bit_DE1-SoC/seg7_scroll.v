// Data written to registers R0 to R5 are sent to the H digits
module seg7_scroll (Data, Addr, Sel, Resetn, Clock, H5, H4, H3, H2, H1, H0);
	input [6:0] Data;
	input [2:0] Addr;
	input Sel, Resetn, Clock;
	output [6:0] H5, H4, H3, H2, H1, H0;
	
	reg [5:0] enable; // Enable signal to decide which 7 segment to write to
	
	// One hot encode the 7 segment to be written to
	always @(*)
	begin
		case(Addr)
			0: enable = 6'b000001;
			1: enable = 6'b000010;
			2: enable = 6'b000100;
			3: enable = 6'b001000;
			4: enable = 6'b010000;
			5: enable = 6'b100000;
			default: enable = 6'b000000;
		endcase
	end
	
	// Registers for the data written to each 7 segment display
	regne HEX0(.R(~Data),.Clock(Clock),.Resetn(Resetn),.E(enable[0] & Sel),.Q(H0));
	regne HEX1(.R(~Data),.Clock(Clock),.Resetn(Resetn),.E(enable[1] & Sel),.Q(H1));
	regne HEX2(.R(~Data),.Clock(Clock),.Resetn(Resetn),.E(enable[2] & Sel),.Q(H2));
	regne HEX3(.R(~Data),.Clock(Clock),.Resetn(Resetn),.E(enable[3] & Sel),.Q(H3));
	regne HEX4(.R(~Data),.Clock(Clock),.Resetn(Resetn),.E(enable[4] & Sel),.Q(H4));
	regne HEX5(.R(~Data),.Clock(Clock),.Resetn(Resetn),.E(enable[5] & Sel),.Q(H5));
	
endmodule

module regne (R, Clock, Resetn, E, Q);
	parameter n = 7;
	input [n-1:0] R;
	input Clock, Resetn, E;
	output [n-1:0] Q;
	reg [n-1:0] Q;	
	
	always @(posedge Clock)
		if (Resetn == 0)
			Q <= {n{1'b1}};
		else if (E)
			Q <= R;
endmodule
