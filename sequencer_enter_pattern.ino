unsigned long time;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long period = 1000;
int countUp = 0;            //creates a variable integer called 'countUp'

//Button/LED/Motor selection pin
// initial button values
int butonPins[8] = {10, 13, 16, 19, 22, 25, 28, 31};
int LEDpins[8] = {11, 14, 17, 20, 23, 26, 29, 32};
bool LEDstates[8] = {false, false, false, false, false, false, false, false};
bool Bstates[8] = {false, false, false, false, false, false, false, false};
bool prevBstates[8] = {false, false, false, false, false, false, false, false};
int motorPins[8] = {12, 15, 18, 21, 24, 27, 30, 33};

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

// Bass, Snare, HHC, HHO , HITom
int myARRAYbass[17] = {202, 149, 120, 110, 96, 83, 72, 68, 67, 65, 64, 62, 60, 58 , 55, 55};
int myARRAYHHC[17] = {222, 43, 6, 3, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0};
int myARRAYHHO[17] = {222, 213, 250, 2, 0, 0, 0, 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0};
int myARRAYHITom[17] = {120, 120, 120, 140, 106, 80, 58, 42, 31, 22, 16, 11, 7, 4, 2, 1, 0};
int myARRAYsnare[17] = {143, 120, 120, 80, 40, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0};
int count[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
int index[8] = { 0, 0, 0, 0, 0, 0, 0, 0};

int speedperiod[8] { 80, 80, 80, 80, 80, 80, 80, 80};
int speedperiod1[8] { 80, 80, 80, 80, 80, 80, 80, 80};
int speedperiod2[8] { 80, 80, 80, 80, 80, 80, 80, 80};
int val = 0;
int idexmax;
int indexbiggest;
int myARRAY1[11] = {217, 150, 115, 115, 114, 116, 119, 119, 87, 30, 0};

unsigned long startMillis1[8] = { 0, 0, 0, 0, 0, 0, 0, 0};  //some global variables available anywhere in the program
unsigned long currentMillis1[8] = { 0, 0, 0, 0, 0, 0, 0, 0};  //some global variables available anywhere in the program

#define LEDmenu 34  // The pin the LED is connected to



int red_light_pin = 35;
int green_light_pin = 36;
int blue_light_pin = 37;

void setup() {

  Serial.begin(115200);

  // setup each sequencer button
  for (int i = 0; i < 8; i++) {
    pinMode(butonPins[i], INPUT_PULLUP);
    pinMode(LEDpins[i], OUTPUT);
    pinMode(motorPins[i], OUTPUT);
  }

  pinMode(menuButtonPin, INPUT_PULLUP);

  pinMode(resetbuttonPin, INPUT_PULLUP);


  pinMode(LEDmenu, OUTPUT); // Declare the LED as an output
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);



}





void loop() {

  //Serial.println(counter);
  // check the menu buttons for press
  timeperperiod();

  // check the sequencer buttons for press
  doButtons();
  //Serial.print("Menu ");
  //Serial.println(counter);


  //Button toggle
  doButtons();

  //Menu button switching
  menubpmbutton();

  resetmode();


  //time per peorid 8 beat
  timeperperiod();
  //menu BPM choices
  BPMchoices();

  //counter if = and led = then

  motorvibrate();

}

void doButtons()
{


  // check for button push on each sequencer button
  for (int i = 0; i < 8; i++) {
    //   Serial.print(Bstates[i]);
    //  Serial.print(" ");
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
    }
  }
}



void menubpmbutton()

{
  prevMenuBstate = menuBstate;
  menuBstate = digitalRead(menuButtonPin);
  if (prevMenuBstate == HIGH && menuBstate == LOW) {
    counter++;   //Adds 1 to the countUp int on every loop
  }
  else {}
  // menu counter
  if (counter > 5) {
    counter = 0;
  }
  for (int i = 0; i < 8; i++) { //Bass, Snare, HHC, HHO , HITom

    if ( counter == 0 )
    { //Serial.println("Bass");
      myARRAY1[index[i]] = myARRAYbass[index[i]];
      idexmax = 16;
      indexbiggest = 17;
    }
    if ( counter == 1 )


    { // Serial.println("Bass");
      myARRAY1[index[i]] = myARRAYbass[index[i]];
      idexmax = 16;
      indexbiggest = 17;

    }
    if ( counter == 2 )


    { //Serial.println("Snare");
      myARRAY1[index[i]] = myARRAYsnare[index[i]];
      idexmax = 16;
      indexbiggest = 17;

    }

    if ( counter == 3)


    { // Serial.println("HHC");
      myARRAY1[index[i]] = myARRAYHHC[index[i]];
      idexmax = 16;
      indexbiggest = 17;
    }
    if ( counter == 4 )
    { //  Serial.println("HHO");
      myARRAY1[index[i]] = myARRAYHHO[index[i]];
      idexmax = 16;
      indexbiggest = 17;


    }

    if ( counter == 5 )
    { //  Serial.println("HITom");
      myARRAY1[index[i]] = myARRAYHITom[index[i]];
      idexmax = 16;
      indexbiggest = 17;


    }
  }


}


void  resetmode()

{
  prevresetbuttonstate = resetbuttonstate;
  resetbuttonstate = digitalRead(resetbuttonPin);
  if (prevresetbuttonstate == HIGH && resetbuttonstate == LOW) {
    counter1++;   //Adds 1 to the countUp int on every loop
  }
  else {}
  // menu counter
  if (counter1 > 1) {
    counter1 = 0;

  }

  if ( counter1 == 1 )  {
    LEDstates[0] == LOW;
    LEDstates[1] == LOW;
    LEDstates[2] == LOW;
    LEDstates[3] == LOW;
    LEDstates[4] == LOW;
    LEDstates[5] == LOW;
    LEDstates[6] == LOW;
    LEDstates[7] == LOW;
    analogWrite(motorPins[0], 0);
    analogWrite(motorPins[1], 0);
    analogWrite(motorPins[2], 0);
    analogWrite(motorPins[3], 0);
    analogWrite(motorPins[4], 0);
    analogWrite(motorPins[5], 0);
    analogWrite(motorPins[6], 0);
    analogWrite(motorPins[7], 0);


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

      if (countUp >= 8) {
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
// testing out the rgb and seeing if it works in the sequence +  colour change activated  
  {
    if (countUp == 0  && LEDstates[0] == LOW) {
      RGB_color(0, 0, 0);
    }
    else {
      RGB_color(255, 255, 255);
    }
    if (LEDstates[0] == HIGH) {
      RGB_color(255, 0, 255);
    }
    else {
    }
  }
}


// rgb sequence light 
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
