#include <Wire.h> // Only needed if you are using the Bricktronics Shield
#include <Bricktronics.h>
#include <Motor.h>
#include <Button.h>
#include <Time.h>

// Make: LEGO and Arduino Projects
// Chapter 4: Clock
// Website: http://www.wayneandlayne.com/bricktronics/

// The minimum number of milliseconds between calls to loop()
#define TIME_STEP 100

time_t t;

//Bricktronics brick = Bricktronics(); // This line is no longer needed with Bricktronics library v2
Motor hourMotor = Motor(Bricktronics::BS_MOTOR_1);
Motor minuteMotor = Motor(Bricktronics::BS_MOTOR_2);
Button hourButton = Button(Bricktronics::BS_SENSOR_1);
Button minuteButton = Button(Bricktronics::BS_SENSOR_2);

void setup()
{
  Serial.begin(115200); // We use the USB serial port for debugging our Arduino code. It allows us to send information from the Arduino to the computer.
  Serial.println("Starting!");

  Bricktronics::begin(); // This line is only needed if you are using the Bricktronics Shield
  minuteMotor.begin();
  hourMotor.begin();
  hourButton.begin();
  minuteButton.begin();
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

void incrementMinute()
{
  adjustTime(60);
  Serial.println("Advance minute pressed");
}

void incrementHour()
{
  adjustTime(3600);
  Serial.println("Advance hour pressed");
}

void checkButtons() // handles most of the logic of the clock, outside of moving the hands. It is only called once each time loop() runs, which is every 100 ms, so debouncing (adding additional program logic to handle electrical noise in button presses) the buttons isn’t really necessary in this application.
{
  // Static means that the variables' values are not reset each time we call checkButtons
  static char lastMinuteStatus = HIGH;
  static char lastHourStatus = HIGH;

  char minuteStatus = minuteButton.isPressed();
  char hourStatus = hourButton.isPressed();

  if (minuteStatus == HIGH && lastMinuteStatus == LOW)
  {
    incrementMinute();
  }

  if (hourStatus == HIGH && lastHourStatus == LOW)
  {
    incrementHour();
  }

  lastMinuteStatus = minuteStatus;
  lastHourStatus = hourStatus;
}


void loop() // runs over and over again. It handles moving the hands, as well as reading the buttons.
{
  long nextLoop = millis() + TIME_STEP; // Used for the inner PID loop, which runs for TIME_STEP.
  checkButtons(); // Checks the minute and hour buttons for presses.
  t = now(); // t holds the current time in a time_t struct.

  int fullHours = hour(t) % 12;
  int minutePosition; // The desired position of the minute hand (in degrees) for the current time, where 0 degrees is at the 12:00 position and increases in a clockwise manner.
  minutePosition = minute(t) * 6 + 360 * fullHours; // To convert from minutes to degrees on a clock, you multiply by 6. For example, 15 minutes*6 = 90 degrees.

  int mPos; // mPos is the position we want the minute hand motor to go to. Because of gearing, this is not just minute_position!
  mPos = -3 * 2 * minutePosition; // The negative sign is here because of the direction the motor is mounted in. The 3 is based on the gear ratio: there is an 8-tooth gear on the motor, geared to a 24-tooth gear which is connected to the hand. This is a 3:1 ratio, so we need to move our motor three degrees for every one degree we want the minute hand to go. The 2 is because we read two steps per degree.

  int hourPosition; // The desired position of the hour hand (in degrees) for the current time, where 0 degrees is at the 12:00 position and increases in a clockwise manner.
  hourPosition = (hour(t) % 12) * 30 + minute(t) / 2; // To get the position of the hour hand, we have to move the hand 30 degrees for every full hour, and then move the hour hand proportionally for how far the minute hand has gone. So, for example, 6:00 would work out to be exactly 180 degrees (hour hand pointing at the 6). And at 1:30, the hour hand will be at exactly 45 degrees.
  int hPos = 2 * 5 * hourPosition; // There is an 8-tooth gear driving a 24-tooth gear, for a 3:1 ratio and then that 24-tooth gear is driving a 40-tooth gear, for a 1.667:1 ratio. This works out to a 5:1 ratio overall. The reason for the 2 in the equation is that we read two steps per degree.
  digitalClockDisplay();
  Serial.println();

  minuteMotor.goToPosition(mPos);
  hourMotor.goToPosition(hPos); // The program will continue to try to get the hands to the right positions for about 100 ms.

  while (millis() < nextLoop)
  {
    hourMotor.update();
    minuteMotor.update();
    delay(50);
  }
}

