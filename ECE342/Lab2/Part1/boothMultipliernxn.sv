// Computes a x b
module boothMultipliernxn #
(
	parameter n
)
(
	input [n-1:0] a,b,
	output [2*n-1:0] product
);
	logic [n:0]q;
	logic [n:0]s [n:0]; 	// Output from each full adder, is n+1xn+1 since
								// there are n+1 full adders in each row
	
	assign q = {a,1'b0}; // q[0] is 0 for input into first booth encoder
	assign s[0][n:0] = 0;
	
	genvar i,j;
	generate
		// Generate each row
		for(i=0; i<n; i++) begin : multiplierRows
			logic [n:0]cin;
			logic [n-1:0]sign_ext;
			logic plus,minus;
			assign cin[0] = minus;
			
			boothEncoder booth_inst	// Here Qi is q[i+1] since Q(a) was left
			(								// shifted to insert 0 into the LSB
				.Qi(q[i+1]),
				.Qi_min_1(q[i]),
				.plus(plus),
				.minus(minus)
			);
			
			// Generate multiplier cells
			for(j=0; j<n; j++) begin : multiplierCells
				multiCell multi_inst
				(
					.plus(plus),.minus(minus),
					.a(s[i][j+1]),
					.b(b[j]),
					.s(s[i+1][j]),
					.cin(cin[j]),
					.cout(cin[j+1]),
					.sign_ext(sign_ext[j]) // Only the last sign_ext is used
				);
			end
			
			// Add an extra full adder onto the end for sign stuff
			fa adder_inst
			(
				.a(s[i][n]),
				.b(sign_ext[n-1]),
				.cin(cin[n]), // cout from last multiplier cell
				.s(s[i+1][n]),
				.cout() // Can discard
			);
			
			// Last bit from each row will end up in the product
			assign product[i] = s[i+1][0]; 
		end
	endgenerate
	
	assign product[2*n-1:n] = s[n][n:1];
endmodule
