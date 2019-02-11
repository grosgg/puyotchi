void updateAlarm() {
  if (!alarmSetOn || alarmRinging) { return; }

  if (currentTime.hour() == alarmHour && currentTime.minute() == alarmMinute) {
    Serial.println("Alarm triggered!");
    alarmRinging = true;
  }
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