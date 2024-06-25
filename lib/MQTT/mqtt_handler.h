#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>
#include "global_pins.h"

// Define pin and delay variables
extern const int pinMQTTMessages;
extern const int blinkDelay;

extern const char *mqtt_broker;
extern const char *topic;
extern const char *mqtt_username;
extern const char *mqtt_password;
extern const int mqtt_port;

void mqtt_setup();
void mqtt_loop();
void publishMessage(const char* m);
void handleReceivedMessage(const String &message);
void callback(char *topic, byte *payload, unsigned int length);

#endif // MQTT_HANDLER_H
