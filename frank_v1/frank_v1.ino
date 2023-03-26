#include <Servo.h>
#include "UltrasonicSen01Handler.h"
#include "EngineHandler.h"

enum EDrivingMode {
  IDLE,
  FORWARD,
  BACKWARD,
  TURNLEFT,
  TURNRIGHT
};

Servo myservo;
UltrasonicSensorHandler sonicHandler;
EngineHandler engineLeft(5, 4);
EngineHandler engineRight(6, 7);
const int baseAngle = 94; 

void setup() {
  myservo.attach(10);
  myservo.write(baseAngle);
  
  Serial.begin(115200);
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
    int distance = sonicHandler.readDistance();
    if (distance > -1 && distance < 50) {
      engineLeft.stop();
      engineRight.stop();
    }
  }
}