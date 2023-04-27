#include <SoftwareSerial.h>
#include <Adafruit_MotorShield.h>
#include <EEPROM.h>
#include <Arduino.h>
#include "CharacterWheel.h"

// Pins for nema 11 motor
const int nema17_dirPin  = 4;
const int nema17_stepPin = 3; 
const int nema17_sleepPin   = 2;

// Pins for nema 11 motor
const int nema11_dirPin  = 7;
const int nema11_stepPin = 6; 
const int nema11_sleepPin = 5;

//enable pin for both motors, to turn both on or off
const int enPin = 8;

//Pins for the HM10 module
const int RX = 13; 
const int TX   = 12;

//nums to refer to motors
const int nema17 = 177;
const int nema11 = 111;


SoftwareSerial HM10(TX, RX); // RX and TX are flipped
char appData;  
String inData = "";

void setup()

{
  //setup hm-10
  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  HM10.begin(9600); // set HM10 serial at 9600 baud rate

  
  pinMode(nema17_stepPin,OUTPUT); 
  pinMode(nema17_dirPin,OUTPUT);
  pinMode(nema17_sleepPin,OUTPUT);

   
  pinMode(nema11_stepPin,OUTPUT); 
  pinMode(nema11_dirPin,OUTPUT);
  pinMode(nema11_sleepPin,OUTPUT);  

  pinMode(enPin,OUTPUT);

  //enPin and sleepPins are negated, i.e driver is enabled when enPin is low
  digitalWrite(enPin,LOW);
  digitalWrite(nema17_sleepPin,LOW);
  digitalWrite(nema11_sleepPin,LOW);

  Wheel wheel;
}


void loop()

{
  
  HM10.listen();  // listen the HM10 port

  while (HM10.available() > 0) {   // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData);  // save the data in string format
    Serial.write(appData);
  }

  if (Serial.available()) {           // Read user input if available.
    delay(10);
    HM10.write(Serial.read());
  }

}

void changeDirection(int motor, int state) {
  if(motor == nema11) {
    digitalWrite(nema11_dirPin,state); 
  } else if(motor == nema17) {
    digitalWrite(nema17_dirPin,state); 
  }
}

void runSteps(int motor, int numSteps) {

  if(motor == nema11) {

    // Makes 200 pulses for making one full cycle rotation
    // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
    for(int x = 0; x < numSteps; x++) {
      digitalWrite(nema11_stepPin,HIGH); 
      delayMicroseconds(100); 

      digitalWrite(nema11_stepPin,LOW); 
      delayMicroseconds(100); 
    }
    delay(1000); // One second delay

  } else if(motor == nema17) {
    
    // Makes 200 pulses for making one full cycle rotation
    // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
    for(int x = 0; x < numSteps; x++) {
      digitalWrite(nema17_stepPin,HIGH); 
      delayMicroseconds(100); 

      digitalWrite(nema17_stepPin,LOW); 
      delayMicroseconds(100); 
    }
    delay(1000); // One second delay

  }

}

//to change between 1 character and the other on the driving gear you need 20 full steps
//to change between 1 wheel and the other on the linear actuator you need 665 full steps
