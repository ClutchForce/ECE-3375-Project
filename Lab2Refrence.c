//Refrence code for Lab 2
//This includes working c code for how the timer can be used to display time on the 7-segment display
#include <stdio.h>
#include <stdint.h>

// Peripheral base addresses
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define KEY_BASE 0xFF200050
#define SW_BASE 0xFF200040

// Timer base address and control registers
#define PERIPH_BASE 0xFFFEC000
#define MPCORE_PRIV_TIMER (PERIPH_BASE + 0x0600)
#define TIMER_CONTROL (MPCORE_PRIV_TIMER + 0x8)
#define TIMER_LOAD (MPCORE_PRIV_TIMER + 0x4)
#define TIMER_INTERRUPT (MPCORE_PRIV_TIMER + 0xC)
#define TIMER_END_OF_INTERRUPT (MPCORE_PRIV_TIMER + 0x10)

// Define pointers to peripherals
volatile int *hex3_hex0 = (int *)HEX3_HEX0_BASE;
volatile int *hex5_hex4 = (int *)HEX5_HEX4_BASE;
volatile int *pushbutton = (int *)KEY_BASE;
volatile int *slide_switch = (int *)SW_BASE;

// Seven segment display encoding
uint8_t seven_seg_encode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// Function to display time on 7-segment display
void display_time(int mm, int ss, int hh) {
    // Arrange the display to show hh:mm:ss from left to right
    *hex5_hex4 = (seven_seg_encode[mm / 10] << 8) | 
                 (seven_seg_encode[mm % 10]);
    *hex3_hex0 = (seven_seg_encode[ss / 10] << 24) | 
                 (seven_seg_encode[ss % 10] << 16) | 
                 (seven_seg_encode[hh / 10] << 8) | 
                 (seven_seg_encode[hh % 10]);
}

// Replace the init_timer function with the correct addresses
void init_timer() {
    volatile int *timer_control = (int *)TIMER_CONTROL;
    volatile int *timer_load = (int *)TIMER_LOAD;

    // Load the timer with the value for a 1-second interval (200MHz clock)
    // The correct value to slow down the timer is calculated based on the desired interval
    *timer_load = 200000000; // 200MHz * 1s = 200,000,000

    // Set the timer control to autoreload and start the timer
    *timer_control = 0x7; // Enable autoreload, interrupt enable and start the timer
}

// Function to handle timer interrupt
void handle_timer_interrupt(int *current_time, int *is_running) {
    volatile int *timer_interrupt = (int *)TIMER_INTERRUPT;
    volatile int *timer_eoi = (int *)TIMER_END_OF_INTERRUPT;

    // Check for timer interrupt only if the timer is running
    if (*timer_interrupt && *is_running) {
        // Acknowledge the interrupt by writing any value to the end of interrupt register
        *timer_eoi = 1;
        // Increment current_time by the appropriate amount to match the real-world time
        *current_time += 1;
    }
}

// Function to handle button presses
void handle_buttons(int *current_time, int *lap_time, int *is_running) {
    int buttons = *pushbutton;
    if (buttons & 0x1) {  // KEY0: Start
        *is_running = 1;
    }
    if (buttons & 0x2) {  // KEY1: Stop
        *is_running = 0; // Stop the timer
	}
	if (buttons & 0x4) { // KEY2: Lap
		*lap_time = *current_time;
	}
	if ((buttons & 0x8) || (*current_time / 6000000) == 60) { // KEY3: Clear OR current_time/6000000 equals 60
		*current_time = 0;
		*lap_time = 0;
	}
}

int main() {
	init_timer();
	int current_time = 0, lap_time = 0, is_running = 0;

	while (1) {
		handle_buttons(&current_time, &lap_time, &is_running);
		
		int time_to_display = (*slide_switch & 0x1) ? lap_time : current_time;
		
		if (is_running) {
			handle_timer_interrupt(&current_time, &is_running);
		}

		// Convert total time to hh:mm:ss format
		int hh = (time_to_display / 1000) % 100; // Hundredths of seconds
		int ss = (time_to_display / 100000) % 60; // Seconds
		int mm = (time_to_display / 6000000); // Minutes

		display_time(mm, ss, hh);
	}

	return 0;
}
