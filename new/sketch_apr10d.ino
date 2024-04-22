#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "twilio.hpp"

// Set these - but DON'T push them to GitHub!
static const char *ssid = "SDSU_Guest";
static const char *password = "";

// Values from Twilio (find them on the dashboard)
static const char *account_sid = "AC7ac2d338bf30b0711a9cabb6e2ab614e";
static const char *auth_token = "8b73ab19b8ae0ffcd19549b4e223b075";
// Phone number should start with "+<countrycode>"
static const char *from_number = "+18664293626";

// You choose!
// Phone number should start with "+<countrycode>"
static const char *to_number = "+16193765816";
static const char *message = "Sent from my ESP32";

Twilio *twilio;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi network ;");
  Serial.print(ssid);
  Serial.println("'...");
  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.println("Connecting...");
  //   delay(500);
  // }
  // Serial.println("Connected!");
  connectToWiFi();
  twilio = new Twilio(account_sid, auth_token);

  delay(1000);
  String response;
  bool success = twilio->send_message(to_number, from_number, message, response);
  if (success) {
    Serial.println("Sent message successfully!");
  } else {
    Serial.println(response);
  }
}
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to the WiFi network");
}
void loop() {
}

// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// #include <sstream>
// #include <curl.h>
// #include <ArduinoJson.h>

// // Wi-Fi credentials
// const char* ssid = "SDSU_Guest";
// const char* password = "";

// // Twilio API credentials
// const char* account_sid = "AC7ac2d338bf30b0711a9cabb6e2ab614e";
// const char* auth_token = "8b73ab19b8ae0ffcd19549b4e223b075";
// const char* twilio_number = "+18664293626";
// const char* to_number = "+16193765816";

// // Tilt switch pin
// const int tiltPin = D2;

// void setup() {
//   Serial.begin(115200);
//   pinMode(tiltPin, INPUT_PULLUP);  // Assume active low tilt switch
//   connectToWiFi();
// }

// void loop() {
//   static bool lastState = HIGH;

//   bool currentState = digitalRead(tiltPin);
//   if (currentState == LOW && lastState == HIGH) {
//     // Tilt detected
//     Serial.println("Tilt detected, sending SMS...");
//     // sendSMS("Tilt detected!");
//     // testHTTPS();
//     testSMS("test")
//   }
//   lastState = currentState;

//   delay(200);  // Debounce delay
// }

// void connectToWiFi() {
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi..");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("Connected to the WiFi network");
// }

// void testSMS(String message) {
//   CURL *curl;
//   curl_global_init(CURL_GLOBAL_ALL);
//   curl = curl_easy_init();

//   // Percent encode special characters
//   char *message_body_escaped = curl_easy_escape(
//           curl,
//           message.c_str(),
//           0
//   );


//   std::stringstream url;
//   std::string url_string;
//   url << "https://api.twilio.com/2010-04-01/Accounts/" << account_sid
//           << "/Messages";
//   url_string = url.str();


//   std::stringstream parameters;
//   std::string parameter_string;
//   parameters << "To=" << to_number << "&From=" << twilio_number
//           << "&Body=" << message;
//   parameter_string = parameters.str();


//   curl_easy_setopt(curl, CURLOPT_POST, 1);
//   curl_easy_setopt(curl, CURLOPT_URL, url_string.c_str());
//   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, parameter_string.c_str());
//   curl_easy_setopt(curl, CURLOPT_USERNAME, account_sid.c_str());
//   curl_easy_setopt(curl, CURLOPT_PASSWORD, auth_token.c_str());
//   if (!verbose) {
//           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _null_write);
//   } else {
//           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _stream_write);
//           curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_stream);
//   }


//   CURLcode res = curl_easy_perform(curl);
//   curl_free(message_body_escaped);
//   curl_easy_cleanup(curl);
//   long http_code = 0;
//   curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

//   // Check for curl errors and Twilio failure status codes.
//   if (res != CURLE_OK) {
//           response = curl_easy_strerror(res);
//           return false;
//   } else if (http_code != 200 && http_code != 201) {
//           response = response_stream.str();
//           return false;
//   } else {
//           response = response_stream.str();
//           return true;
//   }
// }

// void sendSMS(String message) {
//   if (WiFi.status() == WL_CONNECTED) {
    
//     WiFiClientSecure client;  // Use WiFiClientSecure for HTTPS
//     client.setInsecure();  // Add this for testing without SSL verification
//     HTTPClient http;

//     String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(account_sid) + "/Messages.json";
//     Serial.println("Connecting to URL: " + url);

//     http.begin(client, url);  // Begin using the secure client

//     // Basic Auth configuration
//     http.setAuthorization(account_sid, auth_token);

//     // Prepare POST request
//     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//     String postData = "To=" + String(to_number) + "&From=" + String(twilio_number) + "&Body=" + message;
//     Serial.println("Sending data: " + postData);

//     // Send POST request
//     int httpResponseCode = http.POST(postData);
//     Serial.print("HTTP Response Code: ");
//     Serial.println(httpResponseCode);

//     if (httpResponseCode > 0) {
//       String response = http.getString();
//       Serial.println("Response: " + response);
//     } else {
//       Serial.println("Failed to send POST request");
//     }

//     // Free resources
//     http.end();
//   } else {
//     Serial.println("Error in WiFi connection");
//   }
// }
// void testHTTPS() {
//   WiFiClientSecure client;
//   client.setInsecure();  // Add this for testing without SSL verification
//   HTTPClient http;
//   http.begin(client, "https://api.twilio.com");  // Twilio's base URL
//   int httpCode = http.GET();
//   Serial.println("HTTP Response code: " + String(httpCode));
//   if (httpCode > 0) {
//     String payload = http.getString();
//     Serial.println("Received: " + payload);
//   } else {
//     Serial.println("Failed to connect, error: " + String(http.errorToString(httpCode)));
//   }
//   http.end();
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////

// // #include "SR04.h"
// // #include "dht.h"

// // Define the pins for the ultrasonic sensor
// // #define TRIG_PIN D12
// // #define ECHO_PIN D11
// // #define DHT11_PIN D2
// // Create an instance of the SR04 class
// // SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
// // #define DHTPIN D2     // Digital pin connected to the DHT sensor
// // #define DHTTYPE DHT11   // DHT 11 or DHT22/AM2302, etc.
// // 
// // DHT dht(DHTPIN, DHTTYPE);
// // DHT
// // dht DHT;

// const int ledPin = 13;
// int digitalVal;

// void setup() {
//   // Initialize serial communication
//   Serial.begin(115200);
//   // Wait for serial port to connect
//   while (!Serial) {
//     delay(10);
//   }

//    // put your setup code here, to run once:
//   // pinMode(ledPin,OUTPUT);//initialize the ledPin as an output
//   // pinMode(D2,INPUT);
//   // digitalWrite(D2, HIGH);
//   // account_sid='AC7ac2d338bf30b0711a9cabb6e2ab614e';
//   // auth_token='8b73ab19b8ae0ffcd19549b4e223b075';
//   // to_number='+16193765816';
//   // from_number='+18664293626';
//   // message='TESTTEST';
// }

// void loop() {
//   // Perform a distance measurement
//   // long distance = sr04.Distance();
  
//   // Print the measured distance
//   // Serial.print("Distance: ");
//   // Serial.print(distance);
//   // Serial.println(" cm");

//   // Delay before taking the next measurement
//   delay(750); // Adjust delay as needed

//   // put your main code here, to run repeatedly:
//   // digitalVal = digitalRead(D2);
//   // if(HIGH == digitalVal)
//   // {
//   //   // digitalWrite(ledPin,LOW);//turn the led off
//   //   Serial.println("untilted");
//   // }
//   // else if (LOW == digitalVal) 
//   // {
//   //   // digitalWrite(ledPin,HIGH);//turn the led on 
//   //   Serial.println("Tilted");
    
    

//   // }


//   // Serial.println(digitalVal);
//   // Serial.println("tetstesdsda");

//   // DHT

//   // Debugging: Check the return value of the read function
//   int chk = DHT.read11(DHT11_PIN);

//   Serial.print("Read status: ");
//   switch (chk) {
//       case DHTLIB_OK:
//           Serial.println("OK");
//           break;
//       case DHTLIB_ERROR_CHECKSUM:
//           Serial.println("Checksum error");
//           break;
//       case DHTLIB_ERROR_TIMEOUT:
//           Serial.println("Timeout error");
//           break;
//       default:
//           Serial.println("Unknown error");
//           break;
//   }

//   Serial.print("Temperature = ");
//   Serial.println(DHT.temperature);
//   Serial.print("Humidity = ");
//   Serial.println(DHT.humidity);
// }


// void setup() {
//   // Initialize serial communication
//   Serial.begin(9600);
//   // Wait for serial port to connect
//   // while (!Serial) {
//   //   delay(10);
//   // }

//    // put your setup code here, to run once:
//   // pinMode(ledPin,OUTPUT);//initialize the ledPin as an output
//   // pinMode(2,INPUT);
//   // digitalWrite(2, HIGH);
// }

// void loop() {
//   // Perform a distance measurement
//   // long distance = sr04.Distance();
  
//     Serial.print("TEST");
//   // Print the measured distance
//   // Serial.print("Distance: ");
//   // Serial.print(distance);
//   // Serial.println(" cm");

//   // // Delay before taking the next measurement
//   // delay(1000); // Adjust delay as needed

//   // // put your main code here, to run repeatedly:
//   // int digitalVal = digitalRead(2);
//   // if(HIGH == digitalVal)
//   // {
//   //   digitalWrite(ledPin,LOW);//turn the led off
//   // }
//   // else
//   // {
//   //   digitalWrite(ledPin,HIGH);//turn the led on 
//   //   Serial.println("Tilted");
//   // }
// }

