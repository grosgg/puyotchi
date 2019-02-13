void updateFood() {
  if (currentMillis - foodMillis >= foodInterval) {
    if (food > 0) { food--; }
    foodMillis = currentMillis;
  }
}

void updateSleep() {
  if (currentMillis - sleepMillis >= sleepInterval) {
    if (isSleeping) {
      sleep = min(sleep + 3, 20);
    } else {
      sleep = max(sleep - 1, 0);
    }
    sleepMillis = currentMillis;
  }
}

void feed() {
  if (food == 10) { // puyotchi is already full
    setEvent("no", true, 4000);
  } else {
    food = min(food + 2, 10);
    setEvent("eat", true, 8000);
  }
}

void  toggleSleep() {
  if (isSleeping) {
    goIdle();
  } else {
    goToSleep();
  }
}