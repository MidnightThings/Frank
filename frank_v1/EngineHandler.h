class EngineHandler
{
  private:
    int speedPin;
    int directionPin;
    enum EDrivingMode {
      IDLE,
      FORWARD,
      BACKWARD,
      TURNLEFT,
      TURNRIGHT
    };
    EDrivingMode drivingMode;

  public:
    EngineHandler(int speedPin, int directionPin) {
      this->speedPin = speedPin;
      this->directionPin = directionPin;
      pinMode(this->speedPin, OUTPUT);
      pinMode(this->directionPin, OUTPUT);
      digitalWrite(this->speedPin, LOW);
      this->drivingMode = IDLE;
    };

    void advance(int speed) {
      stop();
      delay(50);
      // if (this->drivingMode != IDLE && this->drivingMode != FORWARD) {
      //   stop();
      //   delay(50);
      // }
      analogWrite(this->speedPin, speed);
      digitalWrite(this->directionPin, HIGH);

      this->drivingMode = FORWARD;
    }

    void backOff(int speed) {
      stop();
      delay(50);
      // if (this->drivingMode != IDLE && this->drivingMode != BACKWARD) {
      //   stop();
      //   delay(50);
      // }
      analogWrite(this->speedPin, speed);
      digitalWrite(this->directionPin, LOW);
      this->drivingMode = BACKWARD;
    }

    void stop() {
      analogWrite(this->speedPin, 0);
      digitalWrite(this->directionPin, LOW);
      this->drivingMode = IDLE;
    }

    EDrivingMode getDrivingMode() {
      return this->drivingMode;
    }
};