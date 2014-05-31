/*
    TODO description and copyright notes
*/

#include "Button.h"

Button::Button(const SensorSettings &settings):
    _inputPin(settings.ANA),
    _pinMode(pinMode),
    _pullUp(&::pullUp),
    _digitalRead(&::digitalRead)
{
    // Nothing to do here
}

Button::Button(const SensorSettingsAdvanced &settings):
    _inputPin(settings.ANA),
    _pinMode(settings.pinMode),
    _pullUp(settings.pullUp),
    _digitalRead(settings.digitalRead)
{
    // Nothing to do here
}

void Button::begin(void)
{
    _pinMode(_inputPin, INPUT);
    _pullUp(_inputPin, HIGH);
}

bool Button::isPressed()
{
    return (_digitalRead(_inputPin) == LOW);
}

bool Button::isReleased()
{
    return (_digitalRead(_inputPin) == HIGH);
}

