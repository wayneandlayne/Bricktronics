/*
    TODO description and copyright notes
*/

#include "Button.h"

Button::Button(uint8_t inputPin):
    _inputPin(inputPin),
    _pinMode(&::pinMode),
    _digitalWrite(&::digitalWrite),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

Button::Button(const SensorSettings &settings):
    _inputPin(settings.ANA),
    _pinMode(settings.pinMode),
    _digitalWrite(settings.digitalWrite),
    _digitalRead(settings.digitalRead)
{
    // Nothing to do here
}

void Button::begin(void)
{
    _pinMode(_inputPin, INPUT_PULLUP);
}

bool Button::isPressed()
{
    return (_digitalRead(_inputPin) == LOW);
}

bool Button::isReleased()
{
    return (_digitalRead(_inputPin) == HIGH);
}

