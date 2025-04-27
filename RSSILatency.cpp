#include <WiFi.h>
#include "esp_wifi.h"

// Pins on UNO R3 to communicate with
#define RXD2 3
#define TXD2 40

// WiFi credentials
const char* ssid = "Robot-Control";
const char* password = "12345678";

// Set up web server on default port 80
WiFiServer server(80);
String header;
String currentState = "Stopped";

// Timing variables for connection timeout
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// Adjust transmit power based on RSSI
void adjustTxPower() {
  int8_t rssi = WiFi.RSSI();  

  Serial.print("RSSI: ");
  Serial.println(rssi);

  if (rssi > -50) {
    esp_wifi_set_max_tx_power(40);  // ~10 dBm
  } else if (rssi > -70) {
    esp_wifi_set_max_tx_power(68);  // ~17 dBm
  } else {
    esp_wifi_set_max_tx_power(79);  // 20 dBm (maximum)
  }
}


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("ESP32 Booting...");

  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    adjustTxPower();  // Apply power control on new client connection
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Handle movement commands
            if (header.indexOf("GET /forward") >= 0) {
              Serial2.println("F");
              currentState = "Forward";
            } else if (header.indexOf("GET /backward") >= 0) {
              Serial2.println("B");
              currentState = "Backward";
            } else if (header.indexOf("GET /left") >= 0) {
              Serial2.println("L");
              currentState = "Left";
            } else if (header.indexOf("GET /right") >= 0) {
              Serial2.println("R");
              currentState = "Right";
            } else if (header.indexOf("GET /stop") >= 0) {
              Serial2.println("S");
              currentState = "Stopped";
            }

            // Send HTML + JS response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<style>");
            client.println("html { font-family: Helvetica; text-align: center; }");
            client.println("button { padding: 20px 40px; margin: 10px; font-size: 20px; }");
            client.println("</style>");
            client.println("<script>");
            client.println("function sendCommand(cmd) {");
            client.println("  const start = performance.now();");
            client.println("  fetch('/' + cmd)");
            client.println("    .then(response => response.text())");
            client.println("    .then(data => {");
            client.println("      const latency = performance.now() - start;");
            client.println("      console.log(`Latency for ${cmd}: ` + latency.toFixed(2) + ' ms');");
            client.println("      document.getElementById('status').innerText = cmd.charAt(0).toUpperCase() + cmd.slice(1);");
            client.println("      document.getElementById('latency').innerText = 'Last command latency: ' + latency.toFixed(2) + ' ms';");
            client.println("    });");
            client.println("}");
            client.println("</script>");
            client.println("</head><body>");
            client.println("<h1>Robot Control</h1>");
            client.println("<p>Status: <strong id='status'>" + currentState + "</strong></p>");
            client.println("<p id='latency'>Last command latency: N/A</p>");
            client.println("<button onclick=\"sendCommand('forward')\">Forward</button><br>");
            client.println("<button onclick=\"sendCommand('left')\">Left</button>");
            client.println("<button onclick=\"sendCommand('stop')\">Stop</button>");
            client.println("<button onclick=\"sendCommand('right')\">Right</button><br>");
            client.println("<button onclick=\"sendCommand('backward')\">Backward</button>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // Cleanup on disconnect
    header = "";
    client.stop();
    Serial.println("Client disconnected.\n");
  }
}
