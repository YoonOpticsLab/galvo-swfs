#include <SPI.h>
#define CS_PIN 10

const double V_max_A =  10.35;
const double V_min_A = -10.37;

const double V_max_B =  10.35;
const double V_min_B = -10.333;

void dac8562Write(int mode, uint8_t cmd, uint16_t value) {
  SPISettings settings;
  switch(mode) {
    case 0: settings = SPISettings(1000000, MSBFIRST, SPI_MODE0); break;
    case 1: settings = SPISettings(1000000, MSBFIRST, SPI_MODE1); break;
    case 2: settings = SPISettings(1000000, MSBFIRST, SPI_MODE2); break;
    case 3: settings = SPISettings(1000000, MSBFIRST, SPI_MODE3); break;
  }
  SPI.beginTransaction(settings);
  digitalWrite(CS_PIN, LOW);
  delayMicroseconds(10);
  SPI.transfer(cmd);
  SPI.transfer16(value);
  delayMicroseconds(10);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  delayMicroseconds(100);
}

uint32_t volts_to_bitsA(double V)
{
  double range = (V_max_A - V_min_A);
  double value = (V-V_min_A)/range * 0xffff + 0.5;

  return (uint32_t)value;
}

uint32_t volts_to_bitsB(double V)
{
  double range = (V_max_B - V_min_B);
  double value = (V-V_min_B)/range * 0xffff + 0.5;

  return (uint32_t)value;
}

void tryInit(uint8_t mode) {
  dac8562Write(mode, 0x28, 0x0001);  // reset
  delay(50);
  dac8562Write(mode, 0x20, 0x0003);  // power up
  dac8562Write(mode, 0x38, 0x0001);  // internal ref
  dac8562Write(mode, 0x02, 0x0000);  // gain
  dac8562Write(mode, 0x18, volts_to_bitsA(0));  // DAC A
  dac8562Write(mode, 0x19, 0xffff);  // DAC B
}

void setup() {
  Serial.begin(115200);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();
  delay(500);

  Serial.print("Ok so far"); Serial.println(0);
  tryInit(0);
}

void loop() {
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