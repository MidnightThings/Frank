#include <Servo.h>
Servo myservo;
int baseAngle = 94; 
int engine1Speed = 5; //Speed motor 1
int engine2Speed = 6; //Speed motor 2
int engine1Dir = 4; //Direction motor 1
int engine2Dir = 7; //Direction motor 2

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
  // advance(255, 255);
}
//94
void loop() {
  if(Serial.available()) {
    char val = Serial.read();
    if(val != -1){
      switch(val){
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
}

void advance(int speedM1, int speedM2) {
  analogWrite(engine1Speed, speedM1);
  digitalWrite(engine1Dir, HIGH);
  analogWrite(engine2Speed, speedM2);
  digitalWrite(engine2Dir, HIGH);
}

void backOff(int speedM1, int speedM2) {
  analogWrite(engine1Speed, speedM1);
  digitalWrite(engine1Dir, LOW);
  analogWrite(engine2Speed, speedM2);
  digitalWrite(engine2Dir, LOW);
}

void turnLeft() {
  stop();
  analogWrite(engine1Speed, 200);
  digitalWrite(engine1Dir, HIGH);
  analogWrite(engine2Speed, 200);
  digitalWrite(engine2Dir, LOW);
}

void turnRight() {
  stop();
  analogWrite(engine1Speed, 200);
  digitalWrite(engine1Dir, LOW);
  analogWrite(engine2Speed, 200);
  digitalWrite(engine2Dir, HIGH);
}

void stop() {
  analogWrite(engine1Speed, 0);
  digitalWrite(engine1Dir, LOW);
  analogWrite(engine2Speed, 0);
  digitalWrite(engine2Dir, LOW);
}
