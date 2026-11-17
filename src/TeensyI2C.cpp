/*
 * TeensyI2C.cpp
 *
 *  Created on: Jun 3, 2020
 *      Author: Bryan
 */

#include "TeensyI2C.h"

// The caller supplies and initializes the I2C bus.
TeensyI2C::TeensyI2C(i2c_t3* p_bus_) : p_bus(p_bus_) {}
// TeensyI2C::TeensyI2C(TwoWire* p_bus_) : p_bus(p_bus_) {}

TeensyI2C::~TeensyI2C() {}

// Send the register address first, followed by a single byte transmission in the same transaction.
void TeensyI2C::i2cMasterWrite(uint8_t slave_address, uint8_t register_address, uint8_t tx_byte) {
	p_bus->beginTransmission(slave_address);
	p_bus->write(register_address);
	p_bus->write(tx_byte);
	p_bus->endTransmission();
}

// Send the register address first, followed by a multi-byte (array) transmission in the same transaction.
void TeensyI2C::i2cMasterWrite(uint8_t slave_address, uint8_t register_address, uint8_t* tx_bytes, int n_tx_bytes) {
	p_bus->beginTransmission(slave_address);
	p_bus->write(register_address);

	for (int ii = 0; ii < n_tx_bytes; ii++) {
		p_bus->write(tx_bytes[ii]);
	}
	p_bus->endTransmission();
}

// Select the register in a separate write before requesting a single byte of data.
uint8_t TeensyI2C::i2cMasterRead(uint8_t slave_address, uint8_t register_address) {
	  p_bus->beginTransmission(slave_address);
	  p_bus->write(register_address);
	  p_bus->endTransmission();

	  p_bus->requestFrom(slave_address, (uint8_t)1);
	  // Return zero if no byte arrives.
	  uint8_t rx_byte = 0;
	  if (p_bus->available() > 0) {
		  rx_byte = p_bus->read();
	  }
	  return rx_byte;

}

// Select the register in a separate write before requesting multiple bytes of data.
void TeensyI2C::i2cMasterRead(uint8_t slave_address, uint8_t register_address, uint8_t* rx_bytes, int n_rx_bytes) {
	p_bus->beginTransmission(slave_address);
	p_bus->write(register_address);
	p_bus->endTransmission();

	p_bus->requestFrom(slave_address, (uint8_t)n_rx_bytes);

	// Leave the caller's buffer untouched if the read is short.
	if (p_bus->available() >= n_rx_bytes) {
		for(int ii = 0; ii < n_rx_bytes; ii++) {
			rx_bytes[ii] = p_bus->read();
	  	}
	}
}
