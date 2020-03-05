module tb_LDA_peripheral();
	logic clock,reset;

	logic[2:0] address;
	logic read;
	logic[31:0] readdata;
	logic waitrequest;
	logic write;
	logic[31:0] writedata;
	
	LDA_peripheral DUT
	(
		.clk(clock), .reset(reset),
	
		.avs_s1_address(address),
		.avs_s1_read(read),
		.avs_s1_readdate(readdata),
		.avs_s1_waitrequest(waitrequest),
		.avs_s1_write(write),
		.avs_s1_writedata(writedata)
	);
	
	initial clock = 1'b0;
	always #10 clock = ~clock;
	
	initial begin
		reset = 1'b1;
		@(posedge clock);
		reset = 1'b0;
		address = 3'b011;
		read = 1'b1;
		
		@(posedge clock);
		#1 read = 1'b0;
		
		@(posedge clock);
		@(posedge clock);
		address = 3'b100;
		writedata = 32'd30;
		write = 1;
		
		@(posedge clock);
		write = 0;
		
		@(posedge clock);
		address = 3'b010;
		write = 1;
		
		#1000;
		
		$stop();
	end
endmodule
