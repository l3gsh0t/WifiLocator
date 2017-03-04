/*********************************************************************
Author: Matthew Waddell
Use: ESP SSID Fun
*********************************************************************/

//includes for 1306
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//includes for ESP8266
#include <ESP8266WiFi.h>

//Settings
#define OLED_RESET LED_BUILTIN //4
Adafruit_SSD1306 display(OLED_RESET);
const int led = 13;

//check if screen is set correctly
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const unsigned char PROGMEM logo [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00,
  0x00, 0x1F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 0xFF,
  0xFF, 0xE0, 0x07, 0xFF, 0xC0, 0x00, 0xFF, 0xF8, 0x0F, 0xFC, 0x00, 0x00, 0x0F, 0xFC, 0x1F, 0xF0,
  0x00, 0x00, 0x07, 0xFE, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x07, 0xF8, 0x00, 0x7F,
  0xFE, 0x00, 0xFF, 0xFF, 0xC0, 0x1F, 0x7C, 0x03, 0xFF, 0xFF, 0xF0, 0x0F, 0x38, 0x0F, 0xFF, 0xFF,
  0xFC, 0x07, 0x10, 0x1F, 0xFF, 0xFF, 0xFE, 0x02, 0x00, 0x7F, 0xFE, 0x1F, 0xFF, 0x80, 0x00, 0xFF,
  0xC0, 0x00, 0xFF, 0xC0, 0x01, 0xFF, 0x00, 0x00, 0x3F, 0xE0, 0x01, 0xFC, 0x00, 0x00, 0x0F, 0xE0,
  0x00, 0xF8, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x70, 0x0F, 0xFC, 0x03, 0x80, 0x00, 0x20, 0x3F, 0xFF,
  0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x03,
  0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x03, 0xFC, 0x0F, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x03, 0xE0, 0x00,
  0x00, 0x00, 0xE0, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00,
  0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("Number of available networks: ");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet + 1);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }

  // get top 3 network signals
  int firstpos = -1, secondpos = -1, thirdpos = -1, fourthpos = -1, fifthpos = -1, sixthpos = -1;

  if (numSsid > 0) {
    for (int i=0; i<numSsid; i++) {
      if (firstpos == -1 || WiFi.RSSI(firstpos) < WiFi.RSSI(i)) {
        firstpos = i;
      }
    }
  }
  
  if (numSsid > 1) {
    for (int i=0; i<numSsid; i++) {
      if (i != firstpos) {
        if (secondpos == -1 || WiFi.RSSI(secondpos) < WiFi.RSSI(i)) {
          secondpos = i;
        }
      }
    }
  }

  if (numSsid > 2) {
    for (int i=0; i<numSsid; i++) {
      if (i != firstpos && i != secondpos) {
        if (thirdpos == -1 || WiFi.RSSI(thirdpos) < WiFi.RSSI(i)) {
          thirdpos = i;
        }
      }
    }
  }

  if (numSsid > 3) {
    for (int i=0; i<numSsid; i++) {
      if (i != firstpos && i != secondpos && i != thirdpos) {
        if (fourthpos == -1 || WiFi.RSSI(fourthpos) < WiFi.RSSI(i)) {
          fourthpos = i;
        }
      }
    }
  }

  if (numSsid > 4) {
    for (int i=0; i<numSsid; i++) {
      if (i != firstpos && i != secondpos && i != thirdpos && i != fourthpos) {
        if (fifthpos == -1 || WiFi.RSSI(fifthpos) < WiFi.RSSI(i)) {
          fifthpos = i;
        }
      }
    }
  }

  if (numSsid > 5) {
    for (int i=0; i<numSsid; i++) {
      if (i != firstpos && i != secondpos && i != thirdpos && i != fourthpos && i != fifthpos) {
        if (sixthpos == -1 || WiFi.RSSI(sixthpos) < WiFi.RSSI(i)) {
          sixthpos = i;
        }
      }
    }
  }

  display.clearDisplay();
  
  display.setCursor(0,0);
  display.print("Networks: ");
  display.println(numSsid);

  if (numSsid > 0) {
    display.setCursor(0,8*2);
    display.print(WiFi.SSID(firstpos).substring(0,16));
    display.setCursor(105,8*2);
    display.println(WiFi.RSSI(firstpos));
  }

  if (numSsid > 1) {
    display.setCursor(0,8*3);
    display.print(WiFi.SSID(secondpos).substring(0,16));
    display.setCursor(105,8*3);
    display.println(WiFi.RSSI(secondpos));
  }

  if (numSsid > 2) {
    display.setCursor(0,8*4);
    display.print(WiFi.SSID(thirdpos).substring(0,16));
    display.setCursor(105,8*4);
    display.println(WiFi.RSSI(thirdpos));
  }

  if (numSsid > 3) {
    display.setCursor(0,8*5);
    display.print(WiFi.SSID(fourthpos).substring(0,16));
    display.setCursor(105,8*5);
    display.println(WiFi.RSSI(fourthpos));
  }

  if (numSsid > 4) {
    display.setCursor(0,8*6);
    display.print(WiFi.SSID(fifthpos).substring(0,16));
    display.setCursor(105,8*6);
    display.println(WiFi.RSSI(fifthpos));
  }

  if (numSsid > 5) {
    display.setCursor(0,8*7);
    display.print(WiFi.SSID(sixthpos).substring(0,16));
    display.setCursor(105,8*7);
    display.println(WiFi.RSSI(sixthpos));
  }
  
  display.display();
}

void setup(void)   {                
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);

  // Initialize screen
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(30,0);
  display.println("Wifi Locator");
  display.drawBitmap(40, 15,  logo, 48, 48, WHITE);
  display.display();
  delay(1000);
  
  Serial.println("");
  Serial.println("");

}

void loop() {
  listNetworks();
}

