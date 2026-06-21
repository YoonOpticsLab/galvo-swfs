#include "galvo-swfs.h"

void setup() {
  Serial.begin(115200);
  initDAC(0);
  Serial.print("Galvo sWFS ready."); Serial.println();
}

void loop() {
  loopManual();
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
