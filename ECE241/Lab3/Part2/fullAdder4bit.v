`timescale 1ns / 1ns // `timescale time_unit/time_precision

// Full adder for 2 4-bit numbers
module fullAdder4bit(input [9:0] SW, output [9:0] LEDR);
	wire [3:0] A,B,S; // I/O
	wire Cin,Cout; // I/O wires
	wire C1,C2,C3; // Intermediary wires
	
	// Input assignments
	assign A = SW[3:0];
	assign B = SW[7:4];
	assign Cin = SW[8];
	
	adder bit0(.a(A[0]),.b(B[0]),.s(S[0]),.cin(Cin),.cout(C1));
	adder bit1(.a(A[1]),.b(B[1]),.s(S[1]),.cin(C1),.cout(C2));
	adder bit2(.a(A[2]),.b(B[2]),.s(S[2]),.cin(C2),.cout(C3));
	adder bit3(.a(A[3]),.b(B[3]),.s(S[3]),.cin(C3),.cout(Cout));
	
	// Outputs
	assign LEDR[3:0] = S;
	assign LEDR[9] = Cout;
endmodule

// Adder bit slice
// digit = a XOR b XOR cin, carry = a*b+a*cin+b*cin
module adder(input a,b,cin, output s,cout);
	assign s = a ^ b ^ cin;
	assign cout = (a&b) | (a&cin) | (b&cin);
endmodule
