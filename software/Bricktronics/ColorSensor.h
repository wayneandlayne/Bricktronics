/*
    Bricktronics Library for LEGO NXT Color sensors.
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

#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#define COLOR_BLACK         1
#define COLOR_BLUE          2
#define COLOR_GREEN         3
#define COLOR_YELLOW        4
#define COLOR_RED           5
#define COLOR_WHITE         6

// Don't re-order these, they are used as loop indices
#define INDEX_RED           0
#define INDEX_GREEN         1
#define INDEX_BLUE          2
#define INDEX_BLANK         3

#define TYPE_COLORFULL      13
#define TYPE_COLORRED       14
#define TYPE_COLORGREEN     15
#define TYPE_COLORBLUE      16
#define TYPE_COLORNONE      17

#define ADVOLTS             3300
#define ADMAX               1023
#define SENSORMAX           ADMAX
#define MINBLANKVAL         (214 / (ADVOLTS / ADMAX))

#define CAL_COLUMNS         4
#define CAL_ROWS            3
#define CAL_LIMITS          2

#include "Bricktronics.h"

class ColorSensor
{
    public:
        // Constructor - Simple constructor accepts the clock and data pins
        ColorSensor(uint8_t clockPin, uint8_t dataPin);

        // Constructor - Advanced constructor accepts a SensorSettings
        // struct to also override the low-level Arduino functions
        ColorSensor(const SensorSettings &settings);

        // Starts up the sensor. Since it uses an RGB LED to shine light to
        // detect the reflectance, you can specify what color to shine:
        // Either all colors (TYPE_COLORFULL, the default) or specify a
        // specific color mode type.
        void begin(void);
        void begin(uint8_t modeType);

        // Read the sensor and return a COLOR_* value
        uint8_t getColor(void);

        // Printes out a human-readable color name to the Serial port
        // Pass in the output from getColor()
        // TODO make this Steam-compatible so it can work with other streams?
        void printColor(uint8_t);


    //private:
        // We really don't like to hide things inside private,
        // but if we did, these would be the private items.

        uint32_t _calData[CAL_ROWS][CAL_COLUMNS];

        int32_t _calLimits[CAL_LIMITS];

        uint16_t _rawValues[4];
        uint16_t _calValues[4];

        uint16_t _type;

        uint8_t _clockPin;
        uint8_t _dataPin;

        void _setClock(uint8_t val);
        void _setData(uint8_t val);
        bool _getData();
        int _readData();
        void _sendMode(uint8_t mode);
        uint8_t _readByte();
        unsigned int _calcCRC(unsigned int crc, unsigned int val);
        bool _readCalibration();
        bool _checkSensor();
        int _readFullColorValue(uint8_t newClock);
        void _readSensor();
        int _readRawValue();
        uint8_t _calToColor();

        void _resetSensor();
        void _update();
        int _calibrate();

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

