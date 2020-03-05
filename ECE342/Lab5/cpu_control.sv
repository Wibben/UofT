// CPU control module
module cpu_control
(
	input clk,reset,
	input[4:0] opcode,
	input n,z,
	output logic[2:0] bus_select,
	output logic ldir,inc_pc,
	output logic ldpc,ldpc_7,ldr,ldnz,
	output logic addsub,ld_aluA,ld_ALU_out,
	output logic inst_rd,mem_rd,mem_wr
);
	enum int unsigned
	{
		S_STARTUP,
		S_FETCH_INST,
		S_T0,
		S_T1,
		S_T2,
		S_T3
	} current_state,next_state;
	
	// Internal control for when instruction is done
	logic done;
	
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
	
	// State table
	always_comb begin
		case(current_state)
			S_STARTUP:
				next_state = S_FETCH_INST;
			S_FETCH_INST: 
				next_state = S_T0;
			S_T0: 
				next_state = done ? S_FETCH_INST:S_T1;
			S_T1: 
				next_state = done ? S_FETCH_INST:S_T2;
			S_T2: 
				next_state = done ? S_FETCH_INST:S_T3;
			S_T3: 
				next_state = S_FETCH_INST;
			default: next_state = current_state;
		endcase
	end
	
	// Control signals
	always_comb begin
		bus_select = 3'd0;
		ldir = 1'b0; inc_pc = 1'b0;
		ldpc = 1'b0; ldpc_7 = 1'b0; ldr = 1'b0; ldnz = 1'b0;
		addsub = 1'b0; ld_aluA = 1'b0; ld_ALU_out = 1'b0;
		inst_rd = 1'b0; mem_rd = 1'b0; mem_wr = 1'b0;
		done = 1'b0;
		
		case(current_state)
			S_STARTUP: begin
				inst_rd = 1'b1;
			end
			S_FETCH_INST: begin
				bus_select = 3'd7;
				ldir = 1'b1;
				inc_pc = 1'b1;
			end
			S_T0: begin
				case(opcode)
					mv: begin
						bus_select = 3'd1;
						ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					add: begin
						bus_select = 3'd0;
						ld_aluA = 1'b1;
					end
					sub: begin
						bus_select = 3'd0;
						ld_aluA = 1'b1;
					end
					cmp: begin
						bus_select = 3'd0;
						ld_aluA = 1'b1;
					end
					ld: begin
						bus_select = 3'd1;
						mem_rd = 1'b1;
					end
					st: begin
						bus_select = 3'd0;
						mem_wr = 1'b1;
					end
					mvi: begin
						bus_select = 3'd3;
						ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					addi: begin
						bus_select = 3'd0;
						ld_aluA = 1'b1;
					end
					subi: begin
						bus_select = 3'd0;
						ld_aluA = 1'b1;
					end
					cmpi: begin
						bus_select = 3'd0;
						ld_aluA = 1'b1;
					end
					mvhi: begin
						bus_select = 3'd5;
						ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					jr: begin
						bus_select = 3'd0;
						ldpc = 1'b1;
					end
					jzr: begin
						// Check zero flag
						if(z) begin
							bus_select = 3'd0;
							ldpc = 1'b1;
						end
						else begin
							inst_rd = 1'b1; done = 1'b1;
						end
					end
					jnr: begin
						// Check negative flag
						if(n) begin
							bus_select = 3'd0;
							ldpc = 1'b1;
						end
						else begin
							inst_rd = 1'b1; done = 1'b1;
						end
					end
					callr: begin
						bus_select = 3'd0;
						ldpc = 1'b1; ldpc_7 = 1'b1;
					end
					j: begin
						bus_select = 3'd2;
						ld_aluA = 1'b1;
					end
					jz: begin
						// Check zero flag
						if(z) begin
							bus_select = 3'd2;
							ld_aluA = 1'b1;
						end
						else begin
							inst_rd = 1'b1; done = 1'b1;
						end
					end
					jn: begin
						// Check negative flag
						if(n) begin
							bus_select = 3'd2;
							ld_aluA = 1'b1;
						end
						else begin
							inst_rd = 1'b1; done = 1'b1;
						end
					end
					call: begin
						bus_select = 3'd2;
						ld_aluA = 1'b1;
					end
				endcase
			end
			S_T1: begin
				case(opcode)
					add: begin
						bus_select = 3'd1;
						addsub = 1'b0;
						ld_ALU_out = 1'b1;
					end
					sub: begin
						bus_select = 3'd1;
						addsub = 1'b1;
						ld_ALU_out = 1'b1;
					end
					cmp: begin
						bus_select = 3'd1;
						addsub = 1'b1;
						ld_ALU_out = 1'b1;
					end
					ld: begin
						bus_select = 3'd7;
						ldr = 1'b1;
					end
					st: begin
						inst_rd = 1'b1; done = 1'b1;
					end
					addi: begin
						bus_select = 3'd3;
						addsub = 1'b0;
						ld_ALU_out = 1'b1;
					end
					subi: begin
						bus_select = 3'd3;
						addsub = 1'b1;
						ld_ALU_out = 1'b1;
					end
					cmpi: begin
						bus_select = 3'd3;
						addsub = 1'b1;
						ld_ALU_out = 1'b1;
					end
					jr: begin
						inst_rd = 1'b1; done = 1'b1;
					end
					jzr: begin
						inst_rd = 1'b1; done = 1'b1;
					end
					jnr: begin
						inst_rd = 1'b1; done = 1'b1;
					end
					callr: begin
						inst_rd = 1'b1; done = 1'b1;
					end
					j: begin
						bus_select = 3'd4;
						addsub = 1'b0;
						ld_ALU_out = 1'b1;
					end
					jz: begin
						bus_select = 3'd4;
						addsub = 1'b0;
						ld_ALU_out = 1'b1;
					end
					jn: begin
						bus_select = 3'd4;
						addsub = 1'b0;
						ld_ALU_out = 1'b1;
					end
					call: begin
						bus_select = 3'd4;
						addsub = 1'b0;
						ld_ALU_out = 1'b1;
					end
				endcase
			end
			S_T2: begin
				case(opcode)
					add: begin
						bus_select = 3'd6;
						ldnz = 1'b1; ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					sub: begin
						bus_select = 3'd6;
						ldnz = 1'b1; ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					cmp: begin
						bus_select = 3'd6;
						ldnz = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					ld: begin
						inst_rd = 1'b1; done = 1'b1;
					end
					addi: begin
						bus_select = 3'd6;
						ldnz = 1'b1; ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					subi: begin
						bus_select = 3'd6;
						ldnz = 1'b1; ldr = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					cmpi: begin
						bus_select = 3'd6;
						ldnz = 1'b1;
						inst_rd = 1'b1; done = 1'b1;
					end
					j: begin
						bus_select = 3'd6;
						ldpc = 1'b1;
					end
					jz: begin
						bus_select = 3'd6;
						ldpc = 1'b1;
					end
					jn: begin
						bus_select = 3'd6;
						ldpc = 1'b1;
					end
					call: begin
						bus_select = 3'd6;
						ldpc = 1'b1; ldpc_7 = 1'b1;
					end
				endcase
			end
			S_T3: begin 
				// Only j, jz, jn, and call come here
				// and they do the same thing
				inst_rd = 1'b1;
			end
		endcase
	end
	
	// State FFs
	always_ff@(posedge clk) begin
		if(reset) current_state <= S_STARTUP;
		else current_state <= next_state;
	end
endmodule
