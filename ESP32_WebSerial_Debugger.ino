/*

This is our ESP32 debugger code - local or web

Required hardware:
- ESP32

Libaries:
- https://github.com/ayushsharma82/WebSerial

Notes:
- Remember to the the serial baud rates to match

*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define LED 2 // Define the LED pin

AsyncWebServer server(80); // Create an AsyncWebServer object on port 80

const char* ssid = "REPLACE_WITH_YOUR_SSID";          // Your WiFi SSID
const char* password = "REPLACE_WITH_YOUR_PASSWORD";  // Your WiFi Password

// Function to blink the LED while connecting to WiFi
void blinkLEDWhileConnecting() {
  bool ledState = false;
  unsigned long previousMillis = 0;
  const long interval = 500; // Interval at which to blink (milliseconds)

  while (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED, ledState); // Toggle the LED state
    }
  }
  digitalWrite(LED, HIGH); // Turn the LED on once connected
}

// Method to receive serial data from the web interface
void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);

  // Handle received serial data
  if (d == "ON") {
    digitalWrite(LED, HIGH);
  } else if (d == "OFF") {
    digitalWrite(LED, LOW);
  }
}

void setup() {
  Serial.begin(115200); // Initialize Serial Monitor
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Initialize Serial2 with RX on GPIO 16 and TX on GPIO 17

  delay(3000); // Wait for serial monitor

  pinMode(LED, OUTPUT); // Set the LED pin as output
  digitalWrite(LED, LOW); // Ensure LED is off initially

  WiFi.mode(WIFI_STA); // Set WiFi to station mode
  WiFi.begin(ssid, password); // Start WiFi with provided credentials

  // Blink the LED while connecting to WiFi
  blinkLEDWhileConnecting();

  // Print IP address once connected
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize WebSerial and set callback function
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin(); // Start the server
}

void loop() {
  // Check for data from Serial2 and forward it to WebSerial
  while (Serial2.available()) {
    char c = Serial2.read();
    WebSerial.print(c);
  }
}