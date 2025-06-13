#include <NEXNTC.h>

NTCThermistor ntc(A0, 10000, 10000, 25, 3950);

void setup()
{
    Serial.begin(9600);
    ntc.begin();
}

void loop()
{
    float temp = ntc.readTemperatureC();
    Serial.print("Current: ");
    Serial.print(temp);
    Serial.print("°C | Min: ");
    Serial.print(ntc.getMinTempC());
    Serial.print("°C | Max: ");
    Serial.println(ntc.getMaxTempC());
    delay(1000);
}
