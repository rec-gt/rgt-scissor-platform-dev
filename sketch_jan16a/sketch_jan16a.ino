#include <Servo.h>

Servo servoMotor;  // Create a servo object to control a continuous rotation servo
int servoPin = 2;  // Digital pin to which the servo is connected
int stopPulse = 1500;  // Pulse width to stop the servo
int forwardSpeed = 1600;  // Pulse width for forward rotation
int reverseSpeed = 1400;  // Pulse width for reverse rotation

void setup() {
  servoMotor.attach(servoPin);  // Attach the servo signal pin
}

void loop() {
  // Rotate forward
  servoMotor.writeMicroseconds(forwardSpeed);  // Set the pulse width for forward rotation
  delay(2000);  // Rotate for 2 seconds
}