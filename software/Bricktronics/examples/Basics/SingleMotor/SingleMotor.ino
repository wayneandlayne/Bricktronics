#include <Wire.h>
#include <Bricktronics.h>

// Bricktronics Example: SingleMotor
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Motor.

// This example starts the motor at an intermediate speed,
// then speeds it up to full speed, and does the same in reverse.

// This example uses a motor, so it needs more power than a USB port can give.
// We really don't recommend running motors off of USB ports (they will be
// slow and sluggish, other things won't quite work right, things can get hot)
// it's just not a good idea.  Use an external power supply that provides
// between 7.2 and 9 volts DC, and can provide at least 600 mA per motor
// (1A preferably). Two options that work really well are a 9V wall adapter
// or a 6xAA battery pack (2.1mm plug, center positive).

// Connections:
//      Connect a motor to Motor Port 1.

Motor m = Motor(Bricktronics::BS_MOTOR_1);

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world.");
    Bricktronics::begin();
    m.begin();
}

void loop() 
{
    while (true)
    {
        Serial.println("Going forward.");
        m._rawSetSpeed(75);
        delay(1000);
        m._rawSetSpeed(255);
        delay(500);
        Serial.println("Going in reverse.");
        m._rawSetSpeed(-75);
        delay(1000);
        m._rawSetSpeed(-255);
        delay(500);
        
    }
}
