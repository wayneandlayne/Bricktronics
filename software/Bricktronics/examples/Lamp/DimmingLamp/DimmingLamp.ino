#include <Wire.h>
#include <Bricktronics.h>
#include <SoftwareSerial.h>
#include <TimerOne.h> // TimerOne is an Arduino library, and can be downloaded at http://www.arduino.cc/playground/Code/Timer1

// Make: LEGO and Arduino Projects
// Chapter 9: Lamp
// Website: http://www.wayneandlayne.com/bricktronics/

// Based on BluetoothShield Demo Code PDE by Steve Chang at Seeed Technology, Inc.
// and
// AC Light Control by Ryan McLaughlin.

Bricktronics brick = Bricktronics();
PIDMotor m = PIDMotor(&brick, 1); // Plug a motor into Motor Port 1.
Button lamp_switch = Button(&brick, 1); // Plug a button into Sensor Port 1.

#define RxD 6 // Connect the jumpers on the Bluetooth Shield so RX is connected to pin 6.
#define TxD 7 // Connect the jumpers on the Bluetooth Shield so TX is connected to pin 7.
#define AC_pin 9 // Connect the PowerSSR Tail input pin to pin 9 on the Arduino. While you're at it, make sure you've connected the Zero Cross output pin to pin 3 on the Arduino.

volatile int i = 0;
volatile boolean zero_cross = false;

int dim = 0;
int freqStep = 65; // the delay between dimness checks. 65 works well for 60 Hz power, and 78 works well for 50 Hz power.

long last_encoder_reading = 0;
boolean lamp_lit = false;
long last_released = 0;
int destination_updates;

SoftwareSerial bluetoothSerial(RxD, TxD); // This creates a "software driven serial port" on pins RxD and TxD. It can't go as fast as the built-in Serial port, but it works well for what we need.

void setup() // setup() runs once at startup.
{
    Serial.begin(9600);
    setupBluetoothConnection();
    brick.begin();
    m.begin();
    lamp_switch.begin();

    m.mKP = 1; // This changes the KP, KD, and KI tuning numbers on the PID for the motor object m.
    m.mKD = 1;
    m.mKI = 0.003;

    pinMode(AC_pin, OUTPUT);
    attachInterrupt(1, zero_cross_detect, FALLING); // Every time the signal on pin 3 goes from high to low, run the function zero_cross_detect.
    Timer1.initialize(freqStep); // Initialize the Timer1 library with the period freqStep.
    Timer1.attachInterrupt(dim_check, freqStep); // Every freqStep milliseconds, run the function dim_check.
}


void loop() // loop() runs over and over.
{
    char recvChar;
    if (bluetoothSerial.available()) // Check to see if we have received a character from the Bluetooth module.
    {
        recvChar = bluetoothSerial.read();
        switch (recvChar)
        {
        case 'q': // If the character was "q", it's a query from the Bluetooth module, asking us what our current status is.
            if (lamp_lit)
            {
                bluetoothSerial.print('l');
            } else
            {
                bluetoothSerial.print('m');
            }
            bluetoothSerial.print((char) dim);
            break;
        case 'm': // If the character was "m", the light should be turned off completely.
            lamp_lit = false;
            dim = 128;
            break;
        case 'l': // If the character was "l", the light should be turned on fully.
            lamp_lit = true;
            dim = 0;
            break;
        default: // If the received character hasn't been handled by now, it's either garbage, or a dimness setting.
            if (recvChar >= 0 && recvChar <=99) // If the character has a value between 0 and 99, treat it as a dimness value.
            {
                Serial.println("Setting dimness:");
                dim = recvChar;
                destination_updates = 20; // The next 20 times loop() is run, try to get the motor in the dimmer knob close to the destination.
                m.go_to_pos(dim*4); // Set the motor's destination to the dimness value sent times 0 to 99 on the knob is not very far -- 0 to 396 is much better.
            }
        }
    }

    if (destination_updates) // If destination_updates is greater than zero, then try to get the motor closer to the destination for a little bit, before managing the rest of the logic.
    {
        m.update();
        delay(25);
        destination_updates -= 1;
    } else
    {
        m.stop(); // Make sure the motor stops after it's tried to get to its destination.
    }

    long current_encoder_reading = m.encoder->read();

    if (current_encoder_reading != last_encoder_reading) // If the knob has turned since the last run through loop(), then update
    {
        dim = current_encoder_reading / 4; // This compensates for the increase in range when having the knob remotely set.
        Serial.print("dim: ");
        Serial.println(dim); // Update the Serial console to the PC with some debug information.
        dim = constrain(dim, 0, 100);
        bluetoothSerial.write((byte)dim); // Update the remote Bluetooth device with the new dimness value.
        last_encoder_reading = current_encoder_reading;
    }

    if (! lamp_switch.is_pressed())
    {
        last_released = millis();
    }
    else if (lamp_switch.is_pressed() && last_released > 0
             && last_released - millis() > 50) // If the lamp switch is pressed, and it isn't the first time it was pressed, and the last time it was let go was at least 50 milliseconds ago, this means we want to toggle the light status!
    {
        Serial.println("pressed"); // Update the Arduino Serial Monitor with debug information.
        last_released = 0;
        if (lamp_lit)
        {
            lamp_lit = false;
            dim = 128;
            bluetoothSerial.print("m"); // Update the remote side with the new status that the light is off.
        }
        else {
            lamp_lit = true;
            dim = 0;
            bluetoothSerial.print("l"); // Update the remote side with the new status that the light is on.
        }
    }

    if (Serial.available()) // If the user sends data to the Arduino Serial Monitor, it's a debugging command from the user, and we want to forward that onto the remote Bluetooth device. This lets you control the Bluetooth software through the Arduino Serial Monitor.
    {
        recvChar  = Serial.read();
        bluetoothSerial.print(recvChar);
    }
}

void setupBluetoothConnection()
{
    bluetoothSerial.begin(9600); // This is the start of the function that configures the Bluetooth module. Set the software serial link to 9600 baud.
    bluetoothSerial.print("\r\n+STWMOD=0\r\n"); // Tell the Bluetooth module to switch to slave mode.
    bluetoothSerial.print("\r\n+STNA=Bricktronics\r\n"); // Tell the Bluetooth module to change its name to Bricktronics.
    bluetoothSerial.print("\r\n+STOAUT=1\r\n"); // Tell the Bluetooth module to permit paired connections.
    bluetoothSerial.print("\r\n+STAUTO=0\r\n"); // Tell the Bluetooth module to deny automatic connections.
    delay(2000); // Give the Bluetooth module time to change its settings.
    bluetoothSerial.print("\r\n+INQ=1\r\n"); // Tell the Bluetooth module to enable connection requests.
    Serial.println("The Bluetooth module should be connectable!");
    delay(2000); // Give the Bluetooth module time to change its settings.
    bluetoothSerial.flush(); // Make sure all the data is sent, just in case something got stuck in the Software Serial output queue. This is the end of the function that configures the Bluetooth module.
}

void zero_cross_detect() // called when a zero cross is detected, and it sets a global flag, zero_cross, to true.
{
    zero_cross = true;
}

void dim_check()
{
    if (zero_cross && lamp_lit) // If the lamp is supposed to be on, and there’s been a zero-crossing since the last one we handled, then we go on to the next step.
    {
        if (i >= dim) // If we've gotten as far or further into the waveform as we need to get the dimness we want, then we do everything in the next four steps.
        {
            digitalWrite(AC_pin, HIGH); // Turn on the PowerSSR Tail TRIAC.
            delayMicroseconds(5); // Wait to make sure it turns on.
            digitalWrite(AC_pin, LOW); // Stop turning on the TRIAC. It will turn off on its own at the next zero cross.
            i = 0; // Reset i, and the zero_cross boolean.
            zero_cross = false;
        }
        else
        {
            i++; // It isn’t time to turn off yet, so keep track of how many times we’ve checked.
        }
    }
}

