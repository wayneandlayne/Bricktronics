// Bricktronics Example: ColorButton
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Pushbutton Sensor and Color Sensor.
// 
// When the button is pressed, a single reading from the color sensor is taken
// and converted into a color name and printed over the Serial Console.


// Include the Bricktronics Button and ColorSensor libraries
#include <Button.h>
#include <ColorSensor.h>


// This example can be run in three different ways. Pick one, and un-comment
// the code lines corresponding to your chosen method. Comment-out the lines
// for the other methods that you aren't using.

// 1. With a Bricktronics Shield - Include these lines and be sure to
// call Bricktronics::begin() in the setup() function below.
// Select the sensor ports for the Button (BS_SENSOR_1 through BS_SENSOR_4)
// and ColorSensor (BS_SENSOR_3 and BS_SENSOR_4) below.
// If your chosen port has jumpers, connect pins 2-3 and 4-5 for the button,
// and connect only pins 3-4 for the color sensor.
//
//#include <Wire.h>
//#include <Bricktronics2.h>
//Button b = Button(Bricktronics::BS_SENSOR_1);
//ColorSensor c = ColorSensor(Bricktronics::BS_SENSOR_3);

// 2. With a Bricktronics Megashield - Include these lines but do not
// call Bricktronics::begin() in the setup() function below.
// Select the sensor ports for the Button (BMS_SENSOR_1 through BMS_SENSOR_4)
// and ColorSensor (BMS_SENSOR_1 through BMS_SENSOR_4) below.
// If your chosen port has jumpers, connect pins 2-3 and 4-5 for the button,
// and connect only pins 3-4 for the color sensor.
//
//#include <Bricktronics2.h>
//Button b = Button(Bricktronics::BMS_SENSOR_1);
//ColorSensor c = ColorSensor(Bricktronics::BMS_SENSOR_3);

// 3. With two Bricktronics Breakout boards - No additional includes needed, just
// update the pin assignments in the Button and ColorSensor constructors below.
//
// The Button() argument is simply the pin the button is connected to, that is, wherever
// pin 1 of the Breakout board is connected (also connect the grounds).
//
// The ColorSensor() arguments are: clockPin, dataPin
// There are a few considerations for pin assignments:
// dataPin - This must be an analog pin.
//
//Button b = Button(7);
//ColorSensor c = ColorSensor(8, 16);

void setup() 
{
  // Be sure to set your serial console to 115200 baud
  Serial.begin(115200);

  // Only call this if you are using a Bricktronics Shield,
  // otherwise leave it commented-out.
  //Bricktronics::begin();

  // Initialize the button and color sensor connections
  b.begin();
  c.begin();
}


void loop()
{
  // Wait until the button is pressed
  while (b.isReleased())
  {
    // Nothing to do here
  }
  // To get here, the button was pushed!

  c.printColor(c.getColor());
  Serial.println();

  // In order to debounce the button, we wait a little bit here
  delay(100);

  // Wait until the button is released
  while (b.isPressed())
  {
    // Nothing to do here
  }

  // In order to debounce the button, we wait a little bit here
  delay(100);
}

