#pragma once

#include "DriverPins.h"
#include <Arduino.h>

//NOTE: this needs to be changed if microstepping is used. 

class MotorControl {
public:

    static MotorControl& getInstance() {
        static MotorControl instance; // Guaranteed to be destroyed and instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator to prevent copying
    MotorControl(MotorControl const&) = delete;
    void operator=(MotorControl const&) = delete;

  MotorControl() {
    //TODO: setting pinmode multile times in wheel array and character wheel (having multiple controllers) may be a problem, check if it crashes
      pinMode(nema17_stepPin,OUTPUT); 
      pinMode(nema17_dirPin,OUTPUT);
      pinMode(nema17_sleepPin,OUTPUT);

      pinMode(nema11_stepPin,OUTPUT); 
      pinMode(nema11_dirPin,OUTPUT);
      pinMode(nema11_sleepPin,OUTPUT);  

      pinMode(M_EN,OUTPUT);
      pinMode(PS_EN,OUTPUT);

      //enPin and sleepPins are negated, i.e driver is enaled when enPin is low, and motors sleep when LOW
      digitalWrite(M_EN,LOW);
      digitalWrite(nema17_sleepPin,HIGH);
      digitalWrite(nema11_sleepPin,HIGH);

      //default motor to run forward
      digitalWrite(nema11_dirPin,LOW); 
      digitalWrite(nema17_dirPin,LOW); 
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
      
      if(numSteps > nema11_stepLimit) {
        Serial.println("The number of steps is greater than the nema11 limit: " + nema11_stepLimit);
        return;
      }

      // Makes 200 pulses for making one full cycle rotation
      // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
      for(int x = 0; x < numSteps; x++) {
        digitalWrite(nema11_stepPin,HIGH); 
        delayMicroseconds(4600); 

        digitalWrite(nema11_stepPin,LOW); 
        delayMicroseconds(4600); 
      }
      delay(200); 

    } else if(motor == nema17) {

      if(numSteps > nema17_stepLimit) {
        Serial.println("The number of steps is greater than the nema17 limit: " + nema17_stepLimit);
        return;
      }
      
      while(numSteps > 540) {
        for(int x = 0; x < 540; x++) {
          digitalWrite(nema17_stepPin,HIGH); 
          delayMicroseconds(1000); 

          digitalWrite(nema17_stepPin,LOW); 
          delayMicroseconds(1000); 
        }
        delay(200); 
        numSteps = numSteps - 540;
      }


      if(numSteps > 0) {
        for(int x = 0; x < numSteps; x++) {
          digitalWrite(nema17_stepPin,HIGH); 
          delayMicroseconds(1000); 

          digitalWrite(nema17_stepPin,LOW); 
          delayMicroseconds(1000); 
        }
        delay(200); 
      }
    }
  }

private:

  
};