#include "global_pins.h"
#include <Arduino.h>

void GPIO_Init(void) {
    pinMode(pinWifiLed, OUTPUT);  // Initialize the WiFi LED pin as an output
    pinMode(pinMQTTMessages, OUTPUT);  // Initialize the MQTT message LED pin as an output
    pinMode(pinStartConfirmation, OUTPUT);  
    pinMode(pinHeating, OUTPUT);  
    pinMode(pinEngine, OUTPUT);  
    pinMode(buttonStart, INPUT);
}
