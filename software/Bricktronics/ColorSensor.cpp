/*
   Combined Bricktronics Library
   For Bricktronics Shield, Bricktronics Megashield, and Bricktronics Motor Driver
   Copyright (C) 2014 Adam Wolf, Matthew Beckler, John Baichtal

    The contents of this file are subject to the Mozilla Public License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
    Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

    The Original Code is from leJos.

    The Initial Developer of the Original Code is leJos. All Rights Reserved.

    Contributor(s): Adam Wolf, Matthew Beckler, John Baichtal.

    Most of the stuff here is original to Wayne and Layne, but one or two functions are straight from leJos, an open source alternative firmware for the NXT.
*/

#include "ColorSensor.h"

ColorSensor::ColorSensor(Bricktronics &brick, uint8_t port)
{
    switch (port)
    {
        case 3:
            _clock = BS_SENSOR_3_DA;
            _data = BS_SENSOR_3_ANA;
            break;
        case 4:
            _clock = BS_SENSOR_4_DA;
            _data = BS_SENSOR_4_ANA;
            break;
    }
}

ColorSensor::ColorSensor(BricktronicsMegashield &brick, uint8_t port)
{
    switch (port)
    {
        case 1:
            _clock = BMS_SENSOR_1_DA;
            _data = BMS_SENSOR_1_ANA;
            break;
        case 2:
            _clock = BMS_SENSOR_2_DA;
            _data = BMS_SENSOR_2_ANA;
            break;
        case 3:
            _clock = BMS_SENSOR_3_DA;
            _data = BMS_SENSOR_3_ANA;
            break;
        case 4:
            _clock = BMS_SENSOR_4_DA;
            _data = BMS_SENSOR_4_ANA;
            break;
    }
}

ColorSensor::ColorSensor(uint8_t clockPin, uint8_t dataPin)
{
    _clock = clockPin;
    _data = dataPin;
}

void ColorSensor::begin(void)
{
    begin(TYPE_COLORFULL);
}

void ColorSensor::begin(uint8_t modeType)
{
    mode = modeType;
    type = modeType;

    resetSensor();
    sendMode(mode);
    delayMicroseconds(3200);
    readCalibration();

    // This delay can be removed if you have other setup code
    // (that doesn't use the color sensor) that takes a while.
    // Basically you can't get useful data out of the sensor
    // until at least 120ms has passed since you did the init.
    delay(120);
}

void ColorSensor::setClock(int val)
{
    pinMode(_clock, OUTPUT);
    digitalWriteFast2(_clock, val);
}

void ColorSensor::setData(int val)
{
    pinMode(_data, OUTPUT);
    digitalWriteFast2(_data, val);
}

bool ColorSensor::getData()
{
    pinMode(_data, INPUT);
    return ( digitalReadFast2(_data) != 0 );
}

int ColorSensor::readData()
{
    pinMode(_data, INPUT);
    digitalWriteFast2(_data, LOW);
    return analogRead(_data) * 50/33;
}

void ColorSensor::resetSensor()
{
    setClock(1);
    setData(1);
    delay(1);
    setClock(0);
    delay(1);
    setClock(1);
    delay(1);
    setClock(0);
    delay(100);
}

void ColorSensor::sendMode(unsigned int mode)
{
    for (int i = 0; i < 8; i++)
    {
        setClock(1);
        setData(mode & 1);
        delayMicroseconds(30);
        setClock(0);
        mode >>= 1;
        delayMicroseconds(30);
    }
}

char ColorSensor::readByte()
{
    // This is way slower than how LEGO does it... tens of microseconds slower!
    // When I tested this (matching the speed the LEGO does it) I had some issues.
    // Look into this, TODO.
    unsigned char val = 0;
    for (int i = 0; i< 8; i++)
    {
        setClock(1);
        val >>= 1;
        if (getData())
        {
            val |= 0x80;
        }
        setClock(0);
    }
    return val;
}

unsigned int ColorSensor::calcCRC(unsigned int crc, unsigned int val)
{
    for (int i = 0; i < 8; i++)
    {
        if (((val ^ crc) & 1) != 0)
        {
            crc = ((crc >> 1) ^ 0xa001); // the >> should shift in a zero
        }
        else
        {
            crc >>= 1; // the >> should shift in a zero
        }
        val >>= 1; // the >> should shift in a zero
    }
    return ( crc & 0xffff );
}

bool ColorSensor::readCalibration()
{
    uint16_t crcVal = 0x5aa5;
    uint8_t input;
    pinMode(_data, INPUT);
    for (int i = 0; i < CAL_ROWS; i++)
    {
        for (int col = 0; col < CAL_COLUMNS; col++)
        {
            uint32_t val = 0;
            uint8_t shiftVal = 0;
            for (int k = 0; k < 4; k++)
            {
                input = readByte();
                crcVal = calcCRC(crcVal, input);
                val |= ((uint32_t) input << shiftVal);
                shiftVal += 8;
            }
            calData[i][col] = val;
        }
    }
    for (int i = 0; i < CAL_LIMITS; i++)
    {
        unsigned long val = 0;
        int shift = 0;
        for (int k = 0; k < 2; k++)
        {
            input = readByte();
            crcVal = calcCRC(crcVal, input);
            val |= input << shift;
            shift += 8;
        }

        calLimits[i] = val;
    }
    unsigned int crc = (readByte() << 8);
    crc += readByte();
    crc &= 0xffff;
    delay(2);
    return true;
}

bool ColorSensor::checkSensor()
{
    pinMode(_clock, INPUT);
    delay(2);
    if (digitalRead(_clock))
    {
        return false; // error
    }
    else
    {
        return true; // not error
    }
}

int ColorSensor::readFullColorValue(int newClock)
{
    delayMicroseconds(10);
    int val = readData();
    int val2 = readData();
    setClock(newClock);
    return (val + val2) / 2;
}


void ColorSensor::readSensor()
{
    if (type == TYPE_COLORFULL)
    {
        rawValues[INDEX_BLANK] = readFullColorValue(1);
        rawValues[INDEX_RED] = readFullColorValue(0);
        rawValues[INDEX_GREEN] = readFullColorValue(1);
        rawValues[INDEX_BLUE] = readFullColorValue(0);
    }
    else
    {
        rawValues[type - TYPE_COLORRED] = readData();
    }
}

int ColorSensor::readRawValue()
{
    if (type < TYPE_COLORRED)
    {
        return -1;
    }
    readSensor();
    return rawValues[type - TYPE_COLORRED];
}

// calibrate rawValues to calValues
int ColorSensor::calibrate()
{
    int calTab;
    int blankVal = rawValues[INDEX_BLANK];
    if (blankVal < calLimits[1])
    {
        calTab = 2;
    }
    else if (blankVal < calLimits[0])
    {
        calTab = 1;
    }
    else
    {
        calTab = 0;
    }

    for (int col = INDEX_RED; col <= INDEX_BLUE; col++)
    {
        if (rawValues[col] > blankVal)
        {
            calValues[col] = ((rawValues[col] - blankVal) * calData[calTab][col]) >> 16;
            // TODO check shift!
        }
        else
        {
            calValues[col] = 0;
        }
    }
        
    if (blankVal > MINBLANKVAL)
    {
        blankVal -= MINBLANKVAL;
    }
    else
    {
        blankVal = 0;
    }

    blankVal = (blankVal * 100) / (((SENSORMAX - MINBLANKVAL ) * 100) / ADMAX);

    calValues[INDEX_BLANK] = (blankVal * calData[calTab][INDEX_BLANK]) >> 16 ;
    // TODO CHECK SHIFT
}

void ColorSensor::printColor(uint8_t color)
{
    // TODO: use PROGMEM for these strings
    switch (color)
    {
        case COLOR_BLACK:
            Serial.print("black");
            break;
        case COLOR_BLUE:
            Serial.print("blue");
            break;
        case COLOR_GREEN:
            Serial.print("green");
            break;
        case COLOR_YELLOW:
            Serial.print("yellow");
            break;
        case COLOR_RED:
            Serial.print("red");
            break;
        case COLOR_WHITE:
            Serial.print("white");
            break;
        default:
            Serial.println("error");
    }
}


uint8_t ColorSensor::calToColor()
{
    int red     = calValues[INDEX_RED];
    int blue    = calValues[INDEX_BLUE];
    int green   = calValues[INDEX_GREEN];
    int blank   = calValues[INDEX_BLANK];

    // The following algorithm comes from the 1.29 Lego firmware.
    if (red > blue && red > green)
    {
        // red dominant color
        if (red < 65 || (blank < 40 && red < 110))
            return COLOR_BLACK;
        if ( ((blue >> 2) + (blue >> 3) + blue < green) && ((green << 1) > red))
            return COLOR_YELLOW;
        if ((green << 1) - (green >> 2) < red)
            return COLOR_RED;
        if (blue < 70 || green < 70 || (blank < 140 && red < 140))
            return COLOR_BLACK;
        return COLOR_WHITE;
    }
    else if (green > blue)
    {
        // green dominant color
        if (green < 40 || (blank < 30 && green < 70))
            return COLOR_BLACK;
        if ((blue << 1) < red)
            return COLOR_YELLOW;
        if ((red + (red >> 2)) < green || (blue + (blue>>2)) < green )
            return COLOR_GREEN;
        if (red < 70 || blue < 70 || (blank < 140 && green < 140))
            return COLOR_BLACK;
        return COLOR_WHITE;
    }
    else
    {
        // blue dominant color
        if (blue < 48 || (blank < 25 && blue < 85))
            return COLOR_BLACK;
        if ((((red*48) >> 5) < blue && ((green*48) >> 5) < blue) || ((red*58) >> 5) < blue || ((green*58) >> 5) < blue)
            return COLOR_BLUE;
        if (red < 60 || green < 60 || (blank < 110 && blue < 120))
            return COLOR_BLACK;
        if ((red + (red >> 3)) < blue || (green + (green >> 3)) < blue)
            return COLOR_BLUE;
        return COLOR_WHITE;
    }
}

uint8_t ColorSensor::getColor()
{
    readSensor();
    calibrate();
    return calToColor();
}

