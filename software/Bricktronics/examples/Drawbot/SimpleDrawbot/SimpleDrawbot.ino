#include <Wire.h> // These 2 lines let the Arduino sketch use the Bricktronics library code that simplifies working with motors and sensors.
#include <Bricktronics.h>

// Make: LEGO and Arduino Projects
// Chapter 1: Drawbot
// Website: http://www.wayneandlayne.com/bricktronics/

long timeout = 0;
Bricktronics brick = Bricktronics(); // The Bricktronics object manages many of the Bricktronics library calls.
Motor r = Motor(&brick, 1); // The r and l Motor objects correspond to the motors plugged into Motor Port 1 and 2, respectively, of the Bricktronics Shield.
Motor l = Motor(&brick, 2);
Button front = Button(&brick, 1); // The front Button object corresponds to the button plugged into Sensor Port 1.

void setup() // The setup() function is called once, on power on. Here, it’s used to initialize all the Bricktronics objects.
{
    randomSeed(analogRead(A3)); // This line seeds the pseudorandom number generator with an analog reading from a disconnected pin, A3. This helps the numbers be more random.
    brick.begin();
    r.begin();
    l.begin();
    front.begin();
}

void bumpers_hit() // This function is called when the bumpers are hit. The idea is to reverse each motor, and slow it down a bit, but if it slows down too slowly, to start over with new random values, so it doesn’t get boring.
{
    int r_speed = r.get_speed() * -0.9;
    int l_speed = l.get_speed() * -0.9;

    r.set_speed(r_speed);
    l.set_speed(l_speed);
    delay(500);

    if (abs(l_speed) < 25 && abs(r_speed) < 25)
    {
        timeout_expired();
    }
}

void timeout_expired() // This function sets the motors going at a random speed, and it determines a timeout between 1 and 10 seconds, after which it will be called again.
{
    r.set_speed(random(-127, 127));
    l.set_speed(random(-127, 127));

    timeout = millis() + random(1, 10) * 1000;
}

void loop() // In Arduino, your loop() function is called over and over again, until the Arduino runs out of power.
{
    if (timeout != 0 && timeout < millis()) // If the timeout has been set, and the current time is past the timeout, run the timeout_expired() function.
    {
        timeout_expired();
    }

    if (front.is_pressed()) // If the front bumper is pressed, then call the bumpers_hit() function, which reverses the robot unless the motors are going too slowly.
    {
        bumpers_hit();
    }
}

