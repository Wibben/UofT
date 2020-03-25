// CPU control module
module cpu_control
(
	input clk,reset,
	
	output logic o_pc_rd,
	
	output logic mem_rd,mem_wr,ld_mem_rd,
	
	input[4:0] read_opcode,execute_opcode,
	input n,z, // Negative and zero flags
	
	output logic[2:0] sel_a,sel_b,
	output logic addsub,ldnz,
	output logic ldpc,ldpc_7,ldr
);
	// Instruction set
	localparam
		mv 		= 5'b00000,
		add		= 5'b00001,
		sub		= 5'b00010,
		cmp		= 5'b00011,
		ld		= 5'b00100,
		st		= 5'b00101,
		mvi		= 5'b10000,
		addi	= 5'b10001,
		subi	= 5'b10010,
		cmpi	= 5'b10011,
		mvhi	= 5'b10110,
		jr		= 5'b01000,
		jzr		= 5'b01001,
		jnr		= 5'b01010,
		callr	= 5'b01100,
		j		= 5'b11000,
		jz		= 5'b11001,
		jn		= 5'b11010,
		call	= 5'b11100;
	
	// Control signals
	always_comb begin
		sel_a = 3'd0; sel_b = 3'd0; addsub = 1'b0; ldnz = 1'b0;
		ldpc = 1'b0; ldpc_7 = 1'b0; ldr = 1'b0; 
		mem_rd = 1'b0; mem_wr = 1'b0; ld_mem_rd = 1'b0;
		
		// Fetch Stage
		o_pc_rd = 1'b1;
		
		// Read Stage
		case(read_opcode)
			mv: begin
				sel_a = 3'd1; sel_b = 3'd3; // [Ry],0
				addsub = 1'b0; // +
			end
			add: begin
				sel_a = 3'd0; sel_b = 3'd0; // [Rx],[Ry]
				addsub = 1'b0; // +
				ldnz = 1'b1; 
			end
			sub: begin
				sel_a = 3'd0; sel_b = 3'd0; // [Rx],[Ry]
				addsub = 1'b1; // -
				ldnz = 1'b1;
			end
			cmp: begin
				sel_a = 3'd0; sel_b = 3'd0; // [Rx],[Ry]
				addsub = 1'b1; // -
				ldnz = 1'b1;
			end
			ld: begin
				mem_rd = 1'b1; // Read from mem[Ry_data]
			end
			st: begin
				mem_wr = 1'b1; // Write to mem[Ry_data]
			end
			mvi: begin
				sel_a = 3'd3; sel_b = 3'd3; // s_ext(imm8),0
				addsub = 1'b0; // +
			end
			addi: begin
				sel_a = 3'd0; sel_b = 3'd1; // [Rx],s_ext(imm8)
				addsub = 1'b0; // +
				ldnz = 1'b1;
			end
			subi: begin
				sel_a = 3'd0; sel_b = 3'd1; // [Rx],s_ext(imm8)
				addsub = 1'b1; // -
				ldnz = 1'b1;
			end
			cmpi: begin
				sel_a = 3'd0; sel_b = 3'd1; // [Rx],s_ext(imm8)
				addsub = 1'b1; // -
				ldnz = 1'b1;
			end
			mvhi: begin
				sel_a = 3'd4; sel_b = 3'd3; // imm8,0
				addsub = 1'b0; // +
			end
			jr: begin
				sel_a = 3'd0; sel_b = 3'd3; // [Rx],0
				addsub = 1'b0; // +
				ldpc = 1'b1;
			end
			jzr: begin
				sel_a = 3'd0; sel_b = 3'd3; // [Rx],0
				addsub = 1'b0; // +
				if(z) ldpc = 1'b1;
			end
			jnr: begin
				sel_a = 3'd0; sel_b = 3'd3; // [Rx],0
				addsub = 1'b0; // +
				if(n) ldpc = 1'b1;
			end
			callr: begin
				sel_a = 3'd0; sel_b = 3'd3; // [Rx],0
				addsub = 1'b0; // +
				ldpc = 1'b1;
				ldpc_7 = 1'b1;
			end
			j: begin
				sel_a = 3'd2; sel_b = 3'd2; // PC,2*s_ext(imm11)
				addsub = 1'b0; // +
				ldpc = 1'b1;
			end
			jz: begin
				sel_a = 3'd2; sel_b = 3'd2; // PC,2*s_ext(imm11)
				addsub = 1'b0; // +
				if(z) ldpc = 1'b1;
			end
			jn: begin
				sel_a = 3'd2; sel_b = 3'd2; // PC,2*s_ext(imm11)
				addsub = 1'b0; // +
				if(n) ldpc = 1'b1;
			end
			call: begin
				sel_a = 3'd2; sel_b = 3'd2; // PC,2*s_ext(imm11)
				addsub = 1'b0; // +
				ldpc = 1'b1;
				ldpc_7 = 1'b1;
			end
		endcase
		
		// Write Stage
		case(execute_opcode)
			mv: begin
				ldr = 1'b1; 
			end
			add: begin
				ldr = 1'b1; 
			end
			sub: begin
				ldr = 1'b1; 
			end
			ld: begin
				ldr = 1'b1;
				ld_mem_rd = 1'b1;
			end
			mvi: begin
				ldr = 1'b1;
			end
			addi: begin
				ldr = 1'b1;
			end
			subi: begin
				ldr = 1'b1;
			end
			mvhi: begin
				ldr = 1'b1;
			end
		endcase
	end
endmodule
