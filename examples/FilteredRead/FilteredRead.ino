#include <NEXNTC.h>

NTCThermistor ntc(A0, 10000, 10000, 25, 3950);

void setup()
{
    Serial.begin(9600);
    ntc.setADCFiltering(true, 10); // Enable 10-sample moving average filter
    ntc.begin();
}

void loop()
{
    Serial.print("Filtered Temp (C): ");
    Serial.println(ntc.readTemperatureC());
    delay(1000);
}
