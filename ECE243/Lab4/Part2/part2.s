/* Program that counts consecutive 1's and holds the length of the longest string */

			.text                   // executable code follows
			.global _start                  
_start:     MOV		R4, #TEST_NUM	// R4 will hold the address of the next data word
			MOV		R5, #0			// R5 will hold length of the string of ones
MAIN:		LDR		R1, [R4], #4	// R1 <- next word
			CMP		R1, #0
			BEQ		END				// 0 indicates the end of the list
			BL		ONES			// Count longest string of 1's
			CMP		R5, R0			// Result is returned in 0
			MOVLT	R5, R0			// Store greater value in R5
			B		MAIN			// Keep looping until the list is done

END:		B		END

// Subroutine ONES to find longest string of ones in R1
// Result is returned in R0
ONES:		MOV     R0, #0          // R0 will hold the result
LOOP:   	CMP     R1, #0          
			BEQ     END_ONES        // loop until the data contains no more 1's     
			LSR     R2, R1, #1      // perform SHIFT, followed by AND
			AND     R1, R1, R2      
			ADD     R0, #1          // count the string length so far
			B       LOOP            
END_ONES:   MOV		PC, LR   		// Return
// End of subroutine ONES

TEST_NUM: 	.word   0x103fe00f, 0x111ff332, 0x12345678
			.word	0xaf428039, 0x724c8831, 0xa92ee391
			.word	0xe0d4bd47, 0x8f8adad8, 0xdfa7ea48
			.word	0xe99e1b93, 0xa4cc303b, 0xda87b4e7  
			.word	0

			.end
