#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>

// ----------------------------
// Touch Screen pins
// ----------------------------

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// ----------------------------

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);

  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  
  tft.drawCentreString("Touch Screen to Start", 320/2, 100, 2);
}

void printTouchToSerial(TS_Point p) {
  Serial.print("Pressure: ");
  Serial.print(p.z);
  Serial.print(", x: ");
  Serial.print(p.x);
  Serial.print(", y: ");
  Serial.print(p.y);
  Serial.println();
}

void printTouchToDisplay(TS_Point p) {

  // Clear screen first
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  int x = 320 / 2; // center of display
  int y = 100;
  int fontSize = 2;

  String temp = "Pressure: " + String(p.z);
  tft.drawCentreString(temp, x, y, fontSize);

  y += 16;
  temp = "X: " + String(p.x);
  tft.drawCentreString(temp, x, y, fontSize);

  y += 16;
  temp = "Y: " + String(p.y);
  tft.drawCentreString(temp, x, y, fontSize);
}

void loop() {
  if(ts.tirqTouched() && ts.touched()) {
    TS_Point point = ts.getPoint();
    printTouchToSerial(point);
    printTouchToDisplay(point);
    //delay(100);
  }
}
