#include <Wire.h>
#include <Bricktronics.h>

// Make: LEGO and Arduino Projects
// Chapter 8: Keytar
// Website: http://www.wayneandlayne.com/bricktronics/

Bricktronics brick = Bricktronics();
Motor tone_knob = Motor(&brick, 1);
Motor duration_knob = Motor(&brick, 2);
Button mute_button = Button(&brick, 1);

#define SPEAKER 11 // These defines set up labels for the pins used in this sketch.
#define TONE_SLOT_0 6
#define TONE_SLOT_1 7
#define TONE_SLOT_2 A1
#define TONE_SLOT_3 A2
#define TONE_SLOT_4 A3

#define NUM_OF_SLOTS 5

char slot_button[] = { // slot_button is an array. Conceptually, each button corresponds to a slot. Each slot has a frequency. This array maps slot numbers to the pin that corresponds to that slot’s button.
    TONE_SLOT_0,
    TONE_SLOT_1,
    TONE_SLOT_2,
    TONE_SLOT_3,
    TONE_SLOT_4
};

int slot_frequency[] = {300, 400, 500, 600,700}; // an array of ints that contain the frequencies for each slot. It starts with the initial frequencies for each slot.
int note_duration = 500; // holds the duration of the note in milliseconds. As you play, you’ll change this from the initial value.
int rest_duration = 100; // holds the duration in milliseconds of the rest between tones. As you play, you’ll change this from the initial value.
int current_slot = 0; // a global variable that holds the current slot. As the notes progress, this will increase until it reaches the end of the slots, at which point it will wrap around back to zero.


#define MIN_FREQUENCY 25 // a define that sets the minimum frequency for any slot.
#define MIN_NOTE_DURATION 25

void setup() // The setup() function is called only once—right after the Arduino is powered on.
{
    Serial.begin(115200); // We use the USB serial port for debugging our Arduino code. It allows us to send information from the Arduino to the computer.
    Serial.println("starting!");
    brick.begin();
    tone_knob.begin();
    duration_knob.begin();
    mute_button.begin();

    for (char i = 0; i < NUM_OF_SLOTS; i++) // This for loop initializes each of the slot buttons as an input, with internal pullups turned on.
    {
        pinMode (slot_button[i], INPUT_PULLUP);
    }

    while (!mute_button.is_pressed()) {
        //do nothing
    } // This while loop stops the program from proceeding at startup until the mute button is pressed.
}


void loop() // The loop() function runs over and over until you remove power from the Arduino.
{
    Serial.println(current_slot);
    int current_frequency = slot_frequency[current_slot]; // current_frequency is set to the integer stored in the current_slot index of the array slot_frequency.

    if (!mute_button.is_pressed())
    {
        tone(SPEAKER, current_frequency); // This starts a tone on pin SPEAKER with frequency current_frequency.
    }

    watch_for_input(note_duration, current_slot, false, true); // This calls the function that waits for note_duration milliseconds and responds to any input during that time.
    noTone(SPEAKER);
    watch_for_input(rest_duration, current_slot, false, false); // The details of this function will be presented described shortly; it waits for rest_duration milliseconds and responds to any input during that time.

    current_slot += 1; // The end of this function sets current_slot to the next slot, in preparation for the next run of loop().

    if (current_slot == NUM_OF_SLOTS)
    {
        current_slot = 0;
    }
}

/*
   watch_for_input responds to input for a specific amount of time. It has four input parameters:
     ms             How long to watch for input in milliseconds.
     slot           The current slot that any input will modify.
     in_slot        A boolean that indicates if a slot fret button is pressed.
     is_playing     This indicates whether the note in that slot is actually being played.
*/
void watch_for_input(long ms, char slot, boolean in_slot, boolean is_playing)
{
    long start_time = millis();
    long end_time = start_time + ms;
    tone_knob.encoder->write(0); // By writing a zero to the encoder object, it is easy to see later if the motors have moved. If a later call to read doesn’t return 0, they’ve moved!
    duration_knob.encoder->write(0);
    Serial.print("In slot: ");
    Serial.println(in_slot, DEC);
    while (millis() < end_time) // This while loop keeps checking for input until the current time is the end_time.
    {
        if (!in_slot) // If no fret buttons are pressed, we check every fret button to see if it is newly pushed.
        {
            for (char i = 0; i < NUM_OF_SLOTS; i++)
            {
                if (digitalRead(slot_button[i]) == LOW)
                {
                    Serial.print("Button pressed: ");
                    Serial.println(i, DEC);
                    handle_button_press(i);
                }
            }
        }

        long duration_change = duration_knob.encoder->read();

        if (duration_change != 0)
        {
            Serial.print("duration knob: ");
            Serial.println(duration_change, DEC);
            if (in_slot) // If the duration knob is turned, and a fret button is pushed, change the note time. Otherwise, change the rest time.
            {
                note_duration += duration_change;
                note_duration = max(note_duration, MIN_NOTE_DURATION); // This limits the note duration to a minimum of MIN_NOTE_DURATION.
            }
            else
            {
                rest_duration += duration_change;
                rest_duration = max(rest_duration, 0);
            }
            end_time = millis(); // This statement makes the function exit at the end of this iteration of the while loop. If the knobs are spinning, this means they’ll likely still be spinning a bit at the next slot, which makes an awesome retro arcadestyle chunky sound.
            duration_knob.encoder->write(0); // Now that we’ve handled the duration knob, we set it to 0.
        }

        if (tone_knob.encoder->read() != 0)
        {
            Serial.print("Tone knob: ");
            Serial.println(tone_knob.encoder->read(), DEC);

            slot_frequency[slot] += (tone_knob.encoder->read());
            slot_frequency[slot] = max(MIN_FREQUENCY, slot_frequency[slot]);
            if (is_playing && !mute_button.is_pressed())
            {
                tone(SPEAKER, slot_frequency[slot]);
            }
            tone_knob.encoder->write(0);
        }
    }
}

void handle_button_press(char slot) // handle_button_press handles fret button presses. It contains the main logic for "fret button mode."
{
    Serial.print("Handling button press for slot: ");
    Serial.println(slot);
    noTone(SPEAKER);

    // TODO should this be brick.digitalRead ?
    while (digitalRead(slot_button[slot]) == LOW)
    {
        tone(SPEAKER, slot_frequency[slot]);
        watch_for_input(note_duration, slot, true, true);
        noTone(SPEAKER);
        watch_for_input(rest_duration, slot, true, false);
    }
}

