#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <stdio.h>
	
#define HEX3_HEX0_BASE (uint32_t *) 0xFF200020
#define HEX5_HEX4_BASE (uint32_t *) 0xFF200030

#define TIMER_BASE 0xFF202000
#define adc_base 0xFF204000
#define channel_0 (uint32_t *)(adc_base + 0x00)
#define channel_1 (uint32_t *)(adc_base + 0x04) 

#define switch_data (uint32_t *)(0xFF200040) 
#define push_data (uint32_t *)(0xFF200050) 

#define AUDIO_BASE_ADDRESS 0xFF203040 // Base address of the Audio CODEC
#define AUDIO_DATA_LENGTH 8 // Fixed length of 8 bytes for each audio data (Assumed for simplicity)


// Memory-mapped register offsets
#define AUDIO_CONTROL_REG_OFFSET  0x0  // Control Register
#define AUDIO_FIFOSPACE_REG_OFFSET 0x4  // Fifospace Register
#define AUDIO_LEFT_DATA_REG_OFFSET 0x8  // Left Channel Data Register
#define AUDIO_RIGHT_DATA_REG_OFFSET 0xC // Right Channel Data Register

bool Display(int time, const char* command);
void DisplayHex(const char* command);
void clearDisplay();
void play_audio(int index);

//audio data
#if DEMO
const unsigned int pop_it_data[] = {
    0x012D, 0x021E, 0x01C6, 0x01CE, 0x01A6, 0x01A9, 0x0182, 0x014E,
};

const unsigned int twist_it_data[] = {
    /* ... audio sample data for "twist" sound ... */
    0x0103, 0x00B6, 0x004F, 0xFFE5, 0xFFA1, 0xFF7A, 0xFF5D, 0xFF2F,
};

const unsigned int pull_it_data[] = {
    /* ... audio sample data for "pull" sound ... */
    0x00F7, 0x00A3, 0x0064, 0x002D, 0x0004, 0xFFE5, 0xFFD4, 0xFFD0,
};
#endif

bool game_running = true;


int main() {
    *channel_1 = 1;

    const char* cmds[] = {"Bop", "Twist", "Pull"};
    
	const int numCmds = sizeof(cmds) / sizeof(cmds[0]);
    const int seqLength = 4; 
    
	char* randomSeq[seqLength];

    srand(time(NULL));

	// clear all display
	clearDisplay();
    while(game_running){
        for(int i = 0; i < seqLength; i++) {
            int randIndex = rand() % numCmds; 
            randomSeq[i] = cmds[randIndex];
        }

        for (int j = 0; j < seqLength; j++) {
			int randomDuration = rand() % 3 + 2;

            //Write to audio codec
            //English translation: Play audio based on the command
            #if DEMO
            if (strcmp(randomSeq[j], "Bop") == 0) {
                play_audio(0);
            } else if (strcmp(randomSeq[j], "Twist") == 0) {
                play_audio(1);
            } else if (strcmp(randomSeq[j], "Pull") == 0) {
                play_audio(2);
            }
            #endif

            bool actionResult = Display(4, randomSeq[j]);
            if (!actionResult) {
				// end game loop
                game_running = false; 

				// feedback
                *HEX5_HEX4_BASE = (0x3F << 8) | 0x3E; 
    			*HEX3_HEX0_BASE = (0x79 << 24) | (0x33 << 16);
				
                break; 
            }
        }

        if (!game_running) {
            break;
        }
    };
    return 0;
}

typedef struct _interval_timer
{
    int status;
    int control;
    int low_period;
    int high_period;
    int low_counter;
    int high_counter;
} interval_timer;

volatile interval_timer* const timer = (interval_timer*)TIMER_BASE;

void initialize_timer(int seconds) {
    int ticks = 16000000; // clock rate depending
    
    timer->control = 0;
    timer->low_period = ticks & 0xFFFF;
    timer->high_period = (ticks >> 16) & 0xFFFF;
    timer->control = 0x7;
}

// poll timer to check
bool time_has_elapsed() {
    if (timer->status & 0x1) {
        timer->status = 0; // clear status
        return true;
    }
    return false;
}

uint32_t readPotentiometer() {
    volatile uint32_t *adc = channel_0;
    uint32_t adc_value = *adc; 

    while (!(adc_value & (1 << 15))) {	
        adc_value = *adc;
    }

    if ((adc_value & 0xFFF) > 10){
        return 1;
    }
    else{
        return 0;
    }
}

uint32_t readButton(){
    uint32_t push_val = *push_data & 0x01;

    return push_val;
}

uint32_t readSwitch(){
    uint32_t switch_val = *switch_data & 0x01;

    return switch_val;
}

void DisplayHex(const char* command){
    if (strcmp(command, "Bop") == 0) {
        *HEX5_HEX4_BASE = (0xFF << 8) | 0x3F; // BO
		*HEX3_HEX0_BASE = (0x73 << 24); // P

    } else if (strcmp(command, "Twist") == 0) {
		*HEX5_HEX4_BASE = (0x07 << 8) | 0x31; // T
		*HEX3_HEX0_BASE = (0x3E << 24) | (0x33 << 16) | (0x37 << 8); // URN

    } else if (strcmp(command, "Pull") == 0) {
       *HEX5_HEX4_BASE = (0x73 << 8) | (0x3E); // PU
	   *HEX3_HEX0_BASE = (0x38 << 24) | (0x38 << 16); // LL

    }
}  

bool Display(int duration, const char* command) {
    uint32_t initial_switch_state = readSwitch();
    int elapsed_seconds = 0;
    initialize_timer(duration);

    while (elapsed_seconds < (duration*10)) {
        if (time_has_elapsed()) {
            elapsed_seconds++;
        }

        DisplayHex(command);

        if (strcmp(command, "Bop") == 0) {
            if (readButton() > 0) {
                return true;
            }
        } else if (strcmp(command, "Twist") == 0) {
            if (readPotentiometer() == 1) {
                return true; 
            }
        } else if (strcmp(command, "Pull") == 0) { 
            if (readSwitch() != initial_switch_state) {
                return true;
            }
        }
    }
	
    return false;
}  

void clearDisplay(){
	*HEX5_HEX4_BASE = 0x0000;
    *HEX3_HEX0_BASE = 0x0000;
}

// Function to play audio based on the provided index
#if DEMO
void play_audio(int index) {
    volatile unsigned int *audio_control_reg = (unsigned int *)(AUDIO_BASE_ADDRESS + AUDIO_CONTROL_REG_OFFSET);
    volatile unsigned int *audio_left_data_reg = (unsigned int *)(AUDIO_BASE_ADDRESS + AUDIO_LEFT_DATA_REG_OFFSET);
    volatile unsigned int *audio_fifospace_reg = (unsigned int *)(AUDIO_BASE_ADDRESS + AUDIO_FIFOSPACE_REG_OFFSET);

    const unsigned int *audio_data;
    unsigned int audio_length = AUDIO_DATA_LENGTH / sizeof(unsigned int);

    // Select the appropriate audio data based on the index
    switch (index) {
        case 0:
            audio_data = pop_it_data;
            break;
        case 1:
            audio_data = twist_it_data;
            break;
        case 2:
            audio_data = pull_it_data;
            break;
        default:
            // Invalid index, return without playing audio
            return;
    }

    // Clear the Left Channel Write FIFO
    *audio_control_reg |= 0x4; // Set CW bit to clear Write FIFO
    *audio_control_reg &= ~0x4; // Clear CW bit to stop clearing

    // Write the audio data to the Left Channel Write FIFO
    for (unsigned int i = 0; i < audio_length; i++) {
        unsigned int fifospace = *audio_fifospace_reg & 0xFF; // Get WSLC field (available space in Left Channel Write FIFO)
        while (fifospace < 1) { // Wait for space in the FIFO
            fifospace = *audio_fifospace_reg & 0xFF;
        }
        *audio_left_data_reg = audio_data[i]; // Write data to the Left Channel Write FIFO
    }
    // Only writes to the Left Channel as the audio data is mono

}
#endif
