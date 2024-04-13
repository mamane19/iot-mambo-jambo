// Adafruit IO Publish
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

#include "config.h"
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Set up Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "hidden"
#define AIO_KEY         "hidden"

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("light-intensity");

AdafruitIO_Feed *toggleButton = io.feed("switch-light");

#define LED_RED 2

void toggleButtonControl(AdafruitIO_Data *data)
{
   String relay2status = data->toString();
   Serial.print("Relay 2 is ");  Serial.println(relay2status);
   digitalWrite(LED_RED, relay2status == "HIGH" ? HIGH : LOW);
}

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  toggleButton->onMessage(toggleButtonControl);
  toggleButton->get();

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  pinMode(LED_RED, OUTPUT);

}

void loop() {


  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  int lightResistance = analogRead(A0);

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(lightResistance);
  counter->save(lightResistance);

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}