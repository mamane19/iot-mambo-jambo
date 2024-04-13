#include "ThingSpeak.h"
#include <DHT.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>

char ssid[] = "hidden";
char pass[] = "hidden";

WiFiClient client;

unsigned long channelNumber = 10000000;
const char* writeAPIkey = "hidden";


LiquidCrystal lcd(D1,D2,D3,D5,D6,D7);
static const uint8_t pin = 15;

#define DHTPIN pin
#define DHTTYPE DHT11

DHT DHT(DHTPIN, DHTTYPE);

int sensorPin = 0;

void setup() {
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

  pinMode(DHTPIN, INPUT);

  DHT.begin();

  lcd.begin(16,2);

  Serial.begin(115200);
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("Connected.");
  }

  int moistureVal = analogRead(sensorPin);

  ThingSpeak.setField(1, DHT.readTemperature());
  ThingSpeak.setField(2, moistureVal);

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(DHT.readTemperature());

  lcd.setCursor(8,0);
  lcd.print("H:");
  lcd.print(DHT.readHumidity());

  lcd.setCursor(0,1);
  lcd.print("Moisture: ");
  lcd.print(moistureVal);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(channelNumber, writeAPIkey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(5000);
}