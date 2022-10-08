unsigned long time;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long period = 0;
int countUp = 0;            //creates a variable integer called 'countUp'

//Button/LED/Motor selection pin
// initial button values
int butonPins[16] = {10, 13, 16, 19, 22, 25, 28, 31 , 10, 13, 16, 19, 22, 25, 28, 31};
int LEDpins[16] = {11, 14, 17, 20, 23, 26, 29, 32, 11, 14, 17, 20, 23, 26, 29, 32};
bool LEDstates[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
bool Bstates[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
bool prevBstates[16] = {false};
int motorPins[16] = {12, 15, 18, 21, 24, 27, 30, 33, 12, 15, 18, 21, 24, 27, 30, 33 };


unsigned long p = 1000;
unsigned long sm = 0;  //some global variables available anywhere in the program
unsigned long cm;

// counters
int counter = 0;
unsigned long period1 = 80;
int counting = 0;
int menuButtonPin = 6;
bool menuBstate = false;
int prevMenuBstate = false;

int resetbuttonPin = 2;
bool resetbuttonstate = false;
int prevresetbuttonstate = false;
int counter1 = 0;

int modebuttonpin = 4;
bool modebuttontate = false;
int prevmodebuttonstate = false;
int counter2 = 0;

int      c = 8;

// beat experiences 
int myARRAY1[11] = {217, 150, 115, 115, 114, 116, 119, 119, 0, 0, 0};
int myARRAY2[11] = {140, 180, 100, 145, 253, 100, 235, 238, 0, 0, 0};
int myARRAY3[11] = {60, 80, 100, 160, 250, 100, 80, 30, 0, 0, 0};


//// Bass, Snare, HHC, HHO , HITom
//int myARRAYHHC[17] = {222, 43, 6, 3, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0};
//int myARRAYHHO[17] = {222, 213, 250, 2, 0, 0, 0, 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0};
//int myARRAYHITom[17] = {120, 120, 120, 140, 106, 80, 58, 42, 31, 22, 16, 11, 7, 4, 2, 1, 0};
//int myARRAYsnare[17] = {143, 120, 120, 80, 40, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0};
int count[8] = {0};
int index[8] = {0};

int speedperiod[8] { 80, 80, 80, 80, 80, 80, 80, 80};
int speedperiod1[8] { 80, 80, 80, 80, 80, 80, 80, 80};
int speedperiod2[8] { 80, 80, 80, 80, 80, 80, 80, 80};
int val = 0;
int idexmax;
int indexbiggest;

unsigned long startMillis1[8] = { 0, 0, 0, 0, 0, 0, 0, 0};  //some global variables available anywhere in the program
unsigned long currentMillis1[8] = { 0, 0, 0, 0, 0, 0, 0, 0};  //some global variables available anywhere in the program

#define LEDmenu 34  // The pin the LED is connected to



int red_light_pin = 35;
int green_light_pin = 36;
int blue_light_pin = 37;



#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        8 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 5 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



///// testing with an array see if it works to note down the on and off beats--------------------------------------------------------
int number = 0;
int anArray[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; //an array capable of holding 20 entries numbered 0 to 19
int arrayIndex = 0;


void setup() {

  Serial.begin(115200);
  pixels.begin();

  // setup each sequencer button
  for (int i = 0; i < 8; i++) {
    pinMode(butonPins[i], INPUT_PULLUP);
    pinMode(LEDpins[i], OUTPUT);
    pinMode(motorPins[i], OUTPUT);
  }

  pinMode(menuButtonPin, INPUT_PULLUP);

  pinMode(resetbuttonPin, INPUT_PULLUP);
  pinMode(modebuttonpin, INPUT_PULLUP);


  pinMode(LEDmenu, OUTPUT); // Declare the LED as an output

int x = 0;

int inputtedarray1[16];       // trying to link counter to an array for playing this specific vibraiton of beat 

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
    //  Serial.println("MODE: Write ");

    // check the sequencer buttons for press
    doButtons();
    //8th bar or 16th bar
    resetmode();

  }
  // play modeeeeeeeeeeeeeeee

  if ( counter2 == 1 )  {
    //   Serial.println("MODE: Play ");
    BPMchoices();

    //Serial.println(counter);
    // check the menu buttons for press
    timeperperiod();

    //Menu button switching
    menubpmbutton();
    //menu BPM choices
    //counter if = and led = then
    motorvibrate();
    Ledstatewheninplaymode();
  }

}







void doButtons()
{


  // check for button push on each sequencer button
  for (int i = 0; i < 8; i++) {
    //   Serial.print(Bstates[i]);
    //  Serial.print(" ");
    prevBstates[i] = Bstates[i];
    Bstates[i] = digitalRead(butonPins[i]);
    if (prevBstates[i] == HIGH && Bstates[i] == LOW && i < 8) {
      // the button is pressed
      Serial.print("Button pressed. Num: ");
      Serial.println(i);
      LEDstates[i] = !LEDstates[i];
      digitalWrite(LEDpins[i], LEDstates[i]);


    if ( counter == 0 )
  /////////////////////////// and led inouted states is high writre an array of the hgih ones when the motor is going off when the array number matches the count number        myARRAY1[index[i]]; 






      if (LEDstates[i] == HIGH) {
//        Serial.println("LED is on");
//        Serial.print("order");
//        Serial.print(arrayIndex);
//        Serial.print("number entered");
        anArray[arrayIndex] = i;
//       Serial.println(anArray[arrayIndex]);
//if the arrayIndex number and the number entered  so the count up is eauals to the anArray[arrayIndex]

        arrayIndex++;


      } else {
//        Serial.println("LED is off");
      }

      // Reset the index??
     if (arrayIndex >15)
     {arrayIndex = 0;}
      
    }
    // if led low - idex Shuffle down all the entries one space so the unused one is at the end Decrement the count of entries
if (LEDstates[i] == LOW && prevBstates[i] == LOW && Bstates[i] == HIGH && i < 8)
{ 
 // arrayIndex = arrayIndex -1;        repreats a -1 which is inaccurate it should only be once to move the position back 
        anArray[arrayIndex] = -1;

        Serial.println("yoooo");

}

for(int i = 0; i < 16; i++)
{
  Serial.print(anArray[i]);
}
  }

  
}



void menubpmbutton()

{

                                     // testing different vibration expereinces 
  prevMenuBstate = menuBstate;
  menuBstate = digitalRead(menuButtonPin);
  if (prevMenuBstate == HIGH && menuBstate == LOW) {
    counter++;   //Adds 1 to the countUp int on every loop
  }
  else {}
  // menu counter
  if (counter > 2) {
    counter = 0;
  }
  for (int i = 0; i < 8; i++) {

    if ( counter == 0 )
    {
     myARRAY1[index[i]];
    }
    if ( counter == 1 )
    { 
     myARRAY1[index[i]] = myARRAY2[index[i]];
    }
    if ( counter == 2 )
    { //Serial.println("Snare");
     myARRAY1[index[i]] = myARRAY3[index[i]];
    }

   
  }


}


void  resetmode()  // use this as creating 8 bar and 16 bars swappping from 1-8 -9-17

{



  prevresetbuttonstate = resetbuttonstate;
  resetbuttonstate = digitalRead(resetbuttonPin);
  if (prevresetbuttonstate == HIGH && resetbuttonstate == LOW) {
    counter1++;
  }
  else {}
  // menu counter
  if (counter1 > 1) {
    counter1 = 0;



  }

  if ( counter1 == 0 )  {
    // if only using first page then 8 bars
    // Serial.println ("8 Bar!");

    if (countUp >= 8) {
      countUp = 0;
    }
    c = 8;
    // RBG CHANGE COLOUR
    period = 0;

    for (int i = 0; i < 8; i++) {
      if (LEDstates[i] == HIGH) {
        pixels.setPixelColor(i, pixels.Color(150, 150, 150 ));
        pixels.show();   // Send the updated pixel colors to the hardware.   


        // manybe write array of inputted 1-8 and then extract when going back into this mode

      }
      else {
        pixels.setPixelColor(i, pixels.Color(0 , 0, 0 ));

        pixels.show();   // Send the updated pixel colors to the hardware.
      }

    


    }
  }











  if ( counter1 == 1 )  {

    //


    // if only using first page then 16 bars
    c = 16;
    // also double speed
    //Serial.println ("16 Bar!");
    // RBG CHANGE COLOUR

    //Can enter into part 2 of 9-16 button 1 = 9

    // check for button push on each sequencer button
    for (int i = 8; i < 16; i++) {

      prevBstates[i] = Bstates[i];
      Bstates[i] = digitalRead(butonPins[i]);
      if (prevBstates[i] == HIGH && Bstates[i] == LOW) {
        // the button is pressed
        Serial.print("Button pressed. Num: ");
        Serial.println(i);
        LEDstates[i] = !LEDstates[i];
        digitalWrite(LEDpins[i], LEDstates[i]);
        if (LEDstates[i] == HIGH) {
          Serial.println("LED is on");
        } else {
          Serial.println("LED is off");
        }

        {

          //-8 to get the orignial 0-7 numbers for led
          if (LEDstates[i] == HIGH) {
            pixels.setPixelColor(i - 8, pixels.Color(150, 150, 0 ));
            pixels.show();   // Send the updated pixel colors to the hardware.    };
          }
          else {
            pixels.setPixelColor(i - 8, pixels.Color(0 , 0, 0 ));
            pixels.show();   //(255,255,0) Send the updated pixel colors to the hardware.    }
          }
        }
      }
    }
  }

}




void timeperperiod()

{
  millis();
  //prints time since program started
  // Serial.println(time);
  // wait a second so as not to send massive amounts of data

  {
    currentMillis = millis(); //get the current "time" (actually the number of milliseconds since the program started)

    if (currentMillis - startMillis >= period)  //test whether the period has elapsed
    {


      Serial.print("CurrentMillis: ");
      Serial.println(currentMillis);
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
      analogWrite(LEDmenu, 255); // Turn the LED on

      //add counter to loop

      countUp++;                //Adds 1 to the countUp int on every loop
      Serial.println (period);

      if (countUp >= c) {
        countUp = 0;
      }
      Serial.print("-Steps ");
      Serial.println(countUp);
      Serial.print("period: ");
      Serial.println(period);
    }
    else
    { analogWrite(LEDmenu, 0); // Turn the LED on
    }

  }
}





void BPMchoices()

{

  // Double speed for change of 16bars
  if (c == 16 && period == 500)
  {
    period = 250;
  }
  if (c == 16 && period == 1000)
  {
    period = 501;
  }
  if (c == 16 && period == 750)
  {
    period = 375;
  }


  for (int i = 0; i < 8; i++) {
    int sensorValue = analogRead(A0);
    // print out the value you read:

    if (  sensorValue < 341 && sensorValue > 0)
    { //Serial.println("1");
      //    Serial.println(" 60bpm");
      period = 1000;
      p = 1000;
      speedperiod[i] = speedperiod1[i];

    }
    if ( sensorValue >= 341 && sensorValue < 682)
    { //Serial.println("2");
      //       Serial.println(" 80bpm");
      period = 750;
      p = 750;
      speedperiod[i] = speedperiod[i];


    }
    if ( sensorValue >= 682 && sensorValue < 1023)
    { //Serial.println("3");
      //  Serial.println(" 120bpm");
      period = 500;
      p = 500;
      speedperiod[i] = speedperiod2[i];

    }
  }


}
void motorvibrate()

{
  for (int i = 0; i < 8; i++) {
    if (countUp == i  && LEDstates[i] == HIGH) {

      currentMillis1[i] = millis();
      if (currentMillis1[i] - startMillis1[i] >= speedperiod[i])  //test whether the period has elapsed
      {
        startMillis1[i] = currentMillis1[i];  //IMPORTANT to save the start time of the current LED state.
        count[i] = index[i];
        analogWrite(motorPins[i], myARRAY1[index[i]]);
        index[i]++;
        Serial.print ("motor = ");
        Serial.println (myARRAY1[index[i]]);
      }
      if ( index[i] > 10) {
        index[i] = 11;
      }
    } else {
      index[i] = 0;
      analogWrite(motorPins[i], 0);
    }
  }
}

void  Ledstatewheninplaymode()

{
  for (int i = 0; i < 8; i++) {
    if (countUp == i  && LEDstates[i] == HIGH) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0 ));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
    else {

    }
    //fix up turn green off when not on
    if (countUp == !i  && pixels.Color(0, 150, 0) && LEDstates[i] == HIGH)  {
      pixels.show();   // Send the updated pixel colors to the hardware.
      pixels.setPixelColor(i, pixels.Color(150, 150, 150 ));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }

  }
}




