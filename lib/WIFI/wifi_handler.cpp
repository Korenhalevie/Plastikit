#include "wifi_handler.h"
#include <WiFi.h>
#include "global_pins.h"
#include "lcd_handler.h"

void connectToWiFi(const char* ssid, const char* password, int ledPin, int delayTime) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(ledPin, HIGH);  // Turn the WiFi LED on
        delay(delayTime);
        digitalWrite(ledPin, LOW);   // Turn the WiFi LED off
        delay(delayTime);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    displayMessage("Status:","Wifi Connected");
    digitalWrite(ledPin, HIGH);  // Turn the WiFi LED on
}

void checkWiFiConnection(int wifiLedPin) {
    if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(wifiLedPin, LOW);   // Turn the WiFi LED off if disconnected
        displayMessage("Status:","WifiDisconnected");
        WiFi.reconnect();
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }
        digitalWrite(wifiLedPin, HIGH);  // Turn the WiFi LED on when reconnected
    }
}
