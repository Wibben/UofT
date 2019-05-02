.define LED_ADDRESS 0x1000
.define HEX_ADDRESS 0x2000
.define SW_ADDRESS 0x3000
.define DELAY 3333
.define STACK 256

// Count on HEXs at speed defined by SWs
			mvi 	r1, #1				//load 1 for easy add/sub counting
			mvi		r4, #0				//initially load count to 0
MAIN:		mvi		r6, #STACK			//use as stack pointer
			mvi		r3, #HEX_ADDRESS 	//hex pointer
//for hex4--------------------------------------------------------------//
			mv		r0, r4				//copy r4 to r0 for subroutine
			mv		r5, r7				//subroutine for 10k
			mvi		r7, #DIV10000
			
			mvi 	r5, #DATA			//get hex data to store to hex
			add 	r5, r2
			ld		r2, [r5]
			
			mvi		r5, #4				//store quotient to HEX4
			add		r5, r3
			st		r2, [r5]
//for hex3--------------------------------------------------------------//
			mv		r5, r7				//subroutine for 1k
			mvi		r7, #DIV1000
			
			mvi 	r5, #DATA			//get hex data to store to hex
			add 	r5, r2
			ld		r2, [r5]
			
			mvi		r5, #3				//store quotient to HEX3
			add		r5, r3
			st		r2, [r5]
//for hex2--------------------------------------------------------------//
			mv		r5, r7				//subroutine for 100
			mvi		r7, #DIV100
			
			mvi 	r5, #DATA			//get hex data to store to hex
			add 	r5, r2
			ld		r2, [r5]
			
			mvi		r5, #2				//store quotient to HEX2
			add		r5, r3
			st		r2, [r5]
//for hex1--------------------------------------------------------------//
			mv		r5, r7				//subroutine for 10
			mvi		r7, #DIV10
			
			mvi 	r5, #DATA			//get hex data to store to hex
			add 	r5, r2
			ld		r2, [r5]
			
			mvi		r5, #1				//store quotient to HEX1
			add		r5, r3
			st		r2, [r5]
//for hex0--------------------------------------------------------------//
			mvi		r5, #DATA			//get hex data to store to hex
			add 	r5, r0
			ld 		r0, [r5]
			
			st		r0, [r3]			//store remainder to HEX0
//increment counter-----------------------------------------------------//
			add		r4, r1				//increment counter
			mvi		r7, #COUNT			//delay next loop of program

//Read the delay multiplier from the SWs--------------------------------//
COUNT:		mvi		r3, #DELAY			// delay counter 
 			mv		r5, r7				// save address of next instruction 
OUTER:		mvi		r0, #SW_ADDRESS		// point to SW port 
			ld		r6, [r0]			// load inner loop delay from SW 
			add		r6, r1				// in case 0 was read
 			mv		r2, r7				// save address of next instruction 
INNER:		sub		r6, r1				// decrement inner loop counter 
 			mvnz	r7, r2				// continue inner loop 
 			sub		r3, r1				// decrement outer loop counter 
 			mvnz	r7, r5				// continue outer loop
			
			mvi		r7, #MAIN 			// execute again

// subroutine DIV10 
// This subroutine divides the number in r0 by 10 
// The algorithm subtracts 10 from r0 until r0 < 10, and keeps count in r2 
// input: r0 // returns: quotient Q in r2, remainder R in r0 
DIV10: 
			mvi r1, #1 
			sub r6, r1 					// save registers that are modified 
			st r3, [r6] 
			sub r6, r1 
			st r4, [r6] 				// end of register saving

			mvi r2, #0 					// init Q
			mvi r3, RETDIV 				// for branching 
DLOOP: 		mvi r4, #9 					// check if r0 is < 10 yet 
			sub r4, r0 
			mvnc r7, r3 				// if so, then return

INC: 		add r2, r1 					// but if not, then increment Q 
			mvi r4, #10 
			sub r0, r4 					// r0 -= 10 
			mvi r7, DLOOP 				// continue loop 
RETDIV:
			ld r4, [r6] 				// restore saved regs 
			add r6, r1
			ld r3, [r6] 				// restore the return address 
			add r6, r1
			add r5, r1 					// adjust the return address by 2 
			add r5, r1
			mv r7, r5 					// return results

// subroutine DIV100--------------------------------------------------------------------//
DIV100: 
			mvi r1, #1 
			sub r6, r1 					// save registers that are modified 
			st r3, [r6] 
			sub r6, r1 
			st r4, [r6] 				// end of register saving

			mvi r2, #0 					// init Q
			mvi r3, RETDIV 				// for branching 
DLOOP0: 	mvi r4, #99 					// check if r0 is < 10 yet 
			sub r4, r0 
			mvnc r7, r3 				// if so, then return

INC0: 		add r2, r1 					// but if not, then increment Q 
			mvi r4, #100 
			sub r0, r4 					// r0 -= 10 
			mvi r7, DLOOP 				// continue loop 
RETDIV0:
			ld r4, [r6] 				// restore saved regs 
			add r6, r1
			ld r3, [r6] 				// restore the return address 
			add r6, r1
			add r5, r1 					// adjust the return address by 2 
			add r5, r1
			mv r7, r5 					// return results

// subroutine DIV1000--------------------------------------------------------------------//
DIV1000: 
			mvi r1, #1 
			sub r6, r1 					// save registers that are modified 
			st r3, [r6] 
			sub r6, r1 
			st r4, [r6] 				// end of register saving

			mvi r2, #0 					// init Q
			mvi r3, RETDIV 				// for branching 
DLOOP00: 	mvi r4, #999 					// check if r0 is < 10 yet 
			sub r4, r0 
			mvnc r7, r3 				// if so, then return

INC00: 		add r2, r1 					// but if not, then increment Q 
			mvi r4, #1000 
			sub r0, r4 					// r0 -= 10 
			mvi r7, DLOOP 				// continue loop 
RETDIV00:
			ld r4, [r6] 				// restore saved regs 
			add r6, r1
			ld r3, [r6] 				// restore the return address 
			add r6, r1
			add r5, r1 					// adjust the return address by 2 
			add r5, r1
			mv r7, r5 					// return results

// subroutine DIV10000--------------------------------------------------------------------//
DIV10000: 
			mvi r1, #1 
			sub r6, r1 					// save registers that are modified 
			st r3, [r6] 
			sub r6, r1 
			st r4, [r6] 				// end of register saving

			mvi r2, #0 					// init Q
			mvi r3, RETDIV 				// for branching 
DLOOP000: 	mvi r4, #9999 					// check if r0 is < 10 yet 
			sub r4, r0 
			mvnc r7, r3 				// if so, then return

INC000: 	add r2, r1 					// but if not, then increment Q 
			mvi r4, #10000 
			sub r0, r4 					// r0 -= 10 
			mvi r7, DLOOP 				// continue loop 
RETDIV000:
			ld r4, [r6] 				// restore saved regs 
			add r6, r1
			ld r3, [r6] 				// restore the return address 
			add r6, r1
			add r5, r1 					// adjust the return address by 2 
			add r5, r1
			mv r7, r5 					// return results

//HEX numbers for displaying
DATA:		.word 0b00111111 // ’0’ 
			.word 0b00000110 // ’1’ 
			.word 0b01011011 // ’2’ 
			.word 0b01001111 // ’3’ 
			.word 0b01100110 // ’4’ 
			.word 0b01101101 // ’5
			.word 0b01111101 // ’6’ 
			.word 0b00000111 // ’7’ 
			.word 0b01111111 // ’8’ 
			.word 0b01101111 // ’9’
