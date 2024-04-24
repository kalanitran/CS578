// #include "SR04.h"
// #include "dht.h"

// #define TRIG_PIN D12
// #define ECHO_PIN D11
// #define DHT11_PIN D2

// SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
// dht DHT;

// bool isStolen = false; // Initialize the boolean variable for stolen
// bool isSpoiled = false; // Initialize the boolean variable for spoiled

// void setup() {
//   Serial.begin(9600);
//   pinMode(13, OUTPUT); // Initialize the LED pin as an output
// }

// void loop() {
//   long distance = sr04.Distance();

//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");

//   digitalWrite(13, HIGH); // Turn on LED
//   delay(2000);
//   digitalWrite(13, LOW); // Turn off LED

//   // Check for stolen condition
//   if (distance > 30 && !isStolen) {
//     isStolen = true;
//     Serial.println("STOLEN");
//     // You can add additional actions here if needed
//   }

//   // Check for spoiled condition (humidity or temperature)
//   if (DHT.humidity > 60.00) {
//     if (!isSpoiled) {
//       isSpoiled = true;
//       Serial.println("SPOILED");
//       // You can add additional actions here if needed
//     }
//   } else {
//     isSpoiled = false;
//   }

//   int chk = DHT.read11(DHT11_PIN);

//   Serial.print("Read status: ");
//   switch (chk) {
//     case DHTLIB_OK:
//       Serial.println("OK");
//       break;
//     case DHTLIB_ERROR_CHECKSUM:
//       Serial.println("Checksum error");
//       break;
//     case DHTLIB_ERROR_TIMEOUT:
//       Serial.println("Timeout error");
//       break;
//     default:
//       Serial.println("Unknown error");
//       break;
//   }

//   Serial.print("Temperature = ");
//   Serial.println(DHT.temperature);
//   Serial.print("Humidity = ");
//   Serial.println(DHT.humidity);

//   delay(2000);
// }
