#include "NEXNTC.h"
#include <math.h>

NTCThermistor::NTCThermistor(uint8_t analogPin,
                             float seriesResistor,
                             float nominalResistance,
                             float nominalTempC,
                             float betaCoefficient,
                             int numSamples,
                             float vRef,
                             uint8_t adcResolution)
    : _pin(analogPin),
      _seriesResistor(seriesResistor),
      _nominalResistance(nominalResistance),
      _nominalTempC(nominalTempC),
      _betaCoefficient(betaCoefficient),
      _numSamples(constrain(numSamples, 1, 100)),
      _vRef(constrain(vRef, 1.0f, 5.0f)),
      _adcResolution(constrain(adcResolution, 8, 16)),
      _filterEnabled(false),
      _filterWindowSize(5),
      _filterBuffer(nullptr),
      _filterIndex(0),
      _oversampleBits(0),
      _lastResistance(0.0f),
      _lastTemperatureC(NAN),
      _minTempC(NAN),
      _maxTempC(NAN),
      _minTempF(NAN),
      _maxTempF(NAN),
      _minTempK(NAN),
      _maxTempK(NAN),
      _c1(0.0f),
      _c2(0.0f),
      _c3(0.0f),
      _useSteinhartHart(false),
      _lastReadTime(0),
      _readInterval(1000)
{
  _adcMax = (1 << _adcResolution) - 1;
}

bool NTCThermistor::begin()
{
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
  if (_pin > A7)
    return false;
#elif defined(ARDUINO_SAM_DUE)
  if (_pin > A11)
    return false;
#elif defined(ESP32)
  // All pins can be analog
#else
  if (_pin > A5)
    return false;
#endif

#if defined(ANALOG_RESOLUTION) || defined(ESP32)
  analogReadResolution(_adcResolution);
#endif

  pinMode(_pin, INPUT);

  if (_filterEnabled)
  {
    _filterBuffer = new float[_filterWindowSize];
    for (uint8_t i = 0; i < _filterWindowSize; i++)
      _filterBuffer[i] = 0.0;
  }

  return true;
}

float NTCThermistor::readTemperatureC()
{
  _lastResistance = readResistance();

  float tempC = _useSteinhartHart ? calculateSteinhartHart(_lastResistance)
                                  : calculateTemperature(_lastResistance);

  // Check plausible temperature range for NTC thermistor
  if (tempC < -40.0 || tempC > 125.0)
    return NAN;

  _lastTemperatureC = tempC;

  // Update min/max for Celsius
  if (isnan(_minTempC) || tempC < _minTempC)
    _minTempC = tempC;
  if (isnan(_maxTempC) || tempC > _maxTempC)
    _maxTempC = tempC;

  // Convert to Fahrenheit and Kelvin
  float tempF = tempC * 9.0 / 5.0 + 32.0;
  float tempK = tempC + 273.15;

  // Update min/max for Fahrenheit
  if (isnan(_minTempF) || tempF < _minTempF)
    _minTempF = tempF;
  if (isnan(_maxTempF) || tempF > _maxTempF)
    _maxTempF = tempF;

  // Update min/max for Kelvin
  if (isnan(_minTempK) || tempK < _minTempK)
    _minTempK = tempK;
  if (isnan(_maxTempK) || tempK > _maxTempK)
    _maxTempK = tempK;

  return tempC;
}

float NTCThermistor::readTemperatureF()
{
  float tempC = readTemperatureC();
  if (isnan(tempC))
    return NAN;
  return tempC * 9.0 / 5.0 + 32.0;
}

float NTCThermistor::readTemperatureK()
{
  float tempC = readTemperatureC();
  if (isnan(tempC))
    return NAN;
  return tempC + 273.15;
}

float NTCThermistor::readResistance()
{
  float averageADC = (_oversampleBits > 0) ? oversampleADC() : 0.0;
  if (_oversampleBits == 0)
  {
    long adcSum = 0;
    for (int i = 0; i < _numSamples; i++)
      adcSum += analogRead(_pin);
    averageADC = adcSum / (float)_numSamples;
  }

  if (averageADC <= 1)
    return 0.0;
  if (averageADC >= (_adcMax - 1))
    return INFINITY;

  float resistance = _seriesResistor / ((_adcMax / averageADC) - 1.0);
  if (_filterEnabled)
    resistance = movingAverageFilter(resistance);

  return resistance;
}

float NTCThermistor::calculateTemperature(float resistance)
{
  if (resistance <= 0 || isinf(resistance))
    return NAN;

  float steinhart = resistance / _nominalResistance;
  steinhart = log(steinhart);
  steinhart /= _betaCoefficient;
  steinhart += 1.0 / (_nominalTempC + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  return steinhart;
}

float NTCThermistor::calculateSteinhartHart(float resistance)
{
  if (resistance <= 0 || isinf(resistance))
    return NAN;

  float lnR = log(resistance);
  float invT = _c1 + _c2 * lnR + _c3 * pow(lnR, 3);
  return (1.0 / invT) - 273.15;
}

float NTCThermistor::movingAverageFilter(float newValue)
{
  _filterBuffer[_filterIndex] = newValue;
  _filterIndex = (_filterIndex + 1) % _filterWindowSize;

  float sum = 0.0;
  for (uint8_t i = 0; i < _filterWindowSize; i++)
    sum += _filterBuffer[i];

  return sum / _filterWindowSize;
}

float NTCThermistor::oversampleADC()
{
  uint16_t factor = 1 << (_oversampleBits * 2);
  uint32_t sum = 0;
  for (uint16_t i = 0; i < factor; i++)
    sum += analogRead(_pin);
  return sum / (float)factor;
}

bool NTCThermistor::checkOpenCircuit() const { return isinf(_lastResistance); }
bool NTCThermistor::checkShortCircuit() const { return _lastResistance <= 0; }
float NTCThermistor::getLastResistance() const { return _lastResistance; }
float NTCThermistor::getMinTempC() const { return _minTempC; }
float NTCThermistor::getMaxTempC() const { return _maxTempC; }

void NTCThermistor::setADCFiltering(bool enable, uint8_t windowSize)
{
  _filterEnabled = enable;
  if (_filterBuffer)
    delete[] _filterBuffer;
  _filterBuffer = enable ? new float[windowSize] : nullptr;
  _filterWindowSize = windowSize;
}

void NTCThermistor::setVoltageReference(float vRef) { _vRef = vRef; }

void NTCThermistor::setADCOversampling(uint8_t bits)
{
  _oversampleBits = min(bits, 4);
}

void NTCThermistor::setSteinhartHartCoefficients(float c1, float c2, float c3)
{
  _c1 = c1;
  _c2 = c2;
  _c3 = c3;
  _useSteinhartHart = true;
}

void NTCThermistor::setAsyncInterval(unsigned long intervalMs)
{
  _readInterval = intervalMs;
}

bool NTCThermistor::loop()
{
  if (millis() - _lastReadTime >= _readInterval)
  {
    _lastReadTime = millis();
    readTemperatureC();
    return true;
  }
  return false;
}

NTCThermistor::~NTCThermistor()
{
  if (_filterBuffer)
    delete[] _filterBuffer;
}
void NTCThermistor::resetMinMax()
{
  _minTempC = NAN;
  _maxTempC = NAN;
  _minTempF = NAN;
  _maxTempF = NAN;
  _minTempK = NAN;
  _maxTempK = NAN;
}
