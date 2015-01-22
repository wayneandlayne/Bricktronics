#include <Wire.h>
#include <Bricktronics.h> // These two lines let the Arduino sketch use the Bricktronics library code that simplifies working with motors and sensors.

// Make: LEGO and Arduino Projects
// Chapter 5: Chocolate Milk Maker
// Website: http://www.wayneandlayne.com/bricktronics/

#define PUMP 13
#define STIR 11

#define SYRUP_WAIT 30000 // how long, in milliseconds, to wait while dispensing syrup.
#define PUMP_TIME 22000 // how long, in milliseconds, to wait for the pump to pump milk.
#define STIR_TIME 20000 // how long, in milliseconds, to stir.

Bricktronics brick = Bricktronics();
PIDMotor syrup_arm = PIDMotor(&brick, 1); // The syrup_arm PIDMotor object corresponds to the stir arm motor plugged into Motor Port 1.
PIDMotor stir_arm = PIDMotor(&brick, 2); // The stir_arm PIDMotor object corresponds to the syrup arm motor plugged into Motor Port 2.
Button startstop = Button(&brick, 1); // The startstop Button object corresponds to the start/stop button in Sensor Port 1.
Button endstop = Button(&brick, 2); // The endstop Button object corresponds to the endstop button in Sensor Port 2.

void setup() // The setup() function is called only once right when the Arduino is powered on. It sets up the serial port, prints a startup message, and then initializes the hardware and the software.
{
    Serial.begin(115200);
    Serial.println("starting!");
    brick.begin();
    syrup_arm.begin();
    stir_arm.begin();
    startstop.begin();
    endstop.begin();
    setup_loads();
}

void setup_loads() // Initializes the pins used to start the pump and the stirring Power Functions motor.
{
    pinMode(PUMP, OUTPUT);
    digitalWrite(PUMP, LOW);

    pinMode(STIR, OUTPUT);
    digitalWrite(STIR, LOW);
}

void start_pump() // A simple function that starts the pump.
{
    digitalWrite(PUMP, HIGH);
}

void stop_pump() // Stops the pump.
{
    digitalWrite(PUMP, LOW);
}

void start_stir() // Starts the stirring motor at full speed.
{
    digitalWrite(STIR, HIGH);
}

void start_stir(int speed) // This is a variant of the start_stir() function that takes a parameter that determines how fast to spin the stirrer.
{
    analogWrite(STIR, speed);
}

void stop_stir() // Stops the stirrer.
{
    digitalWrite(STIR, LOW);
}

void loop() // After setup() finishes, loop() runs over and over.
{
    Serial.println("Starting loop!");

    wait_for_start_press_and_release();

    if (endstop.is_pressed())
    {
        Serial.println("Error. Endstop is already pressed at start of run.");
        return;
    }

    pump_milk();
    dispense_syrup();
    drop_stir_arm();
    stir_chocolate_milk();
    raise_stir_arm();
}

void wait_for_start_press_and_release() // wait_for_start_press_and_release() just waits until the start button is pressed and then released. After that, it returns.
{
    Serial.println("Waiting for start press.");
    while (!startstop.is_pressed()) {
        //wait for start to be pressed
    };
    delay(50); //debounce
    while (startstop.is_pressed()) {
        //wait for start to be released
    };
    delay(50); //debounce
    Serial.println("Start button released!");
}

void pump_milk() // starts the pump, and waits for PUMP_TIME milliseconds to stop pumping. While it waits, it watches the start/stop button for a press, indicating emergency stop!
{
    Serial.println("Starting pump.");
    start_pump();

    unsigned long end_time = millis() + PUMP_TIME;
    while (millis() < end_time) {
        if (startstop.is_pressed())
        {
            Serial.println("Pump stopped due to button press.");
            break;
        }
        delay(50);
    }
    stop_pump();
}

void dispense_syrup() // When dispense_syrup() starts, the system doesn’t know exactly where the syrup arm is, just that it’s vaguely upright. This function advances the arm until it hits the endstop, then it marks that as position 0. Then it pushes into the stop, repeatedly, to dispense syrup. It stops after SYRUP_WAIT milliseconds, at which point it slowly moves back to the upright position.
{
    Serial.println("Advancing syrup arm until endstop.");
    syrup_arm.set_speed(255);
    while (!endstop.is_pressed()) { // Wait here until the endstop is pressed.
    };
    syrup_arm.encoder->write(0); // This sets the current position of the motor to 0.
    syrup_arm.stop();
    Serial.println("Endstop pressed!");
    Serial.println("Waiting and dispensing syrup.");

    for (int i = 0; i < 40; i++) // At this point, the syrup container is pointed into the cup. To help the syrup drip out, we push the syrup container and then release it, 40 times. The length of each push is calculated so we are dripping for a total of SYRUP_WAIT milliseconds.
    {
        syrup_arm.go_to_pos(-100);
        Bricktronics::delay_update(SYRUP_WAIT/80, &syrup_arm, NULL); // This is a shortcut for a common idiom with the PIDMotor object. For example, Bricktronics::delay_update(1000, &motor); runs motor.update() for 1000 milliseconds before returning.
        syrup_arm.go_to_pos(0);
        Bricktronics::delay_update(SYRUP_WAIT/80, &syrup_arm, NULL);
    }

    Serial.println("Retreating syrup arm!"); // This multistep movement of the arm is supposed to start slowly, then finish, to help prevent the syrup from flying out the end when the arm is pulled back.
    syrup_arm.go_to_pos(20);
    Bricktronics::delay_update(100, &syrup_arm, NULL);
    syrup_arm.go_to_pos(50);
    Bricktronics::delay_update(100, &syrup_arm, NULL);
    syrup_arm.go_to_pos(225);
    Bricktronics::delay_update(1000, &syrup_arm, NULL);
    syrup_arm.go_to_pos(350);
    Bricktronics::delay_update(1000, &syrup_arm, NULL);
    syrup_arm.stop();
}

void drop_stir_arm() // slowly drops the stir arm until it presses the endstop. Then it marks that position at zero.
{
    Serial.println("Advancing stir arm until endstop.");
    stir_arm.set_speed(-100);

    while (!endstop.is_pressed()) {
        // do nothing
    };

    stir_arm.encoder->write(0);
    stir_arm.stop();
    Serial.println("Endstop pressed!");
}

void stir_chocolate_milk() // stir_chocolate_milk() is very similar to pump_milk(). It starts to stir, then waits for STIR_TIME milliseconds before stopping the stirrer. While waiting, it watches for start/stop to be pressed, which would indicate an emergency stop.
{
    Serial.println("Starting to stir");
    start_stir(255);
    unsigned long end_time = millis() + STIR_TIME;
    while (millis() < end_time) {
        if (startstop.is_pressed())
        {
            Serial.println("Stir stopped due to button press.");
            break;
        }
        delay(50);
    }
    stop_stir();
}

void raise_stir_arm() // raise_stir_arm() tries to prevent a mess. It does this by slowly raising the stir arm a little bit, so the spoon is mostly out of the milk, but it’s still over the cup. It then slowly rotates the spoon, trying to get all the milk off. Then it raises the arm all the way.
{
    Serial.println("Retreating stir arm!");

    stir_arm.go_to_pos(-35);
    Bricktronics::delay_update(1000, &stir_arm, NULL);
    stir_arm.go_to_pos(-60);
    Bricktronics::delay_update(2000, &stir_arm, NULL);
    stir_arm.go_to_pos(-100);
    start_stir(85);
    Bricktronics::delay_update(2000, &stir_arm, NULL);
    stop_stir();

    stir_arm.go_to_pos(-110);
    Bricktronics::delay_update(2000, &stir_arm, NULL);
    stir_arm.go_to_pos(-250);
    Bricktronics::delay_update(1000, &stir_arm, NULL);
    stir_arm.stop();
}

