/* Program that converts a binary number to decimal */
           .text               		// executable code follows
           .global _start
_start:
            MOV     R4, #N
            ADD     R5, R4, #4	 	// R5 points to the decimal digits storage location
            LDR     R4, [R4]     	// R4 holds N
            MOV 	R0, R4       	// dividend for DIVIDE goes in R0
            MOV		R1, #1000		// Get the thousands digit
			BL      DIVIDE
            STRB    R1, [R5, #3] 	// Store the thousands digit
			MOV		R1, #100		// Get the hundreds digit
			BL      DIVIDE
            STRB    R1, [R5, #2] 	// Store the hundreds digit
			MOV		R1, #10			// Get the tens digit
			BL      DIVIDE
            STRB    R1, [R5, #1] 	// Store the tens digit
            STRB    R0, [R5]     	// Ones digit is in R0 (remainder)
END:        B       END

/* Subroutine to perform the integer division R0 / R1.
 * Parameters: dividend in R0, divisor in R1
 * Returns: quotient in R1, and remainder in R0
 */
DIVIDE:     MOV     R2, #0
CONT:       CMP     R0, R1
            BLT     DIV_END
            SUB     R0, R1
            ADD     R2, #1
            B       CONT
DIV_END:    MOV     R1, R2     		// quotient in R1 (remainder in R0)
            MOV     PC, LR

N:          .word   9876         	// the decimal number to be converted
Digits:     .space  4          		// storage space for the decimal digits

            .end
