// LDA Peripheral, drives ASC, LDA, and VGA
module LDA_peripheral
(
	input clk,reset,
	
	// AVS
	input[2:0] avs_s1_address,
	input avs_s1_read,
	output[31:0] avs_s1_readdata,
	output avs_s1_waitrequest,
	input avs_s1_write,
	input[31:0] avs_s1_writedata,
	
	// VGA
   output[7:0] coe_VGA_B_export,
   output coe_VGA_BLANK_N_export,
   output coe_VGA_CLK_export,
   output[7:0] coe_VGA_G_export,
   output coe_VGA_HS_export,
   output[7:0] coe_VGA_R_export,
   output coe_VGA_SYNC_N_export,
   output coe_VGA_VS_export
);

	// VGA adapter and signals
	logic [8:0] vga_x;
	logic [7:0] vga_y;
	logic [2:0] vga_color;
	logic vga_plot;

	// LDA signals
	logic lda_done,lda_start;
	logic[8:0] lda_x0,lda_x1;
	logic[7:0] lda_y0,lda_y1;
	logic[2:0] lda_colour;
	
	// VSC instance
	ASC asc
	(
		.clk(clk),.reset(reset),
		.address(avs_s1_address),
		.read(avs_s1_read),
		.read_data(avs_s1_readdata),
		.wait_request(avs_s1_waitrequest),
		.write(avs_s1_write),
		.write_data(avs_s1_writedata),
		.start(lda_start), .done(lda_done),
		.x0(lda_x0), .x1(lda_x1),
		.y0(lda_y0), .y1(lda_y1),
		.colour(lda_colour)
	);
	
	// LDA instance
	LDA lda
	(
		.clk(clk), .reset(reset),
		.start(lda_start),
		.x0(lda_x0), .x1(lda_x1),
		.y0(lda_y0), .y1(lda_y1),
		.i_colour(lda_colour),
		.done(lda_done),
		.x(vga_x), .y(vga_y),
		.plot(vga_plot), .o_colour(vga_color)
	);
	
	// VGA instance
	vga_adapter #
	(
		.BITS_PER_CHANNEL(1)
	)
	vga_inst
	(
		.clk(clk),
		.VGA_R(coe_VGA_R_export),
		.VGA_G(coe_VGA_G_export),
		.VGA_B(coe_VGA_B_export),
		.VGA_HS(coe_VGA_HS_export),
		.VGA_VS(coe_VGA_VS_export),
		.VGA_SYNC_N(coe_VGA_SYNC_N_export),
		.VGA_BLANK_N(coe_VGA_BLANK_N_export),
		.VGA_CLK(coe_VGA_CLK_export),
		.x(vga_x),
		.y(vga_y),
		.color(vga_color),
		.plot(vga_plot)
	);
endmodule
