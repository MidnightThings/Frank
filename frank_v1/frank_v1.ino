#include "UltrasonicSen01Handler.h"
#include "MovementHandler.h"

UltrasonicSensorHandler sonicHandler;
MovementHandler engineMovement(5, 4, 6, 7);

void setup() {
  Serial.begin(115200);
  sonicHandler.srv.myServo.attach(10);
  sonicHandler.srv.initialize();
  engineMovement.setSensorHandler(sonicHandler);
}

void loop() {
  if(Serial.available()) {
    char val = Serial.read();
    if (val != -1) {
      Serial.println(val);
      switch(val) {
        case 'W':
          engineMovement.advance(255);
          break;

        case 'S':
          engineMovement.backOff(255);
          break;

        case 'A':
          engineMovement.turnLeft(200);
          break;

        case 'D':
          engineMovement.turnRight(200);
          break;

        case 'z':
          Serial.println("Hello");
          break;

        case 'x':
          engineMovement.stop();
          break;
      }
    }
  }
  engineMovement.checkObstacle();
}