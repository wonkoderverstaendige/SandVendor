/* ******* SAND VENDOR SLAVE ******* 
  Reveives position data from Master via I2C,
  or via Serial for debugging purposes. 
  Position data should be sent as byte of
  ASCII value of position number. I.e. position 1
  has byte value 49.

  Moves stepper motor to new position relative to
  Last known state. If not starting from zero,
  will end up offset!
  
  Created April 2013
  Esther Holleman & Ronny Eichler
  
*/


// I2C communication
#include <Wire.h>

// Setup stepper control
#include <AccelStepper.h>
const int pwmA = 3;
const int pwmB = 11;
const int brakeA = 9;
const int brakeB = 8;
const int dirA = 12;
const int dirB = 13;

const int STEPS = 200;   // Steps per revolution
const int MAXPOS = 8;    // Number of stops
const int POS_STEP = 25; // POS_STEP = STEPS / MAXPOS

AccelStepper stepper(AccelStepper::FULL2WIRE, dirA, dirB);;

//#define DEV_ID 2
volatile int currentPos = 0;
volatile int properPos = 0;

void setup()
{
  // join i2c bus with address
  Wire.begin(16);                
  Wire.onReceive(receiveEvent);

  // turn on PWM
  pinMode(pwmA, OUTPUT);
  digitalWrite(pwmA, HIGH);
  pinMode(pwmB, OUTPUT);
  digitalWrite(pwmB, HIGH);
  // turn brakes off
  pinMode(brakeA, OUTPUT);
  digitalWrite(brakeA, LOW);
  pinMode(brakeB, OUTPUT);
  digitalWrite(brakeB, LOW);
  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  stepper.moveTo(0);
  stepper.run();
  
  Serial.begin(9600);
}
  
void loop()
{
//  if (properPos > MAXPOS-1) {
//    properPos = MAXPOS -1;
//  }
//  if (currentPos != properPos) {
//    moveMotor(properPos);
//  }
  stepper.run();
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.print("I2C rx: ");
  while(Wire.available())
  {
    properPos = Wire.read(); // receive byte as a character
    Serial.println(properPos, DEC);
  }
  stepper.moveTo(properPos);
}

void moveMotor(int endPos) {
  Serial.println("Moving...");
  // Move the motor X amount of steps
//  int steps = endPos-currentPos;
//  Serial.println(steps, DEC);
  currentPos = endPos;
  Serial.println("Done!");
}

void serialEvent() {
  int incoming;
  while(Serial.available()) {
    properPos = Serial.read() - 48;
    Serial.print("Serial rx:");
    Serial.println(properPos, DEC); // pos sent as char
  }
}
