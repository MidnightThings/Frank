#include "UltrasonicSen01Handler.h"
#include "EngineHandler.h"

enum EDrivingMode {
  IDLE,
  FORWARD,
  BACKWARD,
  TURNLEFT,
  TURNRIGHT
};

UltrasonicSensorHandler sonicHandler;
EDrivingMode drivingMode = IDLE;
EngineHandler engineRight(5, 4);
EngineHandler engineLeft(6, 7);

void setup() {
  Serial.begin(115200);
  sonicHandler.srv.myServo.attach(10);
  sonicHandler.srv.initialize();
}

void loop() {
  if(Serial.available()) {
    char val = Serial.read();
    if (val != -1) {
      switch(val) {
        case 'U':
          engineLeft.advance(200);
          engineRight.advance(200);
          break;

        case 'D':
          engineLeft.backOff(200);
          engineRight.backOff(200);
          break;

        case 'L':
          engineLeft.backOff(200);
          engineRight.advance(200);
          break;

        case 'R':
          engineLeft.advance(200);
          engineRight.backOff(200);
          break;

        case 'z':
          Serial.println("Hello");
          break;

        case 'x':
          engineLeft.stop();
          engineRight.stop();
          break;
      }
    }
  }
  if (engineLeft.getDrivingMode() == FORWARD && engineRight.getDrivingMode() == FORWARD) {
    if (sonicHandler.obstacleAhead(50)) {
      engineLeft.stop();
      engineRight.stop();
    }
  }
}