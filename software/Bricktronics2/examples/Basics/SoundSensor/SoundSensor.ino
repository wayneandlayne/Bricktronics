// Bricktronics Example: SoundSensor
// http://www.wayneandlayne.com/bricktronics
// This example uses a LEGO NXT Sound Sensor.
//
// Sound sensor readings are as fast a possible and used to generate a
// simple bar graph printed out over the serial console. Be sure to set
// your serial console to 115200 baud. The sound sensor library reports the
// measured sound value with a unitless value between 0 (very quiet) and
// 1023 (very loud).
//
// The sound sensor is described by LEGO as a "sound pressure level" sensor,
// that has two modes: dB and dBA. dB mode measures all frequencies equally,
// while dBA mode (A = "adjusted") tries to mimic human ear sensitivity, by
// ignoring frequencies outside the normal limits of human hearing.


// Include the Bricktronics SoundSensor library
#include <SoundSensor.h>


// This example can be run in three different ways. Pick one, and un-comment
// the code lines corresponding to your chosen method. Comment-out the lines
// for the other methods that you aren't using.

// 1. With a Bricktronics Shield - Include these lines and be sure to
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the sound sensor (BS_SENSOR_1 through BS_SENSOR_4) below.
// For ports 3 and 4, use the jumpers to connect pins 1-2 and 4-5.
//
//#include <Wire.h>
//#include <Bricktronics2.h>
//SoundSensor ss = SoundSensor(Bricktronics::BS_SENSOR_1);

// 2. With a Bricktronics Megashield - Include these lines but do not
// call Bricktronics::begin() in the setup() function below.
// Select the sensor port for the sound sensor (BMS_SENSOR_1 through BMS_SENSOR_4) below.
// Use the jumpers to connect pins 1-2 and 4-5 for the sound sensor.
//
//#include <Bricktronics2.h>
//SoundSensor ss = SoundSensor(Bricktronics::BMS_SENSOR_4);

// 3. With a Bricktronics Breakout board - No additional includes needed, just
// update the pin assignments in the SoundSensor constructor below.
//
// The SoundSensor() arguments are: inputPin, dbPin, dbaPin
// There is one consideration for pin assignments:
// A. inputPin needs to be an analog pin
//
//SoundSensor ss = SoundSensor(14, 12, 11);


void setup() 
{
  // Be sure to set your serial console to 115200 baud
  Serial.begin(115200);

  // Only call this if you are using a Bricktronics Shield,
  // otherwise leave it commented-out.
  //Bricktronics::begin();

  // Initialize the sound sensor connections
  ss.begin();
}

// This function draws a nice bar graph for displaying sound levels.
void printLine(uint16_t val)
{
  #define BUF_LEN 30
  char buffer[BUF_LEN];
  uint8_t x = (val * (uint16_t) BUF_LEN) / 1023;
  for (uint8_t i = 0; i < BUF_LEN; i++)
  {
    buffer[i] = ((i < x) ? '*' : ' ');
  }
  buffer[BUF_LEN - 1] = '\0';
  Serial.println(buffer);
}

long switchTime;

void loop()
{
  // Spend 5 seconds in db mode
  Serial.println("----- Switching to DB mode -----");
  ss.setMode(SOUND_SENSOR_MODE_DB);
  switchTime = millis() + 5000;
  while (millis() < switchTime)
  {
    printLine(ss.value());
  }
  
  // Spend 5 seconds in dba mode
  Serial.println("----- Switching to DBA mode -----");
  ss.setMode(SOUND_SENSOR_MODE_DBA);
  switchTime = millis() + 5000;
  while (millis() < switchTime)
  {
    printLine(ss.value());
  }
}

