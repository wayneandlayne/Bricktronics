#include <Wire.h>
#include <Bricktronics.h>
#include <ColorSensor.h>

// Bricktronics Example: ColorButton
// http://www.wayneandlayne.com/bricktronics
// This example uses a button and a color sensor.
// When the button is pressed, a single reading from the color sensor
// is taken and converted into a color name and printed over the Serial console.

// Connect a button to Sensor Port 1, and a Color Sensor into Sensor Port 3.
// Make sure to adjust the jumpers on Sensor Port 3 so that only pins 3 and 4
// are connected.  There's an image of this on the Bricktronics website.

Bricktronics    brick   = Bricktronics();
Button          button  = Button(brick, 1);
ColorSensor     color   = ColorSensor(brick, 3);

void setup() 
{
    Serial.begin(115200);
    brick.begin();
    button.begin();
    color.begin();
}

void loop()
{
    while (button.isReleased())
    {
        // do nothing
    }
    color.printColor(color.getColor());
    Serial.println();
    delay(100); // simple debounce
    while (button.isPressed())
    {
        // do nothing
    }
    delay(100); // simple debounce
}

