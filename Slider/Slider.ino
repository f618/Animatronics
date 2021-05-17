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

//Speaker
const int speaker = 5;

//Button Values
ezButton button1(0);  //
ezButton button2(2);  //

//Stepper Values
#define MAXACCELERATION 5000
#define MAXVELOCITY 5000

float stallSensitivity = 0.99;
uStepperSLite stepper;

void setup() {
  //Global Setup
  Serial.begin(9600);
  Serial.println("JAILMAKE");
  Serial.println("Slider Online..");

  //Pot Setup
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 1, 1000);
  stepper.setMaxAcceleration(outputValue / 1.1);
  stepper.setMaxVelocity(outputValue);
  Serial.println(outputValue);

  //Speaker Setup
  pinMode(speaker, OUTPUT);
  tone(speaker, 1000);
  delay(100);
  tone(speaker, 2000);
  delay(100);
  noTone(speaker);

  //Button Setup
  button1.setDebounceTime(25); // set debounce time to 50 milliseconds
  button2.setDebounceTime(25); // set debounce time to 50 milliseconds

  //Stepper Setup
  stepper.setup();
  stepper.setMaxAcceleration(5000);
  stepper.setMaxVelocity(5000);
  stepper.runContinous(CCW);
  Serial.println("Rotating Counter Clockwise");
  Serial.println(">>>");
  delay(1000);
  delay(1000);
  stepper.runContinous(CW);
  Serial.println("Rotating Clockwise");
  Serial.println(">>>");

}

void loop() {

  sensorValue = analogRead(analogInPin);
  outputValue = (map(sensorValue, 0, 1023, 1, 10) * 1000);

  if (outputValue != lastoutputValue) {
    lastoutputValue = outputValue;
    stepper.setMaxAcceleration(outputValue * 1.1);
    stepper.setMaxVelocity(outputValue);
    Serial.print("SPEED CHANGED: ");
    Serial.println(outputValue);
    tone(speaker, 100);
    delay(50);
    noTone(speaker);
    delay(100);
  }

  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first

  int btn1State = button1.getState();
  int btn2State = button2.getState();

  if (button1.isPressed()) {
    stepper.hardStop(HARD);
    stepper.runContinous(CCW);
    Serial.println("Rotating: COUNTER CLOCKWISE");
    Serial.println(">>>");
  }

  if (button2.isPressed()) {
    stepper.hardStop(HARD);
    stepper.runContinous(CW);
    Serial.println("Rotating: CLOCKWISE");
    Serial.println(">>>");
    delay(50);
  }

  if (stepper.isStalled(stallSensitivity) != 1) {
    Serial.println("!!! Stall Detected !!!");
    tone(speaker, 500);
    delay(100);
    noTone(speaker);

  }

  if (stepper.getMotorState() == 1) {
    Serial.println("!!! Motor Not Running !!!");
  }


}
