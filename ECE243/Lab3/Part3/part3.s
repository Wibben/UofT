/* Program that finds the largest number in a list of integers	*/

            .text                   // executable code follows
            .global _start                  
_start:                             
            MOV     R4, #RESULT     // R4 points to result location
            LDR     R0, [R4, #4]    // R0 holds the number of elements in the list
            ADD     R1, R4, #8      // R1 points to the start of the list
            BL      LARGE           
            STR     R0, [R4]        // R0 holds the subroutine return value

END:        B       END             

/* Subroutine to find the largest integer in a list
 * Parameters: R0 has the number of elements in the lisst
 *             R1 has the address of the start of the list
 * Returns: R0 returns the largest item in the list
 */
LARGE:      MOV		R2, R0			// R2 now has number of elements in the list
			LDR		R0, [R1]		// R0 holds the largest number so far

LOOP:		SUBS	R2, R2, #1		// Decrement loop counter
			BEQ		DONE			// Loop ends when R2 reaches 0
			ADD		R1, #4			// Go to the next number's address
			LDR		R3, [R1]		// Get the next number
			CMP		R0, R3			// Check if larger number found
			BGE		LOOP			// If not found do not update and go to next iteration
			MOV		R0, R3			// Update largest number
			B		LOOP			// Go to next iteration

DONE:		MOV		pc, lr			// Return to main
// End of LARGE subroutine

RESULT:     .word   0           
N:          .word   7           // number of entries in the list
NUMBERS:    .word   4, 5, 3, 6  // the data
            .word   1, 8, 2                 

            .end                            

