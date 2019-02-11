void initializeDateTime() {
  Serial.print("[HTTP] begin datetime fetch...\n");
  http.begin("https://api.sabu.fr/timestamps/" + timezone, fingerprint);

  int httpCode = http.GET();

  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    // Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);

      String year = payload.substring(0, 4);
      String month = payload.substring(5, 7);
      String day = payload.substring(8, 10);
      String hour = payload.substring(11, 13);
      String minute = payload.substring(14, 16);
      String second = payload.substring(17, 19);
      DateTime timestamp (year.toInt(), month.toInt(), day.toInt(), hour.toInt(), minute.toInt(), second.toInt());
      currentTime = timestamp;
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void updateClock() {
  if (clockTickMillis == 0) { clockTickMillis = currentMillis; } // Initialization
  if (clockSyncMillis == 0) { clockSyncMillis = currentMillis; } // Initialization

  if (currentMillis - clockSyncMillis >= clockSyncInterval) {
    initializeDateTime();
    clockTickMillis = currentMillis;
    clockSyncMillis = currentMillis;
  }

  if (currentMillis - clockTickMillis >= clockTickInterval) {
    currentTime = currentTime + TimeSpan(0, 0, 0, clockTickInterval / 1000);
    // Serial.println(currentTime.second());
    clockTickMillis = currentMillis;
  }
}
