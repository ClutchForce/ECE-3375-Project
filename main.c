//main.c

#include <stdio.h>
#include <stdint.h>

#include "address_map_arm.h"
#include "Feature1.h"
#include "Feature2.h"
#include "Feature3.h"

void logFeatureResponsibility(char *featureDesc);

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
    // while (1) {
    //     // Check for player input and process it
    //     processInput();
        
    //     // Update the game state based on input
    //     updateGameState();
        
    //     // Provide feedback to the player
    //     provideFeedback();
    // }
    
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

void provideFeedback() {
    // Implement feedback provision, possibly delegating to feature-specific functions
}
