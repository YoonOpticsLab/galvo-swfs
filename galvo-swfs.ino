#include "galvo-swfs.h"

const double MAX_MOVEMENT_V=1;

class Galvo {
  public:
    double x=0;
    double y=0;
    double max_movement_v=0;
    
  Galvo(double max_movement) {
    this->x=0;
    this->y=0;
    this->max_movement_v = max_movement;
  };
  
 int moveto1(double x,double y) {
    double dx = this->x - x;
    double dy = this->y - y;
    double distance = std::sqrt(dx * dx + dy * dy);

    // Close enough!
    if (distance < 1e-9) {
        return 1;
    }

    double ratio = max_movement_v / distance;
    double xnew = this->x + dx * ratio;
    double ynew = this->y + dy * ratio;
    Serial.println( xnew, ynew);
    this->x = xnew;
    this->y = ynew;
    return 0;
 };

 void moveto(double x, double y) {
  int done;
   do {
      done = moveto1( x, y);
   } while (!done);
 };
 
};

Galvo theGalvo(MAX_MOVEMENT_V);

void setup() {
  Serial.begin(115200);
  initDAC(0);
  Serial.print("Galvo sWFS ready."); Serial.println();
}

void loop() {
  loopManual();

  theGalvo.moveto(0,0);
  theGalvo.moveto(5,5);
}

// Manual version: This takes commands and moves immediately
// 0A 0B goes to that voltage
// 0a 0b goes to the amount in 16bit values. Corners are at 0a 0b and 65535a 65535b.
void loopManual() {
  String incomingString="";

  // Check if there is data in the serial buffer
  while (Serial.available() > 0) {
    char incomingChar = Serial.read(); // Read one character

    if (incomingChar == 'A') {
      float myFloat = atof(incomingString.c_str());
      dac8562Write(0, 0x18, volts_to_bitsA(myFloat));  // DAC A

      Serial.print("Sending ");
      Serial.println(myFloat);

      // Clear the string for the next message
      incomingString = "";
      
    } else if (incomingChar == 'B') {
      float myFloat = atof(incomingString.c_str());
      dac8562Write(0, 0x19, volts_to_bitsB(myFloat));  // DAC B

      Serial.print("Sending ");
      Serial.println(myFloat);

      // Clear the string for the next message
      incomingString = "";

    // Lower case: integer (to test absolute values)
    } else if (incomingChar == 'a') {
      uint32_t myInt = atoi(incomingString.c_str());
      dac8562Write(0, 0x18, myInt);  // DAC A

      Serial.print("Sending ");
      Serial.println(myInt);

      // Clear the string for the next message
      incomingString = "";
    } else if (incomingChar == 'b') {
      uint32_t myInt = atoi(incomingString.c_str());
      dac8562Write(0, 0x19, myInt);  // DAC B

      Serial.print("Sending ");
      Serial.println(myInt);

      // Clear the string for the next message
      incomingString = "";

    } else {

      // Append the character to our string
      incomingString += incomingChar;
    }
  }
}
