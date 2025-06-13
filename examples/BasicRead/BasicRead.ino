#include <NEXNTC.h>

NTCThermistor ntc(
    A0,    // Analog pin
    10000, // Series resistor (10kΩ)
    10000, // Nominal thermistor resistance (10kΩ)
    25,    // Nominal temperature (°C)
    3950   // Beta coefficient
);

void setup()
{
  Serial.begin(9600);
  if (!ntc.begin())
  {
    Serial.println("NTC sensor init failed!");
    while (1)
      ;
  }
}

void loop()
{
  float tempC = ntc.readTemperatureC();
  Serial.print("Temp (C): ");
  Serial.println(tempC);
  delay(1000);
}
