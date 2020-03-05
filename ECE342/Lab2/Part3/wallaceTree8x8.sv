// 8x8 Wallace tree multiplier
// Writing only an 8 bit one because no idea how to generalize
module wallaceTree8x8
(
	input[7:0] a,b,
	output[15:0] product
);
	// Generate Wallace tree multiplier through:
	// 1. Creating partial products
	// 2. 4 levels of 3/2 reduction
	// 3. CLA to add final 2 rows
	logic[7:0] partial[7:0];
	
	genvar i,j;
	generate
		// Generate partial products
		for(i=0; i<8; i++) begin : argumentA
			for(j=0; j<8; j++) begin : argumentB
				assign partial[i][j] = a[i]&b[j];
			end
		end
		
		// -------------------------------------Level 1-------------------------------------
		// Grouping inputs into FAs for first level (and pad with zeros where necessary)
		// separate into groups 1 and 2 since there's 2 sets of 3/2 reductions
		logic[7:0] L1_input_1[2:0];
		logic[7:0] L1_input_2[2:0];
		assign L1_input_1[0] = {1'b0,partial[0][7:1]};
		assign L1_input_1[1] = partial[1][7:0];
		assign L1_input_1[2] = {partial[2][6:0],1'b0};
		assign L1_input_2[0] = {1'b0,partial[3][7:1]};
		assign L1_input_2[1] = partial[4][7:0];
		assign L1_input_2[2] = {partial[5][6:0],1'b0};
		
		// Creating full adders for L1 and setting outputs
		logic[7:0] L1_s_1,L1_s_2,L1_cout_1,L1_cout_2;
		for(i=0; i<8; i++) begin : level1FAs
			fa L1_fa_1_inst
			(
				.a(L1_input_1[0][i]),
				.b(L1_input_1[1][i]),
				.cin(L1_input_1[2][i]),
				.s(L1_s_1[i]),
				.cout(L1_cout_1[i])
			);
			fa L1_fa_2_inst
			(
				.a(L1_input_2[0][i]),
				.b(L1_input_2[1][i]),
				.cin(L1_input_2[2][i]),
				.s(L1_s_2[i]),
				.cout(L1_cout_2[i])
			);
		end
		
		// Assign bits that will not change anymore into product
		assign product[0] = partial[0][0];
		assign product[1] = L1_s_1[0];
		
		// -------------------------------------Level 2-------------------------------------
		// Grouping inputs into FAs for second level (and pad with zeros where necessary)
		// separate into groups 1 and 2 since there's 2 sets of 3/2 reductions
		logic[7:0] L2_input_1[2:0];
		logic[7:0] L2_input_2[2:0];
		assign L2_input_1[0] = L1_cout_1[7:0];
		assign L2_input_1[1] = {partial[2][7],L1_s_1[7:1]};
		assign L2_input_1[2] = {L1_cout_2[4:0],L1_s_2[0],partial[3][0],1'b0};
		assign L2_input_2[0] = {1'b0,partial[5][7],L1_s_2[7:2]};
		assign L2_input_2[1] = partial[6][7:0];
		assign L2_input_2[2] = {partial[7][6:0],1'b0};
		
		// Creating full adders for L2 and setting outputs
		logic[7:0] L2_s_1,L2_s_2,L2_cout_1,L2_cout_2;
		for(i=0; i<8; i++) begin : level2FAs
			fa L2_fa_1_inst
			(
				.a(L2_input_1[0][i]),
				.b(L2_input_1[1][i]),
				.cin(L2_input_1[2][i]),
				.s(L2_s_1[i]),
				.cout(L2_cout_1[i])
			);
			fa L2_fa_2_inst
			(
				.a(L2_input_2[0][i]),
				.b(L2_input_2[1][i]),
				.cin(L2_input_2[2][i]),
				.s(L2_s_2[i]),
				.cout(L2_cout_2[i])
			);
		end
		
		// Assign bits that will not change anymore into product
		assign product[2] = L2_s_1[0];
		
		// -------------------------------------Level 3-------------------------------------
		// Grouping inputs into FAs for third level (and pad with zeros where necessary)
		logic[9:0] L3_input[2:0];
		assign L3_input[0] = {2'b00,L2_cout_1[7:0]};
		assign L3_input[1] = {L1_cout_2[7:5],L2_s_1[7:1]};
		assign L3_input[2] = {L2_cout_2[5:0],L2_s_2[0],L1_s_2[1],2'b00};
		
		// Creating full adders for L3 and setting outputs
		logic[9:0] L3_s,L3_cout;
		for(i=0; i<10; i++) begin : level3FAs
			fa L3_fa_inst
			(
				.a(L3_input[0][i]),
				.b(L3_input[1][i]),
				.cin(L3_input[2][i]),
				.s(L3_s[i]),
				.cout(L3_cout[i])
			);
		end
		
		// Assign bits that will not change anymore into product
		assign product[3] = L3_s[0];
		
		// -------------------------------------Level 4-------------------------------------
		// Grouping inputs into FAs for third level (and pad with zeros where necessary)
		logic[10:0] L4_input[2:0];
		assign L4_input[0] = {1'b0,L3_cout[9:0]};
		assign L4_input[1] = {L2_cout_2[7:6],L3_s[9:1]};
		assign L4_input[2] = {partial[7][7],L2_s_2[7:1],3'b000};
		
		// Creating full adders for L4 and setting outputs
		logic[10:0] L4_s,L4_cout;
		for(i=0; i<11; i++) begin : level4FAs
			fa L4_fa_inst
			(
				.a(L4_input[0][i]),
				.b(L4_input[1][i]),
				.cin(L4_input[2][i]),
				.s(L4_s[i]),
				.cout(L4_cout[i])
			);
		end
		
		// Assign bits that will not change anymore into product
		assign product[4] = L4_s[0];
		
		// CLA - input results from level 4 and will get the 11 MSBs of product
		// the other 5 LSBs of product were already assigned from previous levels
		logic[10:0] CLA_input_a,CLA_input_b;
		logic[11:0] CLA_s;
		
		assign CLA_input_a = L4_cout[10:0];
		assign CLA_input_b = {1'b0,L4_s[10:1]};
		
		CLA #(.n(11)) CLA_inst
		(
			.a(CLA_input_a),
			.b(CLA_input_b),
			.s(CLA_s)
		);
		
		// Assign final bits into product
		assign product[15:5] = CLA_s[10:0]; // The cout from CLA is discarded as it will always be 0
	endgenerate
	
endmodule
