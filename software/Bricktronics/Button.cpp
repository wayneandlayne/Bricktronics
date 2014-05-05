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

#include "Button.h"

Button::Button(Bricktronics &brick, uint8_t port)
{
    // Keep a pointer to the Bricktronics object so we can use its functions
    b = &brick;
    switch (port)
    {
        case 1:
            inputPin = BS_SENSOR_1_ANA;
            break;
        case 2:
            inputPin = BS_SENSOR_2_ANA;
            break;
        case 3:
            inputPin = BS_SENSOR_3_ANA;
            break;
        case 4:
            inputPin = BS_SENSOR_4_ANA;
            break;
    }
}

Button::Button(BricktronicsMegashield &brick, uint8_t port)
{
    // This indicates that we are not using the Bricktronics object
    b = NULL;
    switch (port)
    {
        case 1:
            inputPin = BMS_SENSOR_1_ANA;
            break;
        case 2:
            inputPin = BMS_SENSOR_2_ANA;
            break;
        case 3:
            inputPin = BMS_SENSOR_3_ANA;
            break;
        case 4:
            inputPin = BMS_SENSOR_4_ANA;
            break;
    }
}

Button::Button(uint8_t inputPin)
{
    // This indicates that we are not using the Bricktronics object
    b = NULL;
    this->inputPin = inputPin;
}

void Button::begin(void)
{
    Serial.println(inputPin);
    if (b)
    {
        b->pinMode(inputPin, INPUT_PULLUP);
    }
    else
    {
        pinMode(inputPin, INPUT_PULLUP);
    }
}

bool Button::isPressed(void)
{
    if (b)
    {
        return (b->digitalRead(inputPin) == LOW);
    }
    else
    {
        return (digitalRead(inputPin) == LOW);
    }
}

bool Button::isReleased(void)
{
    return !isPressed();
}

