#include <Wire.h>
#include <Bricktronics.h>

// Getting started with Lego and Arduino
// Chapter 7: GripperBot: Tank Base
// Website: http://www.wayneandlayne.com/bricktronics

Bricktronics brick = Bricktronics();
Motor r = Motor(&brick, 1);
Motor l = Motor(&brick, 2);

void setup() // The setup() function runs once at startup.
{
    Serial.begin(9600);
    brick.begin();
    r.begin();
    l.begin();
}

void process_incoming_command(char cmd, char arg0) // takes in the two bytes of the body of a command, and then performs the specified action.
{
    int speed = 0;
    switch (cmd)
    {
        case '6':
        case 6: // If the command was "set the speed to motor 6 (left tank tread)," then the code that follows is run.
            speed = arg0*2;
            l.set_speed(speed);
            break;
        case '7':
        case 7: // If the command was "set the speed to motor 7 (right tank tread)," the code that follows is run.
            speed = arg0*2;
            r.set_speed(speed);
            break;
        default:
            break;
    }
}

void loop()
{
    while (Serial.available() < 3)
    {
        //Do nothing while we wait for a full command to come in.
    }
    char start = Serial.read(); // Remove the oldest character from the serial buffer, and put it into start.
    if (start != '*') // This isn't the start of the command, so let's try the next one.
    {
        return; // Start back over at the top of loop().
    }

    char cmd = Serial.read(); // Read the command and argument, and call process_incoming_command().
    char arg0 = Serial.read();
    process_incoming_command(cmd, arg0);
}

