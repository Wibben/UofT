// CPU datapath module
module cpu_datapath
(
	input clk,reset,
	
	output[15:0] o_pc_addr,
	input[15:0] i_pc_rddata,
	
	output logic[15:0] o_ldst_addr,
	input[15:0] i_ldst_rddata,
	output logic[15:0] o_ldst_wrdata,
	
	output[7:0][15:0] o_tb_regs,
	
	input ld_mem_rd,
	
	input[2:0] sel_a,sel_b,
	input addsub,ldnz,
	input ldpc,ldpc_7,ldr,
	
	output logic n,z, // Negative and zero flags
	output logic[4:0] read_opcode,execute_opcode
);
	// Data registers
	logic[15:0] register[7:0]; // 8 16-bit general purpose registers
	logic[15:0] pc,execute_pc; // Program counter
	logic[15:0] ir; // Instruction register
	
	// Internal signals
	logic[2:0] Rx,execute_Rx;
	logic[2:0] Ry;
	logic[7:0] imm8;
	logic[10:0] imm11;
	logic[15:0] Rx_data;
	logic[15:0] Ry_data;
	logic[15:0] aluA,aluB,ALU_result,ALU_out;
	
	// Signals for the dependency checks
	logic[15:0] data_to_reg;
	logic[15:0] execute_alu_Rx,execute_alu_Ry;
	
	// Fetch Stage - Has PC register
	always_ff@(posedge clk) begin
		if(reset) begin
			pc <= 16'd0;
		end
		else begin
			// Program Counter
			if(ldpc) pc <= ALU_result+16'd2;
			else pc <= pc + 16'd2;
		end
	end
	
	// RF Read Stage - Breaks down IR into individual parts
	assign ir = i_pc_rddata;
	assign Rx = ir[7:5];
	assign Ry = ir[10:8];
	assign read_opcode = ir[4:0];
	assign imm8 = ir[15:8];
	assign imm11 = ir[15:5];
	
	always_comb begin
		// Dependency check for [Rx]
		if(ldr & execute_Rx==Rx) Rx_data = data_to_reg;
		else begin
			case(Rx)
				3'd0: Rx_data = register[0];
				3'd1: Rx_data = register[1];
				3'd2: Rx_data = register[2];
				3'd3: Rx_data = register[3];
				3'd4: Rx_data = register[4];
				3'd5: Rx_data = register[5];
				3'd6: Rx_data = register[6];
				3'd7: Rx_data = register[7];
			endcase
		end
		
		// Dependency check for [Ry]
		if(ldr & execute_Rx==Ry) Ry_data = data_to_reg;
		else begin
			case(Ry)
				3'd0: Ry_data = register[0];
				3'd1: Ry_data = register[1];
				3'd2: Ry_data = register[2];
				3'd3: Ry_data = register[3];
				3'd4: Ry_data = register[4];
				3'd5: Ry_data = register[5];
				3'd6: Ry_data = register[6];
				3'd7: Ry_data = register[7];
			endcase
		end
	end
	
	// Execute Stage - either updates ALU_out or reads/writes to memory
	always_comb begin
		// Multiplexer select for aluA and aluB
		case(sel_a)
			3'd0: aluA = Rx_data;
			3'd1: aluA = Ry_data;
			3'd2: aluA = pc;
			3'd3: aluA = { {8{imm8[7]}},imm8[7:0] }; // s_ext(imm8)
			3'd4: aluA = { imm8[7:0],Rx_data[7:0] }; // imm8, Specifically for mvhi
			default: aluA = 16'd0;
		endcase
		
		case(sel_b)
			3'd0: aluB = Ry_data;
			3'd1: aluB = { {8{imm8[7]}},imm8[7:0] }; // s_ext(imm8)
			3'd2: aluB = { {4{imm11[10]}},imm11[10:0],1'b0 }; // 2*s_ext(imm11) = left shift s_ext(imm11)
			3'd3: aluB = 16'd0;
			default: aluB = 16'd0;
		endcase
	end
	
	// ALU: addsub = 0 -> add, = 1 -> subtract
	// XOR bus with sign extended addsub and add addsub 
	// to get 2's complement if subtract, XOR does nothing if add
	assign ALU_result = aluA + (aluB^{16{addsub}}) + addsub;
	
	always_ff@(posedge clk) begin
		if(reset) begin
			execute_opcode <= 5'd0;
			execute_Rx <= 3'd0;
			execute_pc <= 16'd0;
			n <= 1'd0;
			z <= 1'd0;
			ALU_out <= 16'd0;
		end
		else begin
			execute_opcode <= read_opcode;
			execute_Rx <= Rx;
			execute_pc <= pc;
			
			// N and Z flags
			if(ldnz) begin
				n <= ALU_result[15]; // Check bit for if negative
				z <= (ALU_result==16'd0); // Check bit for if zero
			end
			
			// ALU result register
			ALU_out <= ALU_result;
		end
	end
	
	// Write Stage - Stores to Registers
	assign data_to_reg = ld_mem_rd ? i_ldst_rddata:ALU_out;
	
	always_ff@(posedge clk) begin
		if(reset) begin
			register[0] <= 16'd0;
			register[1] <= 16'd0;
			register[2] <= 16'd0;
			register[3] <= 16'd0;
			register[4] <= 16'd0;
			register[5] <= 16'd0;
			register[6] <= 16'd0;
			register[7] <= 16'd0;
		end
		else begin
			// Available registers
			if(ldr & execute_Rx==3'd0) register[0] <= data_to_reg;
			if(ldr & execute_Rx==3'd1) register[1] <= data_to_reg;
			if(ldr & execute_Rx==3'd2) register[2] <= data_to_reg;
			if(ldr & execute_Rx==3'd3) register[3] <= data_to_reg;
			if(ldr & execute_Rx==3'd4) register[4] <= data_to_reg;
			if(ldr & execute_Rx==3'd5) register[5] <= data_to_reg;
			if(ldr & execute_Rx==3'd6) register[6] <= data_to_reg;
			if(ldpc_7) register[7] <= pc;  // PC has a direct line to R7 for callr and call
			else if(ldr & execute_Rx==3'd7) register[7] <= data_to_reg;
		end
	end
	
	// Interface signals
	assign o_pc_addr = ldpc ? ALU_result:pc;
	
	assign o_ldst_addr = Ry_data;
	always_comb begin
		if(execute_Rx==Rx) o_ldst_wrdata = data_to_reg;
		else o_ldst_wrdata = Rx_data;
	end
	
	assign o_tb_regs[0] = register[0];
	assign o_tb_regs[1] = register[1];
	assign o_tb_regs[2] = register[2];
	assign o_tb_regs[3] = register[3];
	assign o_tb_regs[4] = register[4];
	assign o_tb_regs[5] = register[5];
	assign o_tb_regs[6] = register[6];
	assign o_tb_regs[7] = register[7];
endmodule
