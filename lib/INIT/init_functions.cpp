#include "init_functions.h"
#include "global_pins.h"
#include <Arduino.h>

void GPIO_Init(void) {
    pinMode(PIN_WIFI_LED, OUTPUT);  // Initialize the WiFi LED pin as an output
    pinMode(PIN_MQTT_MESSAGES, OUTPUT);  // Initialize the MQTT message LED pin as an output
    pinMode(PIN_START_CONFIRMATION, OUTPUT);  
    pinMode(PIN_HEATING, OUTPUT);  
    pinMode(PIN_ENGINE, OUTPUT);  
    pinMode(BUTTON_START, INPUT);
}
