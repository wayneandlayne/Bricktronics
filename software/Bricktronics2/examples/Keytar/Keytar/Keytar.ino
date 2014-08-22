#include <Wire.h>
#include <Bricktronics.h>
#include <Motor.h>
#include <Button.h>

// Make: LEGO and Arduino Projects
// Chapter 8: Keytar
// Website: http://www.wayneandlayne.com/bricktronics/

Motor toneKnob = Motor(Bricktronics::BS_MOTOR_1);
Motor durationKnob = Motor(Bricktronics::BS_MOTOR_2);
Button muteButton = Button(Bricktronics::BS_SENSOR_1);

#define SPEAKER       11 // These defines set up labels for the pins used in this sketch.
#define TONE_SLOT_0   6
#define TONE_SLOT_1   7
#define TONE_SLOT_2   A1
#define TONE_SLOT_3   A2
#define TONE_SLOT_4   A3

#define NUM_OF_SLOTS 5

char slotButton[] = { // slot_button is an array. Conceptually, each button corresponds to a slot. Each slot has a frequency. This array maps slot numbers to the pin that corresponds to that slot’s button.
  TONE_SLOT_0,
  TONE_SLOT_1,
  TONE_SLOT_2,
  TONE_SLOT_3,
  TONE_SLOT_4
};

int slotFrequency[] = {300, 400, 500, 600, 700}; // an array of ints that contain the frequencies for each slot. It starts with the initial frequencies for each slot.
int noteDuration = 500; // holds the duration of the note in milliseconds. As you play, you’ll change this from the initial value.
int restDuration = 100; // holds the duration in milliseconds of the rest between tones. As you play, you’ll change this from the initial value.
int currentSlot = 0; // a global variable that holds the current slot. As the notes progress, this will increase until it reaches the end of the slots, at which point it will wrap around back to zero.


#define MIN_FREQUENCY       25 // a define that sets the minimum frequency for any slot.
#define MIN_NOTE_DURATION   25

void setup() // The setup() function is called only once—right after the Arduino is powered on.
{
    Serial.begin(115200); // We use the USB serial port for debugging our Arduino code. It allows us to send information from the Arduino to the computer.
    Serial.println("starting!");
    Bricktronics::begin();
    toneKnob.begin();
    durationKnob.begin();
    muteButton.begin();

    for (uint8_t i = 0; i < NUM_OF_SLOTS; i++) // This for loop initializes each of the slot buttons as an input, with internal pullups turned on.
    {
        pinMode (slotButton[i], INPUT_PULLUP);
    }

    while (muteButton.isReleased())
    {
        //do nothing
    } // This while loop stops the program from proceeding at startup until the mute button is pressed.
}


void loop() // The loop() function runs over and over until you remove power from the Arduino.
{
    Serial.println(currentSlot);
    int currentFrequency = slotFrequency[currentSlot]; // currentFrequency is set to the integer stored in the currentSlot index of the array slotFrequency.

    if (muteButton.isReleased())
    {
        tone(SPEAKER, currentFrequency); // This starts a tone on pin SPEAKER with frequency currentFrequency.
    }

    watchForInput(noteDuration, currentSlot, false, true); // This calls the function that waits for noteDuration milliseconds and responds to any input during that time.
    noTone(SPEAKER);
    watchForInput(restDuration, currentSlot, false, false); // The details of this function will be presented described shortly; it waits for restDuration milliseconds and responds to any input during that time.

    currentSlot += 1; // The end of this function sets currentSlot to the next slot, in preparation for the next run of loop().

    if (currentSlot == NUM_OF_SLOTS)
    {
        currentSlot = 0;
    }
}

/*
   watch_for_input responds to input for a specific amount of time. It has four input parameters:
     ms             How long to watch for input in milliseconds.
     slot           The current slot that any input will modify.
     in_slot        A boolean that indicates if a slot fret button is pressed.
     is_playing     This indicates whether the note in that slot is actually being played.
*/
void watchForInput(long ms, char slot, boolean inSlot, boolean isPlaying)
{
    long startTime = millis();
    long endTime = startTime + ms;
    toneKnob.setPosition(0); // By setting the motor's current position to 0, it is easy to see later if the motors have moved. If a later call to read doesn’t return 0, they’ve moved!
    durationKnob.setPosition(0);
    Serial.print("In slot: ");
    Serial.println(inSlot, DEC);
    while (millis() < endTime) // This while loop keeps checking for input until the current time is the endTime.
    {
        if (!inSlot) // If no fret buttons are pressed, we check every fret button to see if it is newly pushed.
        {
            for (uint8_t i = 0; i < NUM_OF_SLOTS; i++)
            {
                if (digitalRead(slotButton[i]) == LOW)
                {
                    Serial.print("Button pressed: ");
                    Serial.println(i, DEC);
                    handleButtonPress(i);
                }
            }
        }

        long durationChange = durationKnob.getPosition();
        if (durationChange != 0)
        {
            Serial.print("duration knob: ");
            Serial.println(durationChange, DEC);
            if (inSlot) // If the duration knob is turned, and a fret button is pushed, change the note time. Otherwise, change the rest time.
            {
                noteDuration += durationChange;
                noteDuration = max(noteDuration, MIN_NOTE_DURATION); // This limits the note duration to a minimum of MIN_NOTE_DURATION.
            }
            else
            {
                restDuration += durationChange;
                restDuration = max(restDuration, 0);
            }
            endTime = millis(); // This statement makes the function exit at the end of this iteration of the while loop. If the knobs are spinning, this means they’ll likely still be spinning a bit at the next slot, which makes an awesome retro arcadestyle chunky sound.
            durationKnob.setPosition(0); // Now that we’ve handled the duration knob, we set it to 0.
        }
        
        long toneChange = toneKnob.getPosition();
        if (toneChange != 0)
        {
            Serial.print("Tone knob: ");
            Serial.println(toneChange, DEC);

            slotFrequency[slot] += toneChange;
            slotFrequency[slot] = max(slotFrequency[slot], MIN_FREQUENCY);
            if (isPlaying && muteButton.isReleased())
            {
                tone(SPEAKER, slotFrequency[slot]);
            }
            toneKnob.setPosition(0);
        }
    }
}

void handleButtonPress(char slot) // handleButtonPress handles fret button presses. It contains the main logic for "fret button mode."
{
    Serial.print("Handling button press for slot: ");
    Serial.println(slot);
    noTone(SPEAKER);

    while (digitalRead(slotButton[slot]) == LOW)
    {
        tone(SPEAKER, slotFrequency[slot]);
        watchForInput(noteDuration, slot, true, true);
        noTone(SPEAKER);
        watchForInput(restDuration, slot, true, false);
    }
}

