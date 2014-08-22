/*
    Bricktronics library for LEGO NXT Sound Sensor.
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

#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

// Arduino header files
#include <inttypes.h>
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Library header files
#include "Settings.h"

// The sound sensor has two modes of operation, DB and DBA.
// DB mode considers all sound frequencies equally.
// DBA mode adapts the sensor's response to try and more-closely
// approximate the human hearing frequency response.
// The sensor uses pins 5 and 6 to determine which mode to use.
// Either pin 5 or pin 6 should be set high, and the other set low.
// Pin 5 high = DB mode, pin 6 hight = DBA mode.
#define SOUND_SENSOR_MODE_DB                            0
#define SOUND_SENSOR_MODE_DBA                           1

// The default mode for the sensor
#define SOUND_SENSOR_MODE_DEFAULT                       SOUND_SENSOR_MODE_DB

// The sound sensor is an analog sensor, and outputs a voltage inversely-
// proportional to the sound level (noisy = low voltage, quiet = high voltage).
// We use this value to flip this relationship,
// so now noisy = high value, quiet = low value, which seems more natural.
#define SOUND_SENSOR_BASE_VALUE                         1023

class SoundSensor
{
    public:
        // Constructor - Simple constructor accepts input pin and DB/DBA output pins.
        // The input pin needs to support analog input.
        // The output pins needs to support digital output (almost every pin).
        SoundSensor(uint8_t inputPin, uint8_t dbPin, uint8_t dbaPin);

        // Constructor - Advanced constructor accepts a SensorSettings
        // struct to also override the low-level Arduino functions.
        // This constructor is used for the Bricktronics Shield and Megashield.
        SoundSensor(const SensorSettings &settings);

        // Starts up the sensor
        void begin(void);

        // Basic sensor read function:
        // Scale is from 0 (very quiet) to 1023 (very loud), but usually the
        // actual range is from about 250 (quiet room) to 980 (loudspeaker).
        uint16_t value(void);

        // Change sensor mode between DB and DBA. Use these two values:
        // SOUND_SENSOR_DB and SOUND_SENSOR_DBA as the function argument.
        void setMode(uint8_t mode);
        uint8_t getMode(void);

    //private:
        // We really don't like to hide things inside private,
        // but if we did, these would be the private items.

        // Pin definitions
        uint8_t _inputPin;
        uint8_t _dbPin;
        uint8_t _dbaPin;

        uint8_t _mode;

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

