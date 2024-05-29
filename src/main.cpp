#include <Arduino.h>
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "global_pins.h"
#include "credentials_and_servers.h"
#include "init_functions.h"
#include "HX711.h"
#include "MeasureW.h"
#include "lcd_handler.h"

HX711 scale;

int buttonStartState = 0;
bool startingFlag = false;
bool heatingState = false;
int lastButtonState = 0;
unsigned long heatingStartTime = 0; // To store the time when heating started
const unsigned long heatingDuration = 5000; // 3.5 minutes in milliseconds (3.5 * 60 * 1000) == 210000
bool heatingMessageSent = false;
bool engineOn = false;

void setup() {
    Serial.begin(115200); 
    GPIO_Init();
    connectToWiFi(ssid, password, pinWifiLed, blinkDelay);
    mqtt_setup();

    // Initialize the scale
    scale.begin(pinHX711_dout, pinHX711_sck);
    scale.set_scale();
    scale.tare(); // Reset the scale to 0
    scale.set_scale(calibration_factor); // Set the calibration factor

    // Initialize the LCD
    initializeLCD();
}

void loop() {
    mqtt_loop();
    checkWiFiConnection(pinWifiLed);
    int buttonStartState = digitalRead(buttonStart);
    

    /********************/
    /* Heating Starting */
    /********************/

    // Check for button press
    if (startingFlag && buttonStartState == HIGH && lastButtonState == LOW && !engineOn) {
        // Toggle heating state
        heatingState = !heatingState;
        digitalWrite(pinHeating, heatingState ? HIGH : LOW);

        // Print the current state
        if (heatingState) {
            Serial.println("Heating ON");
            displayMessage("Status:","Heating ON");
            heatingStartTime = millis(); // Record the time when heating is turned on
        } else {
            heatingStartTime = 0;
            Serial.println("Heating OFF");
            displayMessage("Status:","Heating OFF");
            heatingMessageSent = false;
            digitalWrite(pinEngine, LOW);
            engineOn = false;
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
            engineOn = true;
            if (engineOn)
            {
            ("Status:","Transforming");
            }
        }
    }

    /*******************/
    /* Engine done check */
    /*******************/
    if (engineOn && buttonStartState == HIGH && lastButtonState == LOW) {
        Serial.println("done engine");
        // Turn off the engine
        digitalWrite(pinEngine, LOW);
        heatingStartTime = 0;
        Serial.println("Heating OFF");
        displayMessage("Status:","Waiting to scale");
        heatingMessageSent = false;
        digitalWrite(pinEngine, LOW);
        engineOn = false;
        /*******************/
        /* Scaling */
        /*******************/
        // Read the scale value
        float weight = scale.get_units(10); // Read average of 10 values
        float normalizedWeight = normalizeWeight(weight);
        Serial.print("Weight: ");
        Serial.print(normalizedWeight);
        Serial.println(" grams");

        // Publish the heating message with the scale reading
        String message = String(normalizedWeight) + "g";
        publishMessage(message.c_str());

        message = String(normalizedWeight) + " Grams";
        displayMessage(message.c_str(), "     were added!");

        // Reset engineOn to prevent repeated "done engine" messages
        engineOn = false;
    }

    // Save the current button state for the next loop iteration
    lastButtonState = buttonStartState;
    delay(50); // Small delay to debounce the button
}
