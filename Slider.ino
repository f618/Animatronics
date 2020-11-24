//Libs
#include <i2cMaster.h>
#include <uStepperSLite.h>
#include <TMC2208.h>
#include <uStepperServo.h>
#include <ezButton.h>

//Pot
const int analogInPin = A3;
int sensorValue = 0;
int outputValue = 0;

int lastoutputValue = 0;

//Button Values
ezButton button1(0);  // create ezButton object that attach to pin 6;
ezButton button2(2);  // create ezButton object that attach to pin 7;

//Stepper Values
#define MAXACCELERATION 1000         //Max acceleration = 1500 Steps/s^2
#define MAXVELOCITY 1000           //Max velocity = 1100 steps/s
uStepperSLite stepper;

void setup() {
  //Global Setup
  Serial.begin(9600);
  Serial.println("JAILMAKE");
  Serial.println("Slider Online..");

  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 1, 1000);
  stepper.setMaxAcceleration(outputValue / 1.1);
  stepper.setMaxVelocity(outputValue);
  Serial.println(outputValue);

  //Button Setup
  button1.setDebounceTime(50); // set debounce time to 50 milliseconds
  button2.setDebounceTime(50); // set debounce time to 50 milliseconds

  //Stepper Setup
  stepper.setup();
  stepper.runContinous(CCW);
  Serial.println("Rotating Counter Clockwise");
  Serial.println(">>>");

}

void loop() {

  sensorValue = analogRead(analogInPin);
  outputValue = (map(sensorValue, 0, 1023, 1, 5000) + 50) / 100 * 100;
  
  if (outputValue != lastoutputValue) {
    lastoutputValue = outputValue;
    stepper.setMaxAcceleration(outputValue * 1.1);
    stepper.setMaxVelocity(outputValue);
    Serial.print("SPEED CHANGED: ");
    Serial.println(outputValue * 2);
  }
  delay(50);

  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first

  int btn1State = button1.getState();
  int btn2State = button2.getState();

if (button1.isPressed()) {
    stepper.runContinous(CCW);
    Serial.println("Rotating: COUNTER CLOCKWISE");
    Serial.println(">>>");
  }


  if (button2.isPressed()) {
    stepper.runContinous(CW);
    Serial.println("Rotating: CLOCKWISE");
    Serial.println(">>>");
  }

}
