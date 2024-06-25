#ifndef GLOBAL_PINS_H
#define GLOBAL_PINS_H

#define PIN_WIFI_LED 23
#define PIN_MQTT_MESSAGES 22
#define PIN_START_CONFIRMATION 21
#define PIN_HEATING 4
#define BUTTON_START 19
#define BUTTON_WEIGHT 13
#define PIN_ENGINE 18
#define PIN_HX711_DOUT 5
#define PIN_HX711_SCK 15
#define PIN_THERMO_DO 12
#define PIN_THERMO_CS 27
#define PIN_THERMO_CLK 14

// Define custom I2C pins for LCD
#define I2C_SDA 25
#define I2C_SCL 26

#define BLINK_DELAY 500

extern int buttonStartState;
extern bool startingFlag;
extern bool heatingState;
extern int lastButtonState;
extern unsigned long heatingStartTime;
extern const unsigned long heatingDuration;
extern bool heatingMessageSent;

#endif // GLOBAL_PINS_H
