int i = 0;
String dataMessage;
String dataMessage1;

// LoRa
#include <SPI.h>
#include <LoRa.h>
#define ss 15
#define rst 14
#define dio0 2

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#if __has_include("config.h")
  #include "config.h"
#else
  #define WIFI_SSID "YOUR_WIFI_SSID"
  #define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

// Sensor Kelembapan Tanah
#define YLPIN 34

// Sensor DHT11
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Modul SD Card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// ---------------------------------------------------------------------
// SD Card helper functions
// ---------------------------------------------------------------------
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }

  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }

  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }

  file.close();
}

// ---------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(25200); // GMT+7

  // Memulai LoRa
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  // Memulai sensor
  pinMode(YLPIN, INPUT);
  Serial.println(F("TEST"));
  dht.begin();

  // Memulai SD Card
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD Card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  writeFile(SD, "/dataB.txt", "LoRa B data \r\n");
}

// ---------------------------------------------------------------------
void loop() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  formattedDate = timeClient.getFormattedDate();

  int split = formattedDate.indexOf("T");
  if (split < 0) {
    split = formattedDate.indexOf(" ");
  }

  if (split > 0) {
    dayStamp = formattedDate.substring(0, split);
    timeStamp = formattedDate.substring(split + 1, formattedDate.length() - 1);
  }

  // Membaca Sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  int soilMoisture = analogRead(YLPIN);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  // Counter data
  i++;

  // Input nilai sensor ke SD
  dataMessage = "Lora-B " + String(i) + " "
              + "Humidity: " + String(h) + " % "
              + "Temperature: " + String(t) + " °C "
              + "HeatIndex: " + String(hic) + " °C "
              + "SoilMoisture: " + String(soilMoisture) + "\n";

  dataMessage1 = "Lora-B " + String(i) + " "
               + "Humidity: " + String(h) + " % "
               + "Temperature: " + String(t) + " °C "
               + "HeatIndex: " + String(hic) + " °C "
               + "SoilMoisture: " + String(soilMoisture) + " "
               + formattedDate + "\n";

  Serial.print("Save data: ");
  Serial.println(dataMessage1);
  Serial.print("\n");

  appendFile(SD, "/dataB.txt", dataMessage1.c_str());

  // Mengirim ke LoRa
  Serial.print("Sending packet...");
  LoRa.beginPacket();
  LoRa.print(dataMessage);
  LoRa.endPacket();

  delay(250);
}
