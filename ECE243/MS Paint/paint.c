#include "address_map_arm.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"
#include "exceptions.h"

volatile int pixel_buffer_start; // global variable
short int canvas[240][320]; // Canvas (aka background of the screen)
int prevMouseX,prevMouseY; // Previous mouse position
int mouseX,mouseY; // Mouse position
bool prevLeftBtn,leftBtn; // The state of the left button

/****************************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
****************************************************************************************/
void HEX_PS2(char b1, char b2, char b3) {
    volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
    volatile int * HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
     * a single 7-seg display in the DE2 Media Computer, for the hex digits 0 -
     * F */
    unsigned char seven_seg_decode_table[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int  shift_buffer, nibble;
    unsigned char code;
    int           i;

    shift_buffer = (b1 << 16) | (b2 << 8) | b3;
    for (i = 0; i < 6; ++i) {
        nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
        code   = seven_seg_decode_table[nibble];
        hex_segs[i]  = code;
        shift_buffer = shift_buffer >> 4;
    }
    /* drive the hex displays */
    *(HEX3_HEX0_ptr) = *(int *)(hex_segs);
    *(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
}

// Synchronizes the display with the VGA timing
void wait_for_vsync()
{
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
	register int status;
	
	*pixel_ctrl_ptr = 1; // Start synchronization process
	
	// Keep waiting until the whole screen ahs been drawn
	do {
		status = *(pixel_ctrl_ptr + 3);
	} while((status & 0x01) != 0);
}

int main()
{
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
	volatile int * mouse_port_ptr = (int *)PS2_BASE;
	
	set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
    config_GIC(); // configure the general interrupt controller
    // Set up PS2 mouse
    *(mouse_port_ptr) = 0xFF; // reset
    *(mouse_port_ptr + 1) = 0x1; // write to the PS/2 Control register to enable interrupts
    enable_A9_interrupts(); // enable interrupts
	
	// Initialize canvas
	clear_canvas();
	setup_screen();
	// Initialize mouse position to middle of screen
	prevMouseX = mouseX = 160; prevMouseY = mouseY = 120;
	prevLeftBtn = leftBtn = false;
	// Initialize default toolbar selections
	currentColour = 0;
	currentOpacity = 4;
	currentTool = 0;

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = FPGA_ONCHIP_BASE; // first store the address in the back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    draw_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = DDR_BASE;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
	
	while(1) {
		// Draw the current screen
		draw_screen();
		
		if(byte1&0x08 && byte1<0xE0 && (byte1>>4)<0x0A) HEX_PS2(byte1, byte2, byte3);
		
		wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	}
	
	return 0;
}
