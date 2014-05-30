/*
Combined Bricktronics Library
For Bricktronics Shield and Bricktronics Megashield
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

#ifndef BRICKTRONICS_H
#define BRICKTRONICS_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "utility/Adafruit_MCP23017.h"
#include "utility/digitalWriteFast.h"

// Defines for the Bricktronics Shield - Uses an IO expander chip, which corresponds to pins > 64
MotorSettings BS_MOTOR_1 = {
  79, // enPin
  10, // dirPin
  78, // pwmPin
  2,  // tachPinA
  5,  // tachPinB
  &pinMode,
  &digitalWrite,
  &analogWrite,
};

#define BS_MOTOR_2_EN       77
#define BS_MOTOR_2_PWM      9
#define BS_MOTOR_2_DIR      76
#define BS_MOTOR_2_TACH_0   3
#define BS_MOTOR_2_TACH_1   4

// A0 = 14
#define BS_SENSOR_1_ANA     14
#define BS_SENSOR_1_DA      70
#define BS_SENSOR_1_DB      69

// A1 = 15
#define BS_SENSOR_2_ANA     15
#define BS_SENSOR_2_DA      68
#define BS_SENSOR_2_DB      67

// A2 = 16
#define BS_SENSOR_3_ANA     16
#define BS_SENSOR_3_DA      8
#define BS_SENSOR_3_DB      12

// A3 = 17
#define BS_SENSOR_4_ANA     17
#define BS_SENSOR_4_DA      7
#define BS_SENSOR_4_DB      6

#define BS_TIP_0            11
#define BS_TIP_1            71

class Bricktronics
{
    public:
        Bricktronics();
        void begin(void);
    //private:
        // We don't really like to hide things inside "private", here's why:
        // 1. If you don't know what you're doing, stick to the items above,
        // and you'll be just fine whether or not we hide these items below.
        // 2. If you do know what you're doing, we're saving you a bit of time
        // having to change the library to make these items below accessible.
        Adafruit_MCP23017 mcp;
        void digitalWrite(uint8_t pin, uint8_t level);
        uint8_t digitalRead(uint8_t pin);
        void pinMode(uint8_t pin, uint8_t mode);
};

// Defines for the Bricktronics Megashield - Does not use an IO expander chip
#define BMS_MOTOR_1_EN      55
#define BMS_MOTOR_1_DIR     54
#define BMS_MOTOR_1_PWM     11
#define BMS_MOTOR_1_TACH_0  3
#define BMS_MOTOR_1_TACH_1  12

#define BMS_MOTOR_2_EN      56
#define BMS_MOTOR_2_DIR     57
#define BMS_MOTOR_2_PWM     9
#define BMS_MOTOR_2_TACH_0  2
#define BMS_MOTOR_2_TACH_1  10

#define BMS_MOTOR_3_EN      59
#define BMS_MOTOR_3_DIR     58
#define BMS_MOTOR_3_PWM     8
#define BMS_MOTOR_3_TACH_0  18
#define BMS_MOTOR_3_TACH_1  7

#define BMS_MOTOR_4_EN      61
#define BMS_MOTOR_4_DIR     60
#define BMS_MOTOR_4_PWM     6
#define BMS_MOTOR_4_TACH_0  19
#define BMS_MOTOR_4_TACH_1  14

#define BMS_MOTOR_5_EN      63
#define BMS_MOTOR_5_DIR     62
#define BMS_MOTOR_5_PWM     5
#define BMS_MOTOR_5_TACH_0  20
#define BMS_MOTOR_5_TACH_1  15

#define BMS_MOTOR_6_EN      65
#define BMS_MOTOR_6_DIR     64
#define BMS_MOTOR_6_PWM     4
#define BMS_MOTOR_6_TACH_0  21
#define BMS_MOTOR_6_TACH_1  16

// A12 = 66
#define BMS_SENSOR_1_ANA    66
#define BMS_SENSOR_1_DA     25
#define BMS_SENSOR_1_DB     29

// A13 = 67
#define BMS_SENSOR_2_ANA    67
#define BMS_SENSOR_2_DA     37
#define BMS_SENSOR_2_DB     39

// A14 = 68
#define BMS_SENSOR_3_ANA    68
#define BMS_SENSOR_3_DA     47
#define BMS_SENSOR_3_DB     49

// A15 = 69
#define BMS_SENSOR_4_ANA    69
#define BMS_SENSOR_4_DA     51
#define BMS_SENSOR_4_DB     53

class BricktronicsMegashield
{
    public:
        BricktronicsMegashield();
        void begin(void);
    //private:
        // We don't really like to hide things inside "private", here's why:
        // 1. If you don't know what you're doing, stick to the items above,
        // and you'll be just fine whether or not we hide these items below.
        // 2. If you do know what you're doing, we're saving you a bit of time
        // having to change the library to make these items below accessible.
};

#endif // #ifndef BRICKTRONICS_H

