#include <Servo.h>
#include "UltrasonicSen01Handler.h"

enum EDrivingMode {
  IDLE,
  FORWARD,
  BACKWARD,
  TURNLEFT,
  TURNRIGHT
};

Servo myservo;
UltrasonicSensorHandler sonicHandler;
const int baseAngle = 94; 
const int engine1Speed = 5; //Speed motor 1
const int engine2Speed = 6; //Speed motor 2
const int engine1Dir = 4; //Direction motor 1
const int engine2Dir = 7; //Direction motor 2

EDrivingMode drivingMode = IDLE;

void setup() {
  myservo.attach(10);
  myservo.write(baseAngle);
  
  Serial.begin(115200);
  
  pinMode(engine1Speed, OUTPUT);
  pinMode(engine2Speed, OUTPUT);
  pinMode(engine1Dir, OUTPUT);
  pinMode(engine2Dir, OUTPUT);
  digitalWrite(engine1Speed, LOW);
  digitalWrite(engine2Speed, LOW);
}

void loop() {
  if(Serial.available()) {
    char val = Serial.read();
    if (val != -1) {
      switch(val) {
        case 'U':
          advance(255, 255);
          break;

        case 'D':
          backOff(200, 200);
          break;
          
        case 'L':
          turnLeft();
          break;

        case 'R':
          turnRight();
          break;

        case 'z':
          Serial.println("Hello");
          break;

        case 'x':
          stop();
          break;
      }
    }
  }
  if (drivingMode == FORWARD) {
    int distance = sonicHandler.readDistance();
    if (distance > -1 && distance < 30) stop();
  }
}

void advance(int speedM1, int speedM2) {
  if (drivingMode != IDLE && drivingMode != FORWARD) {
    stop();
    delay(50);
  }
  analogWrite(engine1Speed, speedM1);
  digitalWrite(engine1Dir, HIGH);
  analogWrite(engine2Speed, speedM2);
  digitalWrite(engine2Dir, HIGH);
  drivingMode = FORWARD;
}

void backOff(int speedM1, int speedM2) {
  if (drivingMode != IDLE && drivingMode != BACKWARD) {
    stop();
    delay(50);
  }
  analogWrite(engine1Speed, speedM1);
  digitalWrite(engine1Dir, LOW);
  analogWrite(engine2Speed, speedM2);
  digitalWrite(engine2Dir, LOW);
  drivingMode = BACKWARD;
}

void turnLeft() {
  if (drivingMode != IDLE && drivingMode != TURNLEFT) {
    stop();
    delay(50);
  }
  analogWrite(engine1Speed, 200);
  digitalWrite(engine1Dir, HIGH);
  analogWrite(engine2Speed, 200);
  digitalWrite(engine2Dir, LOW);
  drivingMode = TURNLEFT;
}

void turnRight() {
  if (drivingMode != IDLE && drivingMode != TURNRIGHT) {
    stop();
    delay(50);
  }
  analogWrite(engine1Speed, 200);
  digitalWrite(engine1Dir, LOW);
  analogWrite(engine2Speed, 200);
  digitalWrite(engine2Dir, HIGH);
  drivingMode = TURNRIGHT;
}

void stop() {
  analogWrite(engine1Speed, 0);
  digitalWrite(engine1Dir, LOW);
  analogWrite(engine2Speed, 0);
  digitalWrite(engine2Dir, LOW);
  drivingMode = IDLE;
}
