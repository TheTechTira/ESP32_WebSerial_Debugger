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
