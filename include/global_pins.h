#ifndef GLOBAL_PINS_H
#define GLOBAL_PINS_H

#define pinWifiLed  23  // LED connected to digital pin 23
#define pinMQTTMessages 22  // LED connected to digital pin 22
#define pinStartConfirmation 21  // LED connected to digital pin 21
#define pinHeating 4  // LED connected to digital pin 21
#define buttonStart 19  // LED connected to digital pin 21
#define pinEngine 18  // LED connected to digital pin 21



#define blinkDelay 500  // Delay time in milliseconds


static int buttonStartState = 0;
extern bool startingFlag;

#endif