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

    int readDistance(int dist) {
      digitalWrite(TRIG, LOW);
      digitalWrite(TRIG, HIGH);
      unsigned long timeout = dist * 51;
      unsigned long lowLevelTime = pulseIn(ECHO, LOW, timeout);
      if (lowLevelTime > 50000 || lowLevelTime == 0) return -1;
      return calculateDistance(lowLevelTime);
    }

    /* Checks for an obstacle directly ahead. Will perform 3 readings from different angles at most.
    * Returns value of 'dist' at the first reading whose distance is above 'dist', or the final reading
    .
    * 'dist' sets a timeout for each reading, so it will be the highest possbile distance to be returned (in cm). 
    * 'degs' sets an offset for sensor position.
    */ 
    int obstacleAhead(int dist = 300, int degs = 0) {
      int res = obstacleFromRelativeDegrees(dist, degs) > dist;
      if (res == -1 || res >= dist) return dist;
      res = obstacleFromRelativeDegrees(dist, (verificationDegreeSteps + degs));
      if (res == -1 || res >= dist) return dist;
      res = obstacleFromRelativeDegrees(dist, (-verificationDegreeSteps + degs));
      if (res == -1 || res >= dist) return dist;
      return res;
    }

    int obstacleFromRelativeDegrees(int dist, int degs) {
      int expectedDelay = srv.setRelativeAngle(degs);
      if (expectedDelay > -1) {
        delay(expectedDelay);
        int res = readDistance(dist);
        return res;
      } else return -1;
      
    }
};
