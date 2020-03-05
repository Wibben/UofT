// Booth Encoder
module boothEncoder
(
	input Qi,Qi_min_1,
	output logic plus,minus
);
	assign plus = ~Qi & Qi_min_1;
	assign minus = Qi & ~Qi_min_1;
endmodule
