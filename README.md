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

## Examples


## Documentation

You can find the source files at
https://github.com/primozflander/MAX17262-arduino-library
