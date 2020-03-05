// n bit Carry Lookahead Adder
module CLA #
(
	parameter n
)
(
	input[n-1:0] a,b,
	output logic[n:0] s // cout is included in the 2*n-th bit
);
	genvar i;
	generate
		logic[n:0] cin;
		assign cin[0] = 0;
		
		for(i=0; i<n; i++) begin : carryLookAhead
			logic g,p;
			assign g = a[i] & b[i]; // Generate
			assign p = a[i] | b[i]; // Propogate
			
			assign cin[i+1] = g | p&cin[i]; // Carry
			assign s[i] = a[i] ^ b[i] ^ cin[i]; // Sum
		end
		
		assign s[n] = cin[n]; // Include last carry
	endgenerate
endmodule
