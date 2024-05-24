#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

void connectToWiFi(const char* ssid, const char* password, int ledPin, int delayTime);
void checkWiFiConnection(int wifiLedPin);

#endif // WIFI_HANDLER_H
