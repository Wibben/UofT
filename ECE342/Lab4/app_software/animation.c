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
	int y = 0;
	int prev_y = 0;
	int y_step;
	
	while(1) {
		switch_data = (*switches) & 0xFF;
		*leds = switch_data;
		if(switch_data>>7 == 0) y_step = 1;
		else y_step = -1;
		*mode = (switch_data>>6) & 0x1;
		
		// Draw next line
		if(*mode == 1) while(*status == 1);
		*start = (y << 9) + 0x0;
		*end = (y << 9) + 0x1FF;
		*colour = switch_data;
		*go = 0x1;
		
		// Erase previous line
		if(*mode == 1) while(*status == 1);
		*start = (prev_y << 9) + 0x0;
		*end = (prev_y << 9) + 0x1FF;
		*colour = 0x0;
		*go = 0x1;
		for(int j=0; j<250000; j++);
		
		// Next step
		prev_y = y;
		y += y_step;
		if(y>209) y = 0;
		else if(y<0) y = 209;
	}
}
