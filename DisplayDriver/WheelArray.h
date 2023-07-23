#pragma once

#include "CharacterWheel.h"
#include "MotorControl.h"
#include <Arduino.h>

class WheelArray {
public:
    WheelArray() {
        controller = &MotorControl::getInstance();
        motor_index = 0;
        curr_equationLength = 0;
        new_equationLength = 0;
    }

    void write(std::string equation) {
        new_equationLength = equation.length();
        reset();

        for (int i = 0; i < new_equationLength; i++) {
            //move linear actuator to correct wheel:
            moveTo(i);

            Serial.println("Moved to new wheel");

            //rotate the wheels to the correct character
            wheels[i].rotate_to(equation.at(i));

            Serial.println("Changed the character");
            motor_index = i; 
        }
    }

    void reset() {
        for (int i = motor_index; i >= 0; i--) {
            //move linear actuator to correct wheel:
            moveTo(i);

            //rotate the wheels to the correct character
            wheels[i].rotate_to('_');

            motor_index = i; 
            Serial.println("I reset the motors");
        }
    }

    int getPosition() {
      return motor_index;
    }

private:
    static const size_t numWheels = 20;
    static const size_t stepOffset = 665;

    size_t motor_index;
    size_t curr_equationLength;
    size_t new_equationLength;

    MotorControl* controller;
    Wheel wheels[numWheels];

    void moveTo(int goal) {
      int distance = goal - motor_index;

      //move back, since index 0 is the leftmost wheel, since we write from left to right
      if(distance > 0) {
        controller->changeDirection(nema17, backward);
        controller->runSteps(nema17, distance*stepOffset);
      } 
      //move forward 
      else if(distance < 0) {
        distance = std::abs(distance);
        controller->changeDirection(nema17, forward);
        controller->runSteps(nema17, distance*stepOffset);
      }
    }
};


//Optimized motor control: 
//TODO: change all for loops that run backwards and say i == 0 or such in the condition

        // if (motor_index == 0 && curr_equationLength == 0)
        // {
        //     //write equation from left to right
        //     for (int i = 0; i < new_equationLength; i++) {

        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to(equation[i]);
        //         motor_index == i; 
        //     } 

        // } else if (curr_equationLength < new_equationLength && motor_index == 0 ) {

        //     // Write the new length, then null to current length 

        //     //write equation from left to right
        //     int i = 0;
        //     for (i = 0; i < new_equationLength; i++) {

        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to(equation[i]);
        //         motor_index == i; 
        //     } 

        //     for (; i < new_equationLength; i++)
        //     {
        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to("_");
        //         motor_index == i; 
        //     }
            

        // } else if (curr_equationLength < new_equationLength) {

        //     //go to the new equation and write backwards

        //     for (int i = new_equationLength-1; i == 0; i--) {
        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to(equation[i]);
        //         motor_index == i; 
        //     } 
            
        // } else if (curr_equationLength > new_equationLength) {

        //     // null backwards until the new length, and then write backwards 

        //     for (int i = curr_equationLength-1; i == new_equationLength; i--) {
        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to("_");
        //         motor_index == i; 
        //     } 

        //     for (int i = new_equationLength-1; i == 0; i--) {
        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to(equation[i]);
        //         motor_index == i; 
        //     } 
            
        // } else if (curr_equationLength == new_equationLength && motor_index == curr_equationLength-1) {

        //     // Write backwards
        //     for (int i = curr_equationLength-1; i == 0; i--) {
        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to(equation[i]);
        //         motor_index == i; 
        //     } 

            
        // } else if (curr_equationLength == new_equationLength && motor_index == 0) {
        //     //write forwards

        //     //write equation from left to right
        //     for (int i = 0; i < new_equationLength; i++) {

        //         //move linear actuator to correct wheel:
        //         moveTo(i);

        //         //rotate the wheels to the correct character
        //         wheels[i] = rotate_to(equation[i]);
        //         motor_index == i; 
        //     } 

        // }
    