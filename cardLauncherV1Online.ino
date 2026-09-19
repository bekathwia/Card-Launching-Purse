#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

// ==== Access Point settings ====
const char* apSSID     = "V1";   // Name of the WiFi network it creates
const char* apPassword = "honeydew";                 // Leave blank ("") for open network, or set a password (min 8 chars)

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

DNSServer dnsServer;
WebServer server(80);

#define AIN1   8
#define AIN2   7
#define SLP    9

const int buttonPin = 10;

// Simple HTML page with a "Launch Cards" button
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Card Launcher</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: sans-serif; text-align: center; margin-top: 80px; }
    button {
      font-size: 24px;
      padding: 20px 40px;
      border-radius: 12px;
      border: none;
      background-color: #2e7d32;
      color: white;
      cursor: pointer;
    }
    button:active { background-color: #1b5e20; }
  </style>
</head>
<body>
  <h1>Card Launcher</h1>
  <button onclick="fetch('/launch')">Launch Cards</button>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleLaunch() {
  launchAllCards();
  server.send(200, "text/plain", "Launched!");
}

// Catches any unknown path and redirects to root — this is what
// makes captive portal detection work across most devices.
void handleNotFound() {
  server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(9600);
      // configure pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(SLP, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // enable DRV8833
  digitalWrite(SLP, HIGH);

  // Start access point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(apSSID, apPassword);

  Serial.print("AP started. IP address: ");
  Serial.println(WiFi.softAPIP());

  // DNS server redirects ALL domain requests to our IP (captive portal trick)
  dnsServer.start(DNS_PORT, "*", apIP);

  // Web server routes
  server.on("/", handleRoot);
  server.on("/launch", handleLaunch);

  // Common captive portal detection URLs used by phones/OSes
  server.on("/generate_204", handleRoot);       // Android
  server.on("/gen_204", handleRoot);            // Android
  server.on("/hotspot-detect.html", handleRoot);// Apple
  server.on("/library/test/success.html", handleRoot); // Apple
  server.on("/ncsi.txt", handleRoot);           // Windows
  server.on("/connecttest.txt", handleRoot);    // Windows

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  int sensorVal = digitalRead(buttonPin);
  Serial.println(sensorVal);

  if (sensorVal == LOW) {
    launchCards();
  }else{
    //stop
    digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  }

  delay(10);
}

void launchCards() {
  Serial.println("Forward");
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
}

void launchAllCards() {
  Serial.println("Forward");
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  delay(5000);
  Serial.println("Stop");
    digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}