#include <NEXNTC.h>

NTCThermistor ntc(A0, 10000, 10000, 25, 3950);

void setup()
{
    Serial.begin(9600);
    ntc.setADCOversampling(2); // 2-bit oversampling (4^2 = 16 samples)
    ntc.begin();
}

void loop()
{
    Serial.print("Oversampled Temp (C): ");
    Serial.println(ntc.readTemperatureC());
    delay(1000);
}
