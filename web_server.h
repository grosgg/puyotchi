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