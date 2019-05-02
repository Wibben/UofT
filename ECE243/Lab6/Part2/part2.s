                .section .vectors, "ax"                  
                B       _start				// reset vector
				B       SERVICE_UND			// undefined instruction vector
				B       SERVICE_SVC			// software interrupt vector
				B       SERVICE_ABT_INST	// aborted prefetch vector
				B       SERVICE_ABT_DATA	// aborted data vector
				.word   0					// unused vector
				B       SERVICE_IRQ			// IRQ interrupt vector
				B       SERVICE_FIQ			// FIQ interrupt vector
				.text
				.global _start                        
_start:                                         
/* Set up stack pointers for IRQ and SVC processor modes */
                MOV     R1, #0b11010010		// interrupts masked, MODE = IRQ
				MSR		CPSR_c, R1			// change to IRQ mode
				LDR		SP, =0xFFFFFFFF - 3	// set IRQ stack to A9 onchip memory

				MOV     R1, #0b11010011		// interrupts masked, MODE = SVC
				MSR     CPSR, R1			// change to supervisor mode
				LDR		SP, =0x3FFFFFFF - 3	// set SVC stack to top of DDR3 memory

                BL      CONFIG_GIC       	// configure the ARM generic interrupt controller
                BL      CONFIG_TIMER     	// configure the Interval Timer
                BL      CONFIG_KEYS      	// configure the pushbutton KEYs port

/* Enable IRQ interrupts in the ARM processor */
                MOV      R0, #0b01010011	// IRQ unmasked, MODE = SVC
				MSR      CPSR_c, R0
				
                LDR     R5, =0xFF200000  	// LEDR base address
LOOP:
                LDR     R3, COUNT        	// global variable
                STR     R3, [R5]         	// write to the LEDR lights
                B       LOOP                

/* Configure the Interval Timer to create interrupts at 0.25 second intervals */
CONFIG_TIMER:   LDR		R0, =0xFF202000		// FPGA timer base address
                LDR     R1, =30784			
				STR		R1, [R0, #8]		// Set lower bits of timer
				LDR		R1, =381
				STR		R1, [R0, #12]		// Set upper bits of timer
				MOV     R1, #0b0111			// Control register bits
				STR     R1, [R0, #4]		// Start timer, set auto-reload and enable interrupts
                BX      LR

/* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:                                    
                LDR     R0, =0xFF200050		// KEY address
				MOV     R1, #0xF			// set interrupt mask bits
				STR     R1, [R0, #0x8]		// interrupt mask register (base + 8)
                BX      LR

/* Define the exception service routines */
SERVICE_IRQ:    PUSH    {R0-R7, LR}
				LDR     R4, =0xFFFEC100		// GIC CPU interface base address
				LDR     R5, [R4, #0x0C]		// read the ICCIAR in the CPU interface

FPGA_IRQ1_HANDLER:
				CMP     R5, #73				// check the interrupt ID
				BEQ		KEY_INTERRUPT
				CMP		R5, #72
				BEQ		CLK_INTERRUPT

UNEXPECTED:     B     	UNEXPECTED			// if not recognized, stop here
KEY_INTERRUPT:	BL      KEY_ISR
CLK_INTERRUPT:	BL		TIMER_ISR

EXIT_IRQ:       STR     R5, [R4, #0x10]		// write to the End of Interrupt Register (ICCEOIR)
				POP     {R0-R7, LR}
				SUBS    PC, LR, #4			// return from exception

/* Check if it has been 0.25 seconds and adds RUN to COUNT */
TIMER_ISR:		LDR     R0, =0xFF202000		// base address of FPGA timer
				LDR     R1, [R0]		// read edge capture register
				MOV     R2, #0
				STR     R2, [R0]			// clear the interrupt
				LDR     R0, =RUN			// Load RUN toggle
				LDR		R0, [R0]
				LDR		R1, =COUNT			// Load counter
				LDR		R2, [R1]
				ADD		R2, R0				// Increment counter by RUN
				STR		R2, [R1]			// Store incremented counter
END_TIMER_ISR:	BX		LR					// Return
				
/* Check if a key has been pressed and toggles RUN */
KEY_ISR:		LDR     R0, =0xFF200050		// base address of pushbutton KEY port
				LDR     R1, [R0, #0xC]		// read edge capture register
				MOV     R2, #0xF
				STR     R2, [R0, #0xC]		// clear the interrupt
				LDR     R0, =RUN			// Address of RUN toggle

CHECK_KEYS:		MOV		R3, #0b1111
				ORRS	R3, R1				// Check for any KEY
				BEQ		END_KEY_ISR
				LDR		R1, [R0]			// Get RUN from memory
				EOR		R1, #1				// Toggle RUN
				STR		R1, [R0]			// Set RUN in memory

END_KEY_ISR:	BX		LR					// Return
				
/* Global variables */
                .global COUNT                           
COUNT:          .word   0x0            // used by timer
                .global RUN              	// used by pushbutton KEYs
RUN:            .word   0x1              	// initial value to increment COUNT
                
				.end                                        
