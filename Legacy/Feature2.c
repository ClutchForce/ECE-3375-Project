//Game Logic for the timer
#include "address_map_arm.h"
#include "Feature2.h"
#include <stdio.h>
#include <stdbool.h>

void initFeature2() {
    // Initialize game logic and set initial difficulty level
}

void getFeature2Description(char *featureNumber, char *desc) {
    sprintf(desc, "Feature %s is responsible for Game Logic and Difficulty Levels.", featureNumber);
}

void startCountdown(bool countdown){ 
    //sprintf(desc, "Feature %s is responsible for Game Logic and Difficulty Levels.", featureNumber);
};

// Additional functions for managing game state and difficulty levels
// define memory addresses
# define TIMER_BASE 0xFF202000
//intialize interval timer
typedef struct _interval_timer
{
    int status;
    int control;
    int low_period;
    int high_period;
    int low_counter;
    int high_counter;
} interval_timer;
// Holds values for stopwatch
int ms = 0;
int s = 0;

// interval of 0.1s
int interval = 1000000;
int interval1 = 1000000 >> 16;

//object of type interval_timer at the TIMER_BASE address
volatile interval_timer* const timer = (interval_timer*)TIMER_BASE;


//Method that can be called to check time
int check_time_elapsed(int time_limit) {
    if (timer->status & 1) { // Check if an interval has passed
        timer->status = 0; // Reset the status
        ms++; // Increment milliseconds

        if (ms > 99) { // Check if one second has passed
            s++; // Increment seconds
            ms = 0; // Reset milliseconds
        }

        if (s >= time_limit) { // Check if the set time limit has passed
            s = 0; // Reset seconds for next check
            return 1; // Return 1 as the time limit has elapsed
        }
    }
    return 0; // Return 0 as the time limit has not yet elapsed
}

//Write this into the main method
//writing the interval to the timer
    //timer->low_period = interval;
    //timer->high_period = interval1;
    //Tyler’s display code. If we were going to display time to the user on the board. We could use this code to display ms and seconds on the Hex display from an interval timer.

// display stuff onto a HEX cell
void DisplayHex(int value, int display)
{
    int 
    lookUpTable[16];
    lookUpTable[0] = 0x3F;
    lookUpTable[1] = 0x06;
    lookUpTable[2] = 0x5B;
    lookUpTable[3] = 0x4F;
    lookUpTable[4] = 0x66;
    lookUpTable[5] = 0x6D;
    lookUpTable[6] = 0x7D;
    lookUpTable[7] = 0x07;
    lookUpTable[8] = 0x7F;
    lookUpTable[9] = 0x6F;
    lookUpTable[10] = 0x77;
    lookUpTable[11] = 0x7F;
    lookUpTable[12] = 0x39;
    lookUpTable[13] = 0x3F;
    lookUpTable[14] = 0x79;
    lookUpTable[15] = 0x71;

    unsigned  mask;

    if (display == 0 || display == 4)
        mask = 0xffffff00;
    if (display == 1 || display == 5)
        mask = 0xffff00ff;
    if (display == 2)
        mask = 0xff00ffff;
    if (display == 3)
        mask = 0x00ffffff;
    if (value < 16) {
        if (display <= 3) {
//everything is being cleared but the values you need to change to display to the right Hex display cell
            //int leftValues = *(HEX_ptr1)&mask;
//this is the value going to be displayed this shifts to correct psition in the HEX display power of 256 display cell is 8 bits
            //*(HEX_ptr1) = (lookUpTable[value] * (int)pow(0x100, display)) + leftValues;
        }
        else {
            display = display - 4;
            //int leftValues = *(HEX_ptr2)&mask;
            //*(HEX_ptr2) = (lookUpTable[value] * (int)pow(0x100, display)) + leftValues;
        }
    }
    else {
        //*(HEX_ptr1) = 0x00;
        //*(HEX_ptr2) = 0x00;
    }
}

void startLoop(currentms, sw, currents, time, limit ){
    while (1)
        {
            if (sw == 1) { //if switch 0 in on, displays the lap value

                int a = currentms / 10; //gets the 10th of the value
                int b = currentms - (a * 10); //gets the unit value by subtracting the tenth

                DisplayHex(b, 0);
                DisplayHex(a, 1);

                int c = currents / 10;
                int d = currents - (c * 10);

                DisplayHex(d, 2);
                DisplayHex(c, 3);
            }
    //Paul could also take the time and set a count down through the speakers. Giving the users a countdown for how long they have rather than taking their eyes away from the game. "Paul did not implement this feature" - Paul
    if(currents >=  time-limit-3){
        //signal a countdown to the speakers which tells the users 3 seconds left (ex. “3…2…1”)
        startCountdown(true);
    };
        }
}