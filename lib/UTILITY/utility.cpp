#include "utility.h"
#include "global_pins.h"
#include "lcd_handler.h"
#include "credentials_and_servers.h"
#include "temp_handler.h"
#include "MeasureW.h"
#include <Arduino.h>
#include "HX711.h"
#include "mqtt_handler.h"

extern bool heatingState;
extern bool engineOn;
extern bool engineState;
extern bool transformationDone;
extern bool heatingMessageSent;
extern double currentTemp;
extern HX711 scale;  // Ensure scale is correctly declared as an external variable
extern double heatingStartEngineTemp;  // Ensure heatingStartEngineTemp is declared as an external variable

void resetMachine() {
    heatingState = false;
    heatingMessageSent = false;
    engineOn = false;
    transformationDone = false;
    engineState = false;

    digitalWrite(PIN_HEATING, LOW);
    digitalWrite(PIN_ENGINE, LOW);

    displayMessage("Please start the", "proccess on app");
    publishMessage("Init machine");
    publishMessage("Init machine");
    publishMessage("Init machine");
}

void handleHeating() {
    heatingState = !heatingState;
    digitalWrite(PIN_HEATING, heatingState ? HIGH : LOW);

    if (heatingState) {
        Serial.println("Heating ON");
        displayMessage(("Heating at " + String((int)heatingStartEngineTemp) + "C the").c_str(), "engine starts");
    } else {
        Serial.println("Heating OFF");
        displayMessage("Process is off", "scale or resume");
        heatingMessageSent = false;
        digitalWrite(PIN_ENGINE, LOW);
        engineOn = false;
    }
}

void checkTemperature() {
    if (heatingState) {
        currentTemp = readMax6675();
        publishMessage(("temp:" + String(currentTemp)).c_str());
    }
    
    if (heatingState && currentTemp >= heatingStartEngineTemp && !heatingMessageSent) {
        publishMessage("engine_starting");
        publishMessage("engine_starting");
        publishMessage("engine_starting");
        heatingMessageSent = true;

        digitalWrite(PIN_ENGINE, HIGH);
        engineOn = true;
        engineState = true;
        displayMessage("To stop, long", "click on blue");
    }
}

void handleEngineStop() {
    publishMessage("scaling");
    Serial.println("Engine currently running: Stopping engine on button press");
    heatingState = false;
    displayMessage("Press the yellow", "button to scale");
    digitalWrite(PIN_ENGINE, LOW);
    digitalWrite(PIN_HEATING, heatingState ? HIGH : LOW);
    transformationDone = true;
    heatingMessageSent = false;
    engineOn = false;
    engineState = false;
}

void handleScaling() {
    float weight = scale.get_units(10);
    float normalizedWeight = normalizeWeight(weight) - 10;
    Serial.print("Weight: ");
    Serial.print(normalizedWeight);
    Serial.println(" grams");

    String message = String(normalizedWeight) + "gr";
    publishMessage(message.c_str());

    message = String(normalizedWeight) + " Grams";
    displayMessage(message.c_str(), "were added!");
    delay(3000);
    displayMessage("Thank you", "  for recycling!");
    delay(2000);
    displayMessage("let's recycle", "again!");
    transformationDone = false;

    publishMessage("abort");
    resetMachine();
}
