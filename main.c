//main.c

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "address_map_arm.h"
#include "Feature1.h"
#include "Feature2.h"
#include "Feature3.h"

void logFeatureResponsibility(char *featureDesc);

volatile unsigned int const hex0 = (unsigned int*) HEX3_Hex0_BASE;



int main(void) {
    // Initialization
    initFeature1();
    initFeature2();
    initFeature3();

    //Testing connectivity 
    char featureDesc[100]; // Buffer for feature descriptions
    
    getFeatureDescription("1", featureDesc);
    logFeatureResponsibility(featureDesc);
    
    getFeatureDescription("2", featureDesc);
    logFeatureResponsibility(featureDesc);
    
    getFeatureDescription("3", featureDesc);
    logFeatureResponsibility(featureDesc);
    
    // Main game loop
    while (1) {
        // Check for player input and process it
        processInput();
        
        // Update the game state based on input
        updateGameState();
        
        // Provide feedback to the player
        provideFeedback();
     }
    
    return 0;
}

void logFeatureResponsibility(char *featureDesc) {
    printf("%s\n", featureDesc);
}

void processInput() {
    // Implement input processing, possibly delegating to feature-specific functions
}

void updateGameState() {
    // Implement game state updates, possibly delegating to feature-specific functions
}

void generateSequence() {
    //Command array
    const cmds = ["Bop", "Twist", "Pull"];

    //length of sequence
    const length = rand()%3;

    //Initialize sequence
    const seq = [];

        //Fill the sequence with commands
        for(int i =0; i<length; i++){
            const randNum = rand()%3;
            seq[i]=cmds[randNum];
        }

}

void Display(time, command){
    //If time isn't 0 display command on hex
    while (time!=0)
    {
        DisplayHex(command);
    }
    
    //If time is 0 clear hex
    if(time==0){
        *hex0=0;
    }
}

void DisplayHex(command){
    switch(command){
        case "Bop":
            *hex0 = 0b01111100;
        break;
        case "Twist":
            *hex0 =  0b01010000;
        break;
        case "Pull":
            *hex0 =0b01110011;
        break;
    }
}    
