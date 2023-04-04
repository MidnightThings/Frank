#include "HardwareSerial.h"
#include "WString.h"
#include "Engine.h"

class MovementHandler
{
  private:
    Engine engineLeft;
    Engine engineRight;
    UltrasonicSensorHandler sensorHandler;
    bool autonom = false;
    bool rescueRun = false;

  public:
  enum EDrivingMode {
      IDLE,
      FORWARD,
      BACKWARD,
      TURNLEFT,
      TURNRIGHT
    };
    enum ECurveDirection {
      NONE,
      LEFT,
      RIGHT
    };
    const int maxCurveLevel = 5;
    const int curveLevelSpeedReduction = 45;
    int currentSpeed;
    int curveLevel = 0;
    EDrivingMode drivingMode;
    ECurveDirection curveDirection = NONE;
    
    MovementHandler(int speedPinEngine1, int direcionPinEngine1, int speedPinEngine2, int direcionPinEngine2)
    {
      this->engineLeft.initialize(speedPinEngine2, direcionPinEngine2);
      this->engineRight.initialize(speedPinEngine1, direcionPinEngine1);
      this->drivingMode = IDLE;
    }

    void advance(int speed)
    {  
      if (this->drivingMode != IDLE && this->drivingMode != FORWARD) {
        this->stop();
        delay(50);
      }
      currentSpeed = speed;
      this->engineLeft.advance(speed);
      this->engineRight.advance(speed);
      this->drivingMode = FORWARD;
    }

    void backOff(int speed)
    {
      if (this->drivingMode != IDLE && this->drivingMode != BACKWARD) {
        this->stop();
        delay(50);
      }
      currentSpeed = speed;
      this->engineLeft.backOff(speed);
      this->engineRight.backOff(speed);
      this->drivingMode = BACKWARD;
    }

    void turnLeft(int speed)
    {
      if (this->drivingMode != IDLE && this->drivingMode != TURNLEFT) {
        this->stop();
        delay(50);
      }
      this->engineLeft.backOff(speed);
      this->engineRight.advance(speed);
      this->drivingMode = TURNLEFT;
    }

    void turnRight(int speed)
    {
      if (this->drivingMode != IDLE && this->drivingMode != TURNRIGHT) {
        this->stop();
        delay(50);
      }
      this->engineLeft.advance(speed);
      this->engineRight.backOff(speed);
      this->drivingMode = TURNRIGHT;
    }

    void setCurve(int level, String direction) {
      if (level < 0 || level > maxCurveLevel) return;
      if (drivingMode != FORWARD && drivingMode != BACKWARD) return;
      direction.toLowerCase();
      // Check for changed curve direction, in that case set both engines to current speed (end turn)
      if (curveDirection != NONE) {
        if ((curveDirection == LEFT && direction != "left") || (curveDirection == RIGHT && direction != "right")) {
          stopCurve();
        }
      }
      int desiredSpeed = currentSpeed - (curveLevelSpeedReduction * level);
      if (direction == "left") {
        if (drivingMode == FORWARD) engineLeft.setSpeed(desiredSpeed);
        else engineRight.setSpeed(desiredSpeed);
        curveDirection = LEFT;
      } else {
        if (drivingMode == FORWARD) engineRight.setSpeed(desiredSpeed);
        else engineLeft.setSpeed(desiredSpeed);
        curveDirection = RIGHT;
      }
      curveLevel = level;
    }

    void stopCurve() {
      engineLeft.setSpeed(currentSpeed);
      engineRight.setSpeed(currentSpeed);
      curveLevel = 0;
      curveDirection = NONE;
    }

    void stop(bool fullSystemHalt = false)
    {
      this->engineLeft.stop();
      this->engineRight.stop();
      this->drivingMode = IDLE;
      if (fullSystemHalt) this->autonom = false;
    }

    void checkObstacle()
    {
      String dir;
      int dist = sensorHandler.obstacleAhead(150);
      int distLeft;
      int distRight;
      if(dist <= 40 && this->autonom){
        Serial.println("kleiner 40?");
        Serial.println(dist);
        stopCurve();
        backOff(255);
        delay(500);
        stop();
        this->rescueRun = true;
        checkObstacle();
        return;
      }
      if (dist < 150) {
        if (this->autonom) {
          distLeft = this->sensorHandler.obstacleAhead(150, 50);
          if (distLeft > 150) {
            dir = "left";
            dist = distLeft;
          } else {
            distRight = this->sensorHandler.obstacleAhead(150, -50);
            if (distRight > 150 || distRight > (distLeft + 5)) {
              dir = "right";
              dist = distRight;
            } else {
              dir = "left";
              dist = distLeft;
            }
          }      
          if (this->rescueRun) {
            advance(255);
            setCurve(5, dir);
            this->rescueRun = false;
          }
          else if (dist > 120) setCurve(1, dir);
          else if (dist > 105) setCurve(2, dir);
          else if (dist > 90) setCurve(3, dir);
          else if (dist > 80) setCurve(4, dir);
          else setCurve(5, dir);
          delay(100);
        } else if (dist < 50) stop();
      } else {
        if (this->autonom) {
          stopCurve();
        }
      }
    }

    void setSensorHandler(UltrasonicSensorHandler sensorHandler)
    {
      this->sensorHandler = sensorHandler;
    }

    void autonomous(int speed)
    {
      this->autonom = true;
      this->advance(255);
    }

    void performRoutine()
    {
      if(this->drivingMode == FORWARD || this->autonom){
        this->checkObstacle();
      }
    }
};