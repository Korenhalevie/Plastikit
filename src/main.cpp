#include <Arduino.h>
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "global_pins.h"
#include "credentials_and_servers.h"

bool startingFlag = false;
bool heatingState = false;
int lastButtonState = LOW;

void setup() {
    Serial.begin(115200);
    pinMode(pinWifiLed, OUTPUT);  // Initialize the WiFi LED pin as an output
    pinMode(pinMQTTMessages, OUTPUT);  // Initialize the MQTT message LED pin as an output
    pinMode(pinStartConfirmation, OUTPUT);  
    pinMode(pinHeating, OUTPUT);  
    pinMode(buttonStart, INPUT);  

    // Connect to WiFi
    connectToWiFi(ssid, password, pinWifiLed, blinkDelay);

    // Initialize MQTT connection
    mqtt_setup();
    publishMessage("sending message");
}

void loop() {
    mqtt_loop();
    checkWiFiConnection(pinWifiLed);
    int buttonStartState = digitalRead(buttonStart);

    // Check for button press
    if (startingFlag && buttonStartState == HIGH && lastButtonState == LOW) {
        // Toggle heating state
        heatingState = !heatingState;
        digitalWrite(pinHeating, heatingState ? HIGH : LOW);
// 1. HEATING 3.5 mins
// 2. Starting Engine
// 3.  
        // Print the current state
        if (heatingState) {
            Serial.println("Heating ON");
            publishMessage("heating");
        } else {
            Serial.println("Heating OFF");
        }
    }

    // Save the current button state for the next loop iteration
    lastButtonState = buttonStartState;

    delay(50); // Small delay to debounce the button
}
