`timescale 1ns / 1ns // `timescale time_unit/time_precision

//SW[2:0] data inputs
//SW[9] select signals

//LEDR[0] output display

module L2P2(LEDR, SW);
    input [9:0] SW;
    output [9:0] LEDR;
	 wire S, SX, SY;
	 //invert input from Selector
	 v7404 u0(.pin1(SW[0]),.pin2(S));
	 //Find ~s & x, and s & y
	 v7408 u1(.pin1(S),.pin2(SW[1]),.pin3(SX),.pin4(SW[0]),.pin5(SW[2]),.pin6(SY));
	 //Sum of Products
	 v7432 u2(.pin1(SX),.pin2(SY),.pin3(LEDR[0]));
endmodule


//Hex Inverter
module v7404(pin1,pin2,pin3,pin4,pin5,pin6,pin8,pin9,pin10,pin11,pin12,pin13);
	input pin1, pin3, pin5, pin9, pin11, pin13;
	output pin2, pin4, pin6, pin8, pin10, pin12;
	//invert
	assign pin2 = ~pin1;
	assign pin4 = ~pin3;
	assign pin6 = ~pin5;
	assign pin8 = ~pin9;
	assign pin10 = ~pin11;
	assign pin12 = ~pin13;
endmodule

//Quad dual input AND 
module v7408(pin1,pin2,pin3,pin4,pin5,pin6,pin8,pin9,pin10,pin11,pin12,pin13);
	input pin1,pin2,pin4,pin5,pin9,pin10,pin12,pin13;
	output pin3,pin6,pin8,pin11;
	assign pin3 = pin1 & pin2;
	assign pin6 = pin4 & pin5;
	assign pin8 = pin9 & pin10;
	assign pin11 = pin12 & pin13;
endmodule

//Quad dual input OR
module v7432(pin1,pin2,pin3,pin4,pin5,pin6,pin8,pin9,pin10,pin11,pin12,pin13);
	input pin1,pin2,pin4,pin5,pin9,pin10,pin12,pin13;
	output pin3,pin6,pin8,pin11;
	assign pin3 = pin1 | pin2;
	assign pin6 = pin4 | pin5;
	assign pin8 = pin9 | pin10;
	assign pin11 = pin12 | pin13;
endmodule 