#include "Engine.h"

class MovementHandler
{
  private:
    Engine engineLeft;
    Engine engineRight;
    UltrasonicSensorHandler sensorHandler;
    bool autonom = false;
    
  public:
    enum EDrivingMode {
      IDLE,
      FORWARD,
      BACKWARD,
      TURNLEFT,
      TURNRIGHT
    };
    EDrivingMode drivingMode;
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

    void stop()
    {
      this->engineLeft.stop();
      this->engineRight.stop();
      this->drivingMode = IDLE;
    }

    void checkObstacle()
    {
      if (this->sensorHandler.obstacleAhead(50)) {
        this->stop();
        if(this->autonom){
          if(!this->sensorHandler.obstacleAhead(50, 45)){
            this->turnLeft(200);
            delay(1000);
            this->checkObstacle();
          }else if(!this->sensorHandler.obstacleAhead(50, -45)){
            this->turnRight(200);
            delay(1000);
            this->checkObstacle();
          }
        }
      }else{
        if(this->autonom){
          this->advance(255);
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
};