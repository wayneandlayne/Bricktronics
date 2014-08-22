#include <Wire.h>
#include <Bricktronics2.h> // These three lines let the Arduino sketch use the Bricktronics library code that simplifies working with motors and sensors.
#include <Motor.h>
#include <Button.h>

// Make: LEGO and Arduino Projects
// Chapter 5: Chocolate Milk Maker
// Website: http://www.wayneandlayne.com/bricktronics/

#define PUMP_PIN 13
#define STIR_PIN 11

#define SYRUP_WAIT 30000 // how long, in milliseconds, to wait while dispensing syrup.
#define PUMP_TIME 22000 // how long, in milliseconds, to wait for the pump to pump milk.
#define STIR_TIME 20000 // how long, in milliseconds, to stir.

Motor syrupArm = Motor(Bricktronics::BS_MOTOR_1); // The syrup_arm PIDMotor object corresponds to the stir arm motor plugged into Motor Port 1.
Motor stirArm = Motor(Bricktronics::BS_MOTOR_2); // The stir_arm PIDMotor object corresponds to the syrup arm motor plugged into Motor Port 2.
Button startStop = Button(Bricktronics::BS_SENSOR_1); // The startstop Button object corresponds to the start/stop button in Sensor Port 1.
Button endStop = Button(Bricktronics::BS_SENSOR_2); // The endstop Button object corresponds to the endstop button in Sensor Port 2.

void setup() // The setup() function is called only once right when the Arduino is powered on. It sets up the serial port, prints a startup message, and then initializes the hardware and the software.
{
    Serial.begin(115200);
    Serial.println("starting!");
    Bricktronics::begin(); // This line is only needed if you are using a Bricktronics Shield
    syrupArm.begin();
    stirArm.begin();
    startStop.begin();
    endStop.begin();
    setupLoads();
}

void setupLoads() // Initializes the pins used to start the pump and the stirring Power Functions motor.
{
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW);

    pinMode(STIR_PIN, OUTPUT);
    digitalWrite(STIR_PIN, LOW);
}

void startPump() // A simple function that starts the pump.
{
    digitalWrite(PUMP_PIN, HIGH);
}

void stopPump() // Stops the pump.
{
    digitalWrite(PUMP_PIN, LOW);
}

void startStir() // Starts the stirring motor at full speed.
{
    digitalWrite(STIR_PIN, HIGH);
}

void startStir(uint8_t theSpeed) // This is a variant of the startStir() function that takes a parameter that determines how fast to spin the stirrer.
{
    analogWrite(STIR_PIN, theSpeed);
}

void stopStir() // Stops the stirrer.
{
    digitalWrite(STIR_PIN, LOW);
}

void loop() // After setup() finishes, loop() runs over and over.
{
    Serial.println("Starting loop!");

    waitForStartPressAndRelease();

    if (endStop.isPressed())
    {
        Serial.println("Error. Endstop is already pressed at start of run.");
        return;
    }

    pumpMilk();
    dispenseSyrup();
    dropStirArm();
    stirChocolateMilk();
    raiseStirArm();
}

void waitForStartPressAndRelease() // waitForStartPressAndRelease() just waits until the start button is pressed and then released. After that, it returns.
{
    Serial.println("Waiting for start press.");
    while (startStop.isReleased()) {
        // wait for start to be pressed
    };
    delay(50); // debounce
    while (startStop.isPressed()) {
        // wait for start to be released
    };
    delay(50); // debounce
    Serial.println("Start button released!");
}

void pumpMilk() // starts the pump, and waits for PUMP_TIME milliseconds to stop pumping. While it waits, it watches the start/stop button for a press, indicating emergency stop!
{
    Serial.println("Starting pump.");
    startPump();

    unsigned long endTime = millis() + PUMP_TIME;
    while (millis() < endTime)
    {
        if (startStop.isPressed())
        {
            Serial.println("Pump stopped due to button press.");
            break;
        }
        delay(50);
    }
    stopPump();
}

void dispenseSyrup(void) // When dispenseSyrup() starts, the system doesn’t know exactly where the syrup arm is, just that it’s vaguely upright. This function advances the arm until it hits the endstop, then it marks that as position 0. Then it pushes into the stop, repeatedly, to dispense syrup. It stops after SYRUP_WAIT milliseconds, at which point it slowly moves back to the upright position.
{
    Serial.println("Advancing syrup arm until endstop.");
    syrupArm.rawSetSpeed(255);
    while (endStop.isReleased()) { // Wait here until the endstop is pressed.
    };
    syrupArm.setPosition(0); // This sets the current position of the motor to 0.
    syrupArm.stop();
    Serial.println("Endstop pressed!");
    Serial.println("Waiting and dispensing syrup.");

    for (uint8_t i = 0; i < 40; i++) // At this point, the syrup container is pointed into the cup. To help the syrup drip out, we push the syrup container and then release it, 40 times. The length of each push is calculated so we are dripping for a total of SYRUP_WAIT milliseconds.
    {
        syrupArm.goToPosition(-100);
        syrupArm.delayUpdateMS(SYRUP_WAIT/80); // This is a shortcut for a common idiom with the Motor object. For example, m.delayUpdateMS(1000); periodically calls m.update() for 1000 milliseconds before returning.
        syrupArm.goToPosition(0);
        syrupArm.delayUpdateMS(SYRUP_WAIT/80);
    }

    Serial.println("Retreating syrup arm!"); // This multistep movement of the arm is supposed to start slowly, then finish, to help prevent the syrup from flying out the end when the arm is pulled back.
    syrupArm.goToPosition(20);
    syrupArm.delayUpdateMS(100);
    syrupArm.goToPosition(50);
    syrupArm.delayUpdateMS(100);
    syrupArm.goToPosition(225);
    syrupArm.delayUpdateMS(1000);
    syrupArm.goToPosition(350);
    syrupArm.delayUpdateMS(1000);
    syrupArm.stop();
}

void dropStirArm() // slowly drops the stir arm until it presses the endstop. Then it marks that position at zero.
{
    Serial.println("Advancing stir arm until endstop.");
    stirArm.rawSetSpeed(-100);

    while (endStop.isReleased()) {
        // do nothing
    };

    stirArm.setPosition(0);
    stirArm.stop();
    Serial.println("Endstop pressed!");
}

void stirChocolateMilk() // stirChocolateMilk() is very similar to pumpMilk(). It starts to stir, then waits for STIR_TIME milliseconds before stopping the stirrer. While waiting, it watches for start/stop to be pressed, which would indicate an emergency stop.
{
    Serial.println("Starting to stir");
    startStir(255);
    unsigned long endTime = millis() + STIR_TIME;
    while (millis() < endTime)
    {
        if (startStop.isPressed())
        {
            Serial.println("Stir stopped due to button press.");
            break;
        }
        delay(50);
    }
    stopStir();
}

void raiseStirArm() // raiseStirArm() tries to prevent a mess. It does this by slowly raising the stir arm a little bit, so the spoon is mostly out of the milk, but it’s still over the cup. It then slowly rotates the spoon, trying to get all the milk off. Then it raises the arm all the way.
{
    Serial.println("Retreating stir arm!");

    stirArm.goToPosition(-35);
    stirArm.delayUpdateMS(1000);
    stirArm.goToPosition(-60);
    stirArm.delayUpdateMS(2000);
    stirArm.goToPosition(-100);
    startStir(85);
    stirArm.delayUpdateMS(2000);
    stopStir();

    stirArm.goToPosition(-110);
    stirArm.delayUpdateMS(2000);
    stirArm.goToPosition(-250);
    stirArm.delayUpdateMS(1000);
    stirArm.stop();
}

