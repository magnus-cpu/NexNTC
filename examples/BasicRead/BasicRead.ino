/*
simple to use 
thermistor(analogPin, seriesResistor, nominalResistance, nominalTempC, betaCoefficient,numSamples);
*/



#include <Arduino.h>
#include <NEXNTC.h>


NTCThermistor thermistor(A0, 10000.0, 10000.0, 25.0, 3950.0, 10);

void setup() {
  Serial.begin(9600);
}

void loop() {
  tempC = thermistor.readTemperatureC();
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.println(" °C");
  delay(1000);
}
