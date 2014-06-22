/*
    Bricktronics library for LEGO NXT motors.
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

#include "Motor.h"

// This is the simplified constructor that allows you to specify only the
// five motor pins.
Motor::Motor(uint8_t enPin, uint8_t dirPin, uint8_t pwmPin, uint8_t tachPinA, uint8_t tachPinB):
             _enPin(enPin),
             _dirPin(dirPin),
             _pwmPin(pwmPin),
             _enabled(false),
             _rawSpeed(0),
             _pid(&_pidInput, &_pidOutput, &_pidSetpoint, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD, REVERSE),
             _pidMode(MOTOR_PID_MODE_DISABLED),
             _encoder(tachPinA, tachPinB),
             _angleMultiplier(2),
             _pinMode(&::pinMode),
             _digitalWrite(&::digitalWrite),
             _digitalRead(&::digitalRead)
{
    _pid.SetSampleTime(MOTOR_PID_SAMPLE_TIME_MS);
    _pid.SetOutputLimits(-255, +255);
}

// This is the complicated constructor that allows for overriding the
// low-level Arduino functions.
Motor::Motor(const MotorSettings &settings):
             _enPin(settings.enPin),
             _dirPin(settings.dirPin),
             _pwmPin(settings.pwmPin),
             _enabled(false),
             _rawSpeed(0),
             _pid(&_pidInput, &_pidOutput, &_pidSetpoint, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD, REVERSE),
             _pidMode(MOTOR_PID_MODE_DISABLED),
             _encoder(settings.tachPinA, settings.tachPinB),
             _angleMultiplier(2),
             _pinMode(settings.pinMode),
             _digitalWrite(settings.digitalWrite),
             _digitalRead(settings.digitalRead)
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
    _pid.SetMode(AUTOMATIC);
    _enabled = true;
    stop();
    _pinMode(_dirPin, OUTPUT);
    _pinMode(_pwmPin, OUTPUT);
    _pinMode(_enPin, OUTPUT);
}

void Motor::enable(void)
{
    begin();
}

void Motor::disable(void)
{
    _enabled = false;
    _pinMode(_dirPin, INPUT);
    _pinMode(_pwmPin, INPUT);
    _pinMode(_enPin, INPUT);
}

void Motor::stop(void)
{
  _digitalWrite(_enPin, LOW);
  _digitalWrite(_dirPin, LOW);
  _digitalWrite(_pwmPin, LOW);
}


// RAW UNCONTROLLED SPEED FUNCTION
void Motor::rawSetSpeed(int16_t s)
{
  _rawSpeed = s;
  if (s == 0)
  {
    stop();
  }
  else if (s < 0)
  {
    _digitalWrite(_dirPin, HIGH);
    analogWrite(_pwmPin, 255 + s);
    _digitalWrite(_enPin, HIGH);
  }
  else
  {
    _digitalWrite(_dirPin, LOW);
    analogWrite(_pwmPin, s);
    _digitalWrite(_enPin, HIGH);
  }
}

int16_t Motor::rawGetSpeed(void)
{
    return _rawSpeed;
}


void Motor::goToPosition(int32_t position)
{
  // Swith our internal PID into position mode
  _pidMode = MOTOR_PID_MODE_POSITION;
  _pidSetpoint = position;
}


void Motor::setAngleOutputMultiplier(int8_t multiplier)
{
  // Since the LEGO NXT motor encoders have 720 ticks per revolution,
  // we have to double the user's specified multiplier.
  _angleMultiplier = multiplier << 1;
}

void Motor::goToAngle(int32_t angle)
{
  int16_t delta = (angle % 360) - getAngle();

  while (delta > 180)
    delta -= 360;
  while (delta < -180)
    delta += 360;

  // Now, delta is between -180 and +180

  goToPosition(getPosition() + (delta * _angleMultiplier));
}

uint16_t Motor::getAngle(void)
{
  return ( (getPosition() / _angleMultiplier) % 360 );
}

void Motor::setAngle(int32_t angle)
{
  setPosition((angle % 360) * _angleMultiplier);
}



void Motor::update(void)
{
  switch (_pidMode)
  {
    case MOTOR_PID_MODE_POSITION:
      _pidInput = _encoder.read();
      _pid.Compute();
      rawSetSpeed(_pidOutput);
      break;

    case MOTOR_PID_MODE_SPEED:
      // TODO how can we determine the current speed if this is being called frequently
      break;

    default: // includes MOTOR_PID_MODE_DISABLED
      break;
  }
}

void Motor::delayUpdateMS(int delayMS)
{
  unsigned long endTime = millis() + delayMS;
  while (millis() < endTime)
  {
    update();
    // We could put a delay(5) here, but the PID library already has a 
    // "sample time" parameter to only run so frequent, you know?
  }
}

void Motor::pidSetUpdateFrequencyMS(int timeMS)
{
  _pid.SetSampleTime(timeMS);
}

void Motor::pidPrintValues(void)
{
  Serial.print("SET:");
  Serial.println(_pidSetpoint);
  Serial.print("INP:");
  Serial.println(_pidInput);
  Serial.print("OUT:");
  Serial.println(_pidOutput);
}

double Motor::pidGetKp(void)
{
    return _pid.GetKp();
}

double Motor::pidGetKi(void)
{
    return _pid.GetKi();
}

double Motor::pidGetKd(void)
{
    return _pid.GetKd();
}

void Motor::pidSetKp(double Kp)
{
    _pid.SetTunings(Kp, _pid.GetKi(), _pid.GetKd());
}

void Motor::pidSetKi(double Ki)
{
    _pid.SetTunings(_pid.GetKp(), Ki, _pid.GetKd());
}

void Motor::pidSetKd(double Kd)
{
    _pid.SetTunings(_pid.GetKp(), _pid.GetKi(), Kd);
}

void Motor::pidSetTunings(double Kp, double Ki, double Kd)
{
    _pid.SetTunings(Kp, Ki, Kd);
}

