#include "Bricktronics.h"

// Bricktronics Shield motor settings
const MotorSettings Bricktronics::BS_MOTOR_1 = {
    79, // enPin
    78, // dirPin
    10, // pwmPin
    2,  // tachPinA
    5,  // tachPinB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
    &Bricktronics::digitalRead,
};

const MotorSettings Bricktronics::BS_MOTOR_2 = {
    77, // enPin
    76, // dirPin
    9,  // pwmPin
    3,  // tachPinA
    4,  // tachPinB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
    &Bricktronics::digitalRead,
};


// Bricktronics Shield sensor settings
const SensorSettings Bricktronics::BS_SENSOR_1 = {
    14, // ANA
    70, // DA
    69, // DB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
    &Bricktronics::digitalRead,
};

const SensorSettings Bricktronics::BS_SENSOR_2 = {
    15, // ANA
    68, // DA
    67, // DB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
    &Bricktronics::digitalRead,
};

const SensorSettings Bricktronics::BS_SENSOR_3 = {
    16, // ANA
    8,  // DA
    12, // DB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
    &Bricktronics::digitalRead,
};

const SensorSettings Bricktronics::BS_SENSOR_4 = {
    17, // ANA
    7,  // DA
    6,  // DB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
    &Bricktronics::digitalRead,
};


// Bricktronics Megashield motor settings
const MotorSettings Bricktronics::BMS_MOTOR_1 = {
    55, // enPin
    54, // dirPin
    11, // pwmPin
    3,  // tachPinA
    12, // tachPinB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const MotorSettings Bricktronics::BMS_MOTOR_2 = {
    56, // enPin
    57, // dirPin
    9,  // pwmPin
    2,  // tachPinA
    10, // tachPinB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const MotorSettings Bricktronics::BMS_MOTOR_3 = {
    59, // enPin
    58, // dirPin
    8,  // pwmPin
    18, // tachPinA
    7,  // tachPinB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const MotorSettings Bricktronics::BMS_MOTOR_4 = {
    61, // enPin
    60, // dirPin
    6,  // pwmPin
    19, // tachPinA
    14, // tachPinB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const MotorSettings Bricktronics::BMS_MOTOR_5 = {
    32, // enPin
    62, // dirPin
    5,  // pwmPin
    20, // tachPinA
    15, // tachPinB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const MotorSettings Bricktronics::BMS_MOTOR_6 = {
    65, // enPin
    64, // dirPin
    4,  // pwmPin
    21, // tachPinA
    16, // tachPinB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};


// Bricktronics Megashield sensor settings
const SensorSettings Bricktronics::BMS_SENSOR_1 = {
    66, // ANA
    25, // DA
    29, // DB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const SensorSettings Bricktronics::BMS_SENSOR_2 = {
    67, // ANA
    37, // DA
    39, // DB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const SensorSettings Bricktronics::BMS_SENSOR_3 = {
    68, // ANA
    47, // DA
    49, // DB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};

const SensorSettings Bricktronics::BMS_SENSOR_4 = {
    69, // ANA
    51, // DA
    53, // DB
    &::pinMode,
    &::digitalWrite,
    &::digitalRead,
};


// This library instance does not belong to any instance of Bricktronics,
// and is only used by the three Brucktronics member functions below,
// plus the begin() function.
Adafruit_MCP23017 Bricktronics::mcp;


void Bricktronics::begin()
{
    Wire.begin();
    mcp.begin();
}

void Bricktronics::pinMode(uint8_t pin, uint8_t mode)
{
    // There is a bit of a tricky spot here.
    // In normal Arduino world, pinMode accepts these modes:
    //     INPUT, OUTPUT, INPUT_PULLUP
    // In MCP I/O expander world, pinMode only handles I vs O,
    //     and we have to separately call mcp.pullUp to turn on
    //     pullup resistors.
    if (pin < 64)
    {
        ::pinMode(pin, mode);
    }
    else
    {
        // Strip away the upper bits - Same as subtracting 64.
        uint8_t newPin = pin & BRICKTRONICS_PIN_MASK;
        if (mode == OUTPUT)
        {
            mcp.pinMode(newPin, OUTPUT);
            mcp.pullUp(newPin, LOW);
        }
        else
        {
            // either INPUT or INPUT_PULLUP
            mcp.pinMode(newPin, INPUT);
            if (mode == INPUT_PULLUP)
            {
                mcp.pullUp(newPin, HIGH);
            }
            else
            {
                mcp.pullUp(newPin, LOW);
            }
        }
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
        mcp.digitalWrite(pin & BRICKTRONICS_PIN_MASK, level);
    }
}

int Bricktronics::digitalRead(uint8_t pin)
{
    if (pin < 64)
    {
        return ::digitalRead(pin);
    }
    else
    {
        return mcp.digitalRead(pin & BRICKTRONICS_PIN_MASK);
    }
}

