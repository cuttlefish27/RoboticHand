#include <AccelStepper.h>
#include <MultiStepper.h>
#include <ESP32Servo.h>


#define STEP_PIN 33
#define DIR_PIN 32
#define ForwardPin 13
#define BackwardPin 27
#define RIGHTPIN 22
#define LEFTPIN 23
#define POTRIGHT 34
#define POTLEFT 35

const float stepsPerRevolution = 200;
AccelStepper stepper(AccelStepper::DRIVER, 32, 33);
int stateForward = 0;
int stateBack = 0;
Servo left; 
Servo right;
int potright = 0;
int potleft = 0;
int rServoAngle = 0;
int lServoAngle = 0;


float convert_rotational_position_to_steps(float rotations) {
  return rotations * stepsPerRevolution;
}


void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
  float MAXRPM = 300;
  float Max_Speed_StepsPerSec = stepsPerRevolution * MAXRPM /60;
  stepper.setMaxSpeed(Max_Speed_StepsPerSec);

  float AccelRPMperSec = 60;
  float Accel_StepsPerSec = stepsPerRevolution * AccelRPMperSec / 60;
  stepper.setAcceleration(Accel_StepsPerSec);

  stepper.setCurrentPosition(0);
  pinMode(ForwardPin, INPUT_PULLUP);
  pinMode(BackwardPin, INPUT_PULLUP);
  stepper.setMinPulseWidth(5);

  right.attach(RIGHTPIN);
  left.attach(LEFTPIN);
  right.write(0);
  left.write(0);
}

void loop() {

  stateForward = digitalRead(ForwardPin);
  stateBack = digitalRead(BackwardPin);
  // put your main code here, to run repeatedly:
  if(stateForward == LOW && stateBack == HIGH)  {
    stepper.move(convert_rotational_position_to_steps(0.1));
  }
  else if(stateForward == HIGH && stateBack == LOW) {
    stepper.move(convert_rotational_position_to_steps(-0.1));
  }
  
  stepper.run();
  potright = analogRead(POTRIGHT);
  potleft = analogRead(POTLEFT);

  rServoAngle = map(potright, 0, 4095, 10, 170);
  lServoAngle = map(potleft, 0, 4095, 10, 170);

  right.write(rServoAngle);
  left.write(lServoAngle);


}
