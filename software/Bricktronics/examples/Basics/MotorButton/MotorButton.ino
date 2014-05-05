#include <Wire.h>
#include <Bricktronics.h>
#include <Motor.h>

// Bricktronics Example: MotorButton
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Motor and a button.

// This example starts the motor at full speed, then waits for
// the button to be pressed and released, then reverses direction.
// It does this forever! (or until you turn off the power, unplug stuff,
// or reprogram the Arduino.)

// This example uses a motor, so it needs more power than a USB port can give.
// We really don't recommend running motors off of USB ports--they will be slow
// and sluggish, other things won't quite work right, things can get hot--
// it's just not a good idea.  Use an external power supply that provides
// between 7.2V and 9V, and can provide at least 600 mA per motor (1A preferably).
// Two options that work really well are a 9V wall adapter (2.1mm plug,
// center positive) or a 6xAA battery pack.

// Connect a button to Sensor Port 1 and the motor to Motor Port 1.
// Make sure the jumpers for sensor port 1 are setup to connect pins 2-3
// and pins 4-5.

Bricktronics brick = Bricktronics();
Motor m = Motor(brick, 1);
Button b = Button(brick, 1);

void setup()
{
    brick.begin();
    m.begin();
    b.begin();
}

int speed = 255;

void loop()
{
    m.setSpeed(speed);
    while (b.isReleased())
    {
        // do nothing
    }
    
    // To get here, the button was pushed!
    m.stop();
    // In order to debounce the button, we wait a little bit here
    delay(100);

    // We want to wait until the button is released to go backwards.
    while (b.isPressed())
    {
        // do nothing
    }
    // In order to debounce the button, we wait a little bit here
    delay(100);

    speed = speed * -1; // reverse directions
}

