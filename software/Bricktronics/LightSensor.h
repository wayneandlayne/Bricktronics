/*
    Bricktronics library for LEGO NXT Light Sensor.
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

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

// Arduino header files
#include <inttypes.h>
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Library header files
#include "Settings.h"

#define LIGHT_SENSOR_BASE_VALUE 1023
#define LIGHT_SENSOR_BASE_HIGH_VALUE 990
#define LIGHT_SENSOR_BASE_LOW_VALUE 750

class LightSensor
{
    public:
        // Constructor - Simple constructor accepts input and light control pin
        // You can also specify whether or not to light up the light.
        LightSensor(uint8_t inputPin, uint8_t lightPin, bool floodlight);

        // Constructor - Advanced constructor accepts a SensorSettings
        // struct to also override the low-level Arduino functions.
        // You can also specify whether or not to light up the light.
        LightSensor(const SensorSettings &settings, bool floodlight);

        // Starts up the sensor
        void begin(void);

        // TODO document these functions

        // Scales the raw brightness into an int between 0 and 100
        // Uses _lowValue _highValue for scaling range
        int lightValue();

        // Reads a raw, unscaled brightness value from the sensor
        int value();

        // Detects if the measured brightness is below a threshold
        bool isDark();

        // Calibration functions - Point the light sensor at your bright
        // and dark objects/surfaces, then call this function.
        // Takes the average of 16 samples and updates _lowValue or _highValue.
        bool calibrateLow();
        bool calibrateHigh();

    //private:
        // We really don't like to hide things inside private,
        // but if we did, these would be the private items.
        uint8_t _inputPin;
        uint8_t _lightPin;
        bool _useFloodlight;
        int _highValue;
        int _lowValue;

        void _calibrate(int *limit);

        // For the Bricktronics Shield, which has an I2C I/O expander chip,
        // we need a way to override some common Arduino functions. We use
        // function pointers here to handle this. For the non-Bricktronics
        // Shield cases, the simple constructor above provides the built-in
        // functions.
        void (*_pinMode)(uint8_t, uint8_t);
        void (*_digitalWrite)(uint8_t, uint8_t);
        int (*_digitalRead)(uint8_t);
};

#endif

