void updateAlarm() {
  if (!alarmSetOn) { return; } // Alarm is disabled

  if (currentMillis - alarmSnoozeMillis <= 60000) { return; } // Alarm has been snoozed, don't ring

  if (alarmRinging && currentMillis - alarmRingMillis >= 500) {
    tone(BUZZER_PIN, 2637, noteDuration);
    alarmRingMillis = currentMillis;
  }

  if (!alarmRinging && currentTime.hour() == alarmHour && currentTime.minute() == alarmMinute) {
    Serial.println("Alarm triggered!");
    alarmRinging = true;
    goIdle(); // Wake up puyotchi
  }
}

void snoozeAlarm() {
  Serial.println("Alarm snoozed.");
  alarmRinging = false;
  noTone(BUZZER_PIN);
  alarmSnoozeMillis = currentMillis;
}

void setAlarmTime(String req) {
  if (req.length() != 41) { return; }
  String alarmRequest = req;

  // Triming parameters
  alarmRequest.replace("GET /alarm/set?alarmTime=", "");
  alarmRequest.replace(" HTTP/1.1", "");
  alarmHour = alarmRequest.substring(0, 2).toInt();
  alarmMinute = alarmRequest.substring(5).toInt();
}

String getAlarmTime() {
  String hour = "";
  if (alarmHour < 10) { hour = "0"; }
  hour = hour + String(alarmHour);

  String minute = "";
  if (alarmMinute < 10) { minute = "0"; }
  minute = minute + String(alarmMinute);

  return (hour + ":" + minute);
}