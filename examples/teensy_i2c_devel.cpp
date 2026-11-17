// Arduino Libraries
#include "Arduino.h"

//#include "Wire.h"
#include "i2c_t3.h"

// C/C++ Libraries
#include "stdio.h"

// Custom Libraries
#include "bitwise.h"
#include "TeensyI2C.h"
//#include "teensy_i2c.h"

// Function Prototypes
void timer1ISR(void);
uint16_t readTA(TeensyI2C& my_i2c_);

// Objects
IntervalTimer timer1;
uint32_t timer1_period = 1000;		// timer period [ms]
TeensyI2C my_i2c0(&Wire);
TeensyI2C my_i2c1(&Wire1);
//TeensyI2C my_i2c0(Wire);
//TeensyI2C my_i2c1(Wire1);

// Pin Assignments

// Global Variables
const uint8_t default_MCP9808_address = 0b0011000;
const uint8_t CONFIG = 0b00000001;
//const uint8_t CONFIG = 0x04;
const uint8_t T_UPPER = 0b00000010;
const uint8_t T_LOWER = 0b00000011;
const uint8_t T_CRIT = 0b00000100;
const uint8_t T_A = 0b00000101;

// This script reads the MPL115A1 barometric sensor to confirm the proper
// 	implementation of the SPI functions

// TODO
// - Implement usingInterrupt() for stability
// - Convert to class
// - Use two different settings on same bus

void setup(){
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000); // SCL = 19, SDA = 18, 100 kHz
	// Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_37_38, I2C_PULLUP_EXT, 100000); // SCL = 37, SDA = 38, 100kHz
	//Wire.begin(); // default SCL = 19, SDA = 18, 400 kHz
	//Wire.begin(I2C_MASTER, 0x00, 17, 16); // SCL = 17, SDA = 16, 400 kHz
	//Wire.setClock(100000); // set clock to 100 kHz

	Serial.begin(115200);
	delay(100);

	timer1.begin(timer1ISR, timer1_period*1000);  // start program/timer ISR
}

void loop(){}

void timer1ISR(void){
	//uint16_t TA = readTA(my_i2c0);
	uint16_t TA = readTA(my_i2c1);

	Serial.println(TA);
}

uint16_t readTA(TeensyI2C& my_i2c_){
	uint8_t slave_address = default_MCP9808_address;
	uint8_t rx_bytes[2] = {};
	my_i2c_.i2cMasterRead(slave_address, T_A, rx_bytes, 2);
	return (rx_bytes[0]) | rx_bytes[1];
}
