#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  while (!Serial);
  Serial.println("Stepper test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  myMotor->setSpeed(100);  // 10 rpm

 // Serial.println("Microstep steps");
}

void loop() {
  Serial.println("Single coil steps");
  myMotor->step(500, BACKWARD, MICROSTEP);
  delay(1000);
  // myMotor->step(500, BACKWARD, SINGLE);

  // Serial.println("Double coil steps");
  // myMotor->step(500, FORWARD, DOUBLE);
  // myMotor->step(500, BACKWARD, DOUBLE);

  // Serial.println("Interleave coil steps");
  // myMotor->step(500, FORWARD, INTERLEAVE);
  // myMotor->step(500, BACKWARD, INTERLEAVE);

  // Serial.println("Microstep steps");
  // myMotor->step(200, FORWARD, MICROSTEP);
  // myMotor->step(200, BACKWARD, MICROSTEP);

  // myMotor->step(1000, FORWARD, INTERLEAVE);
  // delay(5000);
}
