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

#include "Bricktronics.h"

Bricktronics::Bricktronics() 
{
}

void Bricktronics::begin()
{
    Wire.begin();
    mcp.begin();
}

void Bricktronics::pinMode(uint8_t pin, uint8_t mode)
{
    if (pin < 64)
    {
        ::pinMode(pin, mode);
    }
    else
    {
        mcp.pinMode(pin - 64, mode);
    }
}

void Bricktronics::digitalWrite(uint8_t pin, uint8_t level)
{
    if (pin < 64)
    {
        ::digitalWrite(pin, level);
    }
    else
    {
        mcp.digitalWrite(pin - 64, level);
    }
}

uint8_t Bricktronics::digitalRead(uint8_t pin)
{
    if (pin < 64)
    {
        return ::digitalRead(pin);
    }
    else
    {
        return mcp.digitalRead(pin - 64);
    }
}

