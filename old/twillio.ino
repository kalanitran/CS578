#include "SR04.h"
#include "dht.h"

// Define the pins for the ultrasonic sensor
#define TRIG_PIN 12
#define ECHO_PIN 11
#define DHT11_PIN 7
// Create an instance of the SR04 class
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

// DHT
dht DHT;

const int ledPin = 13;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  // Wait for serial port to connect
  while (!Serial) {
    delay(10);
  }

   // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);//initialize the ledPin as an output
  pinMode(2,INPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  // Perform a distance measurement
  long distance = sr04.Distance();
  
  // Print the measured distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Delay before taking the next measurement
  delay(1000); // Adjust delay as needed

  // put your main code here, to run repeatedly:
  int digitalVal = digitalRead(2);
  if(HIGH == digitalVal)
  {
    digitalWrite(ledPin,LOW);//turn the led off
  }
  else
  {
    digitalWrite(ledPin,HIGH);//turn the led on 
    Serial.println("Tilted");
  }

  // DHT

  // Debugging: Check the return value of the read function
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
}
