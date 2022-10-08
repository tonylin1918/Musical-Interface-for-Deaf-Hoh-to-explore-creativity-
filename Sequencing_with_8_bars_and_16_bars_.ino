
#define NUM_BARS 2
#define NUM_NOTES 8

unsigned long timer;
int notes[NUM_BARS][NUM_NOTES] = {
  {0, 0, 0, 0, 0, 0, 0, 0},    /*  initializers for row indexed by 0 */
  {0, 0, 0, 0, 0, 0, 0, 0}     /*  initializers for row indexed by 1 */
};
int LEDpins[8] = {11, 14, 17, 20, 23, 26, 29, 32};
int butonPins[8] = {10, 13, 16, 19, 22, 25, 28, 31};
bool LEDstates[8] = {false};
bool Bstates[8] = {false};
bool prevBstates[8] = {false};

int currNote;
int currBar;
int bpm = 60;
float period;
unsigned long countUp = 0;
bool newStep = false;
bool showDebug = true;
unsigned long startMillis1[8] = {0};  //some global variables available anywhere in the program
unsigned long currentMillis1[8] = {0};  //some global variables available anywhere in the program


int modebuttonpin = 2;
bool modebuttontate = false;
int prevmodebuttonstate = false;
int counter2 = 0;

int barbuttonPin = 4;
bool barbuttonstate = false;
int prevbarbuttonstate = false;
int counter1 = 0;


void setup() {
  Serial.begin(115200);

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
  }
  //mode buttoninput
  pinMode(modebuttonpin, INPUT_PULLUP);

  //Bar pin input
  pinMode(barbuttonPin, INPUT_PULLUP);
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
    barbutton();
    buttoninput1();
    buttoninput2();
  }
  if ( counter2 == 1 )  {
    updateTimeStep();
    updateNotes();
  }
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

    playNote();
  }
}

void playNote()
{
  if (notes[currBar][currNote] > 0)
  {
    Serial.println("HIGH");

  } else {
    //          Serial.println("LOW");
  }
}


void barbutton()
{
  prevbarbuttonstate = barbuttonstate;
  barbuttonstate = digitalRead(barbuttonPin);
  if (prevbarbuttonstate == HIGH && barbuttonstate == LOW) {
    counter1++;   //Adds 1 to the countUp int on every loop
  }
  else {}
  if (counter1 > 1) {
    counter1 = 0;
  }
  if ( counter1 == 0 )
  { notes[NUM_BARS][NUM_NOTES] = notes[0][NUM_NOTES];
  }
  if ( counter1 == 1 )
  { notes[NUM_BARS][NUM_NOTES] = notes[1][NUM_NOTES];
  }
}


void buttoninput1()
{
  for (int i = 0; i < 8; i++) {
    if (notes[0]) {
      prevBstates[i] = Bstates[i];
      Bstates[i] = digitalRead(butonPins[i]);
      if (prevBstates[i] == HIGH && Bstates[i] == LOW && i < 8) {
        // the button is pressed
        Serial.print("0 Button");
        Serial.println(i);
        LEDstates[i] = !LEDstates[i];
        digitalWrite(LEDpins[i], LEDstates[i]);
      }
      if (LEDstates[i] == HIGH) {
        notes[0][i] = 1;
      }
      else {
        notes[0][i] = 0;
      }
    }
  }
}
void buttoninput2()
{
  for (int i = 0; i < 8; i++) {
    if (notes[1]) {
      prevBstates[i] = Bstates[i];
      Bstates[i] = digitalRead(butonPins[i]);
      if (prevBstates[i] == HIGH && Bstates[i] == LOW && i < 8) {
        // the button is pressed
        Serial.print("1 Button");
        Serial.println(i);
        LEDstates[i] = !LEDstates[i];
        digitalWrite(LEDpins[i], LEDstates[i]);
      }
      if (LEDstates[i] == HIGH) {
        notes[1][i] = 1;
      }
      else {
        notes[1][i] = 0;
      }
    }
  }
}
