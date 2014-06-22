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

/*
   // TODO Test PID position control
   // TODO Do we need the delay_update functions anymore?
   // TODO isAtPosition(epsilon) check?
   // TODO Speed control
   // TODO Position control
   // TODO Angle control end-user API wrapper around position control
*/


#ifndef MOTOR_H
#define MOTOR_H

// Arduino header files
#include <inttypes.h>

// Library header files
#include "Settings.h"
#include "utility/Encoder.h"
// TODO should we remove the copy of PID from utilities and make the user install it themselves?
#include "utility/PID_v1.h"

#define MOTOR_PID_KP                2.5
#define MOTOR_PID_KI                0.1
#define MOTOR_PID_KD                0.1
// We can have different PID modes, for now speed and position
#define MOTOR_PID_MODE_DISABLED     0
#define MOTOR_PID_MODE_POSITION     1
#define MOTOR_PID_MODE_SPEED        2
// Sample time - Call update() as often as you can, but it will only update
// as often as this value. Can be updated by the user at runtime if desired.
#define MOTOR_PID_SAMPLE_TIME_MS    50

class Motor
{
    public:
        // Constructor - Simple constructor accepts the five motor pins
        Motor(uint8_t enPin,
              uint8_t dirPin,
              uint8_t pwmPin,
              uint8_t tachPinA,
              uint8_t tachPinB);

        // Constructor - Advanced constructor accepts a MotorSettings struct
        // to also override the low-level Arduino functions.
        Motor(const MotorSettings &settings);

        // TODO Reconsider these functions, if they are a good idea, or if they are even needed...
        // Set the dir/pwm/en pins as outputs and stops the motor.
        void begin(void);
        // Another name for begin()
        void enable(void);
        // Set the dir/pwm/en pins as inputs
        void disable(void);
        // Set the dir/pwm/en pins to LOW, turning off the motor
        void stop(void);
        // TODO what about braking and coasting? Replace stop() with brake(strength?) and coast()


        // Read the encoder's current position.
        int32_t getPosition(void);
        // Write the encoder's current position - This will mess up any control in progress!
        //     This only sets the number corresponding to the motor's current position.
        //     Usually you just want to reset the position to zero.
        void setPosition(int32_t pos);


        // Some of the functions below need to periodically check on the motor's operation
        // and update data. Use this update() call to do that.
        // Call this function as often as you can, since it will only actually update as
        // often as the frequency setpoint (defaults to 50ms), which can be updated below.
        void update(void);
        // Update the maximum frequency at which the PID algorithm will actually update.
        // Periodically call update for the specified duration
        void delayUpdateMS(int delayMS);


        // PID related functions
        void pidSetUpdateFrequencyMS(int timeMS);
        // Print out the PID values
        void pidPrintValues(void);
        // Functions for getting and setting the PID tuning parameters
        double pidGetKp(void);
        double pidGetKi(void);
        double pidGetKd(void);
        // Use the combined pidSetTunings if you can, it's faster.
        void pidSetTunings(double Kp, double Ki, double Kd);
        void pidSetKp(double Kp);
        void pidSetKi(double Ki);
        void pidSetKd(double Kd);


        // Raw, uncontroleld speed settings
        // There is no control of the speed here,
        // just set a value between -255 and +255 (0 = stop).
        void rawSetSpeed(int16_t s);


        // Position control functions
        void goToPosition(int32_t position);
        //void gotoPositionWait(int16_t position);
        //void gotoPositionWaitTimeout(int16_t position, uint16_t timeoutMS);

        // Angle control functions - 0 - 359, handles discontinuity nicely.
        // TODO this means we can't do sub-degree positioning, which only
        // really becomes noticable if we scale-up the output by a lot.
        // Can specify any angle, positive or negative. If you say 
        // "go to angle 721" it will be the same as "go to angle 1".
        // Similarly, "go to angle -60" will be "go to angle 300".
        // If you want "go 45 degrees clockwise from here", try using
        // m.goToAngle(m.getAngle() + 45);
        void goToAngle(int32_t angle);
        //void gotoAngleWait(int16_t angle);
        //void gotoAngleWaitTimeout(int16_t angle, uint16_t timeoutMS);
        uint16_t getAngle(void); // Returns the current angle (0-359)
        void setAngle(int32_t angle); // Updates the encoder position to be the specified angle

        // For the angle control, the user can specify a different multiplier
        // between motor encoder ticks and "output rotations", defaults to 1.
        // Use this setting if your motor is connected to a gear train that
        // makes a different number of motor rotations per output rotation.
        // For example, if you have a 5:1 gear train between your motor and
        // the final output, then you can specify this value as 5.
        // Negative numbers? Should work just fine.
        void setAngleOutputMultiplier(int8_t multiplier);

        // Speed control functions
        // The driver will monitor and try to adjust the drive duty cycle
        // to keep the actual speed close to the setpoint.
        // TODO
        


    //private:
        // We really don't like to hide things inside private,
        // but if we did, these would be the private items.
        uint8_t _enPin;
        uint8_t _dirPin;
        uint8_t _pwmPin;

        bool _enabled;
        uint16_t _rawSpeed;

        // PID variables
        PID _pid;
        uint8_t _pidMode;
        double _pidSetpoint, _pidInput, _pidOutput;

        // Tracks the position of the motor
        Encoder _encoder;

        // Check out the comments above for setAngleOutputMultiplier()
        int8_t _angleMultiplier;

        // For the Bricktronics Shield, which has an I2C I/O expander chip, we need a way to
        // override some common Arduino functions. We use function pointers here to handle this.
        // For the non-Bricktronics Shield cases, the simple constructor above provides the built-in functions.
        void (*_pinMode)(uint8_t, uint8_t);
        void (*_digitalWrite)(uint8_t, uint8_t);
        int (*_digitalRead)(uint8_t);
};

#endif

