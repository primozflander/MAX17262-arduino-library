#include <Arduino.h>
#include <MAX17262.h>

#define SDA_PIN 4
#define SCL_PIN 5

TwoWire i2cBus(SDA_PIN, SCL_PIN);
MAX17262 fuelGauge(i2cBus);

void setup()
{
    Serial.begin(115200);
    i2cBus.begin();
    fuelGauge.begin();
}

void loop()
{
    Serial.println("Voltage: " + String(fuelGauge.readVoltage()) + " mV");
    Serial.println("Avg voltage: " + String(fuelGauge.readAvgVoltage()) + " mV");
    Serial.println("Temp: " + String(fuelGauge.readTemp()) + " °C");
    Serial.println("Avg emp: " + String(fuelGauge.readAvgTemp()) + " °C");
    Serial.println("Current: " + String(fuelGauge.readCurrent()) + " mA");
    Serial.println("Avg current: " + String(fuelGauge.readAvgCurrent()) + " mA");
    Serial.println("Soc: " + String(fuelGauge.readSOC()) + " %");
    Serial.println("Avg soc: " + String(fuelGauge.readAvgSOC()) + " %");
    Serial.println("TTE: " + String(fuelGauge.readTimeToEmpty()) + " h");
    Serial.println("Reported cap: " + String(fuelGauge.readReportedCapacity()) + " mAh");
    Serial.println("Remaining cap: " + String(fuelGauge.readRemainingCapacity()) + " mAh");
    delay(1000);
}
