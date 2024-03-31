//Refrence code for Lab 3
// This includes working c code for how potentiometer values can be read and used to control the LEDs
#include <stdint.h>
	
// ADC
#define adc_base 0xFF204000
#define channel_0 (uint32_t *)(adc_base + 0x00) // pot #1 
#define channel_1 (uint32_t *)(adc_base + 0x04) // pot #2 

// GPIO
#define gpi_port 0xFF200060
#define data_reg (uint32_t *)(gpi_port)
#define direction_control_reg (uint32_t *)(gpi_port + 0x04)

// switch 
#define switch_data (uint32_t *)(0xFF200040)
	
uint32_t readPotentiometer(uint8_t channel) {
    volatile uint32_t *adc = (channel == 0) ? channel_0 : channel_1; 
    uint32_t adc_value = *adc; 
	
	// check bit 15 to make sure conversion is complete
    while (!(adc_value & (1 << 15))) {	
        adc_value = *adc;
    }

    return adc_value & 0xFFF; // return first 12 bits
}

void display(uint32_t value) {
    uint32_t scale_value = value / (4096 / 11); // down-scale 12 bit data
    uint32_t output;
	
    if (scale_value >= 11) {
        output = 0x3FF; // max output 
    } else {
        output = (1 << scale_value) - 1; // convert scaled to binary 
    }
	
    *data_reg = output;
}

int main() {
    *channel_1 = 1; // have channels auto-update
    *direction_control_reg |= 0x3FF; // output LEDs (direction register bits 0-9 set to 1)

    while (1) {
        uint32_t switch_val = *switch_data & 0x01; 
        uint32_t display_val = readPotentiometer(switch_val); 
        display(display_val);

        for (volatile int delay_count = 100; delay_count != 0; --delay_count);
    }
	
	return 0;
}

