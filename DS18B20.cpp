#include "DS18B20.h"

// ============================================================================================================================================================
// Initialization routine
// Only works if there is a single DS18B20 on the one wire network
// ============================================================================================================================================================
DS18B20::DS18B20() {
}

// ============================================================================================================================================================
// Sets the temperature measurement resolution of the DS18B20 to either 9, 10, 11, or 12 bits  If any other number is passed, the sensor will be set to 12 bits
// Only works if there is a single DS18B20 on the one wire network
// ============================================================================================================================================================
void DS18B20::SetResolution(uint8_t resolution) {
  reset();
  write(SKIPROM);
  write(WRITESCRATCH);
  write(0x00);
  write(0x00);
  switch (resolution) {
    case 9: write(0x1F);  break;
    case 10: write(0x3F); break;
    case 11: write(0x5F); break;
    default: write(0x7F); break;
  }
  reset();
  write(SKIPROM);
  write(COPYSCRATCH);
  //_delay_ums(15);
}

void DS18B20::StartConversion() {
  reset();
  write(SKIPROM);
  write(STARTCONVO); 
}

uint16_t DS18B20::GetTemperature() {
  uint16_t ret;
  while(!read());  // Read until we get a 1 back from the DS18B20 meaning it is signaling complete
  reset();
  write(SKIPROM);
  write(READSCRATCH);
  ret = read();
  ret |= read()<<8;
  return ((9*ret)/8)+320;
}

uint8_t DS18B20::read() {
  uint8_t r=0;
      
  noInterrupts();
  for (uint8_t bitMask = 0x01; bitMask; bitMask <<= 1) {
    DIRECT_MODE_OUTPUT;
    DIRECT_WRITE_LOW;
    _delay_us(DELAY_A);
    DIRECT_MODE_INPUT;
    DIRECT_WRITE_HIGH;  // New line for no resistor modification / enable pull-up resistor
    _delay_us(DELAY_E);
    if (DIRECT_READ) r |= bitMask;
    _delay_us(DELAY_F);
  }
  interrupts();
  return r;
}

void DS18B20::write(uint8_t v) {
  noInterrupts();
  for (uint8_t bitMask = 0x01; bitMask; bitMask <<= 1) {
    DIRECT_WRITE_LOW;
    DIRECT_MODE_OUTPUT;
    if (bitMask & v) {
      _delay_us(DELAY_A);
      DIRECT_WRITE_HIGH;
      _delay_us(DELAY_B);
    } else {
      _delay_us(DELAY_C);
      DIRECT_WRITE_HIGH;
      _delay_us(DELAY_D);
    }
  }
  DIRECT_MODE_INPUT;
  interrupts();
}

void DS18B20::reset(void) {
  noInterrupts();
  DIRECT_MODE_INPUT;      
  DIRECT_WRITE_LOW;
  DIRECT_MODE_OUTPUT;
  _delay_us(DELAY_H);
  DIRECT_MODE_INPUT;
  DIRECT_WRITE_HIGH;  // New line for no resistor modification / enable pull-up resistor
  _delay_us(DELAY_I);
  //uint8_t ret = !(DIRECT_READ);
  interrupts();
  _delay_us(DELAY_J);
  //return ret;
}
