/*

This is for building an ESP32 debugger using an ESP32 device.
This code allows you to see the connected ESP's serial output on your local serial monitor or on a web interface.

Required hardware:
- ESP32

Hardware Connections:
- Connect GND: Connect the ground (GND) pin of the first ESP32 to the ground (GND) pin of the second ESP32.
- Connect TX to RX: Connect the TX pin of the first ESP32 to the RX pin of the second ESP32.

Libaries:
- https://github.com/ayushsharma82/WebSerial
- https://github.com/lacamera/ESPAsyncWebServer
- https://github.com/dvarrel/AsyncTCP

Tutorial Guidance:
- https://randomnerdtutorials.com/esp32-webserial-library/

Notes:
- Remember to the the serial baud rates to match
- Remember the browser is available at http://device_ip/webserial

*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#define LED 2 // Define the LED pin

AsyncWebServer server(80); // Create an AsyncWebServer object on port 80

const char* ssid = "Your_SSID";          // Your WiFi SSID
const char* password = "Your_Password";  // Your WiFi Password

// Function to blink the LED while connecting to WiFi
void blinkLEDWhileConnecting() {
  bool ledState = false;
  unsigned long previousMillis = 0;
  const long interval = 500; // Interval at which to blink (milliseconds)

  Serial.print("Connecting to WIFI");

  while (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED, ledState); // Toggle the LED state
      Serial.print(".");
    }
  }

  Serial.println("");

  digitalWrite(LED, HIGH); // Turn the LED on once connected
}

// Method to receive serial data from the web interface
void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  Serial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
  Serial.println(d);

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

  // Print web address once connected
  Serial.print("Web Address: ");
  Serial.print(WiFi.localIP());
  Serial.println("/webserial");

  // Initialize WebSerial and set callback function
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin(); // Start the server
}

static String serialBuffer = ""; // Buffer to accumulate serial data

void loop() {
  // Check for data from Serial2 and accumulate it in the buffer
  while (Serial2.available()) {
    char c = Serial2.read();
    Serial.print(c); // Print the character to Serial Monitor

    if (c == '\n') { // Check for newline character
      WebSerial.println(serialBuffer); // Send the accumulated buffer to WebSerial
      serialBuffer = ""; // Clear the buffer
    } else {
      serialBuffer += c; // Append character to buffer
    }
  }
}