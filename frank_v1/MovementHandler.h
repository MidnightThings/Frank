#include "HardwareSerial.h"
#include "Arduino.h"
#include "WString.h"
#include "Engine.h"
#include <time.h>

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

    /* direction must be FORWARD or BACKWARD.
    */
    void bringToSpeed(int speed, EDrivingMode direction) 
    {
      if (direction != FORWARD && direction != BACKWARD) return;
      bool forward = direction == FORWARD;
      int speedDifference = speed - this->currentSpeed;
      int accelerationSteps = speedDifference / 60;
      int accelerationtep = speedDifference / accelerationSteps;
      for (int ii = 0; ii < (accelerationSteps - 1); ii++) {
        int newSpeed = this->currentSpeed + accelerationtep;
        this->engineLeft.setSpeed(newSpeed, forward);
        this->engineRight.setSpeed(newSpeed, forward);
        this->currentSpeed = newSpeed;
        delay(50);
      }
      this->engineLeft.setSpeed(speed, forward);
      this->engineRight.setSpeed(speed, forward);
      this->currentSpeed = speed;
      this->drivingMode = direction;
    }

    void advance(int speed)
    {  
      if (this->drivingMode != IDLE && this->drivingMode != FORWARD) {
        this->stop();
        delay(10);
      }
      this->bringToSpeed(speed, FORWARD);
    }

    void backOff(int speed)
    {
      if (this->drivingMode != IDLE && this->drivingMode != BACKWARD) {
        this->stop();
        delay(10);
      }

      this->bringToSpeed(speed, BACKWARD);
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

    /* Get it?
    */
    void beASunflower(bool left) 
    {
      if (this->drivingMode != IDLE) this->stop();
      int widestSpace = 0;
      if (left) this->turnLeft(200);
      else this->turnRight(200);
      unsigned long start = millis();
      while (true) {
        int dist = this->sensorHandler.obstacleAhead(100, left ? 10 : -10);
        if (dist >= 100) {
          this->stop();
          return;
        }
        unsigned long end = millis();
        int elapsedMillis = end - start;
        if (elapsedMillis > 5000) {
          if (dist >= (widestSpace - (elapsedMillis / 1000))) {
            this->stop();
            return;
          }
        }
        if (dist > widestSpace) widestSpace = dist;
      }
    }

    void setCurve(int level, String direction) {
      if (level < 0 || level > maxCurveLevel) return;
      if (this->drivingMode != FORWARD && this->drivingMode != BACKWARD) return;
      direction.toLowerCase();
      // Check for changed curve direction, in that case set both engines to current speed (end turn)
      if (this->curveDirection != NONE) {
        if ((this->curveDirection == LEFT && direction != "left") || (this->curveDirection == RIGHT && direction != "right")) {
          stopCurve();
        }
      }
      int desiredSpeed = currentSpeed - (curveLevelSpeedReduction * level);
      if (direction == "left") {
        if (this->drivingMode == FORWARD) engineLeft.setSpeed(desiredSpeed, true);
        else engineRight.setSpeed(desiredSpeed, false);
        this->curveDirection = LEFT;
      } else {
        if (this->drivingMode == FORWARD) engineRight.setSpeed(desiredSpeed, true);
        else engineLeft.setSpeed(desiredSpeed, false);
        this->curveDirection = RIGHT;
      }
      this->curveLevel = level;
    }

    void stopCurve() {
      if (curveLevel == 0 && curveDirection == NONE) return;
      if (this->drivingMode == FORWARD) this->advance(currentSpeed);
      else if (this->drivingMode == BACKWARD) this->backOff(currentSpeed);
      curveLevel = 0;
      curveDirection = NONE;
    }

    void stop(bool fullSystemHalt = false)
    {
      if (this->drivingMode == FORWARD || this->drivingMode == BACKWARD) this->bringToSpeed(0, this->drivingMode);
      this->engineLeft.stop();
      this->engineRight.stop();
      this->drivingMode = IDLE;
      this->currentSpeed = 0;
      // Just to be safe call stopCurve(). It won't do nuffin but setting stuff to zero.
      this->stopCurve();
      if (fullSystemHalt) this->autonom = false;
    }

    void checkObstacle()
    {
      String dir;
      int dist = sensorHandler.obstacleAhead(100);
      int distLeft;
      int distRight;
      if(dist <= 40 && this->autonom){
        bool left = this->curveDirection == LEFT;
        this->stop();
        beASunflower(left);
        this->rescueRun = true;
        return;
      }
      if (dist < 100) {
        if (this->autonom) {
          distLeft = this->sensorHandler.obstacleAhead(100, 45);
          if (distLeft >= 100) {
            dir = "left";
          } else {
            distRight = this->sensorHandler.obstacleAhead(100, -45);
            if (distRight >= 100 || distRight > distLeft) {
              dir = "right";
            } else {
              dir = "left";
            }
          }      
          if (this->rescueRun) {
            advance(255);
            this->rescueRun = false;
          }
          if (dist > 90) setCurve(1, dir);
          else if (dist > 80) setCurve(2, dir);
          else if (dist > 70) setCurve(3, dir);
          else if (dist > 60) setCurve(4, dir);
          else setCurve(5, dir);
        } else if (dist < 50) stop();
      } else {
        if (this->autonom) {
          stopCurve();
          if (this->rescueRun) {
            advance(255);
            this->rescueRun = false;
          }
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