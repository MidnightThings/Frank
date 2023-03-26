#include "HardwareSerial.h"
#include <Servo.h>

class UltraSonicServo01Handler {
  private:
    const static int baseAngle = 94;
    
  public:
    Servo myServo;
    int currentAngle;
    UltraSonicServo01Handler() {

    }

    void initialize(int initialAngle = baseAngle) {
      Serial.println("asdasdsadsad");
      myServo.attach(10);
      myServo.write(30);
      initialSetup(initialAngle);
    }

    void initialSetup(int angle) {
      setAngle(angle);
    }

    void setStraight() {
      setAngle(baseAngle);
    }

    bool addDegrees(int deg) {
      return setAngle(currentAngle + deg);
    }

    bool setAngle(int angle) {
      if (angle > 180 || angle < 0) return false;
      if (angle == currentAngle) return true;
      myServo.write(angle);
      currentAngle = angle;
      delay(15);
      return true;
    }

    bool setRelativeAngle(int deg) {
      return setAngle(baseAngle + deg);
    }
};