#include "galvo-swfs.h"

const double MAX_MOVEMENT_V=1;
int debug_prints=0;

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
    double dx = x - this->x;
    double dy = y - this->y;
    double distance = std::sqrt(dx * dx + dy * dy);

    // Close enough! Call it done.
    if (distance < 1e-5) {
        return 1;
    }

    double ratio = max_movement_v / distance;
    double xnew = this->x + dx * ratio;
    double ynew = this->y + dy * ratio;

    if (distance < max_movement_v) {
      xnew = x;
      ynew = y;
    }

    // For debugging the position:
    if (debug_prints) {
      Serial.print( xnew ); Serial.print(" "); Serial.println( ynew );
    };

    this->x = xnew;
    this->y = ynew;

    move_both( this->x, this->y); // ACtually do the physical move
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

  // These would happen once at startup:
  //theGalvo.moveto(-3,-3);
  //theGalvo.moveto(5,5);
  //theGalvo.moveto(0,0);
}

// These are called continuously:
double square_radius=0.5;
void loop() {
  loopManual();

  if (square_radius>0) {
    theGalvo.moveto(-square_radius,-square_radius);
    theGalvo.moveto(-square_radius, square_radius);
    theGalvo.moveto( square_radius, square_radius);
    theGalvo.moveto( square_radius,-square_radius);
  };
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

    } else if (incomingChar == 's') {
      square_radius = atof(incomingString.c_str());
    } else if (incomingChar == 'd') {
      debug_prints = (debug_prints==0);
    } else {

      // Append the character to our string
      incomingString += incomingChar;
    }
  }
}
