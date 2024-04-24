/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-pushover-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// ##################################################
// Pushover stuff
#include <string>
#include <iostream>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "ajajaj";
const char* password = "00000000";

const char* apiToken = "axu3ut9bw2586jz76b99qazse2yveb";
const char* userToken = "utg8mricpxjcr8fjo3dwe442vd6ugi";

//Pushover API endpoint
const char* pushoverApiEndpoint = "https://api.pushover.net/1/messages.json";

//Pushover root certificate (valid from 11/10/2006 to 15/01/2038)
const char *PUSHOVER_ROOT_CA = "-----BEGIN CERTIFICATE-----\n"
                  "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n"
                  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
                  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
                  "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
                  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
                  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
                  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
                  "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n"
                  "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n"
                  "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n"
                  "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n"
                  "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n"
                  "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n"
                  "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n"
                  "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n"
                  "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n"
                  "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n"
                  "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n"
                  "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
                  "MrY=\n"
                  "-----END CERTIFICATE-----\n";

// Create a list of certificates with the server certificate
X509List cert(PUSHOVER_ROOT_CA);

// ######################################################################
// dht, usr, wemos stuff
#include "SR04.h"
#include "dht.h"

#define TRIG_PIN D12
#define ECHO_PIN D11
#define DHT11_PIN D2

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
dht DHT;

bool isStolen = false; // Initialize the boolean variable for stolen
bool isSpoiled = false; // Initialize the boolean variable for spoiled

// void sendMessage(string message){

// }
// void sendMessage(String);


void setup() {



  // WEMOS STUFF
  Serial.begin(115200);
  pinMode(13, OUTPUT); // Initialize the LED pin as an output

  // ###################################################################
  //PUSHOVER STUFF
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));


  //Make HTTPS POST request to send notification
  if (WiFi.status() == WL_CONNECTED) {
    sendMessage("CONNECTED");
  }
}

void sendMessage(String message){
  // sendMessage("connected");
  // Create a JSON object with notification details
  // Check the API parameters: https://pushover.net/api
  StaticJsonDocument<512> notification;
  notification["token"] = apiToken;
  notification["user"] = userToken;
  notification["message"] = message;
  notification["title"] = "ESP8266 Notification";
  notification["url"] = "";
  notification["url_title"] = "";
  notification["html"] = "";
  notification["priority"] = "";
  notification["sound"] = "cosmic";
  notification["timestamp"] = "";

  // Serialize the JSON object to a string
  String jsonStringNotification;
  serializeJson(notification, jsonStringNotification);

  // Create a WiFiClientSecure object
  WiFiClientSecure client;
  // Set the certificate
  client.setTrustAnchors(&cert);

  // Create an HTTPClient object
  HTTPClient http;

  // Specify the target URL
  http.begin(client, pushoverApiEndpoint);

  // Add headers
  http.addHeader("Content-Type", "application/json");

  // Send the POST request with the JSON data
  int httpResponseCode = http.POST(jsonStringNotification);

  // Check the response
  if (httpResponseCode > 0) {
    Serial.printf("HTTP response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println("Response:");
    Serial.println(response);
  } else {
    Serial.printf("HTTP response code: %d\n", httpResponseCode);
  }

  // Close the connection
  http.end();
}

void loop() {
  long distance = sr04.Distance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  digitalWrite(13, HIGH); // Turn on LED
  delay(2000);
  digitalWrite(13, LOW); // Turn off LED

  // Check for stolen condition
  if (distance > 30 && !isStolen) {
    isStolen = true;
    Serial.println("STOLEN");
    sendMessage("STOLEN");
    // You can add additional actions here if needed
  }

  // Check for spoiled condition (humidity or temperature)
  if (DHT.humidity > 60.00) {
    if (!isSpoiled) {
      isSpoiled = true;
      Serial.println("SPOILED");
      sendMessage("SPOILED");
      // You can add additional actions here if needed
    }
  } else {
    isSpoiled = false;
  }

  int chk = DHT.read11(DHT11_PIN);

  Serial.print("Read status: ");
  switch (chk) {
    case DHTLIB_OK:
      Serial.println("OK");
    break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
    break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Timeout error");
    break;
    default:
      Serial.println("Unknown error");
    break;
  }

  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);

  delay(2000);
}

