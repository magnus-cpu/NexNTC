#ifndef NEXNTC_H
#define NEXNTC_H

#include <Arduino.h>

class NTCThermistor {
  public:
    NTCThermistor(uint8_t analogPin,
                  float seriesResistor,
                  float nominalResistance,
                  float nominalTempC,
                  float betaCoefficient,
                  int numSamples);

    float readTemperatureC();

  private:
    uint8_t _pin;
    float _seriesResistor;
    float _nominalResistance;
    float _nominalTempC;
    float _betaCoefficient;
    int _numSamples;
};

#endif
