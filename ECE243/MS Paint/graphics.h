#include <stdbool.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TOOLBAR_WIDTH 13
#define TOOLBAR_HEIGHT 240

extern volatile int pixel_buffer_start; // global variable
extern short int canvas[SCREEN_HEIGHT][SCREEN_WIDTH]; // Canvas (aka background of the screen)
extern int prevMouseX,prevMouseY; // Previous mouse position
extern int mouseX,mouseY; // Mouse position
extern int currentColour,currentOpacity,currentTool; // Toolbar settings
extern volatile char byte1, byte2, byte3; // PS/2 variables
extern bool prevLeftBtn,leftBtn; // The state of the left button

// Canvas modifying functions
void setup_screen(); // Sets up the canvas
void clear_canvas(); // Clears the canvas
void draw_line(int x1, int y1, int x2, int y2); // Draws a line from the previous to the current mouse position
void fill_colour(); // Paint bucket tool

// All of the drawing functions
void plot_pixel(int x, int y, short int line_color); // Draw a single pixel to the screen
void draw_screen(); // Draws the canvas and other items to the screen
void draw_cursor(); // Draws the cursor to the screen
void draw_border(int x, int y); // Draws a 9x9 dark grey border starting at (x,y)

// Helper functions
void swap(int * x, int * y); // Swaps 2 numbers
void toolbar_selection(); // Sees if a initial left click is selecting something on the toolbar
short int getColourAfterOpacity(short int fg, short int bg, int opacity); // Returns the colour after applying the current opacity 
void fill_colour_dfs(int x, int y, short int targetColour); // Uses DFS to change a region of one colour to another
