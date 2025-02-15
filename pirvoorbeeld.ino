#include <ESP8266WiFi.h>

#define PIR_PIN 5  // GPIO5 (D1) connected to PIR sensor OUT
volatile bool motionDetectedFlag = false;  // Flag to indicate motion

void ICACHE_RAM_ATTR motionDetected() {
    motionDetectedFlag = true;  // Set flag when motion is detected
    WiFi.forceSleepWake();  // Wake up WiFi (optional)
}

void setup() {
    Serial.begin(115200);

    // Configure PIR sensor as an interrupt pin
    pinMode(PIR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionDetected, RISING);

    Serial.println("Entering Light Sleep...");
    
    // Enable Light Sleep mode
    WiFi.forceSleepBegin();
    delay(100);
}

void loop() {
    if (motionDetectedFlag) {
        Serial.println("Motion detected, ESP8266 is awake!");

        // Simulate processing time (e.g., send data, trigger action)
        delay(5000);
        
        // Reset the motion flag
        motionDetectedFlag = false;

        Serial.println("Going back to Light Sleep...");
        WiFi.forceSleepBegin();
        delay(100);
    }
}
