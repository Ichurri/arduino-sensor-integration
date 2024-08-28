class UltrasonicSensor {
  private:
    int trigPin;
    int echoPin;

  public:
    UltrasonicSensor(int trigPin, int echoPin) {
      this->trigPin = trigPin;
      this->echoPin = echoPin;
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }

    float getDistance() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      float duration = pulseIn(echoPin, HIGH);
      return duration * 0.034 / 2;
    }
};

class LEDController {
  private:
    int ledPins[3];
    unsigned long previousMillis;
    const long interval;
    bool ledStates[3];

  public:
    LEDController(int pin1, int pin2, int pin3, long interval)
      : ledPins{pin1, pin2, pin3}, interval(interval), previousMillis(0) {
      for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
        ledStates[i] = LOW;
      }
    }

    void blinkLED(int ledPinToBlink) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        for (int i = 0; i < 3; i++) {
          if (ledPins[i] == ledPinToBlink)
            ledStates[i] = !ledStates[i];  
          else
            ledStates[i] = LOW;  
          digitalWrite(ledPins[i], ledStates[i]);
        }
      }
    }
};

const int LEDPINS[] = {27, 26, 25};
float cm = 0;

UltrasonicSensor sensor(22, 23);
LEDController ledController(27, 26, 25, 300);

void setup() {
  Serial.begin(9600); 
}

void loop() {
  float distance = sensor.getDistance();
  int ledPinToBlink = -1;
  cm = distance;  

  if (distance < 15)
    ledPinToBlink = LEDPINS[2];
  else if (distance < 35)
    ledPinToBlink = LEDPINS[1];
  else if (distance < 60)
    ledPinToBlink = LEDPINS[0];

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  ledController.blinkLED(ledPinToBlink);
}