#include <math.h>

#define switches 	(volatile int*) 0x00042010
#define leds 		(int*) 0x00042000
#define mode 		(volatile int*) 0x00700000
#define status 		(volatile int*) 0x00700004
#define go 			(volatile int*) 0x00700008
#define start 		(volatile int*) 0x0070000c
#define end 		(volatile int*) 0x00700010
#define colour 		(volatile int*) 0x00700014

void main()
{
	volatile int switch_data;
	int theta = 0;
	int prev_theta = 0;
	int theta_step;
	int x,y;
	int prev_x,prev_y;
	
	// There is only 1 starting position
	*start = (0x69 << 9) + 0xA8;
	
	while(1) {
		switch_data = (*switches) & 0xFF;
		*leds = switch_data;
		if(switch_data>>7 == 0) theta_step = 1;
		else theta_step = -1;
		*mode = switch_data>>6;
		
		// Erase previous line
		if(*mode == 1) while(*status == 1);
		*end = (prev_y << 9) + prev_x;
		*colour = 0x0;
		*go = 0x1;
		
		// Draw next line
		if(*mode == 1) while(*status == 1);
		*end = (y << 9) + x;
		*colour = switch_data;
		*go = 0x1;
		for(int j=0; j<250000; j++);
		
		// Next step
		prev_x = x;
		prev_y = y;
		prev_theta = theta;
		theta += theta_step;
		if(theta>360) theta = 1;
		else if(theta<0) theta = 359;
		x = 20*cos(theta*3.14/180) + 168;
		y = 20*sin(theta*3.14/180) + 105;
	}
}
