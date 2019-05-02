#include "exceptions.h"
#include "address_map_arm.h"
#include "graphics.h"
#include <stdbool.h>
#include <stdio.h>

volatile char byte1, byte2, byte3; // PS/2 variables

// Interrupt service routine for PS2 mouse
void PS2_ISR( void )
{
  	volatile int * PS2_ptr = (int *)PS2_BASE; // PS/2 port address
	int PS2_data, RAVAIL;
	int dx=0,dy=0;
	bool xSign,ySign;

	PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
	RAVAIL = (PS2_data & 0xFFFF0000) >> 16; // extract the RAVAIL field
	if (RAVAIL > 0)
	{
		/* always save the last three bytes received */
		byte1 = byte2;
		byte2 = byte3;
		byte3 = PS2_data & 0xFF;
		if ( (byte2 == (char) 0xAA) && (byte3 == (char) 0x00) )
			// mouse inserted; initialize sending of data
			*(PS2_ptr) = 0xF4;
	}
	
	// Update mouse position when all 3 bytes are read in
	if(byte1&0x08 && byte1<0xE0 && (byte1>>4)<0x0A) {
		// Update state of left button
		prevLeftBtn = leftBtn;
		leftBtn = byte1&0b1;
		
		// Get x movement
		xSign = byte1 & 0b00010000;
		if(xSign) { // Negative
			dx = byte2-256;
		} else {
			dx = byte2;
		}
		// Get y movement
		ySign = byte1 & 0b00100000;
		if(ySign) { // Negative
			dy = byte3-256;
		} else {
			dy = byte3;
		}
		
		// Update mouse position
		prevMouseX = mouseX;
		prevMouseY = mouseY;
		mouseX += dx;
		mouseY -= dy; // y position is inverted
		// Boundary checks
		if(mouseX<0) mouseX = 0;
		else if(mouseX>319) mouseX = 319;
		if(mouseY<0) mouseY = 0;
		else if(mouseY>239) mouseY = 239;
		
		// Draw a line if the last and current state of the left button are both true (pressed down)
		// On an initial left click (just current state is true) check if it's clicking on the toolbar
		if(prevLeftBtn && leftBtn && currentTool==0) draw_line(prevMouseX,prevMouseY,mouseX,mouseY);
		else if(!prevLeftBtn && leftBtn) {
			if(mouseX<TOOLBAR_WIDTH) toolbar_selection();
			else if(currentTool==1) fill_colour();
		}
	}
	
	return;
}
