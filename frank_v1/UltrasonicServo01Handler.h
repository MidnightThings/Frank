#include <math.h>
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

    // Angle setting methods return expected turning time in milliseconds
    // or -1 if demanded angle cannot be set
    int addDegrees(int deg) {
      return setAngle(currentAngle + deg);
    }

    int setAngle(int angle) {
      if (angle > 180 || angle < 0) return -1;
      if (angle == currentAngle) return 0;
      myServo.write(angle);
      int diff = currentAngle - angle;
      currentAngle = angle;
      return trunc((abs(diff) / (double)60) * 100);
    }

    int setRelativeAngle(int deg) {
      return setAngle(baseAngle + deg);
    }
};