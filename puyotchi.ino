#include <WEMOS_Matrix_LED.h>
#include <ESP8266WiFi.h>

#define BUZZER_PIN 5
MLED mled(1); // LED intensity
WiFiServer server(80);

#include "globals.h"
#include "wifi_settings.h"
#include "wifi.h"
#include "melody.h"
#include "sequences.h"
#include "animation.h"
#include "events.h"
#include "web_page.h"
#include "web_server.h"


void setup() {
  Serial.begin(115200);

  resetMatrix();
  goIdle();

  setupWifi();
  setupWebServer();
}

void loop() {
  currentMillis = millis();
  updateActionEvents();
  updateRandomEvents();
  updateAnimation();
  updateMelody();

  updateWebQuery();

}



