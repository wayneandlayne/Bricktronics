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

#include "Motor.h"

Motor::Motor(Bricktronics &brick, uint8_t port)
{
    // Keep a pointer to the Bricktronics object so we can use its functions
    b = &brick;
    switch (port)
    {
        case 1:
            enPin  = BS_MOTOR_1_EN;
            dirPin = BS_MOTOR_1_DIR;
            pwmPin = BS_MOTOR_1_PWM;
            encoder = new Encoder(BS_MOTOR_1_TACH_0, BS_MOTOR_1_TACH_1);
            break;
        case 2:
            enPin  = BS_MOTOR_2_EN;
            dirPin = BS_MOTOR_2_DIR;
            pwmPin = BS_MOTOR_2_PWM;
            encoder = new Encoder(BS_MOTOR_2_TACH_0, BS_MOTOR_2_TACH_1);
            break;
    }
}

Motor::Motor(BricktronicsMegashield &brick, uint8_t port)
{
    // This indicates that we are not using the Bricktronics object
    b = NULL;
    switch (port)
    {
        case 1:
            enPin  = BMS_MOTOR_1_EN;
            dirPin = BMS_MOTOR_1_DIR;
            pwmPin = BMS_MOTOR_1_PWM;
            encoder = new Encoder(BMS_MOTOR_1_TACH_0, BMS_MOTOR_1_TACH_1);
            break;
        case 2:
            enPin  = BMS_MOTOR_2_EN;
            dirPin = BMS_MOTOR_2_DIR;
            pwmPin = BMS_MOTOR_2_PWM;
            encoder = new Encoder(BMS_MOTOR_2_TACH_0, BMS_MOTOR_2_TACH_1);
            break;
        case 3:
            enPin  = BMS_MOTOR_3_EN;
            dirPin = BMS_MOTOR_3_DIR;
            pwmPin = BMS_MOTOR_3_PWM;
            encoder = new Encoder(BMS_MOTOR_3_TACH_0, BMS_MOTOR_3_TACH_1);
            break;
        case 4:
            enPin  = BMS_MOTOR_4_EN;
            dirPin = BMS_MOTOR_4_DIR;
            pwmPin = BMS_MOTOR_4_PWM;
            encoder = new Encoder(BMS_MOTOR_4_TACH_0, BMS_MOTOR_4_TACH_1);
            break;
        case 5:
            enPin  = BMS_MOTOR_5_EN;
            dirPin = BMS_MOTOR_5_DIR;
            pwmPin = BMS_MOTOR_5_PWM;
            encoder = new Encoder(BMS_MOTOR_5_TACH_0, BMS_MOTOR_5_TACH_1);
            break;
        case 6:
            enPin  = BMS_MOTOR_6_EN;
            dirPin = BMS_MOTOR_6_DIR;
            pwmPin = BMS_MOTOR_6_PWM;
            encoder = new Encoder(BMS_MOTOR_6_TACH_0, BMS_MOTOR_6_TACH_1);
            break;
    }
}

Motor::Motor(uint8_t enPin, uint8_t dirPin, uint8_t pwmPin, uint8_t tachPinA, uint8_t tachPinB)
{
    // This indicates that we are not using the Bricktronics object
    b = NULL;
    this->enPin = enPin;
    this->dirPin = dirPin;
    this->pwmPin = pwmPin;
    encoder = new Encoder(tachPinA, tachPinB);
}

// TODO We are using new to create the Encoder object
// Should we free this in the deconstructor? Or does that never really get called in Arduino?

void Motor::myPinMode(uint8_t pin, uint8_t mode)
{
    if (b)
    {
        b->pinMode(pin, mode);
    }
    else
    {
        pinMode(pin, mode);
    }
}

void Motor::myDigitalWrite(uint8_t pin, uint8_t value)
{
    if (b)
    {
        b->digitalWrite(pin, value);
    }
    else
    {
        digitalWrite(pin, value);
    }
}

void Motor::begin(void)
{
    enabled = true;
    stop();
    myPinMode(dirPin, OUTPUT);
    myPinMode(pwmPin, OUTPUT);
    myPinMode(enPin, OUTPUT);
}

void Motor::enable(void)
{
    begin();
}

void Motor::disable(void)
{
    enabled = false;
    myPinMode(dirPin, INPUT);
    myPinMode(pwmPin, INPUT);
    myPinMode(enPin, INPUT);
}

void Motor::stop(void)
{
    myDigitalWrite(enPin, LOW);
    myDigitalWrite(dirPin, LOW);
    myDigitalWrite(pwmPin, LOW);
}

void Motor::setSpeed(int16_t s)
{
    speed = s;
    if (s == 0)
    {
        stop();
    }
    else if (s < 0)
    {
        myDigitalWrite(dirPin, HIGH);
        analogWrite(pwmPin, 255 + s);
        myDigitalWrite(enPin, HIGH);
    }
    else
    {
        myDigitalWrite(dirPin, LOW);
        analogWrite(pwmPin, s);
        myDigitalWrite(enPin, HIGH);
    }
}

int16_t Motor::getSpeed(void)
{
    return speed;
}

int32_t Motor::getPos(void)
{
    return encoder->read();
}

void Motor::setPos(int32_t pos)
{
    encoder->write(pos);
}

