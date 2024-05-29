#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>

extern const int pinMQTTMessages;
extern const int blinkDelay;

void mqtt_setup();
void mqtt_loop();
void publishMessage(const char* m);
void handleReceivedMessage(const String &message);
void callback(char *topic, byte *payload, unsigned int length);

#endif // MQTT_HANDLER_H
