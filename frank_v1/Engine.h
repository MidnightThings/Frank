class Engine
{
  private:
    int speedPin;
    int directionPin;

  public:
    void initialize(int speedPin, int directionPin) {
      this->speedPin = speedPin;
      this->directionPin = directionPin;
      pinMode(this->speedPin, OUTPUT);
      pinMode(this->directionPin, OUTPUT);
      digitalWrite(this->speedPin, LOW);
    }

    void setSpeed(int speed) {
      analogWrite(this->speedPin, speed);
    }

    void advance(int speed) {
      analogWrite(this->speedPin, speed);
      digitalWrite(this->directionPin, HIGH);
    }

    void backOff(int speed) {
      analogWrite(this->speedPin, speed);
      digitalWrite(this->directionPin, LOW);
    }

    void stop() {
      analogWrite(this->speedPin, 0);
      digitalWrite(this->directionPin, LOW);
    }
};