// Top level CPU module
module cpu
(
	input clk,reset,
	output[15:0] o_mem_addr,
	output o_mem_rd,
	input[15:0] i_mem_rddata,
	output o_mem_wr,
	output[15:0] o_mem_wrdata
);
	// CPU datapath signals
	logic[4:0] opcode;
	logic[2:0] bus_select;
	logic ldir,inc_pc;
	logic ldpc,ldpc_7,ldr,ldnz;
	logic addsub,ld_aluA,ld_ALU_out;
	logic inst_rd,mem_rd,mem_wr;
	logic n,z;
	
	// CPU datapath module
	cpu_datapath cpu_dp
	(
		.clk(clk),.reset(reset),
		.i_mem_rddata(i_mem_rddata),
		.bus_select(bus_select),
		.ldir(ldir), .inc_pc(inc_pc),
		.ldpc(ldpc), .ldpc_7(ldpc_7), .ldr(ldr), .ldnz(ldnz),
		.addsub(addsub), .ld_aluA(ld_aluA), .ld_ALU_out(ld_ALU_out),
		.mem_wr(mem_wr), .mem_rd(mem_rd),
		.o_mem_addr(o_mem_addr), .o_mem_wrdata(o_mem_wrdata),
		.opcode(opcode), .n(n), .z(z)
	);
	
	// CPU control module
	cpu_control cpu_ctrl
	(
		.clk(clk),.reset(reset),
		.opcode(opcode), .n(n), .z(z),
		.bus_select(bus_select),
		.ldir(ldir), .inc_pc(inc_pc),
		.ldpc(ldpc), .ldpc_7(ldpc_7), .ldr(ldr), .ldnz(ldnz),
		.addsub(addsub), .ld_aluA(ld_aluA), .ld_ALU_out(ld_ALU_out),
		.inst_rd(inst_rd), .mem_wr(mem_wr), .mem_rd(mem_rd)
	);
	
	assign o_mem_rd = inst_rd | mem_rd;
	assign o_mem_wr = mem_wr;
endmodule
