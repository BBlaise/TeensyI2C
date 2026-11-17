// Set the MCP9808 resolution to 0.25 C per bit and read the setting back.
#include <TeensyI2C.h>

constexpr uint8_t sensorAddress = 0x18;
constexpr uint8_t resolutionRegister = 0x08;
constexpr uint8_t quarterDegreeResolution = 0x01;
TeensyI2C sensorBus(&Wire);

// Initialize the bus, write the resolution, and report the readback.
void setup() {
    Wire.begin(I2C_MASTER, 0, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000);
    Serial.begin(115200);

    // Give the USB serial monitor time to connect before printing once.
    while (!Serial && millis() < 3000) {
        delay(10);
    }

    sensorBus.i2cMasterWrite(sensorAddress, resolutionRegister, quarterDegreeResolution);
    const uint8_t resolution = sensorBus.i2cMasterRead(sensorAddress, resolutionRegister);

    // The requested setting is nonzero, so the read failure fallback cannot match it.
    if (resolution == quarterDegreeResolution) {
        Serial.println("Resolution readback: 0.25 C per bit");
    } else {
        Serial.println("Resolution readback did not match");
    }
}

// The configuration and readback run once at startup.
void loop() {}
