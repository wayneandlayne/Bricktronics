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

// The light sensor is an analog sensor, and outputs a voltage inversely-
// proportional to the brightness (high-brightness = low voltage,
// low-brightness = high voltage). We use this value to flip this relationship,
// so now bright = high value, dim = low value, which seems more natural.
#define LIGHT_SENSOR_BASE_VALUE                         1023

// If you choose to use the floodlight to illuminate the sensor's target,
// it helps to have a small delay after turning on the floodlight before
// you read the analog value from the sensor. This is the default value,
// but you can always update the delay using the function below.
#define LIGHT_SENSOR_FLOODLIGHT_DELAY_VALUE_IN_MS       50

// These control whether the light is turned on never, only for readings,
// or always. It seems to work better without the light on.
#define LIGHT_SENSOR_FLOODLIGHT_USE_DEFAULT             false
#define LIGHT_SENSOR_FLOODLIGHT_USE_ALWAYS_DEFAULT      false

// This library allows you to calibrate the sensor for "dim" and "bright".
// These constants are the default values used if you don't calibrate it.
#define LIGHT_SENSOR_BASE_HIGH_VALUE                    990
#define LIGHT_SENSOR_BASE_LOW_VALUE                     750

class LightSensor
{
    public:
        // Constructor - Simple constructor accepts input and light control pin
        // The input pin needs to support analog input.
        // The output pin needs to support digital output (almost every pin).
        LightSensor(uint8_t inputPin, uint8_t lightPin);

        // Constructor - Advanced constructor accepts a SensorSettings
        // struct to also override the low-level Arduino functions.
        // This constructor is used for the Bricktronics Shield and Megashield.
        LightSensor(const SensorSettings &settings);

        // Starts up the sensor
        void begin(void);

        // Basic sensor read function:
        // If you want the light on, it turns it on here, delays 50ms,
        // performs the analog read, then turns off the light (if it was on).
        // Scale is from 0 (very dark) to 1023 (very bright), but usually the
        // actual range is 550 to 990.
        uint16_t value(void);

        // Scales the raw brightness from value() into an int between 0 and 100
        // Uses _calibrationLowValue and _calibrationHighValue for scaling range.
        // Constrained to return a value between 0 and 100 inclusive.
        uint8_t scaledValue(void);

        // Calibration functions - Point the light sensor at your bright
        // and dark objects/surfaces, then call this function.
        // Takes the average of 16 samples and updates the internal
        // calibration values for use with lightValue.
        // Both functions return true if the calibration passes a simple
        // sanity check (that is, if _calibrationLowValue < _calibrationHighValue).
        bool calibrateLow(void);    // For your "dark" object/surface
        bool calibrateHigh(void);   // For your "bright" object/surface

        // Manual calibration functions
        void setCalibrationLowValue(uint16_t value);
        uint16_t getCalibrationLowValue(void);
        void setCalibrationHighValue(uint16_t value);
        uint16_t getCalibrationHighValue(void);

        // The sensor has a small light that can be used to illuminate the
        // sensor's target. There are two settings to control this light, and
        // by default the light never turns on.
        // - If both are false, the light is never turned on (default).
        // - If setFloodlight is enabled, the light is turned on for each reading.
        // - If setFloodlightAlways in enabled, the light is always on,
        //   and the value of setFloodlight is ignored.
        // If you want to manually control the light, call setFloodlight(false)
        // and use setFloodlightAlways(true / false) to turn it on and off.
        void setFloodlight(bool enable);
        bool getFloodlight(void);
        void setFloodlightAlways(bool enable);
        bool getFloodlightAlways(void);

        // If you have set the sensor to only turn on the light while reading the
        // sensor's value, this function lets you control how long to delay between
        // turning on the light and taking the reading. Value is in milliseconds.
        void setFloodlightDelayInMs(uint16_t delayInMs);
        uint16_t getFloodlightDelayInMs(void);

    //private:
        // We really don't like to hide things inside private,
        // but if we did, these would be the private items.
        uint8_t _inputPin;
        uint8_t _lightPin;
        uint16_t _floodlightDelayInMs;
        bool _useFloodlight;
        bool _useFloodlightAlways;
        uint16_t _calibrationHighValue;
        uint16_t _calibrationLowValue;

        // Used by the calibration functions above,
        // this performs the actual sampling and averaging.
        void _calibrate(uint16_t *which);

        // This performs a sanity check on the current calibration settings
        bool _calibrationSanityCheck(void);

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

