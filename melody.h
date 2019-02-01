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

// Set melody to start playing
void startSequenceMelody(byte frameArray[][8]) {
  melodyMillis = currentMillis;
  currentNote[0] = 0;
  currentNote[1] = 0;
}