char ssid[] = "xxxxxxxxxxx";
char pass[] = "xxxxxxxxxx";
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define channelID xxxxxxxxxxx
const char mqttUserName[] = "xxxxxxxxxxxxx"; 
const char clientID[] = "xxxxxxxxxxxxx";
const char mqttPass[] = "xxxxxxxxxxxxxx";
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#define mqttPort 1883
WiFiClient client;

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}

void connectWifi()
{
  Serial.print( "Connecting to Wi-Fi..." );
  // Loop until WiFi connection is successful
    while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
    WiFi.begin( ssid, pass );
    delay( 1000 );
    Serial.print( WiFi.status() ); 
  }
  Serial.println( "Connected to Wi-Fi." );
}

void mqttConnect() {
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( 1000 );
    }
  }
}
void setup() {
  Serial.begin( 115200 );
  dht.begin();
   delay(3000);
  // Connect to Wi-Fi network.
  connectWifi();
  // Configure the MQTT client
  mqttClient.setServer( server, mqttPort ); 
  mqttClient.setBufferSize( 2048 );
  }

void loop() {
  // Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  }
  
  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
  }
  float t = dht.readTemperature(); // Read temperature from DHT sensor.
float h = dht.readHumidity(); // Read humidity from DHT sensor.
String msg = String("field1=") + String(t, 2) + "&field2=" + String(h, 2);
Serial.println(msg);
    // Call the loop to maintain connection to the server.
  mqttClient.loop(); 
  mqttPublish( channelID, msg);
  delay(5000);
 }
