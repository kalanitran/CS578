#include <SPI.h>
#include <WiFiNINA.h>

// Replace with your Wi-Fi credentials
char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

// Replace with your Twilio API credentials
String accountSid = "AC7ac2d338bf30b0711a9cabb6e2ab614e";
String authToken = "[AuthToken]";

// Twilio API endpoint
String server = "api.twilio.com";
String resource = "/2010-04-01/Accounts/";
resource += accountSid;
resource += "/Messages.json";

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for Serial port to connect
  }

  connectToWiFi();
  sendTwilioRequest();
}

void loop() {
  // Nothing here
}

void connectToWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendTwilioRequest() {
  if (client.connect(server, 443)) {
    Serial.println("Connected to Twilio API");

    // Build the POST request
    String postData = "To=whatsapp:+16193765816&From=whatsapp:+14155238886&Body=CS%20TEST%20TEST%20TEST";
    String request = String("POST ") + resource + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Content-Type: application/x-www-form-urlencoded\r\n" +
                     "Content-Length: " + postData.length() + "\r\n" +
                     "Authorization: Basic " + base64Credentials() + "\r\n" +
                     "Connection: close\r\n\r\n" +
                     postData;
    client.println(request);

    // Read the response
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
  } else {
    Serial.println("Failed to connect to Twilio API");
  }
}

String base64Credentials() {
  String credentials = accountSid + ":" + authToken;
  int credLength = credentials.length();
  char credArray[credLength];
  credentials.toCharArray(credArray, credLength + 1);
  char base64Cred[68];
  base64_encode(base64Cred, (char *)credArray, credLength);
  return String(base64Cred);
}

// Base64 encoding function (from https://github.com/adamvr/arduino-base64)
void base64_encode(char *output, char *input, int length) {
  // ... (implementation omitted for brevity) ...
}