Bricktronics
============

These are the files for the Bricktronics project, organized into three categories:

1. Hardware - Design files for the circuit boards we made
2. Software - Code to help run the motors and sensors from your Arduino
3. Models - The files for various LEGO models


_Wayne and Layne, LLC and our products are not connected to or endorsed by the LEGO Group. LEGO, Mindstorms, and NXT are trademarks of the LEGO Group._

#include <Motor.h>

// TODO how do we get rid of needing to include Wire.h here?
#include <Wire.h>
#include <Bricktronics.h>

// This struct takes: enPin, dirPin, pwmPin, tachPinA, tachPinB
MotorSettings mySettings = {79, 10, 78, 2, 5};

// These are two different but similar constructors,
// due to the BS_MOTOR_1 being of a different struct type (MotorSettingsAdvanced),
// but you can't tell that here, it's magic!
Motor m1(mySettings);
Motor m2(Bricktronics::BS_MOTOR_1);

void setup() {
  // put your setup code here, to run once:
  m1.begin();
  m2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
