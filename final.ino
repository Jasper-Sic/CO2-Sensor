//Libraries:
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>
#include <MHZ19.h>
#include "ThingSpeak.h"
#include "secrets.h"
//Preferences:
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
#define NUMPIXELS 6
#define LED_PIN 5
#define BlinkSpeed 100
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
#define GlowTrigger 1000
#define BlinkTrigger 1500
#define Interval 16000
//definitions of Objects:
MHZ19 mhz19;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
WiFiClient client;
//Standard Variables that are usually not changed
bool d = true;
long lastTime = (-1) * Interval;
long lastTime2;
long cmpTime;
long lastTime3;
int i;
int keyIndex = 0;
int number;
int temperature;
void check() {

  if (millis() - lastTime >= Interval) {
    number = mhz19.getPPM(UART);
    temperature = mhz19.getTemperature();
    int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
    //    ThingSpeak.setField(1, number);
    //    ThingSpeak.setField(2, temperature);
    //    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    lastTime = millis();
    Serial.println(number);
  }



}
void Wifi() {
//  WiFi.mode(WIFI_STA);
//    if (WiFi.status() != WL_CONNECTED) {
//      Serial.print("Attempting to connect to SSID: ");
      WiFi.begin(ssid, pass);
//      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//      break;
//      }

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}




void setup() {
  
  const int rx_pin = 0;  //Serial rx pin no
  const int tx_pin = 2; //Serial tx pin no
  Serial.begin(115200);  // Initialize serial
  Wifi();
  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(false);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pixels.begin();

  for (int k = 0; k < 4; k++) {
    for (int h = 0; h < 6; h++) {
      pixels.setPixelColor(h, pixels.Color(0, 100, 0));
      pixels.show();
      delay(50);
    }
    delay(300);
    for (int h = 0; h < 6; h++) {
      pixels.setPixelColor(h, pixels.Color(0, 0, 0));
      pixels.show();
      delay(50);
    }
  }

}

void loop() {
    ArduinoOTA.handle();
  
  
  if (millis() - lastTime3 >= 60000) {

    pixels.setPixelColor(3, pixels.Color(0, 0, 90));
    pixels.show();
    lastTime3 = millis();
    delay(30);
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();


  }
    if (millis() - cmpTime > BlinkSpeed && number > BlinkTrigger) {
  
      if (d == true) {
        pixels.setPixelColor(i, pixels.Color(80, 0, 0));
  
      }
      else {
  
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  
      }
      i++;
      if (i >= 6) {
        i = 0;
        if (d == true) {
          d = false;
        }
        else {
          d = true;
        }
      }
      cmpTime = millis();
    }
    if (number > GlowTrigger && number < BlinkTrigger) {
      for (int x = 0; x < 6; x++) {
        pixels.setPixelColor(x, pixels.Color(80, 0, 0));
      }
    }
    if (number < GlowTrigger) {
      for (int x = 0; x < 6; x++) {
        pixels.setPixelColor(x, pixels.Color(0, 0, 0));
      }
    }
//    int colorval = 30 - 2 * map(number, 0, 5000, 0, 30);
//  if (colorval < 0) {
//    colorval = 0;
//  }
//  for (int x = 0; x < 6; x++) {
//    pixels.setPixelColor(x, pixels.Color(map(number, 390, 5000, 0, 255), colorval, 0));
//  }
  pixels.show();
  check();

  delay(10);
}
