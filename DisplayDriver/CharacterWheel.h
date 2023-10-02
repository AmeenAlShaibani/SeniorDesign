/*
This file defines a class for a single character wheel, and includes functions to rotate the wheel to the desired character
*/

#pragma once

#include "MotorControl.h"
#include <Arduino.h>

class Wheel {
public:
    Wheel()  {
        controller = &MotorControl::getInstance();
        current_index = 0;
    }

    void rotate_to(char target) {
        int target_pos = find(target);
        if (target_pos == -1) {
            // Handle error: target character not found in wheel.
            Serial.println("Could not find the target");
            return;
        }

        int right_steps = (target_pos - current_index + bufferSize) % bufferSize;
        int left_steps = (current_index - target_pos + bufferSize) % bufferSize;

        if (left_steps <= right_steps) {
            rotate_left(left_steps);
        } else {
            rotate_right(right_steps);
        }
    }

    //use const to signify that this function must not be used to change a variable. 
    //TODO: see if this should be used in the get position function in the wheel array. 
    char get_top() const {
        return buffer[current_index];
    }

private:
    static const size_t bufferSize = 25;
    static const size_t stepCharOffset = 20;

    char buffer[bufferSize] = {'_','a', 'b', 'c', 'x', 'y', 'm', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '(', ')', '^', '='};
    size_t current_index;

    MotorControl* controller;

    void rotate_left(int steps) {
        controller->changeDirection(NEMA11, BACKWARD);
        controller->runSteps(NEMA11,steps*stepCharOffset);

        current_index = (current_index - steps + bufferSize) % bufferSize;
    }

    void rotate_right(int steps) {
        controller->changeDirection(NEMA11, FORWARD);
        controller->runSteps(NEMA11,steps*stepCharOffset);

        current_index = (current_index + steps) % bufferSize;
    }

    //used to find index of target char
    int find(char target) {
        for (int i = 0; i < bufferSize; i++) {
            if (buffer[i] == target) {
                return i;
            }
        }
        return -1;
    }
};
