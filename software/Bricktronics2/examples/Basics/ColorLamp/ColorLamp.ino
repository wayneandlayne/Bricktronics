// Bricktronics Example: ColorLamp
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Color Sensor.
//
// The Color sensor can also act like a red, green, or blue lamp.
// This example switches between these three colors in turn.


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
  // Only call this if you are using a Bricktronics Shield,
  // otherwise leave it commented-out.
  Bricktronics::begin();

  // Initialize the color sensor connections
  c.begin();
}


void loop()
{
  c.begin(TYPE_COLORRED);
  delay(500);

  c.begin(TYPE_COLORBLUE);
  delay(500);

  c.begin(TYPE_COLORGREEN);
  delay(500);

  c.begin(TYPE_COLORNONE);
  delay(500);
}

