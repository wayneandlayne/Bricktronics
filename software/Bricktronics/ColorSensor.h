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

#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#define COLOR_BLACK         1
#define COLOR_BLUE          2
#define COLOR_GREEN         3
#define COLOR_YELLOW        4
#define COLOR_RED           5
#define COLOR_WHITE         6

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
        // This constructor is for when you are using the Bricktronics Shield
        ColorSensor(Bricktronics &brick, uint8_t port);

        // This constructor is for when you are using the Bricktronics Megashield
        ColorSensor(BricktronicsMegashield &brick, uint8_t port);

        // This constructor is for when you are using the sensor directly
        ColorSensor(uint8_t clockPin, uint8_t dataPin);

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
        void printColor(uint8_t);


    //private:
        // We don't really like to hide things inside "private", here's why:
        // 1. If you don't know what you're doing, stick to the items above,
        // and you'll be just fine whether or not we hide these items below.
        // 2. If you do know what you're doing, we're saving you a bit of time
        // having to change the library to make these items below accessible.
        unsigned long calData[CAL_ROWS][CAL_COLUMNS];

        long calLimits[CAL_LIMITS];

        uint16_t rawValues[4];
        uint16_t calValues[4];

        uint16_t mode;
        uint16_t type;

        uint8_t _data;
        uint8_t _clock;

        void setClock(int val);
        void setData(int val);
        bool getData();
        int readData();
        void sendMode(unsigned int mode);
        char readByte();
        unsigned int calcCRC(unsigned int crc, unsigned int val);
        bool readCalibration();
        bool checkSensor();
        int readFullColorValue(int newClock);
        void readSensor();
        int readRawValue();
        uint8_t calToColor();

        void resetSensor();
        void update();
        int calibrate();
};

#endif

