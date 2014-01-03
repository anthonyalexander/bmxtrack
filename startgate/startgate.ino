
// Button to pin 2
//Speaker to pin 6
//relay pin 5


// http://arduino.cc/en/Tutorial/Tone

//http://arduino.cc/en/Tutorial/BlinkWithoutDelay

// http://www.arduino.cc/en/Tutorial/Button

// http://forum.arduino.cc/index.php?topic=89649.0

// constants won't change. They're used here to 
// set pin numbers:

#include "pitches.h"
long randOn = 0;
// notes in the melody:
int melody[] = {
  NOTE_G3, NOTE_D3, NOTE_C3, NOTE_D3,0, NOTE_E3, NOTE_D3, NOTE_C3,0, NOTE_C3, NOTE_C3, NOTE_E3, NOTE_E3,0 , NOTE_C3, NOTE_C3, NOTE_E3 };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  5, 6, 8, 6 ,3 ,8 ,8 ,3 ,1 ,7 ,5 ,8 ,8 ,2 ,5 ,8 ,8};

const int buttonPin = 2;     // the number of the pushbutton pin Pull Down resistor
int relayPin = 3;   //Relay Pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

long stopwatchStart = 0L;
long stopwatchMax = (31L * 1000L);
int laserPin = 12;
boolean raceStarted = false;
long lastInterval = 0;

enum states 
{
  WAITING,
  STARTING,
  RACING
};

states currentState = WAITING;

void setup() {
  Serial.begin(9600); 

  randomSeed (analogRead (0));

  pinMode(buttonPin, INPUT);
  pinMode(relayPin, OUTPUT); 
}

void loop() {

  switch (currentState)
  {
  case WAITING:

    handleWaitingState();
    break;

  case STARTING:

    handleStartingState();
    break;

  case RACING:

    handleRacingState();
    break;

  default:

    currentState = WAITING;
    break;
  }
}

void handleWaitingState() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {

    currentState = STARTING;

  }
}

void handleStartingState() {

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 17; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(4, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(4);

  } 

  randOn = random (100, 2700);
  delay(randOn); 

  // play a note on pin 6 for 60 ms:
  tone(4, 632);
  delay(60);

  // turn off tone function for pin 6:
  noTone(4);
  delay(60);

  tone(4, 632);
  delay(60);

  // turn off tone function for pin 6:
  noTone(4);
  delay(60);

  tone(4, 632);
  delay(60);

  // turn off tone function for pin 6:
  noTone(4);
  delay(60);

  digitalWrite(relayPin, HIGH);
  //AA: start stopwatch here for keypad-entered number of seconds
  stopwatchStart = millis();

  tone(4, 632);
  delay(2250);

  digitalWrite(relayPin, LOW); //reset the relay

  // turn off tone function for pin 6:
  noTone(4);

  currentState = RACING;
}

void handleRacingState() {

  long interval = millis() - stopwatchStart;
  boolean laserTripped = false;//analogRead(laserPin);

  if (laserTripped) {

    //display interval as the final time

    stopwatchStart = 0; //reset the stopwatch

  } 
  else if (interval > stopwatchMax) {

    //move the servo 180 degrees
    stopwatchStart = 0; //reset the stopwatch

  } 
  else {

    if ((interval / 100L) > (lastInterval / 100L)) {

      Serial.print("stopwatch ");
      Serial.println((interval / 100L) / 10.0);
      lastInterval = interval;

    }

  }

  if (stopwatchStart == 0) {

    lastInterval = 0;
    currentState = WAITING;

  }

}





