
#define NUM_BARS 2
#define NUM_NOTES 8

unsigned long timer;
int notes[NUM_BARS][NUM_NOTES] = {
  {0, 0, 0, 0, 0, 0, 0, 0},    /*  initializers for row indexed by 0 */
  {0, 0, 0, 0, 0, 0, 0, 0}     /*  initializers for row indexed by 1 */
};


unsigned int noteDuration [NUM_BARS][NUM_NOTES] = {
  {180, 180, 180, 180, 180, 180, 180, 180},    /*  initializers for row indexed by 0 */
  {180, 180, 180, 180, 180, 180, 180, 180}     /*  initializers for row indexed by 1 */
};
unsigned int noteTimer [NUM_BARS][NUM_NOTES] = {
  {0, 0, 0, 0, 0, 0, 0, 0},    /*  initializers for row indexed by 0 */
  {0, 0, 0, 0, 0, 0, 0, 0}     /*  initializers for row indexed by 1 */
};
unsigned int vibrationStatus [NUM_BARS][NUM_NOTES] = {
  {0, 0, 0, 0, 0, 0, 0, 0},    /*  initializers for row indexed by 0 */
  {0, 0, 0, 0, 0, 0, 0, 0}     /*  initializers for row indexed by 1 */
};

int LEDpins[8];
int butonPins[8] = {10, 19, 24, 29, 34, 39, 47, 40};
bool LEDstates[8] = {false};
bool Bstates[8] = {false};
bool prevBstates[8] = {false};

int currNote;
int currBar;
int bpm = 60;
float period;
int floatnumber = 60.0f;
unsigned long countUp = 0;
bool newStep = false;
bool showDebug = true;

int modebuttonpin = 2;
bool modebuttontate = false;
int prevmodebuttonstate = false;
int counter2 = 0;

int barbuttonPin = 3;
bool barbuttonstate = false;
int prevbarbuttonstate = false;
int currEditBar = 0;

int musicPin = 4;
bool musicstate = false;
int prevmusicstate = false;
int music = 0;

bool butonoroff[8] = {false};
bool butonoroff1[8] = {false};

int done = 0;
int done1 = 0;
int done2 = 0;

#define NUM_INSTRUMENT 5
#define NUM_VALUES 11
int vibrationData[NUM_INSTRUMENT][NUM_VALUES]  = {
  {217, 150, 115, 115, 114, 116, 119, 119, 0, 0, 0},
  {120, 150, 255, 255, 110, 80, 100, 255, 60, 0, 0},
  {80, 100, 120, 180, 255, 255, 100, 80, 0, 0, 0},
  {255, 230, 200, 180, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 140, 255, 180, 120, 80 , 0, 0}
};

unsigned long currentMillis1[8] = {0};  //some global variables available anywhere in the program
unsigned long startMillis1[8] = {0};  //some global variables available anywhere in the program
int count[8] = {0};
int index[8] = {0};
int motorPins[8] = {12, 17, 23, 28, 33, 38, 43, 14};
int speedperiod[NUM_BARS][NUM_NOTES] = {
  {40, 40, 40, 40, 40, 40, 40, 40},    /*  initializers for row indexed by 0 */
  {40, 40, 40, 40, 40, 40, 40, 40}     /*  initializers for row indexed by 1 */
};
int startMillis[NUM_BARS][NUM_NOTES] = {
  {0, 0, 0, 0, 0, 0, 0, 0},    /*  initializers for row indexed by 0 */
  {0, 0, 0, 0, 0, 0, 0, 0}     /*  initializers for row indexed by 1 */
};

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        15
#define PIN1        7
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS1 3 // Popular NeoPixel ring size
#define NUMPIXELS 8 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels1(NUMPIXELS1, PIN1, NEO_GRB + NEO_KHZ800);



void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels1.begin();

  // calculate the period based on Beats Per Minute (BPM)
  period = (60.0f / bpm) * 1000;
  Serial.print("period = ");
  Serial.println(period);

  // reset the timer
  timer = millis();


  //Button input , LED ,
  for (int i = 0; i < 8; i++) {
    pinMode(butonPins[i], INPUT_PULLUP);
    pinMode(LEDpins[i], OUTPUT);
    pinMode(motorPins[i], OUTPUT);
  }
  //mode buttoninput
  pinMode(modebuttonpin, INPUT_PULLUP);

  //Bar pin input
  pinMode(barbuttonPin, INPUT_PULLUP);

  //Choosing music instruments
  pinMode(musicPin, INPUT_PULLUP);
}

void loop() {
  prevmodebuttonstate = modebuttontate;
  modebuttontate = digitalRead(modebuttonpin);
  if (prevmodebuttonstate == HIGH && modebuttontate == LOW) {
    counter2++;   //Adds 1 to the countUp int on every loop
  }
  else {}
  // menu counter
  if (counter2 > 1) {
    counter2 = 0;

  }
  // write modeeeeeeeeeeeeeeee
  if ( counter2 == 0 )  {
    pixels1.setPixelColor(0, pixels1.Color(235, 50, 50));
    pixels1.show();   // Send the updated pixel colors to the hardware.
    barbutton();
    buttoninput();
    instrumentChoice();
  }
  // Play mode
  if ( counter2 == 1 )  {
    pixels1.setPixelColor(0, pixels1.Color(50, 205, 50 ));
    pixels1.show();   // Send the updated pixel colors to the hardware.
    updateBPM();
    updateTimeStep();
    updateNotes();
  }
}

void updateBPM()
{
  int val = analogRead(A0);
  // Serial.println (val);
  val = map(val, 0, 1023, 60, 240);
  bpm = val;

  // calculate the period based on Beats Per Minute (BPM)
  period = (60.0f / bpm) * 1000;
  Serial.print("period = ");
  Serial.println(period);
}

void updateTimeStep()
{

  if (millis() - timer >= period)  //test whether the period has elapsed
  {
    timer = millis();

    //add counter to loop
    newStep = true;

    countUp++;

    if (showDebug) {
      Serial.print("steps: ");
      Serial.print(countUp);
      Serial.print(" bar: ");
      Serial.print(currBar);
      Serial.print(" note: ");
      Serial.println(currNote);
    }

  }

}

void updateNotes()
{

  if (newStep == true) {
    currNote++;
    if (currNote >= NUM_NOTES)
    {
      currNote = 0;
      currBar++;

      if (currBar >= NUM_BARS)
      {
        currBar = 0;
      }

    }
    newStep = false;
    lightNote();
    playVibration();
    countLight();


  }

  updateVibration();
}


void countLight()

{
  if (notes[currBar][currNote] ==  0) {
    pixels.setPixelColor(currNote, pixels.Color(20, 20, 20 ));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }
}

void lightNote()
{
  if (notes[currBar][currNote] > 0) {
    if (done2 == 0) {
    { for (int i = 0; i < 8; i++) {
          if (LEDpins[i], HIGH) {
            digitalWrite(LEDpins[i], LOW);
            pixels.clear();
          }
        }
      }           done2++;
    }    digitalWrite(LEDpins[currNote], HIGH);
    if (currBar == 0) {
      pixels.setPixelColor(currNote, pixels.Color(0, 150, 0 ));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    if (currBar == 1) {
      pixels.setPixelColor(currNote, pixels.Color(150, 0, 0 ));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    done2 = 0;
  }
  else {
    for (int i = 0; i < 8; i++) {
      if (LEDpins[i], HIGH) {
        digitalWrite(LEDpins[currNote], LOW);
        pixels.clear();

      }
    }

  }

}

void playVibration()
{
  if (notes[currBar][currNote] > 0) {
    vibrationStatus[currBar][currNote] = 1;
    noteTimer[currBar][currNote] = millis();
  }

}

void updateVibration()
{
  for (int i = 0; i < NUM_NOTES; i++)
  {
    unsigned int timer = millis() - noteTimer[currBar][i];
    if ((vibrationStatus[currBar][i] == 1) && (timer >= noteDuration[currBar][i]))
    {
      vibrationStatus[currBar][i] = 0;
    } else if (vibrationStatus[currBar][i] == 1) {
      unsigned int period = noteDuration[currBar][i] / NUM_VALUES;

      unsigned int index = timer / period;


      digitalWrite(motorPins[i], vibrationData[NUM_INSTRUMENT][index]);
    } else {
      digitalWrite(motorPins[i], 0);
    }
  }
}


void barbutton()

// cycle thru bars
{
  // Serial.print("BAR ");
  //        Serial.print(currEditBar);
  //        Serial.print(" NOTE: ");
  //        Serial.print( notes[currEditBar][0]);
  //        Serial.print( notes[currEditBar][1]);
  //        Serial.print( notes[currEditBar][2]);
  //        Serial.print( notes[currEditBar][3]);
  //        Serial.print( notes[currEditBar][4]);
  //        Serial.print( notes[currEditBar][5]);
  //        Serial.print( notes[currEditBar][6]);
  //        Serial.print( notes[currEditBar][7]);
  // Serial.println(" ");

  prevbarbuttonstate = barbuttonstate;
  barbuttonstate = digitalRead(barbuttonPin);
  if (prevbarbuttonstate == HIGH && barbuttonstate == LOW) {
    currEditBar++;   //Adds 1 to the currEditBar int on every loop

  }
  else {}
  if (currEditBar > 1) {
    currEditBar = 0;
  }
  if ( currEditBar == 0 )
  { notes[NUM_BARS][NUM_NOTES] = notes[0][NUM_NOTES];
    pixels1.setPixelColor(1, pixels1.Color(150, 150, 150 ));
    pixels1.show();   // Send the updated pixel colors to the hardware.

  }
  if ( currEditBar == 1 )
  { notes[NUM_BARS][NUM_NOTES] = notes[1][NUM_NOTES];
    pixels1.setPixelColor(1, pixels1.Color(150, 150, 0 ));

  }
}


void buttoninput()


{
  for (int i = 0; i < 8; i++) {

    if (currEditBar == 0) {
      if (done == 0) {
        for (int i = 0; i < 8; i++) {
          if (LEDpins[i], HIGH) {
            digitalWrite(LEDpins[i], LOW);
            pixels.setPixelColor(i, pixels.Color(0 , 0, 0 ));
            pixels.show();
          }
        }
        done++;
      }
      done1 = 0;


      if (notes[0][i] == 1) {
        digitalWrite(LEDpins[i], HIGH);
      }
      for (int i = 0; i < 8; i++) {
        if (butonoroff[i] == HIGH) {
          pixels.setPixelColor(i, pixels.Color(150, 150, 150 ));
          pixels.show();   // Send the updated pixel colors to the hardware.
        }
        else {
          pixels.setPixelColor(i, pixels.Color(0 , 0, 0 ));
          pixels.show();   // Send the updated pixel colors to the hardware.
        }
      }
      prevBstates[i] = Bstates[i];
      Bstates[i] = digitalRead(butonPins[i]);
      if (prevBstates[i] == HIGH && Bstates[i] == LOW) {
        if (butonoroff) {
          LEDstates[i] = !LEDstates[i];
          digitalWrite(LEDpins[i], LEDstates[i]);
          butonoroff[i] = ! butonoroff[i];
        }
      }


      if (butonoroff[i] == true )
      {

        notes[0][i] = 1;

      }
      if (butonoroff[i] == false && notes[0][i] == 1 )
      {
        notes[0][i] = 0;
      }

    }

    if ( currEditBar == 1 ) {

      if (done1 == 0) {
        for (int i = 0; i < 8; i++) {
          if (LEDpins[i], HIGH) {
            digitalWrite(LEDpins[i], LOW);
            pixels.setPixelColor(i, pixels.Color(0 , 0, 0 ));
            pixels.show();
          }
        }           done1++;
      }
      done = 0;

      if (notes[1][i] == 1) {
        digitalWrite(LEDpins[i], HIGH);
      }
      for (int i = 0; i < 8; i++) {
        if (butonoroff1[i] == HIGH) {
          pixels.setPixelColor(i, pixels.Color(150, 150, 0 ));
          pixels.show();   // Send the updated pixel colors to the hardware.
        }
        else {
          pixels.setPixelColor(i, pixels.Color(0 , 0, 0 ));
          pixels.show();   // Send the updated pixel colors to the hardware.
        }
      }
      prevBstates[i] = Bstates[i];
      Bstates[i] = digitalRead(butonPins[i]);
      if (prevBstates[i] == HIGH && Bstates[i] == LOW) {
        if (butonoroff) {
          LEDstates[i] = !LEDstates[i];
          digitalWrite(LEDpins[i], LEDstates[i]);
          butonoroff1[i] = ! butonoroff1[i];
        }
      }
      if (butonoroff1[i] == true )
      { //Serial.println("1 HIGH");
        notes[1][i] = 1;
      }
      if (butonoroff1[i] == false && notes[1][i] == 1  )
      { notes[1][i] = 0;
      }
    }
  }
}

void instrumentChoice()
{

  prevmusicstate = musicstate;
  musicstate = digitalRead(musicPin);
  if (prevmusicstate == HIGH && musicstate == LOW) {
    music++;   //Adds 1 to the currEditBar int on every loop

  }
  else {}
  if (music >= NUM_INSTRUMENT) {
    music = 0;
  }
  if (music == 0)
  {
    pixels1.setPixelColor(2, pixels.Color(57, 0, 153));
    pixels1.show();   // Send the updated pixel colors to the hardware.
  }
  if (music == 1)
  {
    pixels1.setPixelColor(2, pixels.Color(158, 0, 89));
    pixels1.show();   // Send the updated pixel colors to the hardware.
  }
  if (music == 2)
  {
    pixels1.setPixelColor(2, pixels.Color(255, 84, 0));
    pixels1.show();   // Send the updated pixel colors to the hardware.
  }
  if (music == 3)
  {
    pixels1.setPixelColor(2, pixels.Color(255, 189, 0));
    pixels1.show();   // Send the updated pixel colors to the hardware.
  }
  if (music == 4)
  {
    pixels1.setPixelColor(2, pixels.Color(194, 0, 251));
    pixels1.show();   // Send the updated pixel colors to the hardware.
  }

}
