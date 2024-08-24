#include <AccelStepper.h>

// Define pin connections & motor's steps per revolution
const int dirPin1 = 2;
const int stepPin1 = 3;
const int dirPin2 = 4;
const int stepPin2 = 5;
const int stepsPerRevolution = 200;

bool motorsRunning = false;  // Flag to track if motors are running

AccelStepper motor1(AccelStepper::DRIVER, stepPin1, dirPin1);
AccelStepper motor2(AccelStepper::DRIVER, stepPin2, dirPin2);

void setup()
{
  Serial.begin(9600);

  motor1.setMaxSpeed(2000);
  motor1.setAcceleration(1000);
  motor2.setMaxSpeed(2000);
  motor2.setAcceleration(1000);

  stopMotors();  // Initially stop the motors
}

void loop()
{
  // Check for user input
  if (Serial.available() > 0) {
    char command = Serial.read();

    // Start the motors
    if (command == 'r') {
      startMotors();
    }

    // Stop the motors
    if (command == 's') {
      stopMotors();
    }

    // Move motors downward
    if (command == 'd') {
      moveDown();
    }
  }

  // Run the motors
  motor1.runSpeed();
  motor2.runSpeed();
}

// Function to start the motors
void startMotors()
{
  motorsRunning = true;
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor1.setPinsInverted(true);  // Invert the direction for motor1
  motor2.setPinsInverted(true);  // Invert the direction for motor2
  motor1.enableOutputs();
  motor2.enableOutputs();
}

// Function to stop the motors
void stopMotors()
{
  motorsRunning = false;
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor1.runSpeedToPosition();
  motor2.runSpeedToPosition();
  motor1.disableOutputs();
  motor2.disableOutputs();
}

// Function to move motors downward
void moveDown()
{
  motorsRunning = true;
  motor1.setSpeed(100);  // Set negative speed for downward movement
  motor2.setSpeed(100);  // Set negative speed for downward movement
  motor1.setPinsInverted(false);  // Restore normal direction for motor1
  motor2.setPinsInverted(false);  // Restore normal direction for motor2
  motor1.enableOutputs();
  motor2.enableOutputs();
}