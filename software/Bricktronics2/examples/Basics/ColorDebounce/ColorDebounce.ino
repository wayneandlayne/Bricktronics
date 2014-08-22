// Bricktronics Example: ColorButton
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Color Sensor.
//
// Color sensor readings are taken repeatedly, and when they change,
// they're debounces so the colors don't switch back and forth on a
// transition. The debounced color is printed to the serial console.
// This isn't necessarily the simplest or best way to debounce,
// but it works for what we're using it for!


// Include the Bricktronics ColorSensor library
#include <ColorSensor.h>


// This example can be run in three different ways. Pick one, and un-comment
// the code lines corresponding to your chosen method. Comment-out the lines
// for the other methods that you aren't using.

// 1. With a Bricktronics Shield - Include these lines and be sure to
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the ColorSensor (BS_SENSOR_3 and BS_SENSOR_4) below.
// Use the jumpers to connect only pins 3-4 for the color sensor.
//
//#include <Wire.h>
//#include <Bricktronics2.h>
//ColorSensor c = ColorSensor(Bricktronics::BS_SENSOR_3);

// 2. With a Bricktronics Megashield - Include these lines but do not
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the ColorSensor (BMS_SENSOR_1 through BMS_SENSOR_4) below.
// Use the jumpers to connect only pins 3-4 for the color sensor.
//
//#include <Bricktronics2.h>
//ColorSensor c = ColorSensor(Bricktronics::BMS_SENSOR_3);

// 3. With a Bricktronics Breakout board - No additional includes needed, just
// update the pin assignments in the ColorSensor constructor below.
//
// The ColorSensor() arguments are: clockPin, dataPin
// There are a few considerations for pin assignments:
// dataPin - This must be an analog pin.
//
//ColorSensor c = ColorSensor(8, 16);


void setup()
{
  // Be sure to set your serial console to 115200 baud
  Serial.begin(115200);

  // Only call this if you are using a Bricktronics Shield,
  // otherwise leave it commented-out.
  //Bricktronics::begin();

  // Initialize the color sensor connections
  c.begin();
}

char stable = 0;
char lastColor = c.getColor();
char newReading;

void loop()
{
  stable = 0;
  newReading = c.getColor();

  if (newReading != lastColor)
  {
    while (stable < 10)
    {
      delay(10);

      if (c.getColor() == newReading)
      {
        stable += 1;
      }
      else
      {
        newReading = c.getColor();
        stable = 0;
      }
    }

    if (newReading != lastColor)
    {
      c.printColor(newReading);
      Serial.println();
      lastColor = newReading;
    }
  }

  delay(10);
}

