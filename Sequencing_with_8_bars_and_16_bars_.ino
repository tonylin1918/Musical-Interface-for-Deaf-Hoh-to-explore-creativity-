
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

int modebuttonpin = 2;
bool modebuttontate = false;
int prevmodebuttonstate = false;
int counter2 = 0;

int barbuttonPin = 4;
bool barbuttonstate = false;
int prevbarbuttonstate = false;
int currEditBar = 0;
bool butonoroff[8] = {false};
bool butonoroff1[8] = {false};

int done = 0;
int done1 = 0;
int done2 = 0;

int myARRAY1[11] = {217, 150, 115, 115, 114, 116, 119, 119, 0, 0, 0};
unsigned long currentMillis1[8] = {0};  //some global variables available anywhere in the program
unsigned long startMillis1[8] = {0};  //some global variables available anywhere in the program
int count[8] = {0};
int index[8] = {0};
int motorPins[8] = {12, 15, 18, 21, 24, 27, 30, 33};
 int speedperiod[NUM_BARS][NUM_NOTES] = {
  {40, 40, 40, 40, 40, 40, 40, 40},    /*  initializers for row indexed by 0 */
  {40, 40, 40, 40, 40, 40, 40, 40}     /*  initializers for row indexed by 1 */
};
 int startMillis[NUM_BARS][NUM_NOTES] = {
  {0, 0, 0, 0, 0, 0, 0, 0},    /*  initializers for row indexed by 0 */
  {0, 0, 0, 0, 0, 0, 0, 0}     /*  initializers for row indexed by 1 */
};
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
    pinMode(motorPins[i], OUTPUT);
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
    buttoninput();
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
    lightNote();
    playNote();
    
  }
}
void lightNote()
{
   if (notes[currBar][currNote] > 0){
if (done2 == 0) {
        { for (int i = 0; i < 8; i++) {
          if (LEDpins[i], HIGH) {
            digitalWrite(LEDpins[i], LOW);
          }
        } }           done2++;            
        }    digitalWrite(LEDpins[currNote], HIGH);

done2 = 0; 
   }
  else {     
           for (int i = 0; i < 8; i++) {
          if (LEDpins[i], HIGH) {
            digitalWrite(LEDpins[i], LOW);

             }
          }
        }                 
        
 }

void playNote()
{ 
        Serial.println(currNote);
  if(notes[currBar][0] == 1 && LEDstates[0] == HIGH && currNote == 0) {
        analogWrite(motorPins[0], 80);
      }
      else {
        analogWrite(motorPins[0], 0);
      }
        if(notes[currBar][1] == 1 && LEDstates[1] == HIGH && currNote == 1) {
        analogWrite(motorPins[1], 80);
      }
      else {
        analogWrite(motorPins[1], 0);
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

  }
  if ( currEditBar == 1 )
  { notes[NUM_BARS][NUM_NOTES] = notes[1][NUM_NOTES];
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
            //  Serial.print (done);
          }
        }            done++;
                    done1 = 0;
      }
          
      if (notes[0][i] == 1) {
            digitalWrite(LEDpins[i], HIGH);
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
notes[0][i] = 0;      }
 
    }
  
    if ( currEditBar == 1 ) {
      
      if (done1 == 0) {
         for (int i = 0; i < 8; i++) {
          if (LEDpins[i], HIGH) {
            digitalWrite(LEDpins[i], LOW);
            //  Serial.print (done1);
          }
        }            done1++;
                    done = 0;
      }
           if (notes[1][i] == 1) {
            digitalWrite(LEDpins[i], HIGH);
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
      {      notes[1][i] = 0;

        //  Serial.println("1 LOW");
      }
    }
  }
}
