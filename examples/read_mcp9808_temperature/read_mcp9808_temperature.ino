// Read an MCP9808 temperature over I2C and print it in degrees Fahrenheit once per second.
#include <Arduino.h>
#include "i2c_t3.h"
#include "TeensyI2C.h"

namespace {
const uint8_t default_MCP9808_address = 0b0011000;
const uint8_t T_A = 0b00000101;
uint8_t device_address = default_MCP9808_address;
TeensyI2C i2c0(&Wire);

// Read both temperature register bytes and convert the result to degrees Fahrenheit.
float readTemperature(void){
    uint8_t rx_bytes[2] = {};
    float temperature = 0;

    i2c0.i2cMasterRead(device_address, T_A, rx_bytes, 2);
    uint16_t temperature_bits = (rx_bytes[0] << 8) | rx_bytes[1];

    if (temperature_bits != 0xFFFF) {
        // Mask the alert and sign bits, then apply the signed temperature offset.
        temperature = temperature_bits & 0x0FFF;
        temperature /= 16.0;
        if (temperature_bits & 0x1000)
            temperature -= 256;
    }

    temperature = temperature * 9.0 / 5.0 + 32.0;     // convert from degC to degF
    return temperature;
}
}

// Initialize the I2C bus and serial output.
void setup(){
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000);
    delay(100);              // short delay to ensure IC starts up
    Serial.begin(115200);
}

// Print one temperature reading per second.
void loop(){
    float temperature = readTemperature();
    Serial.println(temperature);
    delay(1000);
}
