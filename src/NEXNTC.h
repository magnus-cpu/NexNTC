#ifndef NEXNTC_H
#define NEXNTC_H

#include "Arduino.h"

#ifndef NTC_DEFAULT_SAMPLES
#define NTC_DEFAULT_SAMPLES 5
#endif

// Board-specific defaults, but allow user override
#if defined(ESP32)
#ifndef NTC_DEFAULT_VREF
#define NTC_DEFAULT_VREF 3.3f
#endif
#ifndef NTC_DEFAULT_ADC_RES
#define NTC_DEFAULT_ADC_RES 12
#endif
#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega328P__)
#ifndef NTC_DEFAULT_VREF
#define NTC_DEFAULT_VREF 5.0f
#endif
#ifndef NTC_DEFAULT_ADC_RES
#define NTC_DEFAULT_ADC_RES 10
#endif
#else
#ifndef NTC_DEFAULT_VREF
#define NTC_DEFAULT_VREF 3.3f
#endif
#ifndef NTC_DEFAULT_ADC_RES
#define NTC_DEFAULT_ADC_RES 12
#endif
#endif

class NTCThermistor
{
public:
  NTCThermistor(uint8_t analogPin,
                float seriesResistor,
                float nominalResistance,
                float nominalTempC,
                float betaCoefficient,
                int numSamples = NTC_DEFAULT_SAMPLES,
                float vRef = NTC_DEFAULT_VREF,
                uint8_t adcResolution = NTC_DEFAULT_ADC_RES);

  bool begin();

  float readTemperatureC();
  float readTemperatureF();
  float readTemperatureK();

  // Min/max getters for all scales
  float getMinTempC() const;
  float getMaxTempC() const;
  float getMinTempF() const;
  float getMaxTempF() const;
  float getMinTempK() const;
  float getMaxTempK() const;

  float getLastTemperatureC() const;

  // Resistance diagnostics
  bool checkOpenCircuit() const;
  bool checkShortCircuit() const;
  float getLastResistance() const;

  // Configuration methods
  void setADCFiltering(bool enable, uint8_t windowSize = 5);
  void setVoltageReference(float vRef);
  void setADCOversampling(uint8_t bits);

  void setSteinhartHartCoefficients(float c1, float c2, float c3);

  void setAsyncInterval(unsigned long intervalMs);
  bool loop();

  void resetMinMax();

  ~NTCThermistor();

private:
  // Core calculation methods
  float readResistance();
  float calculateTemperature(float resistance);
  float calculateSteinhartHart(float resistance);

  // Filtering methods
  float movingAverageFilter(float newValue);
  float oversampleADC();

  // Configuration parameters
  uint8_t _pin;
  float _seriesResistor;
  float _nominalResistance;
  float _nominalTempC;
  float _betaCoefficient;
  int _numSamples;
  float _vRef;
  uint16_t _adcMax;
  uint8_t _adcResolution;

  // Filtering parameters
  bool _filterEnabled;
  uint8_t _filterWindowSize;
  float *_filterBuffer;
  uint8_t _filterIndex;

  // Oversampling
  uint8_t _oversampleBits;

  // Last readings
  float _lastResistance;
  float _lastTemperatureC;

  // Min/max temperature tracking for C, F, K
  float _minTempC;
  float _maxTempC;
  float _minTempF;
  float _maxTempF;
  float _minTempK;
  float _maxTempK;

  // Steinhart-Hart coefficients and flag
  float _c1;
  float _c2;
  float _c3;
  bool _useSteinhartHart;

  // Async reading control
  unsigned long _lastReadTime;
  unsigned long _readInterval;
};

#endif