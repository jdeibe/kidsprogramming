/*

  Parts required:
  arduino one
  220 ohm resistor
  10 kilohm resistor
  16x2 LCD screen
  pirSensor
  blue led

  Created 3 Jan 2016
  by Jonas (Anton) Deibe

  with inspiration and credits from
  http://www.arduino.cc/starterKit

*/

// include the library code:
#include <LiquidCrystal.h>

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;

//the time when the sensor outputs a low impulse
long unsigned int lowIn;

//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

int pirPin = 8;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// set up a constant for the tilt switchPin
const int switchPin = 6;

// variable to hold the value of the switchPin
int switchState = 0;

// variable to hold previous value of the switchpin
int prevSwitchState = 0;

// a variable to choose which reply from the crystal ball
int reply;

void setup() {
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);

  // set up the switch pin as an input
  pinMode(switchPin, INPUT);

  //Setup pirsensor
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  lcd.print("calibrating");
  lcd.setCursor(0, 1);
  lcd.print("sensor");
  delay(2000);
  lcd.clear();
  bool oneTime = true;
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    lcd.print(".");
    if (i > 16 && oneTime == true) {
      oneTime = false;
      lcd.setCursor(0, 1);
    }
    delay(1000);
  }
  Serial.println(" done");
  lcd.clear();
  // set the cursor to column 0, line 0
  Serial.println("SENSOR ACTIVE");
  lcd.print("SENSOR ACTIVE");
  lcd.setCursor(0, 1);
  delay(50);

}

void loop() {

  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    if (lockLow) {
      //makes sure we wait for a transition to LOW before any further output is made:
      lockLow = false;
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      lcd.clear();
      lcd.print("motion detected at ");
      lcd.setCursor(0, 1);
      lcd.print(millis() / 1000);
      lcd.print(" sec");
      delay(50);
    }
    takeLowTime = true;
  }

  if (digitalRead(pirPin) == LOW) {
    digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

    if (takeLowTime) {
      lowIn = millis();          //save the time of the transition from high to LOW
      takeLowTime = false;       //make sure this is only done at the start of a LOW phase
    }
    //if the sensor is low for more than the given pause,
    //we assume that no more motion is going to happen
    if (!lockLow && millis() - lowIn > pause) {
      //makes sure this block of code is only executed again after
      //a new motion sequence has been detected
      lockLow = true;
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      lcd.clear();
      lcd.print("motion ended at ");      //output
      lcd.setCursor(0, 1);
      lcd.print((millis() - pause) / 1000);
      lcd.print(" sec");
      delay(50);
    }
  }

}

