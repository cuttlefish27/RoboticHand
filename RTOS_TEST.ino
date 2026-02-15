#include <AccelStepper.h>
#include <MultiStepper.h>
#include <ESP32Servo.h>


#define STEP_PIN 33
#define DIR_PIN 32
#define ForwardPin 13
#define BackwardPin 15
#define RIGHTPIN 22
#define LEFTPIN 23
#define POTRIGHT 34
#define POTLEFT 35

//const float stepsPerRevolution = 200;
//AccelStepper stepper(AccelStepper::DRIVER, 32, 33);

//int stateForward = 0;
//int stateBack = 0;

Servo left; 
Servo right;



//float convert_rotational_position_to_steps(float rotations) {
 // return rotations * stepsPerRevolution;
//}

TaskHandle_t ServoRTaskHandle = NULL;
TaskHandle_t ServoLTaskHandle = NULL;
TaskHandle_t ADCTaskHandle = NULL;
QueueHandle_t potQueue;

void ServoR(void *parameter) {
  int input;
  for(;;) {
    if(xQueuePeek(potQueue, &input, portMAX_DELAY)){
      int rServoAngle = map(input, 0, 4095, 175, 10);
      right.write(rServoAngle);
    }
  }
}

void ServoL(void *parameter) {
  int input;
  for(;;) {
    if (xQueuePeek(potQueue, &input, portMAX_DELAY)) {
      int lServoAngle = map(input, 0, 4095, 10, 175);
      Serial.println(lServoAngle);
      left.write(lServoAngle);
    }
  }
}

void ADC(void *parameter) {
  int pot;
  for(;;) {
    pot = analogRead(POTLEFT);

    xQueueOverwrite(potQueue, &pot);

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}



void setup() {

  Serial.begin(9600);
  // put your setup code here, to run once:
 // float MAXRPM = 300;
 // float Max_Speed_StepsPerSec = stepsPerRevolution * MAXRPM /60;
 // stepper.setMaxSpeed(Max_Speed_StepsPerSec);

// float AccelRPMperSec = 60;
// float Accel_StepsPerSec = stepsPerRevolution * AccelRPMperSec / 60;
  //stepper.setAcceleration(Accel_StepsPerSec);

  //stepper.setCurrentPosition(0);
  //pinMode(ForwardPin, INPUT_PULLUP);
  //pinMode(BackwardPin, INPUT_PULLUP);

  //stepper.setMinPulseWidth(5);

  right.attach(RIGHTPIN);
  left.attach(LEFTPIN);
  analogSetPinAttenuation(POTLEFT, ADC_11db);
  analogSetPinAttenuation(POTRIGHT, ADC_11db);

  potQueue = xQueueCreate(
      1,              // max 10 items in queue
      sizeof(int)      // each item is an int
  );

// xTaskCreatePinnedToCore(
//   ServoR,             // Task function
//   "ServoR",           // Task name
//    20000,             // Stack size (bytes)
//    NULL,              // Parameters
//    1,                 // Priority
//    &ServoRTaskHandle,      // Task handle
//    1                  // Core 1
//  );
  xTaskCreatePinnedToCore(
    ADC,
    "Potentiometer Read",
    20000,
    NULL,
    1,
    &ADCTaskHandle,
    0
  );

  xTaskCreatePinnedToCore(
    ServoL,             // Task function
    "ServoL",           // Task name
    40000,             // Stack size (bytes)
    NULL,              // Parameters
    1,                 // Priority
    &ServoLTaskHandle,      // Task handle
    1                  // Core 1
  );

  xTaskCreatePinnedToCore(
    ServoR,             // Task function
    "ServoR",           // Task name
    40000,             // Stack size (bytes)
    NULL,              // Parameters
    1,                 // Priority
    &ServoRTaskHandle,      // Task handle
    1                  // Core 1
  );


}

void loop() {

  //stateForward = digitalRead(ForwardPin);
  //stateBack = digitalRead(BackwardPin);
  // put your main code here, to run repeatedly:
//  if(stateForward == LOW && stateBack == HIGH)  {
//    stepper.move(convert_rotational_position_to_steps(0.1));
//  }
//  else if(stateForward == HIGH && stateBack == LOW) {
//    stepper.move(convert_rotational_position_to_steps(-0.1));
//  }
  
//  stepper.run();
}