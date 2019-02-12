void updateFood() {
  if (currentMillis - foodMillis >= foodInterval) {
    if (food > 0) { food--; }
    foodMillis = currentMillis;
  }
}

void updateSleep() {
  if (currentMillis - sleepMillis >= sleepInterval) {
    if (sleep > 0) { sleep--; }
    sleepMillis = currentMillis;
  }
}