#include <Wire.h>
#include <Bricktronics.h>
#include <Time.h>
// Requires the Time library from http://playground.arduino.cc/Code/Time

// Make: LEGO and Arduino Projects
// Chapter 4: Clock
// Website: http://www.wayneandlayne.com/bricktronics/

// The minimum number of milliseconds between calls to loop()
#define TIME_STEP 100

time_t t;

Bricktronics brick = Bricktronics();
PIDMotor h = PIDMotor(&brick, 1);
PIDMotor m = PIDMotor(&brick, 2);
Button hour_button = Button(&brick, 1);
Button minute_button = Button(&brick, 2);

void setup()
{
    Serial.begin(115200); // We use the USB serial port for debugging our Arduino code. It allows us to send information from the Arduino to the computer.
    Serial.println("starting!");

    brick.begin();
    m.begin();
    h.begin();
    hour_button.begin();
    minute_button.begin();
}

void digitalClockDisplay() // prints out the time to the serial port.
{
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
}

void printDigits(int digits) // a helper function for digitalClockDisplay().
{
    Serial.print(":");
    if (digits < 10)
    {
        Serial.print('0');
    }
    Serial.print(digits);
}

void increment_minute()
{
    adjustTime(60);
    Serial.println("Advance minute pressed");
}

void increment_hour()
{
    adjustTime(3600);
    Serial.println("Advance hour pressed");
}

void check_buttons() // handles most of the logic of the clock, outside of moving the hands. It is only called once each time loop() runs, which is every 100 ms, so debouncing (adding additional program logic to handle electrical noise in button presses) the buttons isn’t really necessary in this application.
{
    static char last_minute_status = HIGH;
    static char last_hour_status = HIGH;

    char minute_status = minute_button.is_pressed();
    char hour_status = hour_button.is_pressed();

    if (minute_status == HIGH && last_minute_status == LOW)
    {
        increment_minute();
    }

    if (hour_status == HIGH && last_hour_status == LOW)
    {
        increment_hour();
    }

    last_minute_status = minute_status;
    last_hour_status = hour_status;
}


void loop() // runs over and over again. It handles moving the hands, as well as reading the buttons.
{
    long next_loop = millis() + TIME_STEP; // used for the inner PID loop, which runs for TIME_STEP.
    check_buttons(); // checks the minute and hour buttons for presses.
    t = now(); // t holds the current time in a time_t struct.

    int full_hours = hour(t) % 12;
    int minute_position; // the desired position of the minute hand (in degrees) for the current time, where 0 degrees is at the 12:00 position and increases in a clockwise manner.
    minute_position = minute(t)*6 + 360*full_hours; // To convert from minutes to degrees on a clock, you multiply by 6. For example, 15 minutes*6 = 90 degrees.

    int m_pos; // m_pos is the position we want the minute hand motor to go to. Because of gearing, this is not just minute_position!
    m_pos = -3 * 2 * minute_position; // The negative sign is here because of the direction the motor is mounted in. The 3 is based on the gear ratio: there is an 8-tooth gear on the motor, geared to a 24-tooth gear which is connected to the hand. This is a 3:1 ratio, so we need to move our motor three degrees for every one degree we want the minute hand to go. The 2 is because we read two steps per degree.

    int hour_position; // the desired position of the hour hand (in degrees) for the current time, where 0 degrees is at the 12:00 position and increases in a clockwise manner.
    hour_position = (hour(t) % 12) * 30 + minute(t)/2; // To get the position of the hour hand, we have to move the hand 30 degrees for every full hour, and then move the hour hand proportionally for how far the minute hand has gone. So, for example, 6:00 would work out to be exactly 180 degrees (hour hand pointing at the 6). And at 1:30, the hour hand will be at exactly 45 degrees.
    int h_pos = 2 * 5 * hour_position; // There is an 8-tooth gear driving a 24-tooth gear, for a 3:1 ratio and then that 24-tooth gear is driving a 40-tooth gear, for a 1.667:1 ratio. This works out to a 5:1 ratio overall. The reason for the 2 in the equation is that we read two steps per degree.
    digitalClockDisplay();
    Serial.println();

    m.go_to_pos(m_pos);
    h.go_to_pos(h_pos); // The program will continue to try to get the hands to the right positions for about 100 ms.

    while (millis() < next_loop)
    {
        h.update();
        m.update();
        delay(50);
    }
}

