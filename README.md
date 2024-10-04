# TCS34725 Sensor Library

This library provides an interface for the **TCS34725 color sensor**, allowing users to read raw sensor data and convert it to RGB and HSV formats. The library is built on the I2C communication protocol, using the `Wire` library in Arduino.

## Features
- Initialize the TCS34725 color sensor.
- Set integration time and gain for the sensor.
- Read sensor data (raw and converted values in RGB/HSV).
- Set interrupt thresholds and handle persistence filtering.

## Getting Started

### Prerequisites
You will need:
- An Arduino board (or compatible device).
- The TCS34725 color sensor module.
- The `Wire` library, which is usually included by default in the Arduino environment.

### Installation
To install the library:
1. Download or clone this repository.
2. Add the `.h` file to your Arduino project's directory.
3. Include the library in your sketch using:

```cpp
#include "TCS34725.h"
```

### Basic Usage

#### 1. Initializing the Sensor
Before reading sensor data, you must initialize the sensor by calling `begin()`.

```cpp
TCS34725 sensor;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  if (sensor.begin() == 0) {
    Serial.println("Sensor initialized successfully.");
  } else {
    Serial.println("Sensor initialization failed.");
  }
}
```

#### 2. Setting Integration Time
The integration time determines how long the sensor accumulates data for each reading. This can be set using the `set_integration_time()` function, where the time is specified in milliseconds.

```cpp
sensor.set_integration_time(100); // Set to 100ms
```

#### 3. Setting Gain
Gain adjusts the sensitivity of the sensor. Use one of the predefined values:
- `GAIN_1X`
- `GAIN_4X`
- `GAIN_16X`
- `GAIN_60X`

```cpp
sensor.set_gain(GAIN_4X); // Set the gain to 4x
```

#### 4. Reading Data
You can read the sensor data using the `read()` function. This stores the raw data in the `RAW` struct, and if color conversion is enabled, it will also populate the `RGB` and `HSV` structs.

```cpp
sensor.read();
Serial.print("R: "); Serial.println(sensor.RAW.R);
Serial.print("G: "); Serial.println(sensor.RAW.G);
Serial.print("B: "); Serial.println(sensor.RAW.B);
Serial.print("C: "); Serial.println(sensor.RAW.C);
```

#### 5. Enabling Color Conversion
You can enable or disable automatic conversion of raw data to RGB and HSV values by using the `colour_convertion()` function.

```cpp
sensor.colour_convertion(true); // Enable conversion to RGB and HSV
```

#### 6. Interrupts and Thresholds
The library provides functions to set interrupt thresholds, persistence filtering, and to clear interrupts when they are triggered.

- Set interrupt threshold:
```cpp
sensor.set_interrupt_threshold(100, 1000); // Low = 100, High = 1000
```

- Enable interrupt:
```cpp
sensor.turn_interrupt_on(1); // Enable interrupts
```

- Clear interrupt:
```cpp
sensor.clear_interrupt(); // Clears the interrupt
```

### Example Code
Here's a full example demonstrating basic usage:

```cpp
#include "TCS34725.h"

TCS34725 sensor;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  if (sensor.begin() == 0) {
    Serial.println("Sensor initialized.");
  } else {
    Serial.println("Failed to initialize sensor.");
  }

  sensor.set_integration_time(100);
  sensor.set_gain(GAIN_4X);
  sensor.colour_convertion(true);
}

void loop() {
  sensor.read();
  
  // Print RGB values
  Serial.print("R: "); Serial.println(sensor.RGB.R);
  Serial.print("G: "); Serial.println(sensor.RGB.G);
  Serial.print("B: "); Serial.println(sensor.RGB.B);

  delay(1000);
}
```

### API Reference

#### `int8_t begin()`
Initializes the sensor and prepares it for data reading.

#### `int8_t set_integration_time(int integration_time)`
Sets the integration time in milliseconds.

#### `int8_t set_gain(uint8_t gain)`
Sets the sensor gain. Possible values are `GAIN_1X`, `GAIN_4X`, `GAIN_16X`, and `GAIN_60X`.

#### `int8_t read()`
Reads data from the sensor and stores it in the `RAW`, `RGB`, and `HSV` structs if color conversion is enabled.

#### `void colour_convertion(bool state)`
Enables or disables automatic color conversion to RGB and HSV.

#### `int8_t set_interrupt_threshold(uint16_t low, uint16_t high)`
Sets the interrupt threshold values for the clear channel.

#### `int8_t turn_interrupt_on(uint8_t state)`
Enables or disables the interrupt functionality.

#### `int8_t clear_interrupt()`
Clears the sensor's interrupt state after it has been triggered.

#### `int8_t set_persistence_filter(uint8_t value)`
Sets the persistence filter value, determining how often the sensor generates an interrupt.

---

## License
This library is licensed under the MIT License.
```

