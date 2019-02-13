void setupWebServer() {
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void updateWebQuery() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  if (!client.available()) {
    delay(10); // Stopping the clock for only 10ms should not be visible
    if (!client.available()) { return; }
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  
  // Match the request
  if (req.indexOf("automode/toggle") != -1) { automode = !automode; }
  else if (req.indexOf("mute/toggle") != -1) { mute = !mute; }
  else if (req.indexOf("alarm/toggle") != -1) { alarmSetOn = !alarmSetOn; }
  else if (req.indexOf("alarm/snooze") != -1) { snoozeAlarm(); }
  else if (req.indexOf("alarm/set") != -1) { setAlarmTime(req); }

  else if (req.indexOf("feed") != -1) { feed(); }
  else if (req.indexOf("sleep/toggle") != -1) { toggleSleep(); }

  else if (req.indexOf("excited") != -1) { setEvent("excited", true, 5000); }
  else if (req.indexOf("roll") != -1) { setEvent("roll", true, 5000); }
  else if (req.indexOf("look") != -1) { setEvent("look", true, 5000); }
  else if (req.indexOf("unhappy") != -1) { setEvent("unhappy", true, 5000); }
  else if (req.indexOf("happy") != -1) { setEvent("happy", true, 5000); }
  else if (req.indexOf("ok") != -1) { setEvent("ok", true, 5000); }
  else if (req.indexOf("no") != -1) { setEvent("no", true, 5000); }
  else if (req.indexOf("rain") != -1) { setEvent("rain", true, 5000); }

  // Get web page template
  String webPage = getPage();

  // Fill in status values
  webPage.replace("value='food'", "value=" + String(food));
  webPage.replace("value='sleep'", "value=" + String(sleep));
  webPage.replace("Turn light", isSleeping ? "Turn light on" : "Turn light off");

  // Fill in setting values
  webPage.replace("id='mute'", mute ? "id='mute' checked" : "id='mute'");
  webPage.replace("id='automode'", automode ? "id='automode' checked" : "id='automode'");

  // Fill in alarm values
  webPage.replace("00:00", getAlarmTime());
  webPage.replace("id='alarmSetOn'", alarmSetOn ? "id='alarmSetOn' checked" : "id='alarmSetOn'");

  // Fill in debug values
  // webPage.replace("isIdleValue", isIdle ? "true" : "false");
  // webPage.replace("isActionEventValue", isActionEvent ? "true" : "false");
  // webPage.replace("isRandomEventValue", isRandomEvent ? "true" : "false");
  // webPage.replace("eventIntervalValue", String(eventInterval));
  // webPage.replace("eventDurationValue", String(eventDuration));

  // Send the response to the client
  client.print(webPage); 
}
