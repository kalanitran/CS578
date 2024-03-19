#define GPS_RX_PIN 4
#define GSM_TX_PIN 5
#define GSM_RX_PIN 6

TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_TX_PIN, GPS_RX_PIN);
SoftwareSerial gsmSerial(GSM_TX_PIN, GSM_RX_PIN);

void setup() {
  // put your setup code here, to run once:
  pinMode(alarmPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin(9600); // Initialize serial communication for debugging
  gpsSerial.begin(9600); // Initialize GPS serial communication
  gsmSerial.begin(9600); // Initialize GSM serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(sensorPin);
  
  // Adjust the threshold value according to your sensor's sensitivity
  int threshold = 500; 

  if (sensorValue > threshold) {
    // Triggered! Time to sound the alarm
    digitalWrite(alarmPin, HIGH);
    Serial.println("Tupperware alarm triggered!");
    delay(1000); // Add a delay to prevent rapid alarms
    
    // Check GPS location
    if (gps.location.isValid()) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      Serial.print("Current Location: ");
      Serial.print(latitude, 6);
      Serial.print(", ");
      Serial.println(longitude, 6);

      // Check if location is outside a certain radius
      float targetLatitude = /* latitude of target location */;
      float targetLongitude = /* longitude of target location */;
      float distance = TinyGPSPlus::distanceBetween(latitude, longitude, targetLatitude, targetLongitude);
      float radius = /* your desired radius */;
      if (distance > radius) {
        // Send SMS message
        sendSMS("+16199720456", "Tupperware is outside the radius!");

        // Additional actions if needed
      }
    } else {
      Serial.println("GPS location not available.");
    }
  } else {
    // No trigger, keep the alarm off
    digitalWrite(alarmPin, LOW);
  }

  // Update GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // GPS data received
    }
  }

  delay(100); // Adjust delay time according to your needs
}

void sendSMS(String phoneNumber, String message) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(100);
  gsmSerial.print("AT+CMGS=\""); // Set recipient phone number
  gsmSerial.print(phoneNumber);
  gsmSerial.println("\"");
  delay(100);
  gsmSerial.println(message); // Send message
  delay(100);
  gsmSerial.println((char)26); // End SMS message
  delay(1000);
}
