/*
   Motor Driver for Arduino
   This is the version for use with the Bricktronics Motor Driver board.
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

#include "Motor.h"

// Newest plan: Shared Bricktronics library for all platforms (Arduino, ChipKit, Teensy, RasPi, etc).
// Use functions pointers for the three low-level functions (pinMode, digitalWrite, analogWrite).
// Common encoder library for all supported platforms?

Motor::Motor(const MotorSettings &settings):
             _dirPin(settings.dirPin),
             _pwmPin(settings.pwmPin),
             _enPin(settings.enPin),
             _enabled(false),
             _rawSpeed(0),
             _pid(&_pidInput, &_pidOutput, &_pidSetpoint, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD, DIRECT),
             _pidMode(MOTOR_PID_MODE_DISABLED),
             _encoder(settings.tachPinA, settings.tachPinB),
             _pinMode(settings.pinMode),
             _digitalWrite(settings.digitalWrite),
             _analogWrite(settings.analogWrite)
{
    _pid.SetSampleTime(MOTOR_PID_SAMPLE_TIME_MS);
    _pid.SetOutputLimits(-255, +255);
}


int32_t Motor::getPosition(void)
{
   return _encoder.read();
}

void Motor::setPosition(int32_t pos)
{
   _encoder.write(pos);
}



void Motor::begin(void)
{
    _enabled = true;
    stop();
    pinMode(_dirPin, OUTPUT);
    pinMode(_pwmPin, OUTPUT);
    pinMode(_enPin, OUTPUT);
}

void Motor::enable(void)
{
    begin();
}

void Motor::disable(void)
{
    _enabled = false;
    pinMode(_dirPin, INPUT);
    pinMode(_pwmPin, INPUT);
    pinMode(_enPin, INPUT);
}

void Motor::stop(void)
{
    digitalWrite(_enPin, LOW);
    digitalWrite(_dirPin, LOW);
    digitalWrite(_pwmPin, LOW);
}


// RAW UNCONTROLLED SPEED FUNCTION
void Motor::_rawSetSpeed(int16_t s)
{
    _rawSpeed = s;
    if (s == 0)
    {
        stop();
    }
    else if (s < 0)
    {
        digitalWrite(_dirPin, HIGH);
        analogWrite(_pwmPin, 255 + s);
        digitalWrite(_enPin, HIGH);
    }
    else
    {
        digitalWrite(_dirPin, LOW);
        analogWrite(_pwmPin, s);
        digitalWrite(_enPin, HIGH);
    }
}




void Motor::update(void)
{
   switch (_pidMode)
   {
      case MOTOR_PID_MODE_POSITION:
         _pidInput = _encoder.read();
         _pid.Compute();
         _rawSetSpeed(_pidOutput);
         break;

      case MOTOR_PID_MODE_SPEED:
         // TODO how can we determine the current speed if this is being called frequently

         break;

      default: // includes MOTOR_PID_MODE_DISABLED
         break;
   }
}

void Motor::setUpdateFrequencyMS(int timeMS)
{
   _pid.SetSampleTime(timeMS);
}



