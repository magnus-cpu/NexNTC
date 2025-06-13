#include <NEXNTC.h>

NTCThermistor ntc(A0, 10000, 10000, 25, 3950);

void setup()
{
    Serial.begin(9600);
    ntc.setAsyncInterval(2000); // Read every 2 seconds
    ntc.begin();
}

void loop()
{
    if (ntc.loop())
    {
        float c = ntc.readTemperatureC();
        float f = ntc.readTemperatureF();
        float k = ntc.readTemperatureK();
        Serial.print("Celsius: ");
        Serial.print(c);
        Serial.print(", Fahrenheit: ");
        Serial.print(f);
        Serial.print(", Kelvin: ");
        Serial.print(k);
        Serial.println();
    }
}