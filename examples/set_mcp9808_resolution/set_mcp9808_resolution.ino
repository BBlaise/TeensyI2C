// Set an MCP9808 to 0.25 degrees Celsius per bit and print its resolution register once per second.
#include <Arduino.h>
#include "i2c_t3.h"
#include "TeensyI2C.h"

namespace {
const uint8_t default_MCP9808_address = 0b0011000;
const uint8_t RESOLUTION = 0b00001000;
const uint8_t res_0_25 = 0b01;
uint8_t device_address = default_MCP9808_address;
TeensyI2C i2c0(&Wire);

// Write the selected resolution code to the sensor.
void setResolution(uint8_t resolution_){
    i2c0.i2cMasterWrite(device_address, RESOLUTION, resolution_);
}
}

// Initialize the I2C bus and serial output, then set the sensor resolution.
void setup(){
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000);
    delay(100);              // short delay to ensure IC starts up
    Serial.begin(115200);
    setResolution(res_0_25);
}

// Print the resolution register code; the selected setting should read back as 1.
void loop(){
    uint8_t rx_bytes = i2c0.i2cMasterRead(device_address, RESOLUTION);
    Serial.println(rx_bytes);
    delay(1000);
}
