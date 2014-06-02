#include <Wire.h>
#include <Bricktronics.h>

// Bricktronics Example: MotorButton
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Motor and a button.

// This example starts the motor at full speed, then waits for the button to be pressed and released, then reverses direction.  It does this forever! (or until you turn off the power, unplug stuff, or reprogram the Arduino.)

// This example uses a motor, so it needs more power than a USB port can give.  We really don't recommend running motors off of USB ports--they will be slow and sluggish, other things won't quite work right, things can get hot--it's just not a good idea.  Use an external power supply that provides between 9V and 7.2V, and can provide at least 600 mA per motor (1A preferably).  Two options that work really well are a 9V wall adapter (2.1mm plug, center positive) or a 6xAA battery pack.

// Connections:
//  Pushbutton sensor in Sensor Port 1
//  Motor in Motor Port 1

Motor m = Motor(Bricktronics::BS_MOTOR_1);
Button b = Button(Bricktronics::BS_SENSOR_1);

void setup()
{
    Bricktronics::begin();
    m.begin();
    b.begin();
}

int speed = 255;

void loop()
{
    m._rawSetSpeed(speed);
    while (b.isReleased()) {};
    // to get here, the button was pushed!
    m.stop();
    // in order to debounce the button, we wait a little bit here
    delay(100);
    // we want to wait until the button is released to go backwards.
    while (b.isPressed()) {};
    delay(100);
    speed *= -1;  //reverse directions
}
