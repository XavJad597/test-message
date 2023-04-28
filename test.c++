#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>

// Update these with values suitable for your network.
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* mqtt_server = "your_MQTT_broker_IP";

WiFiClient espClient;
PubSubClient client(espClient);
char message[100];

void setup() {
  Serial.begin(115200);

  // Initialize LoRa radio
  LoRa.setPins(4, 15, 16);
  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa init failed");
    while (1);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up MQTT client
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  // Send LoRa message
  LoRa.beginPacket();
  LoRa.print("Hello, MQTT!");
  LoRa.endPacket();

  // Publish MQTT message
  sprintf(message, "Hello, MQTT!");
  client.publish("testtopic", message);

  delay(5000);
}
