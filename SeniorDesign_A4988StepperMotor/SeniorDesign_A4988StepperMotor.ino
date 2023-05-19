
// Define stepper motor connections and steps per revolution:
#define dirPin 15
#define stepPin 2
#define stepsPerRevolution 200


#define sleep2 19
#define sleep1 0



void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600);           // set up Serial library at 9600 bps

  //pinMode(sleep2, OUTPUT);
  //pinMode(sleep1, OUTPUT);
  // digitalWrite(sleep1, HIGH);
  // digitalWrite(sleep2, HIGH);

}

void loop() {
  

  // Set the spinning direction clockwise:
  // digitalWrite(dirPin, HIGH);

  // // Spin the stepper motor 1 revolution slowly:
  // for (int i = 0; i < stepsPerRevolution; i++) {
  //   // These four lines result in 1 step:
  //   digitalWrite(stepPin, HIGH);
  //   delayMicroseconds(2000);
  //   digitalWrite(stepPin, LOW);
  //   delayMicroseconds(2000);
  // }

  // delay(1000);

  // // Set the spinning direction counterclockwise:
  // digitalWrite(dirPin, LOW);

  // // Spin the stepper motor 1 revolution quickly:
  // for (int i = 0; i < stepsPerRevolution; i++) {
  //   // These four lines result in 1 step:
  //   digitalWrite(stepPin, HIGH);
  //   delayMicroseconds(1000);
  //   digitalWrite(stepPin, LOW);
  //   delayMicroseconds(1000);
  // }

  // delay(1000);

  // Set the spinning direction clockwise:
  digitalWrite(dirPin, LOW);

  long int t1 = millis();

  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(4600);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(4600);
  }

   long int t2 = millis();
  Serial.println((t2-t1));

  delay(3000);

  // // Set the spinning direction counterclockwise:
  // digitalWrite(dirPin, LOW);

  // //Spin the stepper motor 5 revolutions fast:
  // for (int i = 0; i < 5 * stepsPerRevolution; i++) {
  //   // These four lines result in 1 step:
  //   digitalWrite(stepPin, HIGH);
  //   delayMicroseconds(550);
  //   digitalWrite(stepPin, LOW);
  //   delayMicroseconds(550);
  // }

  // delay(1000);
}
