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

volatile boolean randPos = 0;
volatile int currentPos = 0;
volatile int nextPos = 0;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  while (randPos && nextPos == currentPos) { 
    nextPos = random(MAXPOS);
  }
  
  if (nextPos > MAXPOS-1) {
    nextPos = MAXPOS -1;
  }
  
  if (currentPos != nextPos) {
    sendPos(nextPos);
  }
}

void serialEvent() {
  while(Serial.available()) {
   nextPos = Serial.read() - 48; // pos sent as char
  }
}

void sendPos(byte pos) {
    Wire.beginTransmission(16); // transmit to a device
    Serial.println(pos, DEC);
    Wire.write(pos);          // sends one byte  
    Wire.endTransmission();    // stop transmitting
    currentPos = pos;
    delay(1000);
}
