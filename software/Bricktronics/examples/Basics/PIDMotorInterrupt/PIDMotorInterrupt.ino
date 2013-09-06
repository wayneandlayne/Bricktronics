#include <Wire.h>
#include <Bricktronics.h>

//Bricktronics Example: PIDMotorInterrupt
//http://www.wayneandlayne.com/bricktronics/
//This example uses a LEGO NXT Motor with the PIDMotor class to provide precision positioning.


// This example uses the FlexiTimer2 library, which is used to automatically call our motor's update() function every 50 milliseconds. You can download this library from:
// http://playground.arduino.cc/Main/FlexiTimer2

// This example uses Timer 2 to make TODO, which breaks the PWM output on the following pins:
//      Arduino UNO: Pins 3 and 11
//          On the Bricktronics Shield, this will only interfere with the TIP120 marked Q4 (you probably aren't using this.)
//      Arduino Mega: Pins 9 and 10
//          TODO megashield?

//TODO This example starts the motor at an intermediate speed, then speeds it up to full speed, and does the same in reverse.

//This example uses a motor, so it needs more power than a USB port can give.  We really don't recommend running motors off of USB ports--they will be slow and sluggish, other things won't quite work right, things can get hot--it's just not a good idea.  Use an external power supply that provides between 9V and 7.2V, and can provide at least 600 mA per motor (1A preferably).  Two options that work really well are a 9V wall adapter (2.1mm plug, center positive) or a 6xAA battery pack.

// Connect a motor to Motor Port 1.

Bricktronics brick = Bricktronics();
PIDMotor m = PIDMotor(&brick, 1);


void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world.");
    brick.begin();
    m.begin();

    m.go_to_pos(0);

    // set up the interrupt to occur every 50 ms
    FlexiTimer2::set(50, update_motor_interrupt);
    FlexiTimer2::start();
}

void update_motor_interrupt(void)
{
    m.update();
}


void loop() 
{
    // This statement doesn't actually move anything, yet.
    // It simply sets the motor's destination position (720 ticks per revolution).
    m.go_to_pos(-100);

    // Since we have set up the Timer 2 interrupt already,
    // we don't have to worry about what we do in loop()
    delay(random(1000));

    Serial.println("Here is a long statement that will take a long time to transmit via serial...!");

    delay(random(1000));

    m.go_to_angle(90);

    delay(random(1000));

    m.go_to_angle(random(360));

    delay(1000);
}

