/*
Combined Bricktronics Library
For Bricktronics Shield, Bricktronics Megashield, and Bricktronics Motor Driver.
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

// We need the definitions of the motor and sensor setting structs
#include "Motor.h"
#include "Sensors.h"

#include <Wire.h> // TODO it seems like we have to include this in the top-level sketch too?! (v1.5 at least)
#include "utility/Adafruit_MCP23017.h"
#include "utility/digitalWriteFast.h" // TODO only used for one type of sensor? Can we remove from here?

// Defines for the Bricktronics Shield - Uses an IO expander chip, which corresponds to pins > 64
#define BS_TIP_0            11
#define BS_TIP_1            71

class Bricktronics
{
    public:
        // Empty constructor
        Bricktronics();

        static void begin(void);

        // Bricktronics Shield motor settings
        static const MotorSettingsAdvanced BS_MOTOR_1;
        static const MotorSettingsAdvanced BS_MOTOR_2;

        // Bricktronics Shield sensor settings
        static const SensorSettings BS_SENSOR_1;
        static const SensorSettings BS_SENSOR_2;
        static const SensorSettings BS_SENSOR_3;
        static const SensorSettings BS_SENSOR_4;

        // Bricktronics Megashield motor settings
        static const MotorSettingsAdvanced BMS_MOTOR_1;
        static const MotorSettingsAdvanced BMS_MOTOR_2;
        static const MotorSettingsAdvanced BMS_MOTOR_3;
        static const MotorSettingsAdvanced BMS_MOTOR_4;
        static const MotorSettingsAdvanced BMS_MOTOR_5;
        static const MotorSettingsAdvanced BMS_MOTOR_6;

        // Bricktronics Megashield sensor settings
        static const SensorSettings BMS_SENSOR_1;
        static const SensorSettings BMS_SENSOR_2;
        static const SensorSettings BMS_SENSOR_3;
        static const SensorSettings BMS_SENSOR_4;

        static void digitalWrite(uint8_t pin, uint8_t level);
        static uint8_t digitalRead(uint8_t pin);
        static void pinMode(uint8_t pin, uint8_t mode);
};

#endif // #ifndef BRICKTRONICS_H

