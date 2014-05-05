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

/* New plan - May 3, 2014
   Combined library for Bricktronics Shield and Megashield.
   Motor now has three constructors:
   1. Motor(Bricktronics &b, uint8_t port)
   2. Motor(BricktronicsMegashield &b, uint8_t port)
   3. Motor(uint8_t dir_p, pwm_p, en_p, tach_a_p, tach_b_p)

   Also move the delayUpdate functions into here.

   // TODO add pid support from the arduino pid library
   // TODO do we need the delay_update functions anymore?
   // TODO speed control
   // TODO position control
*/


#ifndef MOTOR_H
#define MOTOR_H

#include "Bricktronics.h"
#include <inttypes.h>
#include "utility/Encoder.h"


//include <PID_v1.h>
//#define PID_MOTOR_KP 0.8
//#define PID_MOTOR_KD 1.0
//#define PID_MOTOR_KI 0.003

class Motor
{
    public:
        // This constructor is for when you are using the Bricktronics Shield
        Motor(Bricktronics &b,
              uint8_t port);

        // This constructor is for when you are using the Bricktronics Megashield
        Motor(BricktronicsMegashield &b,
              uint8_t port);

        // This constructor is for when you are using the Bricktronics Motor Driver
        // or connecting a motor chip directly to your Arduino.
        Motor(uint8_t enPin,
              uint8_t dirPin,
              uint8_t pwmPin,
              uint8_t tachPinA,
              uint8_t tachPinB);

        // Set the dir/pwm/en pins as outputs and stops the motor.
        void begin(void);
        // Another name for begin()
        void enable(void);
        // Set the dir/pwm/en pins as inputs
        void disable(void);
        // Set the dir/pwm/en pins to LOW, turning off the motor
        void stop(void);

        // Raw, uncontroleld speed settings
        // There is no control of the speed here,
        // just set a value between -255 and +255 (0 = stop).
        void setSpeed(int16_t s);
        // This only reads the current speed setpoint, not the actual speed.
        int16_t getSpeed(void);

        // Read the encoder's value
        int32_t getPos(void);
        // Write the encoder's value
        // This only sets the number corresponding to the motor's current position.
        // Usually just call this with zero as the argument to reset the positio.
        void setPos(int32_t p);

    //private: // We don't really like to hide things inside private
        uint8_t dirPin;
        uint8_t pwmPin;
        uint8_t enPin;
        uint8_t tachPinA;
        uint8_t tachPinB;

        bool enabled;
        uint16_t speed;

        // Tracks the position of the motor
        // Instantiated with new() inside a constructor.
        Encoder *encoder;


        // The Bricktronics Shield uses an IO expander to get more pins.
        // We have internal versions of pinMode() and digitalWrite() so that
        // we can determine whether to use the Bricktronics version or the
        // standard versions of these two functions.
        void myPinMode(uint8_t pin, uint8_t mode);
        void myDigitalWrite(uint8_t pin, uint8_t value);

        // This might be null if we aren't using a Bricktronics Shield
        Bricktronics *b;
};

#endif

