const int MAXPOS = 8;
const int POSWIDTH = 25;
const int MAXSTEPS = 200;

volatile boolean editMode = 0;

// interrupt pins (on Mega!!)
int encoderPin1 = 2;
int encoderPin2 = 3;
int buttonPin = 4; //push button

// rotary encoder wheel stuff
volatile int lastEncoded = 0;
volatile int encoderValue = 1;
int lastencoderValue = 0;

byte pos = 1;
byte lastPos = 1;

int lastMSB = 0;
int lastLSB = 0;

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

void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  pinMode(buttonPin, INPUT);


  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  digitalWrite(buttonPin, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
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

void loop(){ 
  pos = encoderValue/POSWIDTH;
  if (lastPos != pos) {
    lastPos = pos;
    sevenSegWrite(pos+1);
  }
}


void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
    if (encoderValue >= MAXSTEPS) encoderValue = MAXSTEPS-1;
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue --;
    if (encoderValue < 0) encoderValue = 0;
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
  
}
