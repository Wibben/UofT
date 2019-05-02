#include "graphics.h"
#include <stdlib.h>

// Colour palatte
short int palatte[9] = {
	0x0000, 0x7BEF, 0xFFFF,
	0x001F, 0x07E0, 0xF800,
	0xF81F, 0x07FF, 0xFFE0
};
int currentColour,currentOpacity,currentTool;
static int dfsLimit = 4000; // Maximum dfs limit
int dfsCount;

// Sets up the canvas
void setup_screen()
{
	// Draw the toolbar background (light grey)
	for(int x=0; x<TOOLBAR_WIDTH; x++) {
		for(int y=0; y<TOOLBAR_HEIGHT; y++) {
			canvas[y][x] = 0xBDF7;
		}
	}
	
	// Draw the colour palatte
	// Each colour will be 5x5 in size and will get a 9x9 dark grey border if selected
	for(int i=0; i<9; i++) {
		// Draw colour
		for(int x=4; x<9; x++) {
			for(int y=10*(i+1); y<10*(i+1)+5; y++) {
				canvas[y][x] = palatte[i];
			}
		}
	}
	
	// Draw the tools
	for(int dxy=-2; dxy<=2; dxy++) {
		// Cursor - black crosshair
		canvas[135][6+dxy] = 0x0000;
		canvas[135+dxy][6] = 0x0000;
		// Fill - Crosshair that is the current colour and overlaid w an X
		canvas[145][6+dxy] = palatte[currentColour];
		canvas[145+dxy][6] = palatte[currentColour];
		canvas[145+dxy][6+dxy] = palatte[currentColour];
		canvas[145-dxy][6+dxy] = palatte[currentColour];
	} 
	// Centers are grey
	canvas[135][6] = canvas[145][6] = 0xBDF7;
	
	// Draw the opacity settings
	// Icon will be a 4x4 square with the current colour overlaid on top of a 4x4 white square
	for(int i=1; i<=4; i++) {
		// Draw base white square
		for(int x=4; x<8; x++) {
			for(int y=TOOLBAR_HEIGHT-15-10*i; y<TOOLBAR_HEIGHT-11-10*i; y++) {
				canvas[y][x] = 0xFFFF;
			}
		}
		// Draw overlay square
		for(int x=5; x<9; x++) {
			for(int y=TOOLBAR_HEIGHT-14-10*i; y<TOOLBAR_HEIGHT-10-10*i; y++) {
				canvas[y][x] = getColourAfterOpacity(palatte[currentColour],canvas[y][x],i);
			}
		}
	}
	
	// Draw the clear screen 'button'
	// Which is just a 5x5 red X
	for(int i=0; i<5; i++) {
		canvas[TOOLBAR_HEIGHT-15+i][4+i] = 0xF800;
		canvas[TOOLBAR_HEIGHT-15+i][8-i] = 0xF800;
	}
}

// Clears the canvas
void clear_canvas()
{
	// Clear canvas dimensions
	for(int x=TOOLBAR_WIDTH; x<SCREEN_WIDTH; x++) {
		for(int y=0; y<SCREEN_HEIGHT; y++) {
			canvas[y][x] = 0xFFFF;
		}
	}
}

// Draws a line from the previous to the current mouse position
void draw_line(int x1, int y1, int x2, int y2)
{
	// Get the current colour
	int colour = palatte[currentColour];
	
	// Check if it's a dot??
	if(x1==x2 && y1==y2) {
		canvas[y1][x1] = colour;
		return;
	}
	
	// Check steepness of the line, if it is steep, it's better
	// to move along the y-axis when drawing
	bool is_steep = abs(y2-y1) > abs(x2-x1);
	// If it is steep switch the x and y values
	// the drawing loop will decide how the drawing will occur
	if(is_steep) {
		swap(&x1,&y1);
		swap(&x2,&y2);
	}
	
	// We are going to increment from x1 to x2 so 
	// swap the endpoints if x1 > x2
	if(x1 > x2) {
		swap(&x1,&x2);
		swap(&y1,&y2);
	}
	
	int deltax = x2-x1;
	int deltay = abs(y2-y1);
	int error = -(deltax/2);
	int x,y,y_step;
	
	// Figure out how y will be incremented
	if(y1<y2) y_step = 1;
	else y_step = -1;
	
	for(x=x1,y=y1; x<=x2; x++) {
		// If the line is steep the x and y values are swapped
		// Ensure that the drawing is within bounds of the canvas
		if(is_steep && y>=TOOLBAR_WIDTH) {
			canvas[x][y] = getColourAfterOpacity(colour,canvas[x][y],currentOpacity);
		} else if(x>=TOOLBAR_WIDTH) {
			canvas[y][x] = getColourAfterOpacity(colour,canvas[y][x],currentOpacity);
		}
		
		// Check margin of error
		error += deltay;
		if(error>=0) {
			y += y_step; // Increment y val
			error -= deltax; // Reset error
		}
	}
}

// Paint bucket tool
void fill_colour()
{
	// If the picked colour is the same as the canvas colour, do nothing
	if(canvas[mouseY][mouseX]==palatte[currentColour]) return;
	// Otherwise run the DFS to fill the region of the target colour
	dfsCount = 0;
	fill_colour_dfs(mouseX,mouseY,canvas[mouseY][mouseX]);
}

// Draw a single pixel to the screen
void plot_pixel(int x, int y, short int line_color)
{
	// Make sure the pixel is within bounds
	if(x>=0 && x<320 && y>=0 && y<240) {
		*(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
	}
}

// Draws the canvas and other items to the screen
void draw_screen()
{
	// Draw the main screen
	for(int x=0; x<SCREEN_WIDTH; x++) {
		for(int y=0; y<SCREEN_HEIGHT; y++) {
			plot_pixel(x,y,canvas[y][x]);
		}
	}
	
	draw_cursor(); // Draw the cursor
	draw_border(2,10*(currentColour+1)-2); // Border around current colour
	draw_border(2,TOOLBAR_HEIGHT-17-10*currentOpacity); // Border around current opacity
	draw_border(2,131+10*currentTool); // Border around current tool
}

// Draws the appropriate cursor to the screen
// Cursor is 5x5 with a tranparent center
void draw_cursor()
{
	if(currentTool==0) { // Draw tool
		// Draw vertical and horizontal black lines
		for(int dxy=-2; dxy<=2; dxy++) {
			if(mouseY+dxy<SCREEN_HEIGHT && mouseY+dxy>=0 && canvas[mouseY+dxy][mouseX]==0x0000) 
				plot_pixel(mouseX,mouseY+dxy,0xFFFF);
			else plot_pixel(mouseX,mouseY+dxy,0x0000);
			
			if(mouseX+dxy<SCREEN_WIDTH && mouseX+dxy>=0 && canvas[mouseY][mouseX+dxy]==0x0000) 
				plot_pixel(mouseX+dxy,mouseY,0xFFFF);
			else plot_pixel(mouseX+dxy,mouseY,0x0000);
		}
	} else if(currentTool==1) { // Paint bucket tool
		for(int dxy=-2; dxy<=2; dxy++) {
			// Cross
			if(mouseY+dxy<SCREEN_HEIGHT && mouseY+dxy>=0 && 
			   canvas[mouseY+dxy][mouseX]==palatte[currentColour]) 
				plot_pixel(mouseX,mouseY+dxy,0xFFFF-palatte[currentColour]);
			else plot_pixel(mouseX,mouseY+dxy,palatte[currentColour]);
			
			if(mouseX+dxy<SCREEN_WIDTH && mouseX+dxy>=0 && 
			   canvas[mouseY][mouseX+dxy]==palatte[currentColour]) 
				plot_pixel(mouseX+dxy,mouseY,0xFFFF-palatte[currentColour]);
			else plot_pixel(mouseX+dxy,mouseY,palatte[currentColour]);
			
			// Diagonals
			if(mouseX+dxy<SCREEN_WIDTH && mouseX+dxy>=0 && mouseY+dxy<SCREEN_HEIGHT && mouseY+dxy>=0 && 
			   canvas[mouseY+dxy][mouseX+dxy]==palatte[currentColour]) 
				plot_pixel(mouseX+dxy,mouseY+dxy,0xFFFF-palatte[currentColour]);
			else plot_pixel(mouseX+dxy,mouseY+dxy,palatte[currentColour]);
			
			if(mouseX+dxy<SCREEN_WIDTH && mouseX+dxy>=0 && mouseY+dxy<SCREEN_HEIGHT && mouseY+dxy>=0 && 
			   canvas[mouseY+dxy][mouseX-dxy]==palatte[currentColour]) 
				plot_pixel(mouseX-dxy,mouseY+dxy,0xFFFF-palatte[currentColour]);
			else plot_pixel(mouseX-dxy,mouseY+dxy,palatte[currentColour]);
		}
	}
	
	// A transparent center means whatever is on the canvas at the position
	plot_pixel(mouseX,mouseY,canvas[mouseY][mouseX]);
}

// Draw a dark grey 9x9 border given its top left position
void draw_border(int x, int y)
{
	// Draws 4 lines surrounding the colour
	for(int i=0; i<9; i++) {
		plot_pixel(x+i,y,0x39E7); // Top
		plot_pixel(x+i,y+8,0x39E7); // Bottom
		plot_pixel(x,y+i,0x39E7); // Left
		plot_pixel(x+8,y+i,0x39E7);// Right
	}
}

// Swaps 2 numbers
void swap(int * x, int * y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

// Parses an initial left click to select something on the toolbar
// Can stop after finidng a button that bas been clicked
void toolbar_selection()
{
	// Colour selection, just loop through where each colour would be and check
	// if the click is within bounds (colours are 5x5 but bounds will be 7x7)
	for(int i=0; i<9; i++) {
		// Check boundaries
		if(mouseX>=3 && mouseX<=9 && mouseY>=10*(i+1)-1 && mouseY<=10*(i+1)+5) {
			currentColour = i;
			setup_screen(); // Update toolbar (to show selected colour for opacity buttons)
			return; // Stop
		}
	}
	
	// Check if tools are beign clicked
	for(int i=0; i<2; i++) {
		// Check boundaries
		if(mouseX>=3 && mouseX<=9 && mouseY>=132+10*i && mouseY<=138+10*i) {
			currentTool = i;
			return;
		}
	}
	
	// Check if the opacity buttons are being clicked
	for(int i=1; i<=4; i++) {
		// Check boundaries
		if(mouseX>=3 && mouseX<=9 && mouseY>=TOOLBAR_HEIGHT-16-10*i && mouseY<=TOOLBAR_HEIGHT-10-10*i) {
			currentOpacity = i;
			return;
		}
	}
	
	// Check if the clear screen button is being clicked (bounds is 7x7)
	if(mouseX>=3 && mouseX <=9 && mouseY>=TOOLBAR_HEIGHT-16 && mouseY<=TOOLBAR_HEIGHT-10) {
		clear_canvas(); // Set the canvas to its initial state
		draw_border(2,TOOLBAR_HEIGHT-17); // Draw a border around clear button
	}
}

// Returns the colour after applying the current opacity 
short int getColourAfterOpacity(short int fg, short int bg, int opacity)
{
	// Extract r,g,b values from foregound and background colours
	int fgR,fgG,fgB;
	fgB = fg & 0b11111;
	fgG = (fg>>5) & 0b111111;
	fgR = (fg>>11) & 0b11111;
	
	int bgR,bgG,bgB;
	bgB = bg & 0b11111;
	bgG = (bg>>5) & 0b111111;
	bgR = (bg>>11) & 0b11111;
	
	// Mix the colours together and get the new colour after opacity has been applied
	short int newColour;
	int newR,newG,newB;
	newR = (opacity*fgR + (4-opacity)*bgR)/4;
	newG = (opacity*fgG + (4-opacity)*bgG)/4;
	newB = (opacity*fgB + (4-opacity)*bgB)/4;
	
	newColour = (newR<<11) + (newG<<5) + newB;
	
	return newColour;
}

// Uses DFS to change a region of one colour to another
void fill_colour_dfs(int x, int y, short int targetColour)
{
	// Stop if out of bounds or reaches a square that is not the target colour
	// Also stop if it exceeds the limit (otherwise there will be a memory error)
	if(x<TOOLBAR_WIDTH || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT || 
	   canvas[y][x]!=targetColour || dfsCount>dfsLimit) return;
	
	// Update the colour
	dfsCount++; // Because DFS will cause a bunch of stuff to be pushed onto the stack
				// for some reason it's even worse if you use BFS and non-iterative methods
	canvas[y][x] = getColourAfterOpacity(palatte[currentColour],canvas[y][x],currentOpacity);
	
	// Continue searching
	fill_colour_dfs(x+1,y,targetColour);
	fill_colour_dfs(x-1,y,targetColour);
	fill_colour_dfs(x,y+1,targetColour);
	fill_colour_dfs(x,y-1,targetColour);
}
