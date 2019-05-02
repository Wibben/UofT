.define LED_ADDRESS 0x1000
.define SW_ADDRESS 0x3000
.define MAX_SPEED 0x1000

// This program displays a binary counter on the LED ports
// The speed of the counter is controlled by the switches
			mvi 	r0, #0				// Used for counting
			mvi 	r1, #1				// Used for add/sub 1
MAIN:		mvi		r4, #SW_ADDRESS		// Point to switches
			ld 		r6, [r4]			// Read SW values
			add 	r6, r1				// Add 1 for minimum delay
			
// Count down delay until it reaches 0
DELAY1:		mvi		r5, #MAX_SPEED		// Reset max speed delay counter
			mvi 	r3, #DELAY2			// Point to inner delay loop

// Each delay counter will count MAX_SPEED times
DELAY2:		sub		r5, r1				// Count down by 1's
			mvnz	r7, r3				// Continue inner delay loop
// End of DELAY2

			sub		r6, r1				// Count down by 1's
			mvi		r3, #DELAY1			// Point to outer delay loop
			mvnz 	r7, r3				// Continue outer delay loop
// End of DELAY1
			
			add		r0, r1				// Increment binary counter
			mvi 	r4, #LED_ADDRESS	// Point to LED port
			st		r0, [r4]			// Display to LED port
			
			mvi		r7, #MAIN			// Endless looping
			