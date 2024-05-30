#include <Arduino.h>
#include "global_pins.h"
#include "lcd_handler.h"
#include "utility.h"
#include "mqtt_handler.h"
extern bool startingFlag;
extern bool heatingState;
extern bool heatingMessageSent;
extern bool engineOn;
extern bool transformationDone;
extern unsigned long heatingStartTime;
extern unsigned long buttonHoldStartTime;
extern int lastButtonStartState;
extern int lastButtonWeightState;

void resetMachine() {
    // Reset all flags and states
    heatingState = false;
    heatingMessageSent = false;
    engineOn = false;
    transformationDone = false;
    heatingStartTime = 0;
    buttonHoldStartTime = 0;
    lastButtonStartState = 0;
    lastButtonWeightState = 0;

    // Turn off all outputs
    digitalWrite(pinHeating, LOW);
    digitalWrite(pinEngine, LOW);

    // Display reset message
    displayMessage("Click on heating", "to start");
    publishMessage("Restarting");
    
    delay(1000);
}
