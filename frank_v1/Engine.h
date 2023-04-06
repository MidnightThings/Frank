class Engine
{
  private:
    int speedPin;
    int directionPin;

  public:
    enum EDirection {
      FORWARD,
      BACKWARD
    };
    EDirection curDirection;

    void initialize(int speedPin, int directionPin) {
      this->speedPin = speedPin;
      this->directionPin = directionPin;
      pinMode(this->speedPin, OUTPUT);
      pinMode(this->directionPin, OUTPUT);
      digitalWrite(this->speedPin, LOW);
      this->curDirection = BACKWARD;
    }

    void setSpeed(int speed, bool forward) {
      if (forward) this->advance(speed);
      else this->backOff(speed);
    }

    void advance(int speed) {
      if (this->curDirection != FORWARD) digitalWrite(this->directionPin, HIGH);
      analogWrite(this->speedPin, speed);
      this->curDirection = FORWARD;
    }

    void backOff(int speed) {
      if (this->curDirection != BACKWARD) digitalWrite(this->directionPin, LOW);
      analogWrite(this->speedPin, speed);
      this->curDirection = BACKWARD;
    }

    void stop() {
      analogWrite(this->speedPin, 0);
    }
};