// Display on matrix
void setBuffer(byte hexArray[]) {
  for(uint8_t i = 0; i < 8; i++) {
    mled.disBuffer[i] = hexArray[i];
  }
}

// Display next frame on framerate
void updateAnimation() {
  if (currentMillis - animationMillis >= framerate) {
    // Serial.println("New frame");
    setBuffer(sequence[currentFrame]);
    if (currentFrame >= 3) {
      currentFrame = 0;
    } else {
      currentFrame++;
    }
    mled.display();
    animationMillis = currentMillis;
  }
}

void resetMatrix() {
  setBuffer(CLEAR);
}
