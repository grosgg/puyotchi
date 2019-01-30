#include <WEMOS_Matrix_LED.h>
#include <ESP8266WiFi.h>

#include "wifi_settings.h"
#include "wifi.h"
#include "sequences.h"
#include "web_page.h"

#define BUZZER_PIN 5

WiFiServer server(80);

// Global variables
bool mute = false;
bool automode = false;

uint8_t food = 10;
uint8_t happiness = 10;

byte sequence[4][8]; // Sequences are 4 frames
unsigned int framerate = 1000; // Default time for each frame
unsigned long animationMillis = 0; // Animation timer
uint8_t currentFrame = 0; // Current frame in sequence

unsigned int noteDuration = 120;
unsigned long melodyMillis = 0; // Melody length timer

unsigned int rythmn = 125; // noteDuration + pause before next note
unsigned long noteMillis = 0; // Rythmn timer
uint8_t currentNote[] = { 0, 0 }; // note being played now in frame / sequence

unsigned int eventInterval = 0; // Time before next event. Initialized in updateRandomEvents()
unsigned int eventDuration = 7000; // Arbitrary length of events
unsigned long eventMillis = 0; // Event timer

unsigned long currentMillis = millis(); // Clock
bool isIdle = true; // Is puyotchi idle
bool isActionEvent = false; // Is there a manually triggered action event ongoing
bool isRandomEvent = false; // Is there a randomly triggered action event ongoing

MLED mled(1); // LED intensity

void setup() {
  Serial.begin(115200);

  resetMatrix();
  goIdle();

  setupWifi();

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  currentMillis = millis();
  updateActionEvents();
  updateRandomEvents();
  updateAnimation();
  updateMelody();

  updateWebQuery();

  // Serial.println("isIdle ");
  // Serial.println(isIdle);
  // Serial.println("isActionEvent ");
  // Serial.println(isActionEvent);
  // Serial.println("eventInterval ");
  // Serial.println(eventInterval);

}

void updateWebQuery() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1); // Stopping the clock for only 1ms should not be visible
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  if (req.indexOf("excited") != -1) { setEvent("excited", true, 5000); }
  else if (req.indexOf("roll") != -1) { setEvent("roll", true, 5000); }
  else if (req.indexOf("look") != -1) { setEvent("look", true, 5000); }
  else if (req.indexOf("unhappy") != -1) { setEvent("unhappy", true, 5000); }
  else if (req.indexOf("happy") != -1) { setEvent("happy", true, 5000); }
  else if (req.indexOf("ok") != -1) { setEvent("ok", true, 5000); }
  else if (req.indexOf("no") != -1) { setEvent("no", true, 5000); }
  else if (req.indexOf("rain") != -1) { setEvent("rain", true, 5000); }

  else if (req.indexOf("automode/toggle") != -1) { automode = !automode; }
  else if (req.indexOf("mute/toggle") != -1) { mute = !mute; }
  client.flush();

  // Get web page template
  String webPage = getPage();

  // Fill in setting values
  webPage.replace("id='mute'", mute ? "id='mute' checked" : "id='mute'");
  webPage.replace("id='automode'", automode ? "id='automode' checked" : "id='automode'");

  // Fill in debug values
  // webPage.replace("isIdleValue", isIdle ? "true" : "false");
  // webPage.replace("isActionEventValue", isActionEvent ? "true" : "false");
  // webPage.replace("isRandomEventValue", isRandomEvent ? "true" : "false");
  // webPage.replace("eventIntervalValue", String(eventInterval));
  // webPage.replace("eventDurationValue", String(eventDuration));

  // Send the response to the client
  client.print(webPage); 
}

void updateActionEvents() {
  // No action event ongoing, nothing to do
  if (isRandomEvent || isIdle) { return; }

  if (isActionEvent && (currentMillis - eventMillis >= eventDuration)) {
    Serial.println("Action goIdle");
    setRandomEventInterval();
    goIdle();
  }
}

void updateRandomEvents() {
  // Action event ongoing, nothing to do
  if (isActionEvent) { return; }

  // Initialization
  if ( eventInterval == 0) { setRandomEventInterval(); }

  // Serial.println("eventInterval ");
  // Serial.println(eventInterval);
  if (currentMillis - eventMillis >= eventInterval) {
    Serial.println("Trigger random event");

    // Randomly choosing an event
    uint8_t eventType = random(2);
    switch (eventType) {
      case 0:
        setEvent("excited", false, 8000);
        break;
      case 1:
        setEvent("look", false, 9000);
        break;
    }
  }

  // When event is done, return to idle mode
  if (isRandomEvent && (currentMillis - eventMillis >= eventDuration)) {
    Serial.println("Random goIdle");
    // Set interval before next event
    setRandomEventInterval();
    goIdle();
  }
}

// Display next frame on framerate
void updateAnimation() {
  if (currentMillis - animationMillis >= framerate) {
    Serial.println("New frame");
    setBuffer(sequence[currentFrame]);
    if (currentFrame >= 3) {
      currentFrame = 0;
    } else {
      currentFrame++;
    }
    mled.display();
    animationMillis = currentMillis;
  }
}

// Play next note on rythmn
void updateMelody() {
  if (isActionEvent && currentMillis - noteMillis >= rythmn) {
    tone(BUZZER_PIN, sequence[currentNote[0]][currentNote[1]], noteDuration);
    if (currentNote[0] == 4 && currentNote[1] == 7) {
      noTone(BUZZER_PIN);
    } else if (currentNote[1] >= 7) {
      currentNote[0]++;
      currentNote[1] = 0;
    } else {
      currentNote[1]++;
    }
    noteMillis = currentMillis;
  }
}

// Display on matrix
void setBuffer(byte hexArray[]) {
  for(uint8_t i = 0; i < 8; i++) {
    mled.disBuffer[i] = hexArray[i];
  }
}

// Set sequence to start displaying
void setSequence(byte frameArray[][8], int framerateInt) {
  for(uint8_t x = 0; x < 4; x++) {
    for(uint8_t y = 0; y < 8; y++) {
      sequence[x][y] = frameArray[x][y];
    }
  }
  framerate = framerateInt;
  currentFrame = 0;
}

void setEvent(String eventType, bool isActionTriggered, unsigned int duration) {
  if (eventType == "look") { look(); }
  else if (eventType == "happy") { happy(); }
  else if (eventType == "unhappy") { unhappy(); }
  else if (eventType == "excited") { excited(); }
  else if (eventType == "look") { look(); }
  else if (eventType == "roll") { roll(); }
  else if (eventType == "ok") { ok(); }
  else if (eventType == "no") { no(); }
  else if (eventType == "rain") { rain(); }
  else { return; }

  if (isActionTriggered) { startSequenceMelody(sequence); }
  
  isIdle = false;
  isActionEvent = isActionTriggered;
  isRandomEvent = !isActionTriggered;

  eventMillis = currentMillis;
  eventDuration = duration;
}

// Set melody to start playing
void startSequenceMelody(byte frameArray[][8]) {
  melodyMillis = currentMillis;
  currentNote[0] = 0;
  currentNote[1] = 0;
}

void setRandomEventInterval() {
  eventInterval = random(20000, 20000);
}

void resetMatrix() {
  setBuffer(CLEAR);
}

void goIdle() {
  setSequence(IDLE_SEQUENCE, 700);
  isIdle = true;
  isActionEvent = false;
  isRandomEvent = false;
}

void excited() {
  setSequence(EXCITED_SEQUENCE, 700);
}

void happy() {
  setSequence(HAPPY_SEQUENCE, 700);
}

void unhappy() {
  setSequence(UNHAPPY_SEQUENCE, 900);
}

void look() {
  setSequence(LOOK_SEQUENCE, 1000);
}

void roll() {
  setSequence(ROLL_SEQUENCE, 400);
}

void ok() {
  setSequence(OK_SEQUENCE, 500);
}

void no() {
  setSequence(NO_SEQUENCE, 500);
}

void rain() {
  setSequence(RAIN_SEQUENCE, 200);
}
