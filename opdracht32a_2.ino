#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char* ssid = "xxxx";
const char* password = "xxxx";

const char* mqtt_server = "xxxxxxxxxx.s2.eu.hivemq.cloud";
const char* mqtt_username = "xxxxx";
const char* mqtt_password = "xxxxxx";

// Initialize the WiFi client and MQTT client
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}


// Define the MQTT topic to publish to
const char* mqtt_topic = "xxxxxx";

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
   setup_wifi();
espClient.setInsecure();
  // Set MQTT server and callback function
  mqttClient.setServer(mqtt_server, 8883);
}

void loop() {
  // Connect to MQTT server if not already connected
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  // Publish a number to the MQTT topic
  publishNumberToMQTT(12.5); // Change 12.5 to the number you want to send

  // Wait for some time before publishing again
  delay(5000); // Publish every 5 seconds (adjust as needed)
}



void reconnectMQTT() {
  // Loop until we're reconnected to MQTT
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("ArduinoClient", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void publishNumberToMQTT(float number) {
  
  // Convert the number to a string
  char payload[10];
    sprintf(payload, "%.2f", number);
   mqttClient.publish(mqtt_topic, payload);
}
