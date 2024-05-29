#ifndef GLOBAL_PINS_H
#define GLOBAL_PINS_H

#define pinWifiLed  23  // LED connected to digital pin 23
#define pinMQTTMessages 22  // LED connected to digital pin 22
#define pinStartConfirmation 21  // LED connected to digital pin 21
#define pinHeating 4  // LED connected to digital pin 21
#define buttonStart 19  // LED connected to digital pin 21
#define pinEngine 18  // LED connected to digital pin 21
#define pinHX711_dout 5 // mcu > HX711 dout pin
#define pinHX711_sck 15 // mcu > HX711 sck pin

// Define custom I2C pins for LCD
#define I2C_SDA 25
#define I2C_SCL 26


#define blinkDelay 500  // Delay time in milliseconds

extern int buttonStartState;
extern bool startingFlag;
extern bool heatingState;
extern int lastButtonState;
extern unsigned long heatingStartTime;
extern const unsigned long heatingDuration;
extern bool heatingMessageSent;

#endif // GLOBAL_PINS_H
