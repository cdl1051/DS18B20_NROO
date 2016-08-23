#include "Arduino.h"

// ====================================================== Pre-Compiler Definitions ====================================================== 
// Direct port manipulation needed to conduct the OneWire bus
#define   PowerPin              PB4                       // Pin 12 - we will be using this pin to supply Vcc to the DS18B20
#define   POWER_TEMP_PROBE      PORTB |= _BV(PowerPin)    // Define method for powering the DB18B20
#define   DEPOWER_TEMP_PROBE    PORTB &= ~_BV(PowerPin)   // Define method for depowering the DB18B20
#define   Pin                   PB5                       // Set up pin 13 as the data pin
#define   DIRECT_MODE_OUTPUT    DDRB |= _BV(Pin)          // Much faster and smaller version of pinMode(Pin, OUTPUT)
#define   DIRECT_MODE_INPUT     DDRB &= ~_BV(Pin)         // Much faster and smaller version of pinMode(Pin, INPUT)
#define   DIRECT_WRITE_HIGH     PORTB |= _BV(Pin)         // Much faster and smaller version of digitalWrite(Pin, HIGH)
#define   DIRECT_WRITE_LOW      PORTB &= ~_BV(Pin)        // Much faster and smaller version of digitalWrite(Pin, LOW)
#define   DIRECT_READ           PINB & _BV(Pin) ? 1 : 0   // One line if else statement using the format [test ? true return : false return]

// Delay values needed for conducting a OneWire bus
#define   clk_div               1                         // This code assumes a processor frequency of 16MHz but this can be lowered as long as clk_div is updated
#define   DELAY_A               6/clk_div                 // Delay values obtained from http://www.maximintegrated.com/app-notes/index.mvp/id/126
#define   DELAY_B               64/clk_div
#define   DELAY_C               60/clk_div
#define   DELAY_D               10/clk_div
#define   DELAY_E               9/clk_div
#define   DELAY_F               55/clk_div
#define   DELAY_G               0/clk_div
#define   DELAY_H               480/clk_div
#define   DELAY_I               72/clk_div
#define   DELAY_J               410/clk_div

// DS18B20 command codes
#define   READROM               0x33                      // Read the ROM of a OneWire device; there must only be one OneWire device on the bus!
#define   STARTCONVO            0x44                      // Tells device to take a temperature reading and put it on the scratchpad
#define   READSCRATCH           0xBE                      // Read from the scratchpad
#define   WRITESCRATCH          0x4E                      // Write to the scratchpad
#define   COPYSCRATCH           0x48                      // Tells the DS18B20 to copy the contents of the scratchpad to EEPROM
#define   SKIPROM               0xCC                      // Tells all OneWire sensors on the bus that the next command applies to them
#define   MATCHROM              0x55                      // Tells all OneWire sensors on the bus to listen for a specific ROM next

class DS18B20 {
  public:
    DS18B20();
    void SetResolution(uint8_t);
    void StartConversion();
    uint16_t GetTemperature();    // Returns the 10x(temperature in F with one decimal place) as an integer to avoid floats
    
  private:
    void reset();
    uint8_t read();
    void write(uint8_t);
};
