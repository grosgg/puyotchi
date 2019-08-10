#include <WEMOS_Matrix_LED.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <RTClib.h>

#define BUZZER_PIN 5
MLED mled(1); // LED intensity
ESP8266WebServer server(80);
HTTPClient http;

#include "globals.h"
#include "wifi_settings.h"
#include "wifi.h"
#include "melody.h"
#include "sequences.h"
#include "animation.h"
#include "events.h"
#include "clock.h"
#include "alarm.h"
#include "status.h"
//#include "web_page.h"
#include "web_server.h"


void setup() {
  Serial.begin(115200);
  SPIFFS.begin(); // Starts flash memory communication

  resetMatrix();
  goIdle();

  setupWifi();
  setupWebServer();
  initializeDateTime();
}

void loop() {
  currentMillis = millis();
  updateClock();
  updateActionEvents();
  updateRandomEvents();
  updateAnimation();
  updateMelody();
  updateAlarm();
  updateFood();
  updateSleep();

  server.handleClient();
}



