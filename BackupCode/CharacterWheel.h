#pragma once


#include "MotorControl.h"
#include <Arduino.h>


class Wheel {
public:
    Wheel()  {
        current_index = 0;
    }

    void rotate_left(int steps) {
        //TODO: change the forward/backward based on testing 
        controller.changeDirection(nema11, backward);
        controller.runSteps(nema11,steps*stepCharOffset);

        current_index = (current_index - steps + bufferSize) % bufferSize;
    }

    void rotate_right(int steps) {
        //TODO: change the forward/backward based on testing
        
        controller.changeDirection(nema11, forward);
        controller.runSteps(nema11,steps*stepCharOffset);

;
        current_index = (current_index + steps) % bufferSize;
    }

    void rotate_to(char target) {
        int target_pos = find(target);
        if (target_pos == -1) {
            //TODO: Serial may not work in this header file 
            Serial.println("Target character not found in wheel.");
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

    char get_top() const {
        return buffer[current_index];
    }

private:
    static const size_t bufferSize = 26;

    //20 steps to change between characters on wheel
    static const size_t stepCharOffset = 20;


    char buffer[bufferSize] = {'_','A', 'B', 'C', 'X', 'Y', 'M', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', 'x', '/', '(', ')', '^', '='};
    size_t current_index;

    MotorControl controller;

    int find(char target) {
        for (int i = 0; i < bufferSize; ++i) {
            if (buffer[i] == target) {
                return i;
            }
        }
        return -1;
    }

    int distance(int index1, int index2) {
        return index2 - index1;
    }
};
