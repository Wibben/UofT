extern volatile char byte1, byte2, byte3; // PS/2 variables

void set_A9_IRQ_stack(void);
void config_GIC(void);
void config_PS2(void);
void enable_A9_interrupts(void);

void config_interrupt (int, int);
void hw_write_bits(volatile int *, volatile int, volatile int);
void PS2_ISR (void);
