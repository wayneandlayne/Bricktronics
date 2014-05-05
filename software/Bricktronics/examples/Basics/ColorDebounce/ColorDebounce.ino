#include <Wire.h>
#include <Bricktronics.h>
#include <ColorSensor.h>

// Bricktronics Example: ColorDebounce
// http://www.wayneandlayne.com/bricktronics
// This example uses a color sensor.

// Color sensor readings are taken repeatedly, and when they change,
// they're debounced so the colors don't switch back and forth on a transition.
// This isn't the necessarily the simplest or best way to debounce,
// but it works for what we're using it for!

// Connect a Color Sensor into Sensor Port 3.
// Make sure to adjust the jumpers on Sensor Port 3 so that only
// pins 3 and 4 are connected.
// There's an image of this on the Bricktronics website.

Bricktronics brick = Bricktronics();
ColorSensor color = ColorSensor(brick, 3);

void setup()
{
    Serial.begin(115200);
    brick.begin();
    color.begin();
}

char lastColor = color.getColor();

void loop()
{
    char stable = 0;
    char newReading = color.getColor();
    if (newReading != lastColor)
    {
        while (stable < 10)
        {
            delay(10);
            if (color.getColor() == newReading)
            {
                stable += 1;
            } else
            {
                newReading = color.getColor();
                stable = 0;
            }
        }

        if (newReading != lastColor)
        {
            color.printColor(newReading);
            Serial.println();
            lastColor = newReading;
        }
    }
    delay(10);
}

