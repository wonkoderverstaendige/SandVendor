/* ******* SAND VENDOR MASTER ******* 
  Send position data to Slave via I2C.
  Position received via Serial, or by setting the
  randPos flag true, to random positions.
  
  Position data should be sent via Serial as byte of
  ASCII value of position number. I.e. position 1
  has byte value 49.

  Slaves will move stepper motor to new position relative
  to last known state. If not starting from zero,
  will end up offset!
  
  Created April 2013
  Esther Holleman & Ronny Eichler
  
*/

#include <Wire.h>
#define MAXPOS 8
#define POSWIDTH 25
#define MAXSTEPS 200

volatile int currentPos = 0;

// interrupt pins (on Mega!!)
int encoderPin1 = 2;
int encoderPin2 = 3;
int buttonPin = 4; //push button

// rotary encoder wheel stuff
volatile int lastEncoded = 0;
volatile int encoderValue = 1;
volatile boolean editMode = 0;
int lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;
byte pos = 1;
byte lastPos = 1;
byte lastValue = 0;

// seven segment display stuff
const byte dotPin = 12;
const byte digits[10][7] = {{0,0,0,0,0,0,1},  // = 0
                            {1,0,0,1,1,1,1},  // = 1
                            {0,0,1,0,0,1,0},  // = 2
                            {0,0,0,0,1,1,0},  // = 3
                            {1,0,0,1,1,0,0},  // = 4
                            {0,1,0,0,1,0,0},  // = 5
                            {0,1,0,0,0,0,0},  // = 6
                            {0,0,0,1,1,1,1},  // = 7
                            {0,0,0,0,0,0,0},  // = 8
                            {0,0,0,1,1,0,0}};   // = 9
const byte digitPins[8] =   {5,6,7,8,9,10,11,dotPin}; 

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  digitalWrite(buttonPin, HIGH); //turn pullup resistor on
  
  //call when any high/low changed seen
  //on interrupt 0 (pin 2), interrupt 1 (pin 3), interrupt 4 (pin 19)
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
  attachInterrupt(5, buttonPress, CHANGE);
  
  // seven segment display pins
  for (byte i = 0; i < 7; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }
  pinMode(dotPin, OUTPUT);
  digitalWrite(dotPin, HIGH);
  sevenSegWrite(pos);
}

void loop() {
  pos = encoderValue/POSWIDTH;
  if (lastPos != pos) {
    lastPos = pos;
    sevenSegWrite(pos+1);
  }
  if (lastValue != encoderValue) {
    Serial.println(encoderValue, DEC);
    lastValue = encoderValue;
    sendPos(encoderValue);
  }
}

void serialEvent() {
  while(Serial.available()) {
    byte nextPos = Serial.read() - 48;
    if (nextPos < 0) nextPos = 0;
    if (nextPos > MAXPOS) nextPos = MAXPOS;
   encoderValue = (nextPos)*25-12; // pos sent as char
  }
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue +=5;
    if (encoderValue >= MAXSTEPS) encoderValue = 0;
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue -=5;
    if (encoderValue < 0) encoderValue = MAXSTEPS-5;
  }
  lastEncoded = encoded; //store this value for next time
}

void sevenSegWrite(byte digit) {
  byte pin = 5;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin+segCount, digits[digit][segCount]);//digitPins[segCount]
  }
}

void writeDot(byte dot) {
  digitalWrite(12, dot);
}

void buttonPress() {
  Serial.println("Pressed!");
}

void sendPos(byte data) {
    Wire.beginTransmission(16); // transmit to a device
    Wire.write(data);          // sends one byte  
    Wire.endTransmission();    // stop transmitting
    currentPos = data;
}
