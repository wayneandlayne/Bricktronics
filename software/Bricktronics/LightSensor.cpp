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
LightSensor::LightSensor(uint8_t inputPin, uint8_t lightPin):
    _inputPin(inputPin),
    _lightPin(lightPin),
    _floodlightDelayInMs(LIGHT_SENSOR_FLOODLIGHT_DELAY_VALUE_IN_MS),
    _useFloodlight(LIGHT_SENSOR_FLOODLIGHT_USE_DEFAULT),
    _useFloodlightAlways(LIGHT_SENSOR_FLOODLIGHT_USE_ALWAYS_DEFAULT),
    _calibrationHighValue(LIGHT_SENSOR_BASE_HIGH_VALUE),
    _calibrationLowValue(LIGHT_SENSOR_BASE_LOW_VALUE),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

// This is the complicated constructor that allows for overriding the
// low-level Arduino functions.
LightSensor::LightSensor(const SensorSettings &settings):
    _inputPin(settings.ANA),
    _lightPin(settings.DA),
    _floodlightDelayInMs(LIGHT_SENSOR_FLOODLIGHT_DELAY_VALUE_IN_MS),
    _useFloodlight(LIGHT_SENSOR_FLOODLIGHT_USE_DEFAULT),
    _useFloodlightAlways(LIGHT_SENSOR_FLOODLIGHT_USE_ALWAYS_DEFAULT),
    _calibrationHighValue(LIGHT_SENSOR_BASE_HIGH_VALUE),
    _calibrationLowValue(LIGHT_SENSOR_BASE_LOW_VALUE),
    _pinMode(settings.pinMode),
    _digitalWrite(settings.digitalWrite),
    _digitalRead(settings.digitalRead)
{
    // Nothing to do here
}

void LightSensor::begin(void)
{
    // Set input pin as an input (analog input)
    _pinMode(_inputPin, INPUT_PULLUP);

    // Set light pin as an output, default to light off
    _pinMode(_lightPin, OUTPUT);
    _digitalWrite(_lightPin, LOW);
}

void LightSensor::setFloodlight(bool enable)
{
    _useFloodlight = enable;
}
bool LightSensor::getFloodlight(void)
{
    return _useFloodlight;
}

void LightSensor::setFloodlightAlways(bool enable)
{
    _useFloodlightAlways = enable;
    _digitalWrite(_lightPin, enable ? HIGH : LOW);
}
bool LightSensor::getFloodlightAlways(void)
{
    return _useFloodlightAlways;
}

void LightSensor::setFloodlightDelayInMs(uint16_t delayInMs)
{
    _floodlightDelayInMs = delayInMs;
}
uint16_t LightSensor::getFloodlightDelayInMs(void)
{
    return _floodlightDelayInMs;
}

int LightSensor::value(void)
{
    int sensorValue;

    if (_useFloodlight && !_useFloodlightAlways)
    {
        _digitalWrite(_lightPin, HIGH);
        delay(_floodlightDelayInMs);
    }
    sensorValue = analogRead(_inputPin);
    if (_useFloodlight && !_useFloodlightAlways)
    {
        _digitalWrite(_lightPin, LOW);
    }
    return (LIGHT_SENSOR_BASE_VALUE - sensorValue);
}

uint8_t LightSensor::scaledValue(void)
{
    return constrain(map(value(), _calibrationLowValue, _calibrationHighValue, 0, 100), 0, 100);
}

void LightSensor::setCalibrationLowValue(int value)
{
    _calibrationLowValue = value;
}
int LightSensor::getCalibrationLowValue(void)
{
    return _calibrationLowValue;
}

void LightSensor::setCalibrationHighValue(int value)
{
    _calibrationHighValue = value;
}
int LightSensor::getCalibrationHighValue(void)
{
    return _calibrationHighValue;
}

bool LightSensor::calibrateLow()
{
    _calibrate(&_calibrationLowValue);
    return _calibrationSanityCheck();
}

bool LightSensor::calibrateHigh()
{
    _calibrate(&_calibrationHighValue);
    return _calibrationSanityCheck();
}

void LightSensor::_calibrate(int *which)
{
    *which = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        *which += value();
    }
    *which >>= 4;
}

bool LightSensor::_calibrationSanityCheck(void)
{
    // Someday add other checks? This should be good for now.
    return (_calibrationLowValue < _calibrationHighValue);
}

