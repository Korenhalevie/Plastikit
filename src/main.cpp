#include <Arduino.h>
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "global_pins.h"
#include "credentials_and_servers.h"
#include "init_functions.h"
#include "HX711.h"
#include "MeasureW.h"
#include "lcd_handler.h"
#include "utility.h"

HX711 scale;

int buttonStartState = 0;
int buttonWeightState = 0;
bool startingFlag = false;
bool heatingState = false;
int lastButtonStartState = 0;
int lastButtonWeightState = 0;
unsigned long heatingStartTime = 0; // To store the time when heating started
const unsigned long heatingDuration = 5000; // 3.5 minutes in milliseconds (3.5 * 60 * 1000) == 210000
bool heatingMessageSent = false;
bool engineOn = false;
bool transformationDone = false;
unsigned long buttonHoldStartTime = 0; // To store the time when buttons are held
const unsigned long buttonHoldDuration = 5000; // 5 seconds in milliseconds

void setup() {
    Serial.begin(115200); 
    GPIO_Init();
    connectToWiFi(ssid, password, pinWifiLed, blinkDelay);
    mqtt_setup();

    // Initialize the scale
    scale.begin(pinHX711_dout, pinHX711_sck);
    scale.set_scale(calibration_factor); // Set the calibration factor
    scale.tare(); // Reset the scale to 0

    // Initialize the LCD
    initializeLCD();
    delay(1000);

    // Initial reset
    resetMachine();
}

void loop() {
    mqtt_loop();
    checkWiFiConnection(pinWifiLed);
    buttonStartState = digitalRead(buttonStart);
    buttonWeightState = digitalRead(buttonWeight);

    /********************/
    /* Heating Starting */
    /********************/
    if (startingFlag && buttonStartState == HIGH && lastButtonStartState == LOW && !engineOn) {
        // Toggle heating state
        heatingState = !heatingState;
        digitalWrite(pinHeating, heatingState ? HIGH : LOW);

        if (heatingState) {
            Serial.println("Heating ON");
            displayMessage("Status:", "Heating ON");
            heatingStartTime = millis(); // Record the time when heating is turned on
        } else {
            Serial.println("Heating OFF");
            displayMessage("Status:", "Heating OFF");
            heatingMessageSent = false;
            digitalWrite(pinEngine, LOW);
            engineOn = false;
        }
    }

    /************************************************/
    /* Check if heating has reached target duration */
    /************************************************/
    if (heatingState && (millis() - heatingStartTime >= heatingDuration) && !heatingMessageSent) {
        Serial.println("Heating has been on for 3.5 minutes");
        publishMessage("engineStarting");
        heatingMessageSent = true;

        /*******************/
        /* Engine starting */
        /*******************/
        digitalWrite(pinEngine, HIGH);
        engineOn = true;
        displayMessage("Status:", "Transforming");
    }

    /*******************/
    /* Engine done check */
    /*******************/
    if (engineOn && buttonStartState == HIGH && lastButtonStartState == LOW) {
        Serial.println("done engine");
        digitalWrite(pinEngine, LOW);
        heatingState = false;
        displayMessage("Status:", "Waiting to scale");
        transformationDone = true;
        heatingMessageSent = false;
        engineOn = false;
    }

    /*******************/
    /* Scaling */
    /*******************/
    if (transformationDone && buttonWeightState == HIGH && lastButtonWeightState == LOW) {
        float weight = scale.get_units(10); // Read average of 10 values
        float normalizedWeight = normalizeWeight(weight);
        Serial.print("Weight: ");
        Serial.print(normalizedWeight);
        Serial.println(" grams");

        String message = String(normalizedWeight) + "g";
        publishMessage(message.c_str());

        message = String(normalizedWeight) + " Grams";
        displayMessage(message.c_str(), "were added!");
        delay(3000);
        displayMessage("Thank you", "  for recycling!");
        delay(2000);
        displayMessage("let's recycle", "again!");
        transformationDone = false;

        publishMessage("abort");
        // Reset the machine after scaling
        resetMachine();
        return; // Go back to the start of the loop
    }

    // Check if both buttons are held down for 5 seconds to reset the machine
    if (buttonStartState == HIGH && buttonWeightState == HIGH) {
        if (buttonHoldStartTime == 0) {
            buttonHoldStartTime = millis(); // Start the timer
        } else if (millis() - buttonHoldStartTime >= buttonHoldDuration) {
            resetMachine();
            return; // Go back to the start of the loop
        }
    } else {
        buttonHoldStartTime = 0; // Reset the timer if buttons are released
    }

    // Save the current button states for the next loop iteration
    lastButtonStartState = buttonStartState;
    lastButtonWeightState = buttonWeightState;
    delay(50); // Small delay to debounce the button
}
