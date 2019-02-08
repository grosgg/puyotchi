void setDateTime() {
  Serial.print("[HTTP] begin...\n");
  http.begin("https://api.sabu.fr/timestamps/" + timezone, fingerprint);

  int httpCode = http.GET();

  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

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
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
