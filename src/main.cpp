#include <Arduino.h>
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "global_pins.h"
#include "credentials_and_servers.h"
#include "init_functions.h"
#include "HX711.h"
#include "MeasureW.h"
#include "lcd_handler.h"
#include "temp_handler.h"
#include "utility.h"

HX711 scale;

int buttonStartState = LOW;
int lastButtonStartState = LOW;
int buttonWeightState = LOW;
int lastButtonWeightState = LOW;
bool startingFlag = false;
bool heatingState = false;
double heatingStartEngineTemp = 50;
bool heatingMessageSent = false;
bool engineOn = false;
bool transformationDone = false;
bool engineState = false; // Flag to track engine state
unsigned long buttonHoldStartTime = 0; // To store the time when buttons are held
const unsigned long buttonHoldDuration = 5000; // 5 seconds in milliseconds
double currentTemp;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
    Serial.begin(115200);

    // Initialize the LCD
    GPIO_Init();
    initializeLCD();
    connectToWiFi(ssid, password, PIN_WIFI_LED, BLINK_DELAY);
    mqtt_setup();

    // Initialize the scale
    scale.begin(PIN_HX711_DOUT, PIN_HX711_SCK);
    scale.set_scale(calibration_factor); // Set the calibration factor
    scale.tare(); // Reset the scale to 0

    // Initial reset
    resetMachine();
}

void loop() {
    mqtt_loop();
    checkWiFiConnection(PIN_WIFI_LED);
    int readingStart = digitalRead(BUTTON_START);
    int readingWeight = digitalRead(BUTTON_WEIGHT);

    // Debounce the button
    if (readingStart != lastButtonStartState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (readingStart != buttonStartState) {
            buttonStartState = readingStart;

            if (startingFlag && buttonStartState == HIGH && !engineOn && !engineState) {
                handleHeating();
            }

            if (engineOn && buttonStartState == HIGH) {
                handleEngineStop();
            }
        }
    }

    lastButtonStartState = readingStart;

    checkTemperature();

    if (transformationDone && readingWeight == HIGH && lastButtonWeightState == LOW) {
        handleScaling();
        lastButtonWeightState = readingWeight;
    }

    // Check if both buttons are held down for 5 seconds to reset the machine
    if (readingStart == HIGH && readingWeight == HIGH) {
        if (buttonHoldStartTime == 0) {
            buttonHoldStartTime = millis(); // Start the timer
        } else if (millis() - buttonHoldStartTime >= buttonHoldDuration) {
            resetMachine();
            return; // Go back to the start of the loop
        }
    } else {
        buttonHoldStartTime = 0; // Reset the timer if buttons are released
    }

    lastButtonWeightState = readingWeight;
    delay(50); // Small delay to debounce the button
}
