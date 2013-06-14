#include <Wire.h>
#include <ArduinoNunchuk.h> // While there are plenty of Nunchuk libraries, the one we used can be downloaded at http://www.gabrielbianconi.com/projects/arduinonunchuk/

// Make: LEGO and Arduino Projects
// Chapter 7: GripperBot: Right Bracer
// Website: http://www.wayneandlayne.com/bricktronics/

ArduinoNunchuk nunchuk = ArduinoNunchuk();

#define NUM_OF_READINGS 10 // NUM_OF_READINGS controls how many samples are taken in the running average.

char roll_index = 0;
int roll_readings[NUM_OF_READINGS];
int roll_total = 0;
char pitch_index = 0;
int pitch_readings[NUM_OF_READINGS];
int pitch_total = 0;

void setup() // setup() runs once at startup.
{
    Serial.begin(9600);
    init_nunchuk_connections();
    delay(100);
    nunchuk.init();


    for (int i = 0; i < NUM_OF_READINGS; i++) // Initialize the variables for the pitch and roll averaging.
    {
        roll_readings[i] = 0;
        pitch_readings[i] = 0;
    }
}


void loop() // loop() runs repeatedly until power is removed.
{
    nunchuk.update(); // Read new data from the Nunchuk.
    handle_right_tank_tread(nunchuk.accelY);
    handle_wrist(nunchuk.accelX);
    handle_spin(nunchuk.analogX);
    handle_elbow(nunchuk.analogY);
    handle_grip(nunchuk.zButton, nunchuk.cButton);

    delay(50);
}


void init_nunchuk_connections() // The Solarbotics NunChucky plugs the Nunchuk's four connections into the lower analog pins. The Nunchuk only needs a little power, so we can actually power it from the Arduino pins!
{
    pinMode(A2, OUTPUT);
    digitalWrite(A2, LOW);
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);
}

/* average_accel_pitch() makes a running average of the last NUM_OF_
   READINGS readings from the Nunchuk. This is useful because the accelerometer
   on the Nunchuk can be a little noisy, and a very simple way of
   removing some of the noise is to do a running average. See the Arduino
   example sketch “Smoothing” (File | Examples | 03.Analog | Smoothing)
   for more details on running averages. This is also the same function as
   in the left bracer, and nearly the same as the averaging function that
   handles roll. */
int average_accel_pitch(int raw_pitch)
{
    pitch_total -= pitch_readings[pitch_index]; //emove the oldest pitch reading from the total.
    pitch_readings[pitch_index] = raw_pitch; // Replace the oldest pitch reading with the current pitch reading.
    pitch_total += pitch_readings[pitch_index]; // Add the newest pitch reading to the total.

    pitch_index += 1; // Advance the index to the next spot.

    if (pitch_index >= NUM_OF_READINGS) // If the index is past the end, bring it back to zero.
    {
        pitch_index = 0;
    }

    return pitch_total / NUM_OF_READINGS; // Return the average by dividing the total by the number of readings.
}

int average_accel_roll(int raw_roll) // average_accel_roll() averages the raw roll values just like average_accel_pitch() does.
{
    roll_total -= roll_readings[roll_index];
    roll_readings[roll_index] = raw_roll;
    roll_total += roll_readings[roll_index];

    roll_index += 1;
    if (roll_index >= NUM_OF_READINGS)
    {
        roll_index = 0;
    }

    return roll_total / NUM_OF_READINGS;
}

int last_right_speed = 0;

void handle_right_tank_tread(int raw_accel_pitch) // handle_right_tank_tread() takes in the Nunchuk's pitch value, and sends a command, if needed, to the Arduino on the tank body telling it the speed to move motor 7, or the right tank tread.
{
    int avg_pitch = average_accel_pitch(raw_accel_pitch);

#define PITCH_CENTER 550 // The readings out of the Nunchuk are centered at approximately 550. They go up to about 700 tilted forward, and about 300 when tilted back. These may vary on your unit a little -- if so, feel free to adjust the constants to make it more accurate on your Nunchuk. We're going to translate this range to a signed range with 0 pitch being straight up, forward being positive, and back being negative.

#define MAX_PITCH PITCH_CENTER+150
#define MIN_PITCH PITCH_CENTER-150

// We've picked this range as the maximum range that can fit in a signed char, which is what we transmit.
#define MAX_FORWARD 127
#define MAX_BACKWARD -128

    avg_pitch = constrain(avg_pitch, MIN_PITCH, MAX_PITCH);
    int right_speed = map(avg_pitch, MIN_PITCH, MAX_PITCH, MAX_BACKWARD, MAX_FORWARD); // This maps our number from the range MIN_PITCH to MAX_PITCH to the range MAX_BACKWARD to MAX_FORWARD.
    right_speed = constrain(right_speed, MAX_BACKWARD, MAX_FORWARD); // No matter what, our number is going to range from MAX_BACKWARD to MAX_FORWARD.


    if (right_speed != last_right_speed) // There are a lot of things you can do here, to keep the system responsive but cut down on unnecessary radio traffic, which takes power. You can only update if the change goes over a threshold, or add more fancy filtering, but we're going to go with a simple option of "transmit the speed if it isn't the same as the last speed we transmitted."
    {
        last_right_speed = right_speed;
        Serial.write('*'); // In our system, all commands start with an asterisk.
        Serial.write(7); // The second byte of the command is the motor number. The right tank tread motor is 7. Serial.write() transmits the byte without making it printable.
        char out = right_speed; // Even though right_speed currently is between 127 and -128, which fits in a char, we need to send a char, not an int, which is two bytes.
        Serial.write(out); // The third byte of the command is the speed, from -128 to 127.
    }
}

int last_spin_speed = 0;

void handle_spin(int joy_x) // handle_spin() takes in the Nunchuk's joystick's x value, and sends a command, if needed, to the motor controller telling it the speed to move motor 1, or the "spin" motor.
{

#define JOY_X_CENTER 129 // The joystick's x range is centered at 129, and it goes about 101 to the right, and about 97 to the left. These may vary on your unit a little -- if so, feel free to adjust the constants to make it more accurate on your Nunchuk. We're going to translate this range to a signed range with 0 being straight up, right being positive, and left being negative.

#define MAX_JOY_X JOY_X_CENTER+101
#define MIN_JOY_X JOY_X_CENTER-97

#define MAX_RIGHT_SPIN 127
#define MAX_LEFT_SPIN -128

    joy_x = constrain(joy_x, MIN_JOY_X, MAX_JOY_X);
    int spin_speed = map(joy_x, MIN_JOY_X, MAX_JOY_X, MAX_LEFT_SPIN, MAX_RIGHT_SPIN);
    spin_speed = constrain(spin_speed, MAX_LEFT_SPIN, MAX_RIGHT_SPIN);

    if (spin_speed != last_spin_speed)
    {
        last_spin_speed = spin_speed;
        Serial.write('*');
        Serial.write(1);
        char out = spin_speed;
        Serial.write(out);
    }
}

int last_elbow_speed = 0;

void handle_elbow(int joy_y) // handle_elbow() takes in the Nunchuk's joystick's y value, and sends a command, if needed, to the motor controller telling it the speed to move motor 2, or the "elbow".
{

#define JOY_Y_CENTER 126 // The joystick's y range is centered at 126, and it goes about 86 forward, and about 98 backward. These may vary on your unit a little -- if so, feel free to adjust the constants to make it more accurate on your Nunchuk. We're going to translate this range to a signed range with 0 being straight up, forward being positive, and backward being negative.

#define MAX_JOY_Y JOY_Y_CENTER+86
#define MIN_JOY_Y JOY_Y_CENTER-98

#define MAX_UP_ELBOW 127
#define MAX_DOWN_ELBOW -128

    joy_y = constrain(joy_y, MIN_JOY_Y, MAX_JOY_Y);
    int elbow_speed = map(joy_y, MIN_JOY_Y, MAX_JOY_Y, MAX_UP_ELBOW, MAX_DOWN_ELBOW);
    elbow_speed = constrain(elbow_speed, MAX_DOWN_ELBOW, MAX_UP_ELBOW);

    if (elbow_speed != last_elbow_speed)
    {
        last_elbow_speed = elbow_speed;
        Serial.write('*');
        Serial.write(2);
        char out = elbow_speed;
        Serial.write(out);
    }
}

int last_wrist_speed = 0;

void handle_wrist(int raw_accel_roll) // handle_wrist() takes in the Nunchuk's accelerometer's roll value, and sends a command, if needed, to the motor controller telling it the speed to move the motor 3, or the "wrist" motor.
{
    int avg_roll = average_accel_roll(raw_accel_roll);

#define ROLL_CENTER 560 // The joystick's roll range is centered at 560, and it goes about 220 right, and 220 left. These may vary on your unit a little -- if so, feel free to adjust the constants to make it more accurate on your Nunchuk. We're going to translate this range to a signed range with 0 being straight up, right being positive, and left being negative.

#define MAX_ROLL ROLL_CENTER+220
#define MIN_ROLL ROLL_CENTER-220

#define MAX_RIGHT_WRIST 127
#define MAX_LEFT_WRIST -128

    avg_roll = constrain(avg_roll, MIN_ROLL, MAX_ROLL);
    int wrist_speed = map(avg_roll, MIN_ROLL, MAX_ROLL, MAX_LEFT_WRIST, MAX_RIGHT_WRIST);
    wrist_speed = constrain(wrist_speed, MAX_LEFT_WRIST, MAX_RIGHT_WRIST);

    if (wrist_speed != last_wrist_speed)
    {
        last_wrist_speed = wrist_speed;
        Serial.write('*');
        Serial.write(3);
        char out = wrist_speed;
        Serial.write(out);
    }
}

char last_cmd = 0;

void handle_grip(boolean z, boolean c) // handle_grip() takes in the Nunchuk's accelerometer's roll value, and sends a command, if needed, to the motor controller telling it the speed to move motor 4, or the gripper motor.
{
#define GRIP_OUT_SPEED 100
#define GRIP_IN_SPEED -100

    if (z == c)
    {
        if (last_cmd != 0)
        {
            last_cmd = 0;
            Serial.write('*');
            Serial.write(4);
            Serial.write((unsigned byte) 0);
        }
    } else if (z) //out
    {
        if (last_cmd != 'z')
        {
            last_cmd = 'z';
            Serial.write('*');
            Serial.write(4);
            Serial.write(GRIP_OUT_SPEED);
        }
    } else if (c) //in
    {
        if (last_cmd != 'c')
        {
            last_cmd = 'c';
            Serial.write('*');
            Serial.write(4);
            Serial.write(GRIP_IN_SPEED);
        }
    }
}

