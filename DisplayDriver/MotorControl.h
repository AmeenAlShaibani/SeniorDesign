/*

This file defines a singleton class for a motor controller, to control the stepper motors. 

*/

#pragma once

#include "Configs.h"
#include <Arduino.h>

class MotorControl {
public:

    //return a reference to the MotorControl instance
    static MotorControl& getInstance() {
      //The static variable constructor is called on first use and never again, returning the
      // same variable each time the function is called afterwards
        static MotorControl instance; 
        return instance;
    }

    // Delete copy constructor and assignment operator to prevent copying
    MotorControl(MotorControl const&) = delete;
    void operator=(MotorControl const&) = delete;

 
  void changeDirection(int motor, int state) {
    if(motor == NEMA11) {
      digitalWrite(NEMA11_DIRPIN,state); 
    } else if(motor == NEMA17) {
      digitalWrite(NEMA17_DIR_PIN,state); 
    }
  }

  void runSteps(int motor, int numSteps) {

    if(motor == NEMA11) {
      
      //used to prevent NEMA11 from rotating more than needed (full wheel rotation)
      if(numSteps > NEMA11_STEPLIMIT) {
        Serial.println("The number of steps is greater than the NEMA11 limit: " + NEMA11_STEPLIMIT);
        return;
      }

      // Makes 200 pulses for making one full cycle rotation
      // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
      for(int x = 0; x < numSteps; x++) {
        digitalWrite(NEMA11_STEP_PIN,HIGH); 
        delayMicroseconds(NEMA11_PULSE_DELAY_us); 

        digitalWrite(NEMA11_STEP_PIN,LOW); 
        delayMicroseconds(NEMA11_PULSE_DELAY_us); 
      }
      delay(MOTOR_MOVEMENT_DELAY_ms); 

    } else if(motor == NEMA17) {

      //Used to prevent linear actuator from Stalling
      if(numSteps > NEMA17_STEPLIMIT) {
        Serial.println("The number of steps is greater than the NEMA17 limit: " + NEMA17_STEPLIMIT);
        return;
      }
      
      //The ARB_NEMA17_LIMIT is a number that specifies the max number of steps that the nema17 can move at once.
      //We do not know why this happens, but to avoid this issue, we simply keep a counter, and decrement it ARB_NEMA17_LIMIT steps at a time
      // to reach the desired number of steps 
      while(numSteps > ARB_NEMA17_LIMIT) {
        for(int x = 0; x < ARB_NEMA17_LIMIT; x++) {
          digitalWrite(NEMA17_STEP_PIN,HIGH); 
          delayMicroseconds(NEMA17_PULSE_DELAY_us); 

          digitalWrite(NEMA17_STEP_PIN,LOW); 
          delayMicroseconds(NEMA17_PULSE_DELAY_us); 
        }
        delay(MOTOR_MOVEMENT_DELAY_ms); 
        numSteps = numSteps - ARB_NEMA17_LIMIT;
      }

      // if the number of steps is less than ARB_NEMA17_LIMIT and greater than 0, then move the specified number of steps 
      if(numSteps > 0) {
        for(int x = 0; x < numSteps; x++) {
          digitalWrite(NEMA17_STEP_PIN,HIGH); 
          delayMicroseconds(NEMA17_PULSE_DELAY_us); 

          digitalWrite(NEMA17_STEP_PIN,LOW); 
          delayMicroseconds(NEMA17_PULSE_DELAY_us); 
        }
        delay(MOTOR_MOVEMENT_DELAY_ms); 
      }
    }
  }

private:

 MotorControl() {
      pinMode(NEMA17_STEP_PIN,OUTPUT); 
      pinMode(NEMA17_DIR_PIN,OUTPUT);
      pinMode(NEMA17_SLEEP_PIN,OUTPUT);

      pinMode(NEMA11_STEP_PIN,OUTPUT); 
      pinMode(NEMA11_DIRPIN,OUTPUT);
      pinMode(NEMA11_SLEEP_PIN,OUTPUT);  

      pinMode(M_EN,OUTPUT);
      pinMode(PS_EN,OUTPUT);

      //enPin and sleepPins are idle high, i.e driver is enabled when enPin is low, and motors sleep when LOW
      digitalWrite(M_EN,LOW);
      digitalWrite(NEMA17_SLEEP_PIN,HIGH);
      digitalWrite(NEMA11_SLEEP_PIN,HIGH);

      //default motor to run forward
      digitalWrite(NEMA11_DIRPIN,LOW); 
      digitalWrite(NEMA17_DIR_PIN,LOW); 
  }

  
};