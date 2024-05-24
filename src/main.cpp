#include <Arduino.h>
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "global_pins.h"
#include "credentials_and_servers.h"

void setup() {
    Serial.begin(115200);
    pinMode(pinWifiLed, OUTPUT);  // Initialize the WiFi LED pin as an output
    pinMode(pinMQTTMessages, OUTPUT);  // Initialize the MQTT message LED pin as an output
    pinMode(pinStartConfirmation, OUTPUT);  // Initialize the MQTT message LED pin as an output

    // Connect to WiFi
    connectToWiFi(ssid, password, pinWifiLed, blinkDelay);

    // Initialize MQTT connection
    mqtt_setup();
    publishMessage("sending message");
}

void loop() {
    mqtt_loop();
    checkWiFiConnection(pinWifiLed);
    delay(500);
}
