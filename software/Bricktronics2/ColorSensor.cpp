/*
    Bricktronics library for LEGO NXT Color sensors.
    Copyright (C) 2014 Adam Wolf, Matthew Beckler, John Baichtal

    The contents of this file are subject to the Mozilla Public License Version 1.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
    Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for the specific language governing rights and limitations under the License.

    The Original Code is from leJos.

    The Initial Developer of the Original Code is leJos. All Rights Reserved.

    Contributor(s): Adam Wolf, Matthew Beckler, John Baichtal.

    Most of the stuff here is original to Wayne and Layne, but one or two functions are straight from leJos, an open source alternative firmware for the NXT.

    Wayne and Layne, LLC and our products are not connected to or endorsed by the LEGO Group.
    LEGO, Mindstorms, and NXT are trademarks of the LEGO Group.
*/

#include "ColorSensor.h"

ColorSensor::ColorSensor(uint8_t clockPin, uint8_t dataPin):
    _clockPin(clockPin),
    _dataPin(dataPin),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

ColorSensor::ColorSensor(const SensorSettings &settings):
    _clockPin(settings.DA),
    _dataPin(settings.ANA),
    _pinMode(settings.pinMode),
    _digitalWrite(settings.digitalWrite),
    _digitalRead(settings.digitalRead)
{
    // Nothing to do here
}

void ColorSensor::begin(void)
{
    begin(TYPE_COLORFULL);
}

void ColorSensor::begin(uint8_t modeType)
{
    _type = modeType;

    _resetSensor();
    _sendMode(_type);
    delayMicroseconds(3200);
    _readCalibration();

    // This delay can be removed if you have other setup code
    // (that doesn't use the color sensor) that takes a while.
    // Basically you can't get useful data out of the sensor
    // until at least 120ms has passed since you did the init.
    delay(120);
}

uint8_t ColorSensor::getColor()
{
    _readSensor();
    _calibrate();
    return _calToColor();
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

void ColorSensor::_setClock(uint8_t val)
{
    _pinMode(_clockPin, OUTPUT);
    digitalWriteFast2(_clockPin, val);
}

void ColorSensor::_setData(uint8_t val)
{
    _pinMode(_dataPin, OUTPUT);
    digitalWriteFast2(_dataPin, val);
}

bool ColorSensor::_getData()
{
    _pinMode(_dataPin, INPUT);
    return ( digitalReadFast2(_dataPin) != 0 );
}

int ColorSensor::_readData()
{
    _pinMode(_dataPin, INPUT);
    digitalWriteFast2(_dataPin, LOW);
    return analogRead(_dataPin) * 50/33; // TODO add () to avoid ambiguity
}

void ColorSensor::_resetSensor()
{
    _setClock(1);
    _setData(1);
    delay(1);
    _setClock(0);
    delay(1);
    _setClock(1);
    delay(1);
    _setClock(0);
    delay(100);
}

void ColorSensor::_sendMode(uint8_t mode)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        _setClock(1);
        _setData(mode & 1);
        delayMicroseconds(30);
        _setClock(0);
        mode >>= 1;
        delayMicroseconds(30);
    }
}

uint8_t ColorSensor::_readByte()
{
    // This is way slower than how LEGO does it... tens of microseconds slower!
    // When I tested this (matching the speed the LEGO does it) I had some issues.
    // TODO look into this
    uint8_t val = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        _setClock(1);
        val >>= 1;
        if (_getData())
        {
            val |= 0x80;
        }
        _setClock(0);
    }
    return val;
}

uint16_t ColorSensor::_calcCRC(uint16_t crc, uint16_t val)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (((val ^ crc) & 1) != 0)
        {
            crc = ((crc >> 1) ^ 0xA001);
        }
        else
        {
            crc >>= 1;
        }
        val >>= 1;
    }
    return ( crc & 0xFFFF );
}

bool ColorSensor::_readCalibration()
{
    uint16_t crcVal = 0x5AA5;
    uint8_t input;
    _pinMode(_dataPin, INPUT);
    for (uint8_t row = 0; row < CAL_ROWS; row++)
    {
        for (uint8_t col = 0; col < CAL_COLUMNS; col++)
        {
            uint32_t val = 0;
            uint8_t shift = 0;
            for (uint8_t k = 0; k < 4; k++)
            {
                input = _readByte();
                crcVal = _calcCRC(crcVal, input);
                val |= ((uint32_t) input << shift);
                shift += 8;
            }
            _calData[row][col] = val;
        }
    }
    for (uint8_t i = 0; i < CAL_LIMITS; i++)
    {
        uint32_t val = 0;
        uint8_t shift = 0;
        for (uint8_t k = 0; k < 2; k++)
        {
            input = _readByte();
            crcVal = _calcCRC(crcVal, input);
            val |= ((uint32_t) input << shift);
            shift += 8;
        }
        _calLimits[i] = val;
    }
    uint16_t crc = (_readByte() << 8);
    crc += _readByte();
    crc &= 0xffff;
    // TODO what is crc used for?
    delay(2);

    return true;
}

bool ColorSensor::_checkSensor()
{
    _pinMode(_clockPin, INPUT);
    delay(2);
    if (_digitalRead(_clockPin))
    {
        return false; // error
    }
    else
    {
        return true; // not error
    }
}

int ColorSensor::_readFullColorValue(uint8_t newClock)
{
    delayMicroseconds(10);
    int val = _readData();
    int val2 = _readData();
    _setClock(newClock);
    return (val + val2) >> 1; // divide by 2
}


void ColorSensor::_readSensor()
{
    if (_type == TYPE_COLORFULL)
    {
        _rawValues[INDEX_BLANK]  = _readFullColorValue(1);
        _rawValues[INDEX_RED]    = _readFullColorValue(0);
        _rawValues[INDEX_GREEN]  = _readFullColorValue(1);
        _rawValues[INDEX_BLUE]   = _readFullColorValue(0);
    }
    else
    {
        _rawValues[_type - TYPE_COLORRED] = _readData();
    }
}

int ColorSensor::_readRawValue()
{
    if (_type < TYPE_COLORRED)
    {
        return -1;
    }
    _readSensor();
    return _rawValues[_type - TYPE_COLORRED];
}

// calibrate rawValues to calValues
void ColorSensor::_calibrate()
{
    uint8_t calTab;
    uint16_t blankVal = _rawValues[INDEX_BLANK];
    if (blankVal < _calLimits[1])
    {
        calTab = 2;
    }
    else if (blankVal < _calLimits[0])
    {
        calTab = 1;
    }
    else
    {
        calTab = 0;
    }

    for (uint8_t col = INDEX_RED; col <= INDEX_BLUE; col++)
    {
        if (_rawValues[col] > blankVal)
        {
            _calValues[col] = ((_rawValues[col] - blankVal) * _calData[calTab][col]) >> 16;
            // TODO check shift!
        }
        else
        {
            _calValues[col] = 0;
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

    // TODO check for integer overflow here, might need to cast multiplicand bigger
    blankVal = (blankVal * 100) / (((SENSORMAX - MINBLANKVAL ) * 100) / ADMAX);

    _calValues[INDEX_BLANK] = (blankVal * _calData[calTab][INDEX_BLANK]) >> 16 ;
    // TODO CHECK SHIFT
}

uint8_t ColorSensor::_calToColor()
{
    uint16_t red     = _calValues[INDEX_RED];
    uint16_t blue    = _calValues[INDEX_BLUE];
    uint16_t green   = _calValues[INDEX_GREEN];
    uint16_t blank   = _calValues[INDEX_BLANK];

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


