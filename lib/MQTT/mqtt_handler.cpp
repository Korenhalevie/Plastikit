#include "mqtt_handler.h"
#include "global_pins.h"
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);
String receivedMessage = "";

void mqtt_setup() {
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public emqx mqtt broker connected");
            client.subscribe("esp32/plastikit/start");  // Subscribe to the topic
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
    digitalWrite(pinMQTTMessages, HIGH);  // Turn the LED on
    delay(blinkDelay);
    digitalWrite(pinMQTTMessages, LOW);  // Turn the LED off
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
  receivedMessage = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
    receivedMessage += (char) payload[i];
  }
  Serial.println();
  Serial.println("-----------------------");

  // Trim the received message to remove any leading or trailing whitespace
  receivedMessage.trim();

  // Blink the LED when a message is received
  if (String(topic) == "esp32/plastikit/start") {
    digitalWrite(pinMQTTMessages, HIGH);  // Turn the LED on
    delay(blinkDelay);
    digitalWrite(pinMQTTMessages, LOW);  // Turn the LED off
  }

  // Perform actions based on the received message
  if (String(topic) == "esp32/plastikit/start") {
    handleReceivedMessage(receivedMessage);
  }
}




void handleReceivedMessage(const String &message) {
  if (message.equals("12")) {
    digitalWrite(pinMQTTMessages, HIGH);  // Turn the LED on
    delay(blinkDelay);
    digitalWrite(pinMQTTMessages, LOW);  // Turn the LED off
    Serial.println("Performing action1");
  } else if (message.equals("13")) {
    digitalWrite(pinMQTTMessages, HIGH);  // Turn the LED on
    Serial.println("Performing action2");
  } else if (message.equals("14")) {
    digitalWrite(pinMQTTMessages, LOW);  // Turn the LED off
    Serial.println("Performing action3");
  } else if (message.equals("blink")) {
    for (int i = 0; i < 3; i++)
    {
    digitalWrite(pinMQTTMessages, HIGH);  // Turn the LED on
    delay(blinkDelay);
    digitalWrite(pinMQTTMessages, LOW);  // Turn the LED off
    delay(blinkDelay);
    }
    Serial.println("Performing blink");
  }
  else if (message.equals("Start_confirmation")) {
     for (int i = 0; i < 2; i++)
    {
    digitalWrite(pinStartConfirmation, HIGH);  // Turn the LED on
    delay(blinkDelay);
    digitalWrite(pinStartConfirmation, LOW);  // Turn the LED off
    delay(blinkDelay);
    }
    digitalWrite(pinStartConfirmation, HIGH);  // Turn the LED on
  } 
  else if (message.equals("abort")) {
    digitalWrite(pinStartConfirmation, LOW);  // Turn the LED on
    Serial.println("Performing action2");
  } else {
    Serial.println("Unknown action");
  }
}
