# Documentation Images

This folder contains system architecture and hardware design diagrams for the LoRa-based wireless sensor network monitoring system.

## Images

### `point-to-point-topology.png`

System architecture for point-to-point LoRa communication.

This topology uses one ESP32 transmitter node and one ESP32 receiver node. The transmitter collects data from DHT11 and soil moisture sensors, stores the data on an SD card, and sends it to the receiver using LoRa.

### `dual-transmitter-topology.png`

System architecture for dual-transmitter to single-receiver LoRa communication.

This topology uses two ESP32 transmitter nodes and one ESP32 receiver node. Both transmitters send sensor data to the same receiver using LoRa.

### `transmitter-hardware-design.png`

Hardware block diagram of the transmitter node.

The transmitter node consists of:

- ESP32 microcontroller
- DHT11 sensor
- Soil moisture sensor
- LoRa RFM95 module
- SD card module
- Battery / power supply

### `receiver-hardware-design.png`

Hardware block diagram of the receiver node.

The receiver node receives sensor data from the transmitter through the LoRa RFM95 module. The received data is processed by the ESP32 microcontroller and then uploaded to the database through a WiFi connection.

The receiver node consists of:

- ESP32 microcontroller
- LoRa RFM95 module
- WiFi connection
- Database / local server
