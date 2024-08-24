#include <AccelStepper.h>

// Rotary encoder connections for motor1
const int encoderPinA1 = 30;
const int encoderPinB1 = 32;
const int encoderPinZ1 = 28;

// Rotary encoder connections for motor2
const int encoderPinA2 = 47;
const int encoderPinB2 = 55;
const int encoderPinZ2 = 43;

// Stepper motor connections for motor1
const int dirPin1 = 2;
const int stepPin1 = 3;
#define motorInterfaceType1 1
AccelStepper motor1(motorInterfaceType1, stepPin1, dirPin1);

// Stepper motor connections for motor2
const int dirPin2 = 10;
const int stepPin2 = 11;
#define motorInterfaceType2 1
AccelStepper motor2(motorInterfaceType2, stepPin2, dirPin2);

// Variables to store encoder state for motor1
volatile int count1 = 0;
volatile bool indexTriggered1 = false;

// Variables to store encoder state for motor2
volatile int count2 = 0;
volatile bool indexTriggered2 = false;

// Motor control variables for motor1
bool isMoving1 = false;
bool isMovingUp1 = false;
bool isMovingDown1 = false;

// Motor control variables for motor2
bool isMoving2 = false;
bool isMovingRight2 = false;
bool isMovingLeft2 = false;

void setup() {
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

  // Start serial communication
  Serial.begin(9600);
}

void loop() {
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
      isMovingUp1 = true;
    } else if (command == 'd') { // 'd' for "down"
      isMovingDown1 = true;
    } else if (command == 's') { // 's' for "stop"
      isMovingUp1 = false;
      isMovingDown1 = false;
      motor1.stop();
    }
  }

  if (isMovingUp1 && !isMovingDown1) {
    if (!motor1.isRunning()) {
      // Move one step up
      motor1.moveTo(motor1.currentPosition() + 1);
    }
    motor1.run();
  } else if (!isMovingUp1 && isMovingDown1) {
    if (!motor1.isRunning()) {
      // Move one step down
      motor1.moveTo(motor1.currentPosition() - 1);
    }
    motor1.run();
  }

  // Motor control logic for motor2
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'r') { // 'r' for "right"
      isMovingRight2 = true;
    } else if (command == 'l') { // 'l' for "left"
      isMovingLeft2 = true;
    } else if (command == 's') { // 's' for "stop"
      isMovingRight2 = false;
      isMovingLeft2 = false;
      motor2.stop();
    }
  }

  if (isMovingRight2 && !isMovingLeft2) {
    if (!motor2.isRunning()) {
      // Move one step right
      motor2.moveTo(motor2.currentPosition() + 1);
    }
    motor2.run();
  } else if (!isMovingRight2 && isMovingLeft2) {
    if (!motor2.isRunning()) {
      // Move one step left
      motor2.moveTo(motor2.currentPosition() - 1);
    }
    motor2.run();
  }

  // Additional program logic can go here
  // Note: Keep loop() as short and fast as possible
}

void handleEncoderInterrupt1() {
  int newStateA1 = digitalRead(encoderPinA1);
  int newStateB1 = digitalRead(encoderPinB1);

  if (newStateA1 != newStateB1) {
    count1++;
  } else {
    count1--;
  }
}

void handleIndexInterrupt1() {
  indexTriggered1 = true;
  // Additional logic for index pulse handling can be added here
}

void handleEncoderInterrupt2() {
  int newStateA2 = digitalRead(encoderPinA2);
  int newStateB2 = digitalRead(encoderPinB2);

  if (newStateA2 != newStateB2) {
    count2++;
  } else {
    count2--;
  }
}

void handleIndexInterrupt2() {
  indexTriggered2 = true;
  // Additional logic for index pulse handling can be added here
}
