#include <NEXNTC.h>

NTCThermistor ntc(A0, 10000, 10000, 25, 3950);

void setup()
{
    Serial.begin(9600);
    // Coefficients for specific thermistor (example)
    ntc.setSteinhartHartCoefficients(1.009249522e-03, 2.378405444e-04, 2.019202697e-07);
    ntc.begin();
}

void loop()
{
    Serial.print("Steinhart-Hart Temp (C): ");
    Serial.println(ntc.readTemperatureC());
    delay(1000);
}
