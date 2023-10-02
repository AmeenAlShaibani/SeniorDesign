/*
This file includes all the constants used throughout the project.
*/


#pragma once

#define SERVICE_UUID        "b0c43b43-7026-4d99-be83-4945b2e33b6f"
#define CHARACTERISTIC_UUID "79f8a643-66bb-430b-ab16-41d1a23999e3"

// Pins for nema 17 motor
static const int NEMA17_DIR_PIN  = 5;
static const int NEMA17_STEP_PIN = 18; 
static const int NEMA17_SLEEP_PIN = 19;

// Pins for nema 11 motor
static const int NEMA11_DIRPIN  = 15;
static const int NEMA11_STEP_PIN = 2; 
static const int NEMA11_SLEEP_PIN = 0;

//enable pin for both motors, to turn both on or off
static const int M_EN = 16;

//Power Supply
static const int PS_EN = 27;

//Motors Number Alias
enum MotorType {
    NEMA17 = 177,
    NEMA11 = 111
};

//Definition of forward and backward for motors
//m is used to prevent clash with FORWARD definition in stepper motor library
static const int FORWARD = 0;
static const int BACKWARD = 1;

//step limits for motors based on the size of the box
static const int NEMA11_STEPLIMIT = 500;
static const int NEMA17_STEPLIMIT = 12635;

//Microsecond delays
static const int NEMA11_PULSE_DELAY_us = 4600;
static const int NEMA17_PULSE_DELAY_us = 1000;

//Millisecond delays
static const int STARTUP_DELAY_ms = 2000;
static const int MOTOR_MOVEMENT_DELAY_ms = 200; // this is a delay used after moving a motor. ensures smooth movement. 
static const int BLE_NOTIF_DELAY_ms = 200;

//SERIAL BAUD RATE
static const int SERIAL_BAUDRATE = 115200;

//MAGIC NUMBERS!
static const int ARB_NEMA17_LIMIT = 540; //TODO: for some reason if the nema17 is told to move beyond 540 steps, it does not move. Should investigate.

