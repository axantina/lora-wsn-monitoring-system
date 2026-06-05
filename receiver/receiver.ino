#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 14
#define dio0 2

#if __has_include("config.h")
  #include "config.h"
#else
  #define WIFI_SSID "YOUR_WIFI_SSID"
  #define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
  #define SERVER_URL "http://your-server/post-esp-data.php"
  #define API_KEY "YOUR_API_KEY"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* serverName = SERVER_URL;

String apiKeyValue = API_KEY;

void setup() {
  Serial.begin(115200);

  while (!Serial);

  Serial.println("LoRa Receiver");

  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (WiFi.status() == WL_CONNECTED) {
    if (packetSize) {
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      Serial.print("Received packet '");

      while (LoRa.available()) {
        String LoRaData = LoRa.readString();

        String alldata = String(LoRaData) + " " + "RSSI: " + String(LoRa.packetRssi()) + "\n";
        Serial.print(alldata);

        int ind1 = alldata.indexOf(' ');
        String tipe = alldata.substring(0, ind1);

        int ind2 = alldata.indexOf(' ', ind1 + 1);
        String counter = alldata.substring(ind1 + 1, ind2 + 1);

        int ind3 = alldata.indexOf(' ', ind2 + 1);
        String tiga = alldata.substring(ind2 + 1, ind3 + 1);

        int ind4 = alldata.indexOf(' ', ind3 + 1);
        String humidity = alldata.substring(ind3 + 1, ind4 + 1);

        int ind5 = alldata.indexOf(' ', ind4 + 1);
        String lima = alldata.substring(ind4 + 1, ind5 + 1);

        int ind6 = alldata.indexOf(' ', ind5 + 1);
        String enam = alldata.substring(ind5 + 1, ind6 + 1);

        int ind7 = alldata.indexOf(' ', ind6 + 1);
        String temperature = alldata.substring(ind6 + 1, ind7 + 1);

        int ind8 = alldata.indexOf(' ', ind7 + 1);
        String delapan = alldata.substring(ind7 + 1, ind8 + 1);

        int ind9 = alldata.indexOf(' ', ind8 + 1);
        String sembilan = alldata.substring(ind8 + 1, ind9 + 1);

        int ind10 = alldata.indexOf(' ', ind9 + 1);
        String heatindex = alldata.substring(ind9 + 1, ind10 + 1);

        int ind11 = alldata.indexOf(' ', ind10 + 1);
        String sebelas = alldata.substring(ind10 + 1, ind11 + 1);

        int ind12 = alldata.indexOf(' ', ind11 + 1);
        String duabelas = alldata.substring(ind11 + 1, ind12 + 1);

        int ind13 = alldata.indexOf(' ', ind12 + 1);
        String soilmoisture = alldata.substring(ind12 + 1, ind13 + 1);

        // NOTE:
        // Data yang dipakai:
        // tipe, counter, humidity, temperature, heatindex, soilmoisture, rssi
        String httpRequestData = "api_key=" + apiKeyValue
                               + "&tipe=" + tipe
                               + "&counter=" + counter
                               + "&humidity=" + humidity
                               + "&temperature=" + temperature
                               + "&heatindex=" + heatindex
                               + "&soilmoisture=" + soilmoisture
                               + "&rssi=" + String(LoRa.packetRssi());

        Serial.print("httpRequestData: ");
        Serial.println(httpRequestData);

        int httpResponseCode = http.POST(httpRequestData);

        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
        } else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
      }

      http.end();
    }
  } else {
    Serial.println("WiFi Disconnected");
  }
}
