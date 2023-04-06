# MAX 17262

Arduino library to interface MAX 17262 fuel gauge. Sensor uses I2C communication.

## API

```shell
    MAX17262(TwoWire &i2cInstance);
    bool begin();
    unsigned int readVoltage();
    unsigned int readAvgVoltage();
    int readTemp();
    int readAvgTemp();
    unsigned int readSOC();
    unsigned int readAvgSOC();
    int readAvgCurrent();
    int readCurrent();
    unsigned int readTimeToEmpty();
    unsigned int readTimeToFull();
    unsigned int readRemainingCapacity();
    unsigned int readReportedCapacity();
    unsigned int readBatCycles();
    unsigned int readDesignCapacity();
    unsigned int readIChgTerm();
    unsigned int readVEmpty();
    int readPower();
    int readAvgPower();
    void setDesignCapacity(uint16_t capacity);
    void setIChgTerm(uint16_t iTerm);
    void setVEmpty(uint16_t vEmpty);
    void readLearnedParameters(u_int16_t &rComp0, u_int16_t &tempCo, u_int16_t &fullCapRep, u_int16_t &cycles, u_int16_t &fullCapNom);
    void writeLearnedParameters(u_int16_t rComp0, u_int16_t tempCo, u_int16_t fullCapRep, u_int16_t cycles, u_int16_t fullCapNom);
    void setLCOchemistry();
    void setNCRchemistry();
```

For full list of function calls see MAX17262.h

## Example
```shell
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
```

## Documentation

You can find the source files at
https://github.com/primozflander/MAX17262-arduino-library
