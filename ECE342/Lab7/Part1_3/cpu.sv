module cpu
(
	input clk,
	input reset,
	
	output [15:0] o_pc_addr,
	output o_pc_rd,
	input [15:0] i_pc_rddata,
	
	output [15:0] o_ldst_addr,
	output o_ldst_rd,
	output o_ldst_wr,
	input [15:0] i_ldst_rddata,
	output [15:0] o_ldst_wrdata,
	
	output [7:0][15:0] o_tb_regs
);
	// CPU datapath signals
	wire ctrl_fetch_v,ctrl_read_v,ctrl_execute_v,ctrl_write_v;
	wire mem_rd,mem_wr,ld_mem_rd;
	wire[2:0] sel_a,sel_b;
	wire addsub,ldnz;
	wire ldpc,ldpc_7,ldr;
	wire n,z;
	wire[4:0] read_opcode,execute_opcode;
	wire fetch_v,read_v,execute_v,write_v;
	
	// CPU datapath module
	cpu_datapath cpu_dp 
	(
		.clk(clk), .reset(reset),
		
		.o_pc_addr(o_pc_addr), .i_pc_rddata(i_pc_rddata),
		.o_ldst_addr(o_ldst_addr), .i_ldst_rddata(i_ldst_rddata), .o_ldst_wrdata(o_ldst_wrdata),
		.o_tb_regs(o_tb_regs),
		
		.ctrl_fetch_v(ctrl_fetch_v), .ctrl_read_v(ctrl_read_v),
		.ctrl_execute_v(ctrl_execute_v), .ctrl_write_v(ctrl_write_v),
		.mem_rd(mem_rd), .mem_wr(mem_wr), .ld_mem_rd(ld_mem_rd),
		
		.sel_a(sel_a), .sel_b(sel_b),
		.addsub(addsub), .ldnz(ldnz),
		.ldpc(ldpc), .ldpc_7(ldpc_7), .ldr(ldr),
		
		.n(n), .z(z),
		.read_opcode(read_opcode), .execute_opcode(execute_opcode),
		.fetch_v(fetch_v), .read_v(read_v),
		.execute_v(execute_v), .write_v(write_v)
	);
	
	// CPU control module
	cpu_control cpu_ctrl
	(
		.clk(clk), .reset(reset),
		.o_pc_rd(o_pc_rd),
		
		.ctrl_fetch_v(ctrl_fetch_v), .ctrl_read_v(ctrl_read_v),
		.ctrl_execute_v(ctrl_execute_v), .ctrl_write_v(ctrl_write_v),
		.mem_rd(mem_rd), .mem_wr(mem_wr), .ld_mem_rd(ld_mem_rd),
		
		.fetch_v(fetch_v), .read_v(read_v),
		.execute_v(execute_v), .write_v(write_v),
		.read_opcode(read_opcode), .execute_opcode(execute_opcode),
		.n(n), .z(z),
		
		.sel_a(sel_a), .sel_b(sel_b),
		.addsub(addsub), .ldnz(ldnz),
		.ldpc(ldpc), .ldpc_7(ldpc_7), .ldr(ldr)
	);
	
	assign o_ldst_rd = mem_rd & ctrl_execute_v & read_v;
	assign o_ldst_wr = mem_wr & ctrl_execute_v & read_v;
endmodule

