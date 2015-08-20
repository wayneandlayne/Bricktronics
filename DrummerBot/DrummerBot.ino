#include <Wire.h>
#include <Bricktronics.h>

Bricktronics brick = Bricktronics();
Ultrasonic ultrasonic = Ultrasonic(&brick, 4); //Plug an Ultrasonic Sensor into Sensor Port 4, and use the jumpers to connect pins 1 and 2, and 3 and 4.
PIDMotor m1 = PIDMotor(&brick, 1);
PIDMotor m2 = PIDMotor(&brick, 2);

void setup() 
{
  Serial.begin(115200);
  brick.begin();
  ultrasonic.begin();
  m1.begin();
  m2.begin();
}

void loop()
{
  if (ultrasonic.getDistance() < 100);
  {
    m1.go_to_pos(-100);

    for (int i = 0; i < 5; i++)
    {
        m1.update();
        delay(100);
    }

    m1.go_to_pos(0);
    
    m2.go_to_pos(-100);
    for (int i = 0; i < 5; i++)
    {
        m1.update();
        m2.update();
        delay(100);
    }
    m1.stop();

    m2.go_to_pos(0);

    {
        m2.update();
        delay(100);
    }
    m2.stop();

    delay(ultrasonic.getDistance()*5);

  }
}

