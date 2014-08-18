/*
   Bricktronics library for LEGO NXT Light sensors.
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

   Wayne and Layne, LLC and our products are not connected to or endorsed by the LEGO Group.
   LEGO, Mindstorms, and NXT are trademarks of the LEGO Group.
 */

#include "LightSensor.h"

// This is the simplified constructor that allows you to specify only the
// two pins to connect the Light Sensor.
LightSensor::LightSensor(uint8_t inputPin, uint8_t lightPin, bool floodlight):
    _inputPin(inputPin),
    _lightPin(lightPin),
    _useFloodlight(floodlight),
    _highValue(LIGHT_SENSOR_BASE_HIGH_VALUE),
    _lowValue(LIGHT_SENSOR_BASE_LOW_VALUE),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

// This is the complicated constructor that allows for overriding the
// low-level Arduino functions.
LightSensor::LightSensor(const SensorSettings &settings, bool floodlight):
    _inputPin(settings.ANA),
    _lightPin(settings.DA),
    _useFloodlight(floodlight),
    _highValue(LIGHT_SENSOR_BASE_HIGH_VALUE),
    _lowValue(LIGHT_SENSOR_BASE_LOW_VALUE),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

void LightSensor::begin(void)
{
    // TODO do we really want to set the input pin as a input w/pullup?
    // we are going to be doing analogRead(_inputPin) later on...
    _pinMode(_inputPin, INPUT_PULLUP);
    _digitalWrite(_inputPin, HIGH);

    _pinMode(_lightPin, OUTPUT);
    _digitalWrite(_lightPin, LOW); // default to light off, right? TODO
}

bool LightSensor::isDark(void)
{
    return (lightValue() < 50); //TODO fix magic number
}

int LightSensor::lightValue()
{
    return 100 * ( (float)( value() - _lowValue ) / (float)( _highValue - _lowValue ) );
}

int LightSensor::value(void)
{
    if (_useFloodlight)
    {
        _digitalWrite(_lightPin, HIGH);
    }
    _digitalWrite(_inputPin, HIGH);
    int sensorValue = analogRead(_inputPin);
    delay(50); // TODO magic number, also how long does this really need to be?
    if (_useFloodlight)
    {
        _digitalWrite(_lightPin, LOW);
    }
    return (LIGHT_SENSOR_BASE_VALUE - sensorValue);
}

void LightSensor::_calibrate(int *limit)
{
    int calibrateValue = 0;
    for (uint8_t i = 0; i < 16; i++ )
    {
        calibrateValue += value();
    }
    *limit = calibrateValue >> 4;
}

bool LightSensor::calibrateLow()
{
    _calibrate(&_lowValue);
    return (_lowValue < _highValue);
}

bool LightSensor::calibrateHigh()
{
    _calibrate(&_highValue);
    return (_lowValue < _highValue);
}

