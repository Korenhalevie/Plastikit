#include "mqtt_handler.h"
#include "credentials_and_servers.h"
#include "global_pins.h"
#include <WiFi.h>
#include "lcd_handler.h"

WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_setup() {
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "Plastikit_prototype";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public emqx mqtt broker connected");
            client.subscribe(topic);  // Subscribe to the topic
        } else {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void publishMessage(const char* m) {
    Serial.println("Publishing message...");
    if (client.publish(topic, m)) {
        Serial.println("Message published to topic");
        digitalWrite(PIN_MQTT_MESSAGES, HIGH);  // Turn the LED on
        delay(BLINK_DELAY);
        digitalWrite(PIN_MQTT_MESSAGES, LOW);  // Turn the LED off
        Serial.println("MQTT message published and LED blinked");
    } else {
        Serial.println("Failed to publish message");
    }
}

void mqtt_loop() {
    client.loop();
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    String receivedMessage = "";
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        receivedMessage += (char) payload[i];
    }
    Serial.println();
    Serial.println("-----------------------");

    // Trim the received message to remove any leading or trailing whitespace
    receivedMessage.trim();

    // Blink the LED when a message is received
    if (String(topic) == "plastikit/status") {
        digitalWrite(PIN_MQTT_MESSAGES, HIGH);  // Turn the LED on
        delay(BLINK_DELAY);
        digitalWrite(PIN_MQTT_MESSAGES, LOW);  // Turn the LED off
    }

    // Perform actions based on the received message
    handleReceivedMessage(receivedMessage);
}

void handleReceivedMessage(const String &message) {
    if (message.equals("test1")) {
        digitalWrite(PIN_MQTT_MESSAGES, HIGH);  // Turn the LED on
        delay(BLINK_DELAY);
        digitalWrite(PIN_MQTT_MESSAGES, LOW);  // Turn the LED off
        Serial.println("Performing action1");
    } else if (message.equals("test2")) {
        digitalWrite(PIN_MQTT_MESSAGES, HIGH);  // Turn the LED on
        Serial.println("Performing action2");
    } else if (message.equals("test3")) {
        digitalWrite(PIN_MQTT_MESSAGES, LOW);  // Turn the LED off
        Serial.println("Performing action3");
    } else if (message.equals("blink")) {
        for (int i = 0; i < 3; i++) {
            digitalWrite(PIN_MQTT_MESSAGES, HIGH);  // Turn the LED on
            delay(BLINK_DELAY);
            digitalWrite(PIN_MQTT_MESSAGES, LOW);  // Turn the LED off
            delay(BLINK_DELAY);
        }
        Serial.println("Performing blink");
    } else if (message.equals("Start_confirmation")) {
        startingFlag = true;
        for (int i = 0; i < 2; i++) {
            digitalWrite(PIN_START_CONFIRMATION, HIGH);  // Turn the LED on
            delay(BLINK_DELAY);
            digitalWrite(PIN_START_CONFIRMATION, LOW);  // Turn the LED off
            delay(BLINK_DELAY);

        }
        publishMessage("Start_confirmation_recieved");
        displayMessage("Click on blue", "button to start");
        digitalWrite(PIN_START_CONFIRMATION, HIGH);  // Turn the LED on
    } else if (message.equals("abort")) {
        startingFlag = false;
        digitalWrite(PIN_START_CONFIRMATION, LOW);  // Turn the LED off
        Serial.println("Performing action2");
    } else {
        Serial.println("Unknown action");
    }
}
