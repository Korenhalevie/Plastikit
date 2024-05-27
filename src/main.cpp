#include <Arduino.h>
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "global_pins.h"
#include "credentials_and_servers.h"
#include "init_functions.h"

bool startingFlag = false;
bool heatingState = false;
int lastButtonState = LOW;
unsigned long heatingStartTime = 0; // To store the time when heating started
const unsigned long heatingDuration = 5000; // 3.5 minutes in milliseconds (3.5 * 60 * 1000) == 210000
bool heatingMessageSent = false;

void setup() {
    Serial.begin(115200); 
    GPIO_Init();
    // Connect to WiFi
    connectToWiFi(ssid, password, pinWifiLed, blinkDelay);

    // Initialize MQTT connection
    mqtt_setup();
}

void loop() {
    mqtt_loop();
    checkWiFiConnection(pinWifiLed);
    int buttonStartState = digitalRead(buttonStart);

    /********************/
    /* Heating Starting */
    /********************/

    // Check for button press
    if (startingFlag && buttonStartState == HIGH && lastButtonState == LOW) {
        // Toggle heating state
        heatingState = !heatingState;
        digitalWrite(pinHeating, heatingState ? HIGH : LOW);

        // Print the current state
        if (heatingState) {
            Serial.println("Heating ON");
            publishMessage("heating");
            heatingStartTime = millis(); // Record the time when heating is turned on
        } else {
            heatingStartTime = 0;
            Serial.println("Heating OFF");
            heatingMessageSent = false;
            digitalWrite(pinEngine, LOW);
        }
    }

    /************************************************/
    /* Check if heating has reached target duration */
    /************************************************/

    // Check if heating has been on for more than 3.5 minutes
    if (heatingState && (millis() - heatingStartTime >= heatingDuration)) {
        if (!heatingMessageSent) {
            Serial.println("Heating has been on for 3.5 minutes");
            publishMessage("engineStarting");
            heatingMessageSent = true;

            /*******************/
            /* Engine starting */
            /*******************/

            digitalWrite(pinEngine, HIGH);
        }
    }

    // Save the current button state for the next loop iteration
    lastButtonState = buttonStartState;
    delay(50); // Small delay to debounce the button
}
