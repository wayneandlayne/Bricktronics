/* 
    Bricktronics Library for LEGO NXT Pushbutton sensors.
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

#ifndef BUTTON_H
#define BUTTON_H

#include "Sensors.h"

// Need this for the functions line pinMode, digitalRead, etc
// TODO is this still true?
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Button
{
    public:
        // Constructor - Simple constructor accepts an input pin
        Button(uint8_t inputPin);

        // Constructor - Advanced constructor accepts a SensorSettings
        // struct to also override the low-level Arduino functions.
        Button(const SensorSettings &settings);

        void begin(void);

        bool isPressed();
        bool isReleased();


    //private:
        // We really don't like to hide things inside private,
        // but if we did, these would be the private items.
        uint8_t _inputPin;

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

