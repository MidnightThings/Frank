#include "HardwareSerial.h"
#include "Arduino.h"
#include <string.h>

const int ECHO = 8;
const int TRIG = 9;

void initialize() {
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, HIGH);
  pinMode(ECHO, INPUT);
}

int calculateDistance(unsigned long lowLevelTime) {
  String msg = "lowLevelTime: ";

  Serial.println(lowLevelTime);

  // Serial.println("lowLevelTime: ".append(lowLevelTime) +  " - Distance in cm: " + (lowLevelTime / 50));
  return lowLevelTime / 50;
}

int readDistance() {
  digitalWrite(TRIG, LOW);
  digitalWrite(TRIG, HIGH);
  unsigned long lowLevelTime = pulseIn(ECHO, LOW);
  if (lowLevelTime > 50000) return -1;
  return calculateDistance(lowLevelTime);
}

