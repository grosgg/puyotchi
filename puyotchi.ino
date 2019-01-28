#include <WEMOS_Matrix_LED.h>
#include <ESP8266WiFi.h>

#include "wifi_settings.h"
#include "wifi.h"
#include "sequences.h"
#include "web_page.h"

#define BUZZER_PIN 5

WiFiServer server(80);

byte sequence[4][8]; // Sequences are 4 frames
unsigned int framerate = 1000; // Default time for each frame
unsigned long animationMillis = 0; // Animation timer
uint8_t currentFrame = 0; // Current frame in sequence

unsigned int noteDuration = 120;
unsigned long melodyMillis = 0; // Melody length timer

unsigned int rythmn = 125; // noteDuration + pause before next note
unsigned long noteMillis = 0; // Rythmn timer
uint8_t currentNote[] = { 0, 0 }; // note being played now in frame / sequence

unsigned int eventInterval = 0; // Time before next event. Initialized in updateEvents()
unsigned int eventDuration = 7000; // Arbitrary length of events
unsigned long eventMillis = 0; // Event timer

unsigned long currentMillis = millis(); // Clock
bool idleMode = true; // Is there an event ongoing

// LED intensity
MLED mled(1);

void setup() {
  Serial.begin(115200);

  resetMatrix();
  idle();

  setupWifi();

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  currentMillis = millis();
  updateEvents();
  updateAnimation();
  updateMelody();

  updateWebQuery();
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
  if (req.indexOf("/roll") != -1) {
    // roll();
  }

  // Send the response to the client
  client.print(getPage()); 
}

void updateEvents() {
  // First event initialization
  if (eventInterval == 0) { setRandomEventInterval(); }

  if (currentMillis - eventMillis >= eventInterval) {
    // Randomly choosing an event
    uint8_t eventType = random(3);
    switch (eventType) {
      case 0:
        excited();
        break;
      case 1:
        happy();
        break;
      case 2:
        look();
        break;
      case 3:
        roll();
        break;
    }
    eventMillis = currentMillis;
    // Set interval before next event
    setRandomEventInterval();
  }

  // When event is done, return to idle mode
  if (!idleMode && (currentMillis - eventMillis >= eventDuration)) { idle(); }
}

// Display next frame on framerate
void updateAnimation() {
  if (currentMillis - animationMillis >= framerate) {
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
  if (!idleMode&& currentMillis - melodyMillis >= rythmn) {
    tone(BUZZER_PIN, sequence[currentNote[0]][currentNote[1]], noteDuration);
    if (currentNote[0] == 4 && currentNote[1] == 7) {
      noTone(BUZZER_PIN);
    } else if (currentNote[1] >= 7) {
      currentNote[0]++;
      currentNote[1] = 0;
    } else {
      currentNote[1]++;
    }
    melodyMillis = currentMillis;
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

// Set melody to start playing
void startSequenceMelody(byte frameArray[][8]) {
  melodyMillis = currentMillis;
  currentNote[0] = 0;
  currentNote[1] = 0;
}

void setRandomEventInterval() {
  eventInterval = random(30000, 60000);
}

void resetMatrix() {
  setBuffer(CLEAR);
}

void idle() {
  setSequence(IDLE_SEQUENCE, 700);
  idleMode = true;
}

void excited() {
  setSequence(EXCITED_SEQUENCE, 700);
  idleMode = false;
}

void happy() {
  setSequence(HAPPY_SEQUENCE, 700);
  idleMode = false;
}

void unhappy() {
  setSequence(UNHAPPY_SEQUENCE, 900);
  idleMode = false;
}

void look() {
  setSequence(LOOK_SEQUENCE, 1000);
  idleMode = false;
}

void roll() {
  setSequence(ROLL_SEQUENCE, 400);
  idleMode = false;
}

void ok() {
  setSequence(OK_SEQUENCE, 500);
  idleMode = false;
}

void no() {
  setSequence(NO_SEQUENCE, 500);
  idleMode = false;
}
