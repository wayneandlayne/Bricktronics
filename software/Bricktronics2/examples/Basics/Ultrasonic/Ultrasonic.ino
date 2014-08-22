// Bricktronics Example: UltrasonicSerial
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Ultrasonic Sensor.
//
// Ultrasonic readings are taken every 100 milliseconds, and 
// printed out over the serial console. Be sure to set your serial
// console to 115200 baud. The ultrasonic sensor reports the distance
// to an obstruction in front of the sensor (in centimeters). It has
// a range of 2.5 meters, a resolution of 1 cm, and reported accuracy
// of +/- 3 cm. 255 is sometimes reported as an error state.
//
// This example uses an Ultrasonic Sensor, so it needs more voltage
// than a USB port usually gives. Use an external power supply that
// provides between 7.2 and 9 volts DC. Two options that work really
// well are a 9V wall adapter or a 6xAA battery pack (2.1mm plug).


// Include the Bricktronics Ultrasonic library
#include <Ultrasonic.h>


// This example can be run in three different ways. Pick one, and un-comment
// the code lines corresponding to your chosen method. Comment-out the lines
// for the other methods that you aren't using.

// 1. With a Bricktronics Shield - Include these lines and be sure to
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the Ultrasonic (BS_SENSOR_3 and BS_SENSOR_4) below.
// Use the jumpers to connect pins 1-2 and 4-5 for the ultrasonic sensor.
//
//#include <Wire.h>
//#include <Bricktronics2.h>
//Ultrasonic u = Ultrasonic(Bricktronics::BS_SENSOR_4);

// 2. With a Bricktronics Megashield - Include these lines but do not
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the Ultrasonic (BMS_SENSOR_1 through BMS_SENSOR_4) below.
// Use the jumpers to connect pins 1-2 and 4-5 for the ultrasonic sensor.
//
//#include <Bricktronics2.h>
//Ultrasonic u = Ultrasonic(Bricktronics::BMS_SENSOR_4);

// 3. With a Bricktronics Breakout board - No additional includes needed, just
// update the pin assignments in the Ultrasonic constructor below.
//
// The Ultrasonic() arguments are: sclPin, sdaPin
//
//Ultrasonic u = Ultrasonic(8, 12);


void setup() 
{
  // Be sure to set your serial console to 115200 baud
  Serial.begin(115200);

  // Only call this if you are using a Bricktronics Shield,
  // otherwise leave it commented-out.
  //Bricktronics::begin();

  // Initialize the ultrasonic sensor connections
  u.begin();
}


void loop()
{
  Serial.println(u.getDistance());

  delay(100);
}

