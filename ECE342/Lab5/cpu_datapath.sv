// CPU datapath module
module cpu_datapath
(
	input clk,reset,
	input[15:0] i_mem_rddata,
	input[2:0] bus_select,
	input ldir,inc_pc,
	input ldpc,ldpc_7,ldr,ldnz,
	input addsub,ld_aluA,ld_ALU_out,
	input mem_wr,mem_rd,
	output logic[15:0] o_mem_addr,
	output logic[15:0] o_mem_wrdata,
	output logic n,z, // Negative and zero flags
	output logic[4:0] opcode
);
	// Data registers
	logic[15:0] register[7:0]; // 8 16-bit general purpose registers
	logic[15:0] pc; // Program counter
	logic[15:0] ir; // Instruction register
	
	// Internal signals
	logic[2:0] Rx,Ry;
	logic[7:0] imm8;
	logic[10:0] imm11;
	logic[15:0] Rx_data,Ry_data;
	logic[15:0] aluA,ALU_out;
	logic[15:0] bus;
	
	// Assigning basic internal signals
	assign opcode = ir[4:0];
	assign Rx = ir[7:5];
	assign Ry = ir[10:8];
	assign imm8 = ir[15:8];
	assign imm11 = ir[15:5];
	
	always_comb begin
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
	
	// Multiplexer select for bus
	always_comb begin
		case(bus_select)
			3'd0: bus = Rx_data;
			3'd1: bus = Ry_data;
			3'd2: bus = pc;
			3'd3: bus = { {8{imm8[7]}},imm8[7:0] }; // s_ext(imm8)
			3'd4: bus = { {4{imm11[10]}},imm11[10:0],1'b0 }; // 2*s_ext(imm11) = left shift s_ext(imm11)
			3'd5: bus = { imm8[7:0],Rx_data[7:0] }; // Specifically for mvhi
			3'd6: bus = ALU_out;
			3'd7: bus = i_mem_rddata;
			default: bus = 16'd0;
		endcase
	end
	
	// Storage FFs
	always_ff@(posedge clk) begin
		if(reset) begin
			n <= 1'd0;
			z <= 1'd0;
			pc <= 16'd0;
			ir <= 16'd0;
			register[0] <= 16'd0;
			register[1] <= 16'd0;
			register[2] <= 16'd0;
			register[3] <= 16'd0;
			register[4] <= 16'd0;
			register[5] <= 16'd0;
			register[6] <= 16'd0;
			register[7] <= 16'd0;
			aluA <= 16'd0;
			ALU_out <= 16'd0;
		end
		
		// Instruction Register
		if(ldir) ir <= bus;
		
		// N and Z flags
		if(ldnz) begin
			n <= bus[15]; // Check bit for if negative
			z <= (bus==16'd0); // Check bit for if zero
		end
		
		// Program Counter
		if(ldpc) pc <= bus;
		else if(inc_pc) pc <= pc + 16'd2;
		
		// Available registers
		if(ldr & Rx==3'd0) register[0] <= bus;
		if(ldr & Rx==3'd1) register[1] <= bus;
		if(ldr & Rx==3'd2) register[2] <= bus;
		if(ldr & Rx==3'd3) register[3] <= bus;
		if(ldr & Rx==3'd4) register[4] <= bus;
		if(ldr & Rx==3'd5) register[5] <= bus;
		if(ldr & Rx==3'd6) register[6] <= bus;
		if(ldr & Rx==3'd7) register[7] <= bus;
		else if(ldpc_7) register[7] <= pc; // PC has a direct line to R7 for callr and call
		
		// ALU: addsub = 0 -> add, = 1 -> subtract
		// XOR bus with sign extended addsub and add addsub 
		// to get 2's complement if subtract, XOR does nothing if add
		if(ld_aluA) aluA <= bus;
		if(ld_ALU_out) ALU_out <= aluA + (bus^{16{addsub}}) + addsub;
	end
	
	// Interface FFs
	always_comb begin
		if(mem_rd) begin // Load instruction
			o_mem_addr = bus;
			o_mem_wrdata = 16'd0;
		end
		else if(mem_wr) begin // Store instruction
			o_mem_addr = Ry_data; // Address always comes from Ry
			o_mem_wrdata = bus; // Data always comes from Rx
		end
		else begin // Normally address is PC
			o_mem_addr = pc;
			o_mem_wrdata = 16'd0;
		end
	end
endmodule
