
void setRandomEventInterval() {
  eventInterval = random(20000, 20000);
}

void setEvent(String eventType, bool isActionTriggered, unsigned int duration) {
  if (alarmRinging || isSleeping) { return; }

  if (eventType == "look") { look(); }
  else if (eventType == "happy") { happy(); }
  else if (eventType == "unhappy") { unhappy(); }
  else if (eventType == "excited") { excited(); }
  else if (eventType == "look") { look(); }
  else if (eventType == "roll") { roll(); }
  else if (eventType == "ok") { ok(); }
  else if (eventType == "no") { no(); }
  else if (eventType == "rain") { rain(); }
  else if (eventType == "eat") { eat(); }
  else { return; }

  if (isActionTriggered) { startSequenceMelody(sequence); }
  
  isIdle = false;
  isActionEvent = isActionTriggered;
  isRandomEvent = !isActionTriggered;

  eventMillis = currentMillis;
  eventDuration = duration;
}

void updateActionEvents() {
  // No action event ongoing, nothing to do
  if (isRandomEvent || isIdle) { return; }

  if (isActionEvent && (currentMillis - eventMillis >= eventDuration)) {
    // Serial.println("Action goIdle");
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
    // Serial.println("Trigger random event");

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
    // Serial.println("Random goIdle");
    // Set interval before next event
    setRandomEventInterval();
    goIdle();
  }
}
