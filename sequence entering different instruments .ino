#include "Arduino.h"
#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>

SoftwareSerial DF1201SSerial(52, 53);  //RX  TX
DFRobot_DF1201S DF1201S;

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

int speedperiod[8] { 60 , 60, 60, 60, 60, 60, 60, 60};
int speedperiod1[8] { 60 , 60, 60, 60, 60, 60, 60, 60};
int speedperiod2[8] { 60 , 60, 60, 60, 60, 60, 60, 60};
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

#include <SPI.h>
#include "vs10xx_uc.h" // From VLSI website: http://www.vlsi.fi/en/support/software/microcontrollersoftware.html

// Uncomment to perform a test play of the instruments and drums on power up
#define SOUND_CHECK 1

//#define VS1053_MP3_SHIELD 1
#define VS1003_MODULE 1

#ifdef VS1003_MODULE
extern "C" {
#include "rtmidi1003b.h"
#include "vs1003inst.h"
}
#endif

#define MIDI_CHANNEL 10 // For drums!

#ifdef VS1003_MODULE
// VS1003 Module pin definitions
#define VS_XCS    40 // Control Chip Select Pin (for accessing SPI Control/Status registers)
#define VS_XDCS   41 // Data Chip Select / BSYNC Pin
#define VS_DREQ   38 // Data Request Pin: Player asks for more data
#define VS_RESET  53 // Reset is active low
#endif
// VS10xx SPI pin connections (both boards)
// Provided here for info only - not used in the sketch as the SPI library handles this
#define VS_MOSI         51
#define VS_MISO        50
#define VS_SCK        52
#define VS_SS    53

// Optional - use Digital IO as the power pins
//#define VS_VCC    6
//#define VS_GND    5

// Define the drum patterns.
// There are four instruments, each controlled by switches in the keypad.
#define BEATS 8
#define DRUMS 8
#ifdef VS1003_MODULE
byte drums[DRUMS] = {VS1003_D_BASS, VS1003_D_MARACAS, VS1003_D_SNARE, VS1003_D_HHC, VS1003_D_HHO, VS1003_D_HITOM, VS1003_D_CRASH, VS1003_D_TAMB};
#endif
#ifdef VS1053_MP3_SHIELD
int drums[DRUMS] = {35, 70, 38, 42, 46, 50, 57, 54}; // Bass, Snare, HHC, HHO , HITom,
#endif
byte pattern[BEATS][DRUMS];
int  seqstep; // index in the pattern 0 to BEATS-1

#ifdef TEST
char teststr[32];
#endif

int number = 0;

int anArray[8] =  {-1, -1, -1, -1, -1 , -1 , -1, -1};  //an array capable of holding 20 entries numbered 0 to 19
int arrayIndex = 0;


int x = 0;




void setup() {

  Serial.begin(115200);


#ifdef VS_VCC
  pinMode(VS_VCC, OUTPUT);
  digitalWrite(VS_VCC, HIGH);
#endif // VS_VCC
#ifdef VS_GND
  pinMode(VS_GND, OUTPUT);
  digitalWrite(VS_GND, LOW);
#endif // VS_GND


  // put your setup code here, to run once:
  initialiseVS10xx();

  // setup each sequencer button
  for (int i = 0; i < 8; i++) {
    pinMode(butonPins[i], INPUT_PULLUP);
    pinMode(LEDpins[i], OUTPUT);
    pinMode(motorPins[i], OUTPUT);
  }

  pinMode(menuButtonPin, INPUT_PULLUP);

  pinMode(LEDmenu, OUTPUT); // Declare the LED as an output
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);



}









void loop() {


//Serial.println (       drums[0]
//);
//     Serial.println(anArray[0]);
//
//        Serial.print(anArray[1]);
//
//        Serial.print(anArray[2]);
//
//        Serial.print(anArray[3]);
//      Serial.print(anArray[4]);
//
//        Serial.print(anArray[5]);
//
//        Serial.print(anArray[6]);
//
//        Serial.print(anArray[7]);




  //Serial.println(counter);
  // check the menu buttons for press
  timeperperiod();

  // check the sequencer buttons for press
  doButtons();
  //Serial.print("Menu ");
  //Serial.println(counter);


    resetmode();

  //Menu button switching
  menubpmbutton();

  //time per peorid 8 beat
  timeperperiod();
  //menu BPM choices
  BPMchoices();

  //counter if = and led = then

  motorvibrate();

  sound();


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



      LEDstates[i] = !LEDstates[i];
      digitalWrite(LEDpins[i], LEDstates[i]);
      if (LEDstates[i] == HIGH) {
        //increment the array index
        //put a value in entry 0
        Serial.print("order");
        Serial.print(arrayIndex);
        Serial.print("number entered");
        anArray[arrayIndex] = i;
       Serial.println(anArray[arrayIndex]);
//if the arrayIndex number and the number entered  so the count up is eauals to the anArray[arrayIndex]

        arrayIndex++;

        
      }


    }

////////////////////0
    { if (countUp == 0 && anArray[0] == 0){               drums[0] = 35;

drums[0] = drums[0]; 
} }

    { if (countUp == 0 && anArray[0] == 1)
      {
        drums[0] = drums[1]; } }
    { if (countUp == 0 && anArray[0] == 2)
    {drums[0] = drums[2]; }}
    { if (countUp == 0 && anArray[0] == 3)
      {
        drums[0] = drums[3];} }
    { if (countUp == 0 && anArray[0] == 4){
       drums[0] = drums[4]; } }
    { if (countUp == 0 && anArray[0] == 5)
      {
        drums[0] = drums[5];  } }
    { if (countUp == 0 && anArray[0] == 6){
        drums[0] = drums[6]; } }
    { if (countUp == 0 && anArray[0] == 7)
      {
       drums[0] = drums[7]; } }
  
///////////////////////////1
    { if (countUp == 1 && anArray[1] == 0){       drums[0] = 35;
drums[0] = drums[0]; } }
    { if (countUp == 1 && anArray[1] == 1)
      {
        drums[0] = drums[1];  } }
    { if (countUp == 1 && anArray[1] == 2 ){
        drums[0] = drums[2];  } }
    { if (countUp == 1 && anArray[1] == 3)
      {
      drums[0] = drums[3];  } }
    { if (countUp == 1 && anArray[1] == 4){
       drums[0] = drums[4]; } }
    { if (countUp == 1 && anArray[1] == 5)
      {
       drums[0] = drums[5]; } }
    { if (countUp == 1 && anArray[1] == 6){
       drums[0] = drums[6];  } }
    { if (countUp == 1 && anArray[1] == 7)
      {
        drums[0] = drums[7];  } }
  
///////////////////////////2
    { if (countUp == 2 && anArray[2] == 0 ){       drums[0] = 35;

        drums[0] = drums[0]; } }
    { if (countUp == 2 && anArray[2] == 1)
      {
        drums[0] = drums[1];  } }
    { if (countUp == 2 && anArray[2] == 2){
        drums[0] = drums[2];  } }
    { if (countUp == 2 && anArray[2] == 3)
      {
   drums[0] = drums[3];  } }
    { if (countUp == 2 && anArray[2] == 4){
      drums[0] = drums[4];  } }
    { if (countUp == 2 && anArray[2] == 5)
      {
       drums[0] = drums[5]; } }
    { if (countUp == 2 && anArray[2] == 6){
        drums[0] = drums[6];  } }
    if (number == 2)
    { if (countUp == 2 && anArray[2] == 7)
      {
      drums[0] = drums[7];} }
  
///////////////////////////3
    { if (countUp == 3 && anArray[3] == 0 ){       drums[0] = 35;

        drums[0] = drums[0];  } }
    { if (countUp == 3 && anArray[3] == 1)
      {
        drums[0] = drums[1]; } }
    { if (countUp == 3 && anArray[3] == 2){
       drums[0] = drums[2]; } }
    { if (countUp == 3 && anArray[3] == 3)
      {
       
       drums[0] = drums[3]; } }
    { if (countUp == 3 && anArray[3] == 4){
        drums[0] = drums[4];  } }
    { if (countUp == 3 && anArray[3] == 5)
      {
       drums[0] = drums[5]; } }
    { if (countUp == 3 && anArray[3] == 6){
        drums[0] = drums[6];} }
    { if (countUp == 3 && anArray[3] == 7)
      {
        drums[0] = drums[7];  } }

        
  















////////////////////4
    { if (countUp == 4 && anArray[4] == 0){          drums[0] = 35;
     
drums[0] = drums[0]; 

} }
    { if (countUp == 4 && anArray[4] == 1)
      {
        drums[0] = drums[1]; } }
    { if (countUp == 4 && anArray[4] == 2){drums[0] = drums[2]; }
    { if (countUp == 4 && anArray[4] == 3)
      {
        drums[0] = drums[3];} }
    { if (countUp == 4 && anArray[4] == 4){
       drums[0] = drums[4]; } }
    { if (countUp == 4 && anArray[4] == 5)
      {
        drums[0] = drums[5];  } }
    { if (countUp == 4 && anArray[4] == 6){
        drums[0] = drums[6]; } }
    { if (countUp == 4 && anArray[4] == 7)
      {
       drums[0] = drums[7]; } }
  
///////////////////////////1
    { if (countUp == 5 && anArray[5] == 0){
             drums[0] = 35;
drums[0] = drums[0]; } }
    { if (countUp == 5 && anArray[5] == 1)
      {
        drums[0] = drums[1];  } }
    { if (countUp == 5 && anArray[5] == 2 ){
        drums[0] = drums[2];  } }
    { if (countUp == 5 && anArray[5] == 3)
      {
      drums[0] = drums[3];  } }
    { if (countUp == 5 && anArray[5] == 4){
       drums[0] = drums[4]; } }
    { if (countUp == 5 && anArray[5] == 5)
      {
       drums[0] = drums[5]; } }
    { if (countUp == 5 && anArray[5] == 6){
       drums[0] = drums[6];  } }
    { if (countUp == 5 && anArray[5] == 7)
      {
        drums[0] = drums[7];  } }
  
///////////////////////////2
    { if (countUp == 6 && anArray[6] == 0 ){
             drums[0] = 35;
  drums[0] = drums[0]; } }
    { if (countUp == 6 && anArray[6] == 1)
      {
        drums[0] = drums[1];  } }
    { if (countUp == 6 && anArray[6] == 2){
        drums[0] = drums[2];  } }
    { if (countUp == 6 && anArray[6] == 3)
      {
       drums[0] = drums[3];  } }
    { if (countUp == 6 && anArray[6] == 4){
      drums[0] = drums[4];  } }
    { if (countUp == 6 && anArray[6] == 5)
      {
       drums[0] = drums[5]; } }
    { if (countUp == 6 && anArray[6] == 6){
        drums[0] = drums[6];  } }
    if (number == 2)
    { if (countUp == 6 && anArray[6] == 7)
      {
      drums[0] = drums[7];} }
  
///////////////////////////3
    { if (countUp == 7 && anArray[7] == 0 ){
              drums[0] = 35;
 drums[0] = drums[0];  } }
    { if (countUp == 7 && anArray[7] == 1)
      {
        drums[0] = drums[1]; } }
    { if (countUp == 7 && anArray[7] == 2){
       drums[0] = drums[2]; } }
    { if (countUp == 7 && anArray[7] == 3)
      {
     
       drums[0] = drums[3]; } }
    { if (countUp == 7 && anArray[7] == 4){
        drums[0] = drums[4];  } }
    { if (countUp == 7 && anArray[7] == 5)
      {
       drums[0] = drums[5]; } }
    { if (countUp == 7 && anArray[7] == 6){
        drums[0] = drums[6];} }
    { if (countUp == 7 && anArray[7] == 7)
      {
        drums[0] = drums[7];  } }

  }
  
      }


       //Serial.println(drums[0]);

//// how mny note note on has been triggered
  
    for (int i = 0; i < 8; i++) {
      if (countUp == i && LEDstates[i] == HIGH)
      {
        cm = millis();
      
        if (cm - sm >= p)  //test whether the period has elapsed
        { 
       talkMIDI (0x90|(MIDI_CHANNEL-1), drums[0], 60);
      Serial.println ("on");

          sm = cm;   //IMPORTANT to save the start time of the current LED state.
}
      }
      else {     

      }
  
    
  
  }


}




 void  resetmode()

{
  prevresetbuttonstate = resetbuttonstate;
  resetbuttonstate = digitalRead(resetbuttonPin);
  if (prevresetbuttonstate == HIGH && resetbuttonstate == LOW) {
    counter1++;   //Adds 1 to the countUp int on every loop    
    Serial.print ( "hi" );

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
void menubpmbutton()

{

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

      //     Serial.print("CurrentMillis: ");
      //     Serial.println(currentMillis);
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
      analogWrite(LEDmenu, 255); // Turn the LED on

      //add counter to loop
talkMIDI (0x80|(MIDI_CHANNEL-1), drums[0], 0);
      Serial.println ("off");
for (int i = 0; i < 8; i++){      analogWrite(motorPins[i], 0);}

      countUp++;                //Adds 1 to the countUp int on every loop
index[x] = 0;                                                                                 /////////////////////////                                       see if will work
      if (countUp >= 8) {
        countUp = 0;

      }
      //    Serial.print("-Steps ");
      //     Serial.println(countUp);
      //Serial.print("period: ");
      //    Serial.println(period);
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
      period = 200;
      p = 200;
      speedperiod[i] = speedperiod2[i];

    }
  }
}
void motorvibrate()


   // issue here counting seqnece not the sae as order of beats activated 
    {
for (int i = 0; i < 8; i++)  {  for (int x = 0; x < 17; x++) {
             if (countUp == i && LEDstates[i] == HIGH){
      //anArray[i] == i! anArray[i] == -1
      currentMillis1[i] = millis();
      if (currentMillis1[i] - startMillis1[i] >= speedperiod[i])  //test whether the period has elapsed
      { 
        startMillis1[i] = currentMillis1[i];  //IMPORTANT to save the start time of the current LED state.
        count[i] = index[x];
        analogWrite(motorPins[i], myARRAY1[index[x]]);
        index[x]++;
                Serial.println (myARRAY1[index[x]]);
          if( index[x] > 16)
         { index[x] = 0;}
      }}
     else {
     }
      }
    }}

    
  


void sound()

{

}





















void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}


void sendMIDI(byte data) {
  SPI.transfer(0);
  SPI.transfer(data);
}

void talkMIDI(byte cmd, byte data1, byte data2) {
  //
  // Wait for chip to be ready (Unlikely to be an issue with real time MIDI)
  //
  while (!digitalRead(VS_DREQ)) {
  }
  digitalWrite(VS_XDCS, LOW);

  sendMIDI(cmd);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if ( (cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) >= 0xE0) {
    sendMIDI(data1);
    sendMIDI(data2);
  } else {
    sendMIDI(data1);
  }

  digitalWrite(VS_XDCS, HIGH);
}


/***********************************************************************************************

   Code from here on is the magic required to initialise the VS10xx and
   put it into real-time MIDI mode using an SPI-delivered patch.

   Here be dragons...

   Based on VS1003b/VS1033c/VS1053b Real-Time MIDI Input Application
   http://www.vlsi.fi/en/support/software/vs10xxapplications.html

   With some input from MP3_Shield_RealtimeMIDI.ino by Matthias Neeracher
   which was based on Nathan Seidle's Sparkfun Electronics example code for the Sparkfun
   MP3 Player and Music Instrument shields and and VS1053 breakout board.

 ***********************************************************************************************
*/

void initialiseVS10xx () {
  // Set up the pins controller the SPI link to the VS1053
  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XCS, OUTPUT);
  pinMode(VS_XDCS, OUTPUT);
  pinMode(VS_RESET, OUTPUT);

  // Setup SPI
  // The Arduino's Slave Select pin is only required if the
  // Arduino is acting as an SPI slave device.
  // However, the internal circuitry for the ATmeta328 says
  // that if the SS pin is low, the MOSI/MISO lines are disabled.
  // This means that when acting as an SPI master (as in this case)
  // the SS pin must be set to an OUTPUT to prevent the SPI lines
  // being automatically disabled in hardware.
  // We can still use it as an OUTPUT IO pin however as the value
  // (HIGH or LOW) is not significant - it just needs to be an OUTPUT.
  // See: http://www.gammon.com.au/spi
  //
  pinMode(VS_SS, OUTPUT);

  // Now initialise the VS10xx
  digitalWrite(VS_XCS, HIGH);  //Deselect Control
  digitalWrite(VS_XDCS, HIGH); //Deselect Data
  digitalWrite(VS_RESET, LOW); //Put VS1053 into hardware reset

  // And then bring up SPI
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  //From page 12 of datasheet, max SCI reads are CLKI/7. Input clock is 12.288MHz.
  //Internal clock multiplier is 1.0x after power up.
  //Therefore, max SPI speed is 1.75MHz. We will use 1MHz to be safe.
  SPI.setClockDivider(SPI_CLOCK_DIV16); //Set SPI bus speed to 1MHz (16MHz / 16 = 1MHz)
  SPI.transfer(0xFF); //Throw a dummy byte at the bus

  delayMicroseconds(1);
  digitalWrite(VS_RESET, HIGH); //Bring up VS1053

  // Dummy read to ensure VS SPI bus in a known state
  VSReadRegister(SCI_MODE);

  // Perform software reset and initialise VS mode
  VSWriteRegister16(SCI_MODE, SM_SDINEW | SM_RESET);
  delay(200);
  VSStatus();
#ifdef TEST
#ifdef SINTEST
  // Output a test sine wave to check everything is working ok
  VSSineTest();
  delay(100);
  VSStatus();
#endif // SINTEST
#endif // TEST

  // Enable real-time MIDI mode
  VSLoadUserCode();
  VSStatus();

  // Set the default volume
  //  VSWriteRegister(SCI_VOL, 0x20, 0x20);  // 0 = Maximum; 0xFEFE = silence
  VSStatus();
}

// This will read key status and mode registers from the VS10xx device
// and dump them to the serial port.
//
void VSStatus (void) {
#ifdef TEST
  // Print out some of the VS10xx registers
  uint16_t vsreg = VSReadRegister(SCI_MODE); // MODE Mode Register
  sprintf(teststr, "Mode=0x%04x b", vsreg);
  Serial.print(teststr);
  Serial.println(vsreg, BIN);
  vsreg = VSReadRegister(SCI_STATUS);
  sprintf(teststr, "Stat=0x%04x b", vsreg);
  Serial.print(teststr);
  Serial.print(vsreg, BIN);
  switch (vsreg & SS_VER_MASK) {
    case SS_VER_VS1001: Serial.println(" (VS1001)"); break;
    case SS_VER_VS1011: Serial.println(" (VS1011)"); break;
    case SS_VER_VS1002: Serial.println(" (VS1002)"); break;
    case SS_VER_VS1003: Serial.println(" (VS1003)"); break;
    case SS_VER_VS1053: Serial.println(" (VS1053)"); break;
    case SS_VER_VS1033: Serial.println(" (VS1033)"); break;
    case SS_VER_VS1063: Serial.println(" (VS1063)"); break;
    case SS_VER_VS1103: Serial.println(" (VS1103)"); break;
    default: Serial.println(" (Unknown)"); break;
  }
  vsreg = VSReadRegister(SCI_VOL); // VOL Volume
  sprintf(teststr, "Vol =0x%04x\n", vsreg);
  Serial.print(teststr);
  vsreg = VSReadRegister(SCI_AUDATA); // AUDATA Misc Audio data
  sprintf(teststr, "AUDA=0x%04x (%uHz)\n", vsreg, (vsreg & 0xFFFE));
  Serial.print(teststr);
  Serial.println();
#endif
}

// This sends a special sequence of bytes to the device to
// get it to output a test sine wave.
//
// See the datasheets for details.
//
void VSSineTest () {
  VSWriteRegister16(SCI_MODE, SM_SDINEW | SM_RESET | SM_TESTS);
  delay(100);
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XDCS, LOW); //Select control

  //Special 8-byte sequence to trigger a sine test
  SPI.transfer(0x53);
  SPI.transfer(0xef);
  SPI.transfer(0x6e);
  SPI.transfer(0x63);  // FIdx(7:5)=b011; S(4:0)=b00011 ==> F= ~517Hz
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XDCS, HIGH); //Deselect Control

  delay (2000);

  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XDCS, LOW); //Select control

  //Special 8-byte sequence to disable the sine test
  SPI.transfer(0x45);
  SPI.transfer(0x78);
  SPI.transfer(0x69);
  SPI.transfer(0x74);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XDCS, HIGH); //Deselect Control

  delay(100);
  VSWriteRegister16(SCI_MODE, SM_SDINEW | SM_RESET);
  delay(200);
}

// Write to VS10xx register
// SCI: Data transfers are always 16bit. When a new SCI operation comes in
// DREQ goes low. We then have to wait for DREQ to go high again.
// XCS should be low for the full duration of operation.
//
void VSWriteRegister(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte) {
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XCS, LOW); //Select control

  //SCI consists of instruction byte, address byte, and 16-bit data word.
  SPI.transfer(0x02); //Write instruction
  SPI.transfer(addressbyte);
  SPI.transfer(highbyte);
  SPI.transfer(lowbyte);
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XCS, HIGH); //Deselect Control
}

// 16-bit interface to the above function.
//
void VSWriteRegister16 (unsigned char addressbyte, uint16_t value) {
  VSWriteRegister (addressbyte, value >> 8, value & 0xFF);
}

// Read a VS10xx register using the SCI (SPI command) bus.
//
uint16_t VSReadRegister(unsigned char addressbyte) {
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XCS, LOW); //Select control

  SPI.transfer(0x03); //Read instruction
  SPI.transfer(addressbyte);
  delayMicroseconds(10);
  uint8_t d1 = SPI.transfer(0x00);
  uint8_t d2 = SPI.transfer(0x00);
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XCS, HIGH); //Deselect control

  return ((d1 << 8) | d2);
}

// Load a user plug-in over SPI.
//
// See the application and plug-in notes on the VLSI website for details.
//
void VSLoadUserCode(void) {
#ifdef TEST
  Serial.print("Loading User Code");
#endif
  for (int i = 0; i < VS10xx_CODE_SIZE; i++) {
    uint8_t addr = pgm_read_byte_near(&vs10xx_atab[i]);
    uint16_t dat = pgm_read_word_near(&vs10xx_dtab[i]);
#ifdef TEST
    if (!(i % 8)) Serial.print(".");
    //    sprintf(teststr, "%4d --> 0x%04X => 0x%02x\n", i, dat, addr);
    //    Serial.print(teststr);
#endif
    VSWriteRegister16 (addr, dat);
  }

  // Set the start address of the application code (see rtmidi.pdf application note)
#ifdef VS1003_MODULE
  VSWriteRegister16(SCI_AIADDR, 0x30);
#endif
#ifdef VS1053_MP3_SHIELD
  VSWriteRegister16(SCI_AIADDR, 0x50);
#endif

#ifdef TEST
  Serial.print("Done\n");
#endif
}
