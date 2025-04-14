#include <WiFi.h>

#define RXD2 3
#define TXD2 40

// WiFi credentials
const char* ssid = "Robot-Control";
const char* password = "12345678";

// Set up web server on port 80
WiFiServer server(80);
String header;

// Movement states
String currentState = "Stopped";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

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
            // Process HTTP request
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

            // Send HTML response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<style>html { font-family: Helvetica; text-align: center; }");
            client.println("button { padding:20px 40px; margin:10px; font-size:20px; }");
            client.println("</style></head><body>");
            client.println("<h1>Robot Control</h1>");
            client.println("<p>Status: <strong>" + currentState + "</strong></p>");
            client.println("<a href='/forward'><button>Forward</button></a><br>");
            client.println("<a href='/left'><button>Left</button></a>");
            client.println("<a href='/stop'><button>Stop</button></a>");
            client.println("<a href='/right'><button>Right</button></a><br>");
            client.println("<a href='/backward'><button>Backward</button></a>");
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
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println();
  }
}
