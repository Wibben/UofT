// nxn bit carry save adder
module carrySavenxn #
(
	parameter n
)
(
	input [n-1:0] a,b,
	output logic [2*n-1:0] product
);
	logic[n:0] s[n:0];
	logic[n-1:0] cin[n:0];
	logic[n:0] s_CLA;
	
	assign cin[0][n-1:0] = 0;
	assign s[0][n:0] = 0;
	
	// Create adder array
	genvar i,j;
	generate
		// Generate each row
		for(i=0; i<n; i++) begin : multiplierRows
			// Generate each full adder
			for(j=0; j<n; j++) begin : fullAdders
				fa fa_inst
				(
					.a(s[i][j+1]),
					.b(a[i]&b[j]),
					.cin(cin[i][j]),
					.s(s[i+1][j]),
					.cout(cin[i+1][j]) // Cout from each row becomes cin of same index on next row
				);
			end
			assign s[i+1][n] = 0; // Input a into the last FA on the next row is always 0
			assign product[i] = s[i+1][0];
		end
	endgenerate
	
	// Fast adder(CLA) for final part
	CLA #(.n(n)) carryLookAhead
	(
		.a(s[n][n:1]),
		.b(cin[n]),
		.s(s_CLA)
	);
	
	assign product[2*n-1:n] = s_CLA[n-1:0]; // Discard carry out bit from CLA (it should always be 0 anyways)
endmodule
