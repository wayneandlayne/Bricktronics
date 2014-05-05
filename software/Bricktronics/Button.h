/*
   Combined Bricktronics Library
   For Bricktronics Shield, Bricktronics Megashield, and Bricktronics Motor Driver
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
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "Bricktronics.h"

class Button
{
    public:
        // This constructor is for when you are using the Bricktronics Shield
        Button(Bricktronics &brick, uint8_t port);

        // This constructor is for when you are using the Bricktronics Megashield
        Button(BricktronicsMegashield &brick, uint8_t port);

        // This constructor is for when you are directly connecting to the sensor
        Button(uint8_t inputPin);

        void begin(void);

        bool isPressed(void);
        bool isReleased(void);

    //private:
        // We don't really like to hide things inside "private", here's why:
        // 1. If you don't know what you're doing, stick to the items above,
        // and you'll be just fine whether or not we hide these items below.
        // 2. If you do know what you're doing, we're saving you a bit of time
        // having to change the library to make these items below accessible.
        Bricktronics *b;
        uint8_t inputPin;
};

#endif

