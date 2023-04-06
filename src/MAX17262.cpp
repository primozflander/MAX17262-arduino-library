#include <MAX17262.h>

MAX17262::MAX17262(TwoWire &i2cInstance)
{
    ptr = &i2cInstance;
}

bool MAX17262::begin()
{
    // Serial.println("begin fuel gauge init");
    if (readRegister(DEV_NAME_REG) == 0x4039)
    {
        if (readRegBit(STATUS_REG, POR_BIT) == true) // read POR bit
        {
            // Serial.println("POR event detected");
            while (readRegBit(F_STAT_REG, DNR_BIT)) // wait for data ready
            {
                delay(10);
            }
            uint16_t tempHibCfgReg = readRegister(HIB_CFG_REG);

            writeRegister(SOFT_WAKEUP_REG, 0x90); // Exit Hibernate Mode step 1
            writeRegister(HIB_CFG_REG, 0x0);      // Exit Hibernate Mode step 2
            writeRegister(SOFT_WAKEUP_REG, 0x0);  // Exit Hibernate Mode step 3

            // EZ Config
            setDesignCapacity(3400); // 3400 mAh
            setIChgTerm(101);
            setVEmpty(3000);                                         // 3000 mV
            writeRegister(MODEL_CFG_REG, 0x8000);                    // Write ModelCFG
            while (readRegBit(MODEL_CFG_REG, MODEL_CFG_REFRESH_BIT)) // do not continue until ModelCFG.Refresh==0
            {
                delay(10);
            }

            writeRegister(HIB_CFG_REG, tempHibCfgReg); // Restore Original HibCFG value
            setRegBit(STATUS_REG, 0, POR_BIT);         // clear POR bit
        }
        else
        {
            // Serial.println("No POR event detected - skip device configuration");
        }
        return true;
    }
    return false;
}

unsigned int MAX17262::readVoltage()
{
    return readRegister(VCELL_REG) * VOLTAGE_MULTIPLIER;
}

unsigned int MAX17262::readAvgVoltage()
{
    return readRegister(AVG_VCELL_REG) * VOLTAGE_MULTIPLIER;
}

int MAX17262::readTemp()
{
    return readRegister(TEMP_REG) >> 8;
}

int MAX17262::readAvgTemp()
{
    return readRegister(AVG_TA_REG) >> 8;
}

unsigned int MAX17262::readSOC()
{
    return readRegister(REP_SOC_REG) >> 8;
    // return (float)read16(REP_SOC_REG) * PERCENTAGE_MULTIPLIER;
}

unsigned int MAX17262::readAvgSOC()
{
    return readRegister(AV_SOC_REG) >> 8;
    // return (float)read16(AV_SOC_REG) * PERCENTAGE_MULTIPLIER;
}

int MAX17262::readAvgCurrent()
{
    return (int16_t)readRegister(AVG_CURRENT_REG) * CURRENT_MULTIPLIER;
}

int MAX17262::readCurrent()
{
    return (int16_t)readRegister(CURRENT_REG) * CURRENT_MULTIPLIER;
}

unsigned int MAX17262::readTimeToEmpty()
{
    return readRegister(TTE_REG) * TIME_MULTIPLIER / 60000;
}

unsigned int MAX17262::readTimeToFull()
{
    return readRegister(TTF_REG) * (TIME_MULTIPLIER / 60000);
}

unsigned int MAX17262::readRemainingCapacity()
{
    return readRegister(REP_CAP_REG) * CAP_MULTIPLIER;
}

unsigned int MAX17262::readReportedCapacity()
{
    return readRegister(FULL_CAP_REP_REG) * CAP_MULTIPLIER;
}

unsigned int MAX17262::readBatCycles()
{
    return readRegister(CYCLES_REG);
}

unsigned int MAX17262::readDesignCapacity()
{
    return readRegister(DESIGN_CAP_REG) * CAP_MULTIPLIER;
}

unsigned int MAX17262::readIChgTerm()
{
    return readRegister(I_CHG_TERM_REG) * CURRENT_MULTIPLIER;
}

unsigned int MAX17262::readVEmpty()
{
    return readRegister(V_EMPTY_REG) * VOLTAGE_MULTIPLIER;
}

int MAX17262::readPower()
{
    return (int16_t)readRegister(POWER_REG);
}

int MAX17262::readAvgPower()
{
    return (int16_t)readRegister(AVG_POWER_REG);
}

void MAX17262::setDesignCapacity(uint16_t capacity)
{
    writeRegister(DESIGN_CAP_REG, (uint16_t)(capacity / CAP_MULTIPLIER));
}

void MAX17262::setIChgTerm(uint16_t iTerm)
{
    writeRegister(I_CHG_TERM_REG, (uint16_t)(iTerm / CURRENT_MULTIPLIER));
}

void MAX17262::setVEmpty(uint16_t vEmpty)
{
    writeRegister(V_EMPTY_REG, (uint16_t)(vEmpty / VOLTAGE_MULTIPLIER));
}

void MAX17262::setLCOchemistry()
{
    uint16_t temp = readRegister(MODEL_CFG_REG) & 0xFF0F;
    writeRegister(MODEL_CFG_REG, temp);
}

void MAX17262::setNCRchemistry()
{
    uint16_t temp = readRegister(MODEL_CFG_REG) & 0xFF2F;
    writeRegister(MODEL_CFG_REG, temp);
}

void MAX17262::writeRegister(uint8_t reg, uint16_t dat)
{
    ptr->beginTransmission(DEVICE_ADDRESS);
    ptr->write(reg);
    ptr->write(dat);
    ptr->write(dat >> 8);
    ptr->endTransmission();
}

uint16_t MAX17262::readRegister(uint8_t reg)
{
    uint16_t temp;
    ptr->beginTransmission(DEVICE_ADDRESS);
    ptr->write(reg);
    ptr->endTransmission();
    ptr->requestFrom(DEVICE_ADDRESS, 2);
    temp = (uint16_t)ptr->read();
    temp |= (uint16_t)ptr->read() << 8;
    ptr->endTransmission();
    return temp;
}

bool MAX17262::writeAndVerify(uint8_t reg, uint16_t dat)
{
    writeRegister(reg, dat);
    delay(10);
    if (readRegister(reg) == dat)
    {
        return true;
    }
    return false;
}

void MAX17262::setRegBit(uint8_t reg, uint16_t value, uint8_t offset)
{
    uint16_t temp = readRegister(reg);
    bitWrite(temp, offset, value);
    writeRegister(reg, temp);
}

bool MAX17262::readRegBit(uint8_t reg, uint8_t offset)
{
    return bitRead(readRegister(reg), offset);
}

// It is recommended saving the learned capacity parameters every time bit 6 of the Cycles register toggles (so that it is saved every 64% change in the battery) so that if power is lost, the values can easily be restored.
void MAX17262::readLearnedParameters(u_int16_t &rComp0, u_int16_t &tempCo, u_int16_t &fullCapRep, u_int16_t &cycles, u_int16_t &fullCapNom)
{
    rComp0 = readRegister(R_COMP_0_REG);
    tempCo = readRegister(TEMP_CO_REG);
    fullCapRep = readRegister(FULL_CAP_REP_REG);
    cycles = readRegister(CYCLES_REG);
    fullCapNom = readRegister(FULL_CAP_NOM_REG);
}

void MAX17262::writeLearnedParameters(u_int16_t rComp0, u_int16_t tempCo, u_int16_t fullCapRep, u_int16_t cycles, u_int16_t fullCapNom)
{
    writeRegister(R_COMP_0_REG, rComp0);
    writeRegister(TEMP_CO_REG, tempCo);
    writeRegister(FULL_CAP_REP_REG, fullCapRep);
    writeRegister(CYCLES_REG, cycles);
    writeRegister(FULL_CAP_NOM_REG, fullCapNom);
}