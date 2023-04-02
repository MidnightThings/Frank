#include "HardwareSerial.h"
#include "Arduino.h"
#include "UltrasonicServo01Handler.h"

class UltrasonicSensorHandler {
  private:
    
    const static int ECHO = 8;
    const static int TRIG = 9;
    const static int verificationDegreeSteps = 25;

    void _initialize() {
      pinMode(TRIG, OUTPUT);
      digitalWrite(TRIG, HIGH);
      pinMode(ECHO, INPUT);
    }

    int calculateDistance(unsigned long lowLevelTime) {
      return lowLevelTime / 50;
    }

  public:
    UltraSonicServo01Handler srv;

    UltrasonicSensorHandler() {
      _initialize();
    }

    int readDistance() {
      digitalWrite(TRIG, LOW);
      digitalWrite(TRIG, HIGH);
      unsigned long lowLevelTime = pulseIn(ECHO, LOW);
      if (lowLevelTime > 50000) return -1;
      return calculateDistance(lowLevelTime);
    }

    int obstacleAhead(int dist, int degs = 0) {
      if (obstacleFromRelativeDegrees(dist, 0) > dist) return dist;
      if (obstacleFromRelativeDegrees(dist, (verificationDegreeSteps + degs)) > dist) return dist;
      return obstacleFromRelativeDegrees(dist, (-verificationDegreeSteps + degs));
    }

    int obstacleFromRelativeDegrees(int dist, int degs) {
      int expectedDelay = srv.setRelativeAngle(degs);
      if (expectedDelay > -1) {
        delay(expectedDelay);
        int res = readDistance();
        if (res == -1) return -1;
        return res;
      } else return -1;
      
    }
};
