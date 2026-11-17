# TeensyI2C

TeensyI2C is the Teensy bus adapter for platform-agnostic I²C device drivers. It wraps `i2c_t3` and exposes `i2cMasterRead` and `i2cMasterWrite` for single-byte and multi-byte register access.

## Why use a bus object?

The IC driver owns the device-specific logic, such as which MCP9808 register holds the temperature and how to decode it. The I²C object handles the platform-specific transfers. The application initializes the board's bus and passes the adapter to the driver. This keeps the register logic reusable across microcontrollers and lets the application choose which physical bus to use.

MCP9808 is a class template that takes the adapter's pointer type. Both setups below use the same driver.

```cpp
// Teensy
TeensyI2C i2c(&Wire);
MCP9808<TeensyI2C*> sensor(&i2c);
```

```cpp
// Arduino
ArduinoI2C i2c(&Wire);
MCP9808<ArduinoI2C*> sensor(&i2c);
```

The same pattern applies to MSP432 with an `I2CMSP` adapter that provides the register read and write methods the driver calls. To port a driver, implement those methods for the target platform and select the adapter in the application. The device's register map and logic stay in one place.

The examples in this repository target Teensy boards. Other boards require a different I²C backend.

## Install

Install the [Teensy board package](https://www.pjrc.com/teensy/td_download.html) and select **Teensy 3.6**. Copy this repository's `TeensyI2C` folder into your Arduino sketchbook's `libraries` folder. The library requires [`i2c_t3`](https://github.com/nox771/i2c_t3), which comes with Teensyduino.

## Usage

Pass the I²C bus to TeensyI2C and initialize it before the first transfer. This sketch prints the raw MCP9808 resolution setting as a decimal number at 115200 baud.

```cpp
#include <TeensyI2C.h>

TeensyI2C i2c(&Wire);

void setup() {
    Wire.begin(I2C_MASTER, 0, I2C_PINS_18_19, I2C_PULLUP_EXT, 100000);
    Serial.begin(115200);
}

void loop() {
    Serial.println(i2c.i2cMasterRead(0x18, 0x08));
    delay(1000);
}
```

Reads send a STOP after selecting the register. A single-byte read returns zero if no byte arrives. A short multi-byte read leaves the destination buffer untouched.

## Examples

Connect an MCP9808 at address `0x18` to Teensy 3.6 SDA pin 18 and SCL pin 19. Use 3.3 V logic, a common ground, and pull-ups to 3.3 V on both bus lines. The breakout may already supply pull-ups.

- [Read ambient temperature](examples/read_mcp9808_temperature/read_mcp9808_temperature.ino) — Reads two register bytes and prints the temperature in Celsius once per second.
- [Set the resolution](examples/set_mcp9808_resolution/set_mcp9808_resolution.ino) — Writes a one-byte resolution setting and prints whether the readback matches. Open the serial monitor at 115200 baud and reset the board to repeat it.
