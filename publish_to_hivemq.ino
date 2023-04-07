#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

/**** DHT11 sensor Settings *******/
#define DHTPIN 2   //Set DHT pin as GPIO2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/****** WiFi Connection Details *******/
const char* ssid = "xxxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxxxxxx";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "xxxxxxxxxxxxxxxxx";
const char* mqtt_username = "xxxxxxxxxxxxxxxxx";
const char* mqtt_password = "xxxxxxxxxxxxxxxxxx";
const int mqtt_port =8883;

/**** Secure WiFi Connectivity Initialisation *****/
WiFiClientSecure espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message published ["+String(topic)+"]: "+payload);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);
  setup_wifi();
  dht.begin();
  espClient.setInsecure();
//setInsecure() - this bypasses the checking of the server certificate completely, 
//so although the TLS connection is established we cannot guarantee that the server is actually HiveMQ.
  client.setServer(mqtt_server, mqtt_port);
  }

void loop() {
 if (!client.connected()) reconnect(); // check if client is connected
  client.loop();

//read DHT11 temperature and humidity reading
  delay(1000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.println(humidity);
  Serial.println(temperature);

  DynamicJsonDocument doc(1024);
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;

  char mqtt_message[128];
  serializeJson(doc, mqtt_message);
  publishMessage("test", mqtt_message, true);
    delay(5000);
}
