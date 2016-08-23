//#include <uLCD.h>
//#include <util/delay.h>
#include "DS18B20.h"
  
DS18B20 Sensor;   // Insert the three legs of the sensor into GND, Pin 13, and Pin 12.  No resistor is needed, we will use internal pullups instead.
//uLCD lcd;

int main() {  
  uint16_t temp;
  
  // Timer 2 initialization from wiring.c for an ATmega 328P (Arduino Uno rev 3) + 20 bytes to sketch size
  TCCR2A |= _BV(COM2A1) | _BV(WGM20);    // Enable timer 2 to _delay_ms() works properly
  TCCR2B |= CS22;                        // set clkT2S/64 (From prescaler)
  OCR2A = 40;                            // turn the LED on at 0.1% of full intensity
  
  DDRB = B00011000;      // Set Pin 12 as an output
  POWER_TEMP_PROBE;      // This isn't neccesary but I show it to demonstrate how to make the project conserve energy
  
  // Set the sensor's resolution to 11 bits
  Sensor.SetResolution(12);
  //lcd.write("Room Temp:");
  
  for(;;) {  // Loop forever
    Sensor.StartConversion();
    // Do other work for 100ms to 600ms or else sleep for that amount of time
    temp = Sensor.GetTemperature(); 
    //lcd.setCursor(0,1);
    //lcd.write(temp);
      
    _delay_ms(4000);
  }// End for
} // End main 

// Converts the raw temperature from a DS18B20 directly to a string containing the temperature in °F with 1 decimal place
// avoids unnecessary floating point math, float variables, and casts, and 32-bit math
// TODO: May not work properly with temperatures below 32°F
/*static inline void txRawTempAsFloat(uint16_t raw) {  
  char buffer[6];

  uint8_t decimalPos = 2;  // default case of a temp between 0 and 99.9
  uint8_t nullPos = 4;     // default case of a temp between 0 and 99.9
  uint16_t temp;
  
  // Check to see if the temperature passed in is negative
  if (raw>>11) {
    // Can't get here unless one of the 5 most-significant bits are ones which means we have a negative number, convert it
    raw = ~(raw-1);    // Convert the two's compliment number back into one's compliment
    
    if (raw > 284) {  // This temperature is far enough negative in the celcius scale that it is also negative on the farhenheit scale
      decimalPos += 1;   // Account for the negative sign's place in the string
      nullPos += 1;      // Account for the negative sign's place in the string
      buffer[0] = '-';   // Write the negative sign in the string
    }
    temp = (9*raw)/8-320;                   // Keeps only 1 decimal place but uses 16-bit math
  } else {
    temp = (9*raw)/8+320;                   // Keeps only 1 decimal place but uses 16-bit math
  }


  // Convert the raw temperature into the temperature in Fx10 so that one decimal place is kept
  //uint32_t temp = (raw*1125ul+320000ul)/1000ul;  // Keeps all 4 decimal places but uses 32-bit math  
  if(temp>=1000) {  // We're looking at a positive number with three digits
    decimalPos += 1;
    nullPos += 1;
  }
  
  buffer[nullPos--] = '\0';
  do {
    if (nullPos==decimalPos) buffer[nullPos--] = '.';
    buffer[nullPos--] = temp % 10 + '0';
    temp /= 10;
  } while (temp);  
  
  simpletx(buffer);
}*/
