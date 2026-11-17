// Read the MCP9808 ambient temperature once per second and print it in Celsius.
#include <TeensyI2C.h>

constexpr uint8_t sensorAddress = 0x18;
constexpr uint8_t temperatureRegister = 0x05;
TeensyI2C sensorBus(&Wire);

// Initialize the I2C bus and serial output.
void setup() {
    Wire.begin(I2C_MASTER, 0, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000);
    Serial.begin(115200);
}

// Read both register bytes and convert the signed temperature field to Celsius.
void loop() {
    // A short read leaves the buffer untouched. The sentinel encodes -256 C, outside the sensor's range.
    uint8_t bytes[2] = {0x10, 0x00};
    sensorBus.i2cMasterRead(sensorAddress, temperatureRegister, bytes, sizeof(bytes));
    if (bytes[0] == 0x10 && bytes[1] == 0x00) {
        Serial.println("Temperature read failed");
    } else {
        // Discard the three alert flags and sign-extend the 13-bit temperature.
        int16_t rawTemperature = ((bytes[0] & 0x1F) << 8) | bytes[1];
        if (rawTemperature & 0x1000) {
            rawTemperature -= 0x2000;
        }
        Serial.print(rawTemperature / 16.0f, 4);
        Serial.println(" C");
    }

    delay(1000);
}
