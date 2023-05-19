#pragma once

#define SERVICE_UUID        "b0c43b43-7026-4d99-be83-4945b2e33b6f"
#define CHARACTERISTIC_UUID "79f8a643-66bb-430b-ab16-41d1a23999e3"

// Pins for nema 17 motor
const int nema17_dirPin  = 5;
const int nema17_stepPin = 18; 
const int nema17_sleepPin = 19;

// Pins for nema 11 motor
const int nema11_dirPin  = 15;
const int nema11_stepPin = 2; 
const int nema11_sleepPin = 0;

//enable pin for both motors, to turn both on or off
const int M_EN = 16;
const int PS_EN = 27;

//nums to refer to motors
const int nema17 = 177;
const int nema11 = 111;

const int forward = 0;
const int backward = 1;

//step limits for motors
const int nema11_stepLimit = 500;
const int nema17_stepLimit = 12635;
