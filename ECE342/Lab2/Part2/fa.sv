// Full adder
module fa
(
	input a,b,cin,
	output logic s,cout
);
	assign s = a ^ b ^ cin;
	assign cout = a&b | a&cin | b&cin;
endmodule
