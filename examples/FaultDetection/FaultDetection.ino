#include <NEXNTC.h>

NTCThermistor ntc(A0, 10000, 10000, 25, 3950);

void setup()
{
    Serial.begin(9600);
    ntc.begin();
}

void loop()
{
    ntc.readTemperatureC();

    if (ntc.checkOpenCircuit())
        Serial.println("⚠️  Thermistor Open Circuit!");

    else if (ntc.checkShortCircuit())
        Serial.println("⚠️  Thermistor Short Circuit!");

    else
        Serial.printf("Temp: %.2f°C\n", ntc.readTemperatureC());

    delay(1000);
}
