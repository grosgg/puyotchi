void updateAlarm() {
  if (!alarmOn || alarmSnoozed) { return; }

  if (currentTime.hour() == alarmHour && currentTime.minute() == alarmMinute) {
    Serial.println("Alarm triggered!");
    alarmSnoozed = true;
  }
}