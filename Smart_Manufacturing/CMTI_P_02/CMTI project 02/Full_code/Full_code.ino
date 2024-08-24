#include <AccelStepper.h>

// Rotary encoder connections for motor1
const int encoderPinA1 = 2;
const int encoderPinB1 = 3;
const int encoderPinZ1 = 4;

// Rotary encoder connections for motor2
const int encoderPinA2 = 7;
const int encoderPinB2 = 8;
const int encoderPinZ2 = 9;

// Stepper motor connections for motor1
const int dirPin1 = 5;
const int stepPin1 = 6;
#define motorInterfaceType1 1
AccelStepper motor1(motorInterfaceType1, stepPin1, dirPin1);

// Stepper motor connections for motor2
const int dirPin2 = 10;
const int stepPin2 = 11;
#define motorInterfaceType2 1
AccelStepper motor2(motorInterfaceType2, stepPin2, dirPin2);

// Define pin connections & motor's steps per revolution for motor3 and motor4
const int dirPin3 = 12;
const int stepPin3 = 13;
const int dirPin4 = 14;
const int stepPin4 = 15;
const int stepsPerRevolution = 200;

bool motorsRunning = false;  // Flag to track if motors are running

AccelStepper motor3(AccelStepper::DRIVER, stepPin3, dirPin3);
AccelStepper motor4(AccelStepper::DRIVER, stepPin4, dirPin4);

// Variables to store encoder state for motor1
volatile int count1 = 0;
volatile bool indexTriggered1 = false;

// Variables to store encoder state for motor2
volatile int count2 = 0;
volatile bool indexTriggered2 = false;

void setup()
{
  // Set encoder pins as input for motor1
  pinMode(encoderPinA1, INPUT);
  pinMode(encoderPinB1, INPUT);
  pinMode(encoderPinZ1, INPUT);

  // Set encoder pins as input for motor2
  pinMode(encoderPinA2, INPUT);
  pinMode(encoderPinB2, INPUT);
  pinMode(encoderPinZ2, INPUT);

  // Enable internal pull-up resistors for the encoder pins for motor1
  digitalWrite(encoderPinA1, HIGH);
  digitalWrite(encoderPinB1, HIGH);
  digitalWrite(encoderPinZ1, HIGH);

  // Enable internal pull-up resistors for the encoder pins for motor2
  digitalWrite(encoderPinA2, HIGH);
  digitalWrite(encoderPinB2, HIGH);
  digitalWrite(encoderPinZ2, HIGH);

  // Attach interrupts to the encoder pins for motor1
  attachInterrupt(digitalPinToInterrupt(encoderPinA1), handleEncoderInterrupt1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB1), handleEncoderInterrupt1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinZ1), handleIndexInterrupt1, RISING);

  // Attach interrupts to the encoder pins for motor2
  attachInterrupt(digitalPinToInterrupt(encoderPinA2), handleEncoderInterrupt2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB2), handleEncoderInterrupt2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinZ2), handleIndexInterrupt2, RISING);

  // Set motor pin modes for motor1
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  // Set motor pin modes for motor2
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);

  // Set motor pin modes for motor3 and motor4
  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin4, OUTPUT);
  pinMode(stepPin4, OUTPUT);

  // Set initial motor configuration for motor1
  motor1.setMaxSpeed(10000);
  motor1.setAcceleration(500);
  motor1.setSpeed(1000);
  motor1.setCurrentPosition(0);

  // Set initial motor configuration for motor2
  motor2.setMaxSpeed(10000);
  motor2.setAcceleration(500);
  motor2.setSpeed(1000);
  motor2.setCurrentPosition(0);

  // Set initial motor configuration for motor3
  motor3.setMaxSpeed(2000);
  motor3.setAcceleration(1000);

  // Set initial motor configuration for motor4
  motor4.setMaxSpeed(2000);
  motor4.setAcceleration(1000);

  // Start serial communication
  Serial.begin(9600);
}

void loop()
{
  // Read the encoder state for motor1
  int stateA1 = digitalRead(encoderPinA1);
  int stateB1 = digitalRead(encoderPinB1);
  int stateZ1 = digitalRead(encoderPinZ1);

  // Read the encoder state for motor2
  int stateA2 = digitalRead(encoderPinA2);
  int stateB2 = digitalRead(encoderPinB2);
  int stateZ2 = digitalRead(encoderPinZ2);

  // Send encoder states to Serial Plotter
  Serial.print(stateA1);
  Serial.print("\t");
  Serial.print(stateB1);
  Serial.print("\t");
  Serial.print(stateZ1);
  Serial.print("\t");
  Serial.print(stateA2);
  Serial.print("\t");
  Serial.print(stateB2);
  Serial.print("\t");
  Serial.println(stateZ2);

  // Motor control logic for motor1
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'u') { // 'u' for "up"
      motor1.move(1);
    } else if (command == 'd') { // 'd' for "down"
      motor1.move(-1);
    } else if (command == 's') { // 's' for "stop"
      motor1.stop();
    }
  }
  motor1.run();

  // Motor control logic for motor2
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'r') { // 'r' for "right"
      motor2.move(1);
    } else if (command == 'l') { // 'l' for "left"
      motor2.move(-1);
    } else if (command == 's') { // 's' for "stop"
      motor2.stop();
    }
  }
  motor2.run();

  // Check for user input
  if (Serial.available() > 0) {
    char command = Serial.read();

    // Start the motors
    if (command == 'q') {
      startMotors();
    }

    // Stop the motors
    if (command == 'w') {
      stopMotors();
    }

    // Move motors downward
    if (command == 'e') {
      moveDown();
    }
  }

  // Run the motors
  if (motorsRunning) {
    motor3.runSpeed();
    motor4.runSpeed();
  }

  // Additional program logic can go here
  // Note: Keep loop() as short and fast as possible
}

// Function to start the motors
void startMotors()
{
  motorsRunning = true;
  motor3.setSpeed(100);
  motor4.setSpeed(100);
  motor3.setPinsInverted(true);  // Invert the direction for motor3
  motor4.setPinsInverted(true);  // Invert the direction for motor4
  motor3.enableOutputs();
  motor4.enableOutputs();
}

// Function to stop the motors
void stopMotors()
{
  motorsRunning = false;
  motor3.setSpeed(0);
  motor4.setSpeed(0);
  motor3.runSpeedToPosition();
  motor4.runSpeedToPosition();
  motor3.disableOutputs();
  motor4.disableOutputs();
}

// Function to move motors downward
void moveDown()
{
  motorsRunning = true;
  motor3.setSpeed(100);  // Set negative speed for downward movement
  motor4.setSpeed(100);  // Set negative speed for downward movement
  motor3.setPinsInverted(false);  // Restore normal direction for motor3
  motor4.setPinsInverted(false);  // Restore normal direction for motor4
  motor3.enableOutputs();
  motor4.enableOutputs();
}

// Interrupt service routine for encoder of motor1
void handleEncoderInterrupt1()
{
  if (digitalRead(encoderPinA1) == digitalRead(encoderPinB1)) {
    count1++;
  } else {
    count1--;
  }
}

// Interrupt service routine for index of motor1
void handleIndexInterrupt1()
{
  indexTriggered1 = true;
}

// Interrupt service routine for encoder of motor2
void handleEncoderInterrupt2()
{
  if (digitalRead(encoderPinA2) == digitalRead(encoderPinB2)) {
    count2++;
  } else {
    count2--;
  }
}

// Interrupt service routine for index of motor2
void handleIndexInterrupt2()
{
  indexTriggered2 = true;
}