#define CS_PIN 10

const double V_max_A =  10.35;
const double V_min_A = -10.37;

const double V_max_B =  10.35;
const double V_min_B = -10.333;

void initDAC(uint8_t mode);
void dac8562Write(int mode, uint8_t cmd, uint16_t value);
uint32_t volts_to_bitsA(double V);
uint32_t volts_to_bitsB(double V);

void move_both(double A, double B);