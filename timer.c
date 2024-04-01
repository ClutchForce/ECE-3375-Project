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
    timer->low_period = interval;
    timer->high_period = interval1;
