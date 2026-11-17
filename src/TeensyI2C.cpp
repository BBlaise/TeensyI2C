/*
 * TeensyI2C.cpp
 *
 *  Created on: Jun 3, 2020
 *      Author: Bryan
 */

#include "TeensyI2C.h"

// Store the I2C bus supplied and initialized by the caller.
TeensyI2C::TeensyI2C(i2c_t3* p_bus_) : p_bus(p_bus_) {}
// TeensyI2C::TeensyI2C(TwoWire* p_bus_) : p_bus(p_bus_) {}

// Leave the caller-owned I2C bus in place.
TeensyI2C::~TeensyI2C() {}

// Write the register address and one data byte in a single transaction.
void TeensyI2C::i2cMasterWrite(uint8_t slave_address, uint8_t register_address, uint8_t tx_byte) {
	p_bus->beginTransmission(slave_address);
	p_bus->write(register_address);
	p_bus->write(tx_byte);
	p_bus->endTransmission();
}

// Queue the register address followed by the payload bytes, then transmit.
void TeensyI2C::i2cMasterWrite(uint8_t slave_address, uint8_t register_address, uint8_t* tx_bytes, int n_tx_bytes) {
	p_bus->beginTransmission(slave_address);
	p_bus->write(register_address);

	for (int ii = 0; ii < n_tx_bytes; ii++) {
		p_bus->write(tx_bytes[ii]);
	}
	p_bus->endTransmission();
}

// Write the register address, end with a STOP, and request one byte.
uint8_t TeensyI2C::i2cMasterRead(uint8_t slave_address, uint8_t register_address) {
	  p_bus->beginTransmission(slave_address);
	  p_bus->write(register_address);
	  p_bus->endTransmission();

	  p_bus->requestFrom(slave_address, (uint8_t)1);
	  // Return zero if no byte arrives, which is also a valid register value.
	  uint8_t rx_byte = 0;
	  if (p_bus->available() > 0) {
		  rx_byte = p_bus->read();
	  }
	  return rx_byte;

}

// Write the register address, end with a STOP, and request multiple bytes.
void TeensyI2C::i2cMasterRead(uint8_t slave_address, uint8_t register_address, uint8_t* rx_bytes, int n_rx_bytes) {
	p_bus->beginTransmission(slave_address);
	p_bus->write(register_address);
	p_bus->endTransmission();

	// Narrow the caller's byte count to eight bits for this request.
	p_bus->requestFrom(slave_address, (uint8_t)n_rx_bytes);

	// Leave the caller's buffer untouched if the read is short.
	if (p_bus->available() >= n_rx_bytes) {
		for(int ii = 0; ii < n_rx_bytes; ii++) {
			rx_bytes[ii] = p_bus->read();
	  	}
	}
}
