/*
   Combined Bricktronics Library
   For Bricktronics Shield, Bricktronics Megashield, and Bricktronics Motor Driver
   Copyright (C) 2014 Adam Wolf, Matthew Beckler, John Baichtal

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(Bricktronics &brick, uint8_t port)
{
    switch (port)
    {
        case 3:
            sclPin = BS_SENSOR_3_DA;
            sdaPin = BS_SENSOR_3_DB;
            break;
        case 4:
            sclPin = BS_SENSOR_4_DA;
            sdaPin = BS_SENSOR_4_DB;
            break;
    }
}

Ultrasonic::Ultrasonic(BricktronicsMegashield &brick, uint8_t port)
{
    switch (port)
    {
        case 1:
            sclPin = BMS_SENSOR_1_DA;
            sdaPin = BMS_SENSOR_1_DB;
            break;
        case 2:
            sclPin = BMS_SENSOR_2_DA;
            sdaPin = BMS_SENSOR_2_DB;
            break;
        case 3:
            sclPin = BMS_SENSOR_3_DA;
            sdaPin = BMS_SENSOR_3_DB;
            break;
        case 4:
            sclPin = BMS_SENSOR_4_DA;
            sdaPin = BMS_SENSOR_4_DB;
            break;
    }
}

Ultrasonic::Ultrasonic(uint8_t sclPin, uint8_t sdaPin)
{
    this->sclPin = sclPin;
    this->sdaPin = sdaPin;
}

void Ultrasonic::begin(void)
{
    i2c.init(sclPin, sdaPin);
}

char* Ultrasonic::readString(uint8_t startRegister, uint8_t bytes)
{
    readBytes(startRegister, bytes, bBuf);
    bBuf[BUFF_LEN - 1] = 0;
    return (char*) bBuf;
}

uint8_t Ultrasonic::readBytes(uint8_t startRegister, uint8_t bytes, uint8_t* buf)
{
    if (!(i2c.start(ULTRASONIC_ADDRESS | I2C_WRITE))) // Try to start, with a write address
    {
        return false; // If it fails, return false.
    }

    if (!(i2c.write(startRegister))) // Try to start a write
    {
        return false; // If it fails, return false.
    }

    // Do an i2c restart. See HDK for details.
    if (!(i2c.restart(ULTRASONIC_ADDRESS | I2C_READ)))
    {
        return false;
    }

    for (uint8_t i = 0; i < bytes-1; i++)
    {
        buf[i] = i2c.read(false); // Read, and then send ack
    }
    buf[bytes - 1] = i2c.read(true); // Read the last byte, then send nak

    i2c.stop();
    return true;
}

uint8_t Ultrasonic::readByte(uint8_t address)
{
    readBytes(address, 1, bBuf);
    return bBuf[0];
}

bool Ultrasonic::writeBytes(uint8_t firstRegisterAddress,
                            uint8_t numOfBytes,
                            uint8_t *buffer)
{
    if (!buffer)
    {
        buffer = bBuf;
    }

    if (!i2c.start(ULTRASONIC_ADDRESS | I2C_WRITE))
    {
        return false;
    }

    if (!i2c.write(firstRegisterAddress))
    {
        return false;
    }

    for (int i = 0; i < numOfBytes; i++)
    {
        if (!i2c.write(buffer[i]))
        {
            return false;
        }
    }
    i2c.stop();
    return true;
}

uint8_t Ultrasonic::writeByte(uint8_t address, uint8_t data)
{
    return writeBytes(address, 1, &data);
}

char* Ultrasonic::getVersion(void)
{
    return readString(GET_VERSION, 8);
}

char* Ultrasonic::getProductID(void)
{
    return readString(GET_PRODUCT_ID, 8);
}

char* Ultrasonic::getSensorType(void)
{
    return readString(GET_SENSOR_TYPE, 8);
}

uint8_t Ultrasonic::getDistance(void)
{
    writeByte(READ_COMMAND, ULTRASONIC_ADDRESS);
    delay(20);
    return readByte(READ_MEASUREMENT_BYTE_ZERO);
}

