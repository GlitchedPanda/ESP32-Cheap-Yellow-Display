#include <WiFi.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

const char* SSID = "ssid";
const char* PASSWORD = "password";

WiFiServer server(80);
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  String connectingMsg = "Connecting to " + String(SSID) + "...";
  tft.drawString(connectingMsg, 10, 10, 2);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();

  tft.fillScreen(TFT_BLACK);
  
  String connectedMsg = "Connected to " + String(SSID);
  tft.drawString(connectedMsg, 10, 10, 2);

  String localIPMsg = "Local IP: " + WiFi.localIP().toString();
  tft.drawString(localIPMsg, 10, 30, 2);
}

void loop() {
  WiFiClient client = server.available();

  if(client) {
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";    

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if(client.available()) {
        char data = client.read();
        header += data;
        if (data == '\n') {
            if(currentLine.length() == 0) {
              // Headers to tell the client what to do
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // Website content
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
              client.println("<body><p>Hello, World!</p></body></html>");

              // It has to end with a blank line
              client.println();
              break;
            } else {
              currentLine = "";
            }
        } else if (data != '\r') { 
          currentLine += data; 
        }
      }
    }

    header = "";
    client.stop();
  }
}
