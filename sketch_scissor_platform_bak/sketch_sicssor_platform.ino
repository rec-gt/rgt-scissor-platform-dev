// laser sensors pins
const int sensorPins[] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9 };
const int numSensors = 10;

const int buttonPin = 2;
const int relayPin = 12;
const int warningLightPin = 13;
const int powerLightPin = 14;
const int switchPin = 15;

int previousSwitchStatus = LOW;

enum Status {
  STOPPED,
  RUNNING,
  ALLOW_10S,
};

Status currentStatus = RUNNING;
int buttonState = 0;

// init timer
const unsigned long TIMER_DURATION = 10 * 1000;
unsigned long startMillis = millis();

void setup() {
  // init serial communication
  Serial.begin(9600);

  // init inputs
  pinMode(buttonPin, INPUT);

  // init outputs
  pinMode(relayPin, OUTPUT);
  pinMode(warningLightPin, OUTPUT);
  pinMode(powerLightPin, OUTPUT);


  // init power light, always on
  handlePowerLight();
  // init NC relay
  handleRelay(true);
}

void printDistanceData(int i, float distance) {
  Serial.print("Sensor ");
  Serial.print(i);
  Serial.print(" - Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

int thresholdDistance() {
  // base 500mm, buffer 300mm
  int base = 500;
  int buffer = 300;
  return base + buffer;
}

void handleChangeStatus(Status status) {
  currentStatus = status;
}

void handlePowerLight() {
  digitalWrite(powerLightPin, HIGH);
}

void handleWarningLight(bool toggle) {
  digitalWrite(warningLightPin, toggle ? HIGH : LOW);
}

void handleRelay(bool toggle) {
  digitalWrite(relayPin, toggle ? HIGH : LOW);
}

void listenSensors() {
  // listen 10 sensor
  for (int i = 0; i < numSensors; i++) {
    int sensorValue = analogRead(sensorPins[i]);  // Read the analog sensor value
    float distance = sensorValue * 5.0 / 1023.0;  // Convert sensor value to distance (assuming a linear relationship)

    if (distance > thresholdDistance()) {
      handleChangeStatus(STOPPED);
      handleWarningLight(true);
      handleRelay(false);
    }
  }
}

void listenButton() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {  // once button is pressed
    handleWarningLight(false);
    handleRelay(true);

    // remember current time
    startMillis = millis();
    handleChangeStatus(ALLOW_10S);
  }
}

void listenSwitch() {
  int currentSwitchState = digitalRead(switchPin);  // Read the current state of the switch

  if (currentSwitchState != previousSwitchStatus) {
    if (currentSwitchState == HIGH) {
      Serial.println("Switch is ON");
    } else {
      Serial.println("Switch is OFF");
    }
    previousSwitchStatus = currentSwitchState;  // Update the previous state
  }
}



void handleTimer() {
  unsigned long currentMillis = millis();
  if (currentMillis - startMillis >= TIMER_DURATION) {
    handleChangeStatus(RUNNING);
  }
}

void loop() {
  // listen 10 sensor
  if (currentStatus == RUNNING) {
    listenSensors();
  }

  if (currentStatus == STOPPED) {
    listenButton();
  }

  if (currentStatus == ALLOW_10S) {
    handleTimer();
  }

  delay(10);
  //
}
