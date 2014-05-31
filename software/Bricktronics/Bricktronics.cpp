#include "Bricktronics.h"

// Bricktronics Shield motor settings
const MotorSettingsAdvanced Bricktronics::BS_MOTOR_1 = {
    79, // enPin
    10, // dirPin
    78, // pwmPin
    2,  // tachPinA
    5,  // tachPinB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
};

const MotorSettingsAdvanced Bricktronics::BS_MOTOR_2 = {
    77, // enPin
    9,  // dirPin
    76, // pwmPin
    3,  // tachPinA
    4,  // tachPinB
    &Bricktronics::pinMode,
    &Bricktronics::digitalWrite,
};


// Bricktronics Shield sensor settings
const SensorSettingsAdvanced Bricktronics::BS_SENSOR_1 = {
    14, // ANA
    70, // DA
    69, // DB
    &Bricktronics::pinMode,
    &Bricktronics::pullUp,
    &Bricktronics::digitalRead,
};

const SensorSettingsAdvanced Bricktronics::BS_SENSOR_2 = {
    15, // ANA
    68, // DA
    67, // DB
    &Bricktronics::pinMode,
    &Bricktronics::pullUp,
    &Bricktronics::digitalRead,
};

const SensorSettingsAdvanced Bricktronics::BS_SENSOR_3 = {
    16, // ANA
    8,  // DA
    12, // DB
    &Bricktronics::pinMode,
    &Bricktronics::pullUp,
    &Bricktronics::digitalRead,
};

const SensorSettingsAdvanced Bricktronics::BS_SENSOR_4 = {
    17, // ANA
    7,  // DA
    6,  // DB
    &Bricktronics::pinMode,
    &Bricktronics::pullUp,
    &Bricktronics::digitalRead,
};


// Bricktronics Megashield motor settings
const MotorSettings Bricktronics::BMS_MOTOR_1 = {
    55, // enPin
    54, // dirPin
    11, // pwmPin
    3,  // tachPinA
    12, // tachPinB
};

const MotorSettings Bricktronics::BMS_MOTOR_2 = {
    56, // enPin
    57, // dirPin
    9,  // pwmPin
    2,  // tachPinA
    10, // tachPinB
};

const MotorSettings Bricktronics::BMS_MOTOR_3 = {
    59, // enPin
    58, // dirPin
    8,  // pwmPin
    18, // tachPinA
    7,  // tachPinB
};

const MotorSettings Bricktronics::BMS_MOTOR_4 = {
    61, // enPin
    60, // dirPin
    6,  // pwmPin
    19, // tachPinA
    14, // tachPinB
};

const MotorSettings Bricktronics::BMS_MOTOR_5 = {
    32, // enPin
    62, // dirPin
    5,  // pwmPin
    20, // tachPinA
    15, // tachPinB
};

const MotorSettings Bricktronics::BMS_MOTOR_6 = {
    65, // enPin
    64, // dirPin
    4,  // pwmPin
    21, // tachPinA
    16, // tachPinB
};


// Bricktronics Megashield sensor settings
const SensorSettings Bricktronics::BMS_SENSOR_1 = {
    66, // ANA
    25, // DA
    29, // DB
};

const SensorSettings Bricktronics::BMS_SENSOR_2 = {
    67, // ANA
    37, // DA
    39, // DB
};

const SensorSettings Bricktronics::BMS_SENSOR_3 = {
    68, // ANA
    47, // DA
    49, // DB
};

const SensorSettings Bricktronics::BMS_SENSOR_4 = {
    69, // ANA
    51, // DA
    53, // DB
};


// This library instance does not belong to any instance of Bricktronics,
// and is only used by the three Brucktronics member functions below,
// plus the begin() function.
Adafruit_MCP23017 mcp;
// TODO should this belong inside the Bricktronics class as a static member variable?



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
        mcp.pinMode(pin & BRICKTRONICS_PIN_MASK, mode);
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

void Bricktronics::pullUp(uint8_t pin, uint8_t level)
{
    if (pin < 64)
    {
        // TODO while this should technically work,
        // maybe we should instead use pinMode(pin, INPUT_PULLUP)?
        ::digitalWrite(pin, level);
    }
    else
    {
        mcp.pullUp(pin & BRICKTRONICS_PIN_MASK, level);
    }
}

