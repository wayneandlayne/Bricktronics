// Bricktronics Example: LightSensor
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Light Sensor.
//
// Light sensor readings are taken every 100 milliseconds, and
// printed out over the serial console. Be sure to set your serial
// console to 115200 baud. The light sensor library reports the measured
// light value with a unitless value between 0 (very dim) and
// 1023 (very bright). We can also set a calibration range, and the
// library can scale the raw value to a range of 0-100 based on the
// calibration values. Both these values are printed to the serial console.
//
// We also flash the Light Sensor's LED between each sample. We've found
// that the light doesn't help much in distinguishing between light and dark
// surfaces, but your mileage may vary, so give it a try.


// Include the Bricktronics LightSensor library
#include <LightSensor.h>


// This example can be run in three different ways. Pick one, and un-comment
// the code lines corresponding to your chosen method. Comment-out the lines
// for the other methods that you aren't using.

// 1. With a Bricktronics Shield - Include these lines and be sure to
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the Light sensor (BS_SENSOR_1 through BS_SENSOR_4) below.
// For ports 3 and 4, use the jumpers to connect pins 1-2 and 4-5.
//
//#include <Wire.h>
//#include <Bricktronics.h>
//LightSensor ls = LightSensor(Bricktronics::BS_SENSOR_1);

// 2. With a Bricktronics Megashield - Include these lines but do not
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the light sensor (BMS_SENSOR_1 through BMS_SENSOR_4) below.
// Use the jumpers to connect pins 1-2 and 4-5 for the light sensor.
//
//#include <Bricktronics.h>
//LightSensor ls = LightSensor(Bricktronics::BMS_SENSOR_4);

// 3. With a Bricktronics Breakout board - No additional includes needed, just
// update the pin assignments in the LightSensor constructor below.
//
// The LightSensor() arguments are: inputPin, lightPin
// There is one consideration for pin assignments:
// A. inputPin needs to be an analog pin
//
//LightSensor ls = LightSensor(14, 12);


void setup() 
{
  // Be sure to set your serial console to 115200 baud
  Serial.begin(115200);

  // Only call this if you are using a Bricktronics Shield,
  // otherwise leave it commented-out.
  //Bricktronics::begin();

  // Initialize the light sensor connections
  ls.begin();
}


void loop()
{
  Serial.print("Raw value: ");
  Serial.print(ls.value());
  Serial.print(" - scaled value: ");
  Serial.println(ls.scaledValue());

  // Flash the built-in led on the light sensor
  ls.setFloodlightAlways(true);
  delay(100);
  ls.setFloodlightAlways(false);

  delay(500);
}

