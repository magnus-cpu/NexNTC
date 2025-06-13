# 📟 NEXNTC Library

A flexible and accurate library for reading NTC thermistor temperatures using an analog input on microcontrollers (ArduinoUno, ArduinoMega, ESP32, etc). Supports Steinhart-Hart equation, min/max tracking, oversampling, and moving average filtering.

---

## ✅ Features

- Beta formula and Steinhart-Hart support
- Min/Max temperature tracking (C, F, K)
- Oversampling for better resolution
- Configurable moving average filter
- Async temperature update with `loop()`
- Open/Short circuit detection

---

## 🔧 Constructor

```cpp
NTCThermistor thermistor(
  uint8_t analogPin,
  float seriesResistor,
  float nominalResistance,
  float nominalTempC,
  float betaCoefficient,
  int numSamples = 5,
  float vRef = 3.3,  // based on the board type
  uint8_t adcResolution = 12 // based on the board type
);
```

---

## 📦 Example Usage

```cpp
#include "NEXNTC.h"

NTCThermistor thermistor(
  A0,        // Analog pin
  10000.0,   // Series resistor (Ohms)
  10000.0,   // Thermistor nominal resistance at 25°C
  25.0,      // Nominal temperature (°C)
  3950.0     // Beta coefficient
);

void setup() {
  Serial.begin(9600);
  if (!thermistor.begin()) {
    Serial.println("Thermistor init failed!");
    while (1);
  }

  thermistor.setADCFiltering(true, 10);     // Enable moving average filter
  thermistor.setADCOversampling(2);         // Optional: enable oversampling
  thermistor.setAsyncInterval(500);         // Update every 500ms
}

void loop() {
  if (thermistor.loop()) {  // Non-blocking update
    Serial.print("Temp (C): ");
    Serial.println(thermistor.readTemperatureC());

    Serial.print("Temp (F): ");
    Serial.println(thermistor.readTemperatureF());

    Serial.print("Min (C): ");
    Serial.println(thermistor.getMinTempC());

    Serial.print("Max (C): ");
    Serial.println(thermistor.getMaxTempC());
  }
}
```

---

## ⚙️ Optional Features

### Enable Steinhart-Hart Coefficients
```cpp
thermistor.setSteinhartHartCoefficients(0.001129148, 0.000234125, 0.0000000876741);
```

### Reset Min/Max
```cpp
thermistor.resetMinMax();
```

### Voltage Reference
```cpp
thermistor.setVoltageReference(3.3);
```

---

## 📖 API Summary

| Method | Description |
|--------|-------------|
| `begin()` | Initialize thermistor |
| `readTemperatureC/F/K()` | Read temperature in °C/°F/K |
| `getMinTempC()` | Get minimum recorded temperature |
| `getMaxTempC()` | Get maximum recorded temperature |
| `setADCFiltering(bool, windowSize)` | Enable/disable moving average filter |
| `setADCOversampling(bits)` | Enable ADC oversampling (0-4 bits) |
| `loop()` | Call periodically for async updates |
| `checkOpenCircuit()` | Detect open thermistor |
| `checkShortCircuit()` | Detect shorted thermistor |

---

## 🔋 Requirements

- Arduino-compatible MCU (AVR, ESP32, STM32, etc)
- Compatible NTC thermistor
- Pull-up resistor (e.g., 10k)
