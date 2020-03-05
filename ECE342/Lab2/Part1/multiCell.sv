// Multiplier cell
module multiCell
(
	input plus,minus,
	input a,b,cin,
	output logic s,cout,sign_ext
);
	assign sign_ext = plus&b | minus&~b;
	fa full_adder(
		.a(a),
		.b(sign_ext),
		.cin(cin),
		.s(s),
		.cout(cout)
	);
endmodule
