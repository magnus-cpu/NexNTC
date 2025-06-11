#include "NEXNTC.h"

NTCThermistor::NTCThermistor(uint8_t analogPin,
                             float seriesResistor,
                             float nominalResistance,
                             float nominalTempC,
                             float betaCoefficient,
                             int numSamples) {
  _pin = analogPin;
  _seriesResistor = seriesResistor;
  _nominalResistance = nominalResistance;
  _nominalTempC = nominalTempC;
  _betaCoefficient = betaCoefficient;
  _numSamples = numSamples;
}

float NTCThermistor::readTemperatureC() {
  long adcSum = 0;
  for (int i = 0; i < _numSamples; i++) {
    adcSum += analogRead(_pin);
    delayMicroseconds(10);
  }

  float averageADC = adcSum / (float)_numSamples;
  float resistance = _seriesResistor / ((1023.0 / averageADC) - 1.0);

  float steinhart = resistance / _nominalResistance;
  steinhart = log(steinhart);
  steinhart /= _betaCoefficient;
  steinhart += 1.0 / (_nominalTempC + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  return steinhart;
}
