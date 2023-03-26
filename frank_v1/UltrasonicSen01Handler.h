#include "HardwareSerial.h"
#include "Arduino.h"
#include "UltrasonicServo01Handler.h"

class UltrasonicSensorHandler {
  private:
    
    const int ECHO = 8;
    const int TRIG = 9;
    const int verificationDegreeSteps = 30;

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

    bool obstacleAhead(int dist) {
      if (!obstacleFromRelativeDegrees(dist, 0)) return false;
      if (!obstacleFromRelativeDegrees(dist, verificationDegreeSteps)) return false;
      if (!obstacleFromRelativeDegrees(dist, -verificationDegreeSteps)) return false;
      return true;
    }

    bool obstacleFromRelativeDegrees(int dist, int degs) {
      srv.setRelativeAngle(degs);
      delay(50);
      int res = readDistance();
      if (res < dist) return true;
      return false;
    }
};
