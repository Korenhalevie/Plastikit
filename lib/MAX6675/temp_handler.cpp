#include "temp_handler.h"
#include "global_pins.h"
#include <max6675.h>
#include <Arduino.h>

// Declare the thermocouple object as a global variable
MAX6675 thermocouple(PIN_THERMO_CLK, PIN_THERMO_CS, PIN_THERMO_DO);

double readMax6675() {
    double tempC = thermocouple.readCelsius();
    if (isnan(tempC)) {
        Serial.println("Failed to read from MAX6675 sensor!");
        return NAN;
    } else {
        Serial.print("Temperature: ");
        Serial.print(tempC);
        Serial.println(" °C");
    }
    delay(150);
    return tempC;
}
