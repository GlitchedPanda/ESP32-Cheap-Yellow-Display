#include <TFT_eSPI.h>
#include <SD.h>

#define CS 5

TFT_eSPI tft = TFT_eSPI();

File rootDir;

void printFiles(File root)
{
  tft.println("MicroSD Card Contents: \n");
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
      break; // No (more) files

    tft.println(entry.name());
    entry.close();
  }
}

void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.fillScreen(TFT_BLACK);
  
  if (!SD.begin(CS))
  {
    tft.println("Could not initialize MicroSD card...");
    return;
  }

  if(SD.cardType() == CARD_NONE)
  {
    tft.println("No MicroSD Card Attached...");
    return;
  }

  rootDir = SD.open("/");
  printFiles(rootDir);
}

void loop() {
  // Nothing
}
