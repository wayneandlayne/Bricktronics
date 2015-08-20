#include <Wire.h>
#include <ArduinoNunchuk.h> // While there are plenty of Nunchuk libraries, the one we used can be downloaded at http://www.gabrielbianconi.com/projects/arduinonunchuk/.

// Make: LEGO and Arduino Projects
// Chapter 7: GripperBot: Left Bracer
// In the default configuration, the left bracer controls the left tank tread. When the Nunchuk is tilted forward, the left tank tread goes forward. When the Nunchuk is tilted back, the left tank tread goes backward.
// Website: http://www.wayneandlayne.com/bricktronics/

ArduinoNunchuk nunchuk = ArduinoNunchuk();

#define NUM_OF_READINGS 10 // NUM_OF_READINGS define controls how many samples are taken in the running average.

char pitch_index = 0;
int pitch_readings[NUM_OF_READINGS];
int pitch_total = 0;

void setup() // setup() runs once at startup.
{
    Serial.begin(9600);
    init_nunchuk_connections();
    delay(100);
    nunchuk.init();

    for (int i = 0; i < NUM_OF_READINGS; i++) // Initialize the variables for the pitch averaging.
    {
        pitch_readings[i] = 0;
    }
}

void loop() // loop() runs repeatedly until power is removed.
{
    nunchuk.update(); // Updating the nunchuk object reads new data from the sensors.
    handle_left_tank_tread(nunchuk.accelY);
    delay(50);
}

void init_nunchuk_connections() // The Solarbotics NunChucky plugs the Nunchuk's 4 connections into the lower analog pins. The Nunchuk only needs a little power, so we can actually power it from the Arduino pins!
{
    pinMode(A2, OUTPUT);
    digitalWrite(A2, LOW);
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);
}

/*  average_accel_pitch makes a running average of the last NUM_OF_
    READINGS readings from the Nunchuk. This is useful because the accelerometer
    on the Nunchuk can be a little noisy, and a very simple way of
    removing some of the noise is to do a running average. See the Arduino
    example sketch “Smoothing” for more details on running averages.
*/
int average_accel_pitch(int raw_pitch)
{

    pitch_total -= pitch_readings[pitch_index]; // Remove the oldest pitch reading from the total.
    pitch_readings[pitch_index] = raw_pitch; // Replace the oldest pitch reading with the current pitch reading.
    pitch_total += pitch_readings[pitch_index]; // Add the newest pitch reading to the total.

    pitch_index += 1; // Advance the index to the next spot.
    if (pitch_index >= NUM_OF_READINGS) // If the index is past the end, bring it back to zero.
    {
        pitch_index = 0;
    }

    return pitch_total / NUM_OF_READINGS; // Return the average by dividing the total by the number of readings.
}

int last_left_speed = 0;

void handle_left_tank_tread(int raw_accel_pitch) // handle_left_tank_tread() takes in the Nunchuk's pitch value, and sends a command, if needed, to the Arduino on the body telling it the speed to move the left tank tread.
{
    int avg_pitch = average_accel_pitch(raw_accel_pitch);

#define PITCH_CENTER 550 // The readings out of the Nunchuk are centered at approximately 550. It goes to about 700 forward, and to about 300 backward. These may vary on your unit a little -- if so, feel free to adjust the constants to make it more accurate on your Nunchuk. We’re going to translate this range to a signed range with 0 pitch being straight up, forward being positive, and back being negative.
#define MAX_PITCH PITCH_CENTER+150
#define MIN_PITCH PITCH_CENTER-150

#define MAX_FORWARD 127 // We've picked this range because it is the maximum range that can fit in a signed byte.
#define MAX_BACKWARD -128

    avg_pitch = constrain(avg_pitch, MIN_PITCH, MAX_PITCH);
    int left_speed = map(avg_pitch, MIN_PITCH, MAX_PITCH, MAX_BACKWARD, MAX_FORWARD); // This maps our number from the range MIN_PITCH to MAX_PITCH to the range MAX_BACKWARD to MAX_FORWARD.
    left_speed = constrain(left_speed, MAX_BACKWARD, MAX_FORWARD); // No matter what, our number is going to range from MAX_BACKWARD to MAX_FORWARD.

        /* There are a lot of things you can do here, to keep the system responsive but cut down on unnecessary radio traffic, which takes power. You can only update if the change goes over a threshold, or add more fancy filtering, but we're going to go with a simple option of "transmit the speed if it isn't the same as the last speed we transmitted." */
    if (left_speed != last_left_speed)
    {
        last_left_speed = left_speed;
        Serial.write('*'); // In our system, all commands start with an asterisk.
        Serial.write(6); // The second byte of the command is the motor number. The left motor is 6. Serial.write() transmits the byte without making it "printable".
        char out = left_speed; // Even though left_speed currently is between 127 and -128, which fits in a char, we need to make it a char so we only send one byte.
        Serial.write(out); // The third byte of the command is the speed, from -128 to 127.
    }
}

