`timescale 1ns / 1ns // `timescale time_unit/time_precision

// 2 to 1 multiplexer
// s = SW[2], x = SW[1], y = SW[0]
// L = LEDR[0]
module mux2to1(input [9:0] SW
			   output [9:0] LEDR);
	wire A, B, C, D;
	
	// L = !sx + sy
	v7404 c1(.pin1(SW[2]),.pin2(A)); // A = !s
	v7408 c2(.pin1(A),.pin2(SW[1]),.pin3(B),.pin4(sw[2]),.pin5(SW[0]),.pin6(C)); // B = Ax and C = sy
	v7432 c3(.pin1(B),.pin2(C),.pin3(D)); // D = B + C
	
	assign LEDR[0] = D;
endmodule;

// NOT chip
module v7404(input pin1, pin3, pin5, pin9, pin11, pin13
			 output pin2, pin4, pin6, pin8, pin10, pin12);
	assign pin2 = !pin1;
	assign pin4 = !pin3;
	assign pin6 = !pin5;
	assign pin8 = !pin9;
	assign pin10 = !pin11;
	assign pin12 = !pin13;
endmodule;

// AND chip
module v7408(input pin1, pin2, pin4, pin5, pin9, pin10, pin12, pin13
			 output pin3, pin6, pin8, pin11);
	assign pin3 = pin1 & pin2;
	assign pin6 = pin4 & pin5;
	assign pin8 = pin9 & pin10;
	assign pin11 = pin12 & pin13;
endmodule;

// OR chip
module v7432(input pin1, pin2, pin4, pin5, pin9, pin10, pin12, pin13
			 output pin3, pin6, pin8, pin11);
	assign pin3 = pin1 | pin2;
	assign pin6 = pin4 | pin5;
	assign pin8 = pin9 | pin10;
	assign pin11 = pin12 | pin13;
endmodule;
