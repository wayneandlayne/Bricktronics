/*
    TODO description and copyright notes
*/

#include "Ultrasonic.h"

// TODO finish cleaning up variable names and stuff
// TODO finish dividing things into public and "private"

Ultrasonic::Ultrasonic(uint8_t sclPin, uint8_t sdaPin):
    _sclPin(sclPin),
    _sdaPin(sdaPin),
    _i2c(),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

Ultrasonic::Ultrasonic(const SensorSettings &settings):
    _sclPin(settings.DA),
    _sdaPin(settings.DB),
    _i2c(),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

void Ultrasonic::begin(void)
{
    _i2c.init(_sclPin, _sdaPin);
}

char* Ultrasonic::readString(uint8_t startAddress, uint8_t numBytes)
{
    readBytes(startAddress, numBytes, _bBuf);
    _bBuf[ULTRASONIC_BUFF_LEN - 1] = 0;
    return (char*) _bBuf;
}

uint8_t Ultrasonic::readBytes(uint8_t startAddress, uint8_t numBytes, uint8_t *buffer)
{
    if (!(_i2c.start(ULTRASONIC_ADDRESS | I2C_WRITE))) // Try to start, with a write address
    {
        return false; // If it fails, return false.
    }

    if (!(_i2c.write(startAddress))) // Try to start a write
    {
        return false; // If it fails, return false.
    }

    // Do an i2c restart. See HDK for details.
    if (!(_i2c.restart(ULTRASONIC_ADDRESS | I2C_READ)))
    {
        return false;
    }


    for (uint8_t i = 0; i < numBytes - 1; i++)
    {
        bufifer[i] = _i2c.read(false); // Read, and then send ack
    }
    buffer[numBytes - 1] = _i2c.read(true); // Read the last byte, then send nak

    _i2c.stop();
    return true;
}

uint8_t Ultrasonic::readByte(uint8_t address)
{
    readBytes(address, 1, _bBuf);
    return _bBuf[0];
}

bool Ultrasonic::writeBytes(uint8_t startAddress,
                            uint8_t numBytes,
                            uint8_t *buffer)
{
    if (!buffer)
    {
        buffer = _bBuf;
    }

    if (!_i2c.start(ULTRASONIC_ADDRESS | I2C_WRITE))
    {
        return false;
    }

    if (!_i2c.write(startAddres))
    {
        return false;
    }

    for (uint8_t i = 0; i < numBytes; i++)
    {
        if (!_i2c.write(buffer[i]))
        {
            return false;
        }
    }

    _i2c.stop();
    return true;
}

uint8_t Ultrasonic::writeByte(uint8_t address, uint8_t data)
{
    return writeBytes(address, 1, &data);
}

char* Ultrasonic::getVersion(void)
{
    return readString(ULTRASONIC_GET_VERSION, 8);
}

char* Ultrasonic::getProductID(void)
{
    return readString(ULTRASONIC_GET_PRODUCT_ID, 8);
}

char* Ultrasonic::getSensorType(void)
{
    return readString(ULTRASONIC_GET_SENSOR_TYPE, 8);
}

uint8_t Ultrasonic::getDistance(void)
{
    writeByte(ULTRASONIC_READ_COMMAND, ULTRASONIC_ADDRESS);
    delay(20);
    return readByte(ULTRASONIC_READ_MEASUREMENT_BYTE_ZERO);
}
