/*
 * TeensyI2C.h
 *
 *  Created on: Jun 3, 2020
 *      Author: Bryan
 */

#ifndef TEENSYI2C_H_
#define TEENSYI2C_H_

// Arduino Libraries
#include "Arduino.h"
#include "i2c_t3.h"
// #include "Wire.h"

// C/C++ Libraries
#include <stdio.h>

class TeensyI2C{
public:
	TeensyI2C(i2c_t3* p_bus_);
	// TeensyI2C(TwoWire* p_bus_);
	virtual ~TeensyI2C();

	i2c_t3* p_bus;
	// TwoWire* p_bus;

	void i2cMasterWrite(uint8_t slave_address, uint8_t register_address, uint8_t tx_byte);
	void i2cMasterWrite(uint8_t slave_address, uint8_t register_address, uint8_t* tx_bytes, int n_tx_bytes);
	uint8_t i2cMasterRead(uint8_t slave_address, uint8_t register_address);
	void i2cMasterRead(uint8_t slave_address, uint8_t register_address, uint8_t* rx_bytes, int n_rx_bytes);
};

#endif /* TEENSYI2C_H_ */
