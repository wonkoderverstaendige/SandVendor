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
#include <Stepper.h>
const int pwmA = 3;
const int pwmB = 11;
const int brakeA = 9;
const int brakeB = 8;
const int dirA = 12;
const int dirB = 13;
Stepper myStepper(STEPS, dirA, dirB);

const int STEPS = 200;   // Steps per revolution
const int MAXPOS = 8;    // Number of stops
const int POS_STEP = 25; // POS_STEP = STEPS / MAXPOS

//#define DEV_ID 2
volatile int currentPos = 0;
volatile int properPos = 0;

void setup()
{
  // join i2c bus with address
  Wire.begin(16);                
  Wire.onReceive(receiveEvent);

  // Set the RPM of the motor
  // Around 75 is the max without a load
  // lower is with load!
  myStepper.setSpeed(75);
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
  
  Serial.begin(9600);
}
  
void loop()
{
  if (properPos > MAXPOS-1) {
    properPos = MAXPOS -1;
  }
    if (currentPos != properPos) {
    moveMotor(properPos);
  }
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
}

void moveMotor(int endPos) {
  Serial.println("Moving...");
  // Move the motor X amount of steps
  int steps = endPos-currentPos;
//  Serial.println(steps, DEC);
  myStepper.step(steps*POS_STEP);
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
