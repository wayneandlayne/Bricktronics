#include <Wire.h>
#include <Bricktronics.h>

//Bricktronics Example: PIDMotorSingle
//http://www.wayneandlayne.com/bricktronics/
//This example uses a LEGO NXT Motor with the PIDMotor class to provide precision positioning.

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
}

void loop() 
{
    // This statement doesn't actually move anything, yet.
    // It simply sets the motor's destination position (720 ticks per revolution).
    m.go_to_pos(-100);

    // To actually move the motor to the desired destination position, we need to repeatedly
    // call the update() function, which runs the PID algorithm and decides how to move the
    // motor in the next time interval.

    // If we have nothing else to do except wait for the motor to reach its destination, we 
    // can use this shortcut function below, which repeatedly calls the motor's update() function
    // while waiting for the specified period of time (here, 1000 milliseconds).
    Bricktronics::delay_update(1000, &m, NULL);


    // Now we want to move the motor to a different position
    m.go_to_pos(100);

    // This time, let's pretend we have other things to do right now, and can't simply use delay_update(), so we can do something like this:

    long end_time_overall = millis() + 1000; // this is the time when we want to be done
    while (millis() < end_time_overall)
    {
        // we only want to call the update function every 50 milliseconds, so we need an inner timing
        // loop to keep track of that
        long end_time_inner = millis() + 50; // this is the time when we next need to update() the motor

        Serial.println("Doing work (< 50ms each time) while motors are moving");
        // do_something();

        while (millis() < end_time_inner); // just wait here until 50ms has elapsed

        m.update();
    }
}

