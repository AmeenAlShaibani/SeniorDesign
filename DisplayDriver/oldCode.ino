// #include <Adafruit_MotorShield.h>
// #include <EEPROM.h>
// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>
// #include "CharacterWheel.h"
// #include "DriverPins.h"
// #include "WheelArray.h"
// #include "MotorControl.h"


// //refers to the current position of the NEMA17
// int nema17_pos = 0;

// class MyCallbacks: public BLECharacteristicCallbacks {

//   const int NEMA17 = 177;
//   const int NEMA11 = 111;

//   void runSteps(int motor, int numSteps) {

//     if(motor == NEMA11) {
//       // Makes 200 pulses for making one full cycle rotation
//       // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//       for(int x = 0; x < numSteps; x++) {
//         digitalWrite(NEMA11_STEP_PIN,HIGH); 
//         delayMicroseconds(4600); 

//         digitalWrite(NEMA11_STEP_PIN,LOW); 
//         delayMicroseconds(4600); 
//       }
//       delay(500); // One second delay

//     } 
//     else if(motor == NEMA17) {
//       //Add numSteps to global NEMA17 position
//       nema17_pos += numSteps;

//       // Makes 200 pulses for making one full cycle rotation
//       // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//       for(int x = 0; x < numSteps; x++) {
//         digitalWrite(NEMA17_STEP_PIN,HIGH); 
//         delayMicroseconds(1000); 

//         digitalWrite(NEMA17_STEP_PIN,LOW); 
//         delayMicroseconds(1000); 
//       }
//       delay(500); // One second delay
    
//     }

//   }

//   void toBase() { //resets the nema 17 motor to 0
//   // TODO reset all wheels to the null character using nema 11
//   // will need to change the while loop to increment the NEMA17 in 665 step intervals

//     //Set to move backwards
//     digitalWrite(NEMA17_DIR_PIN, HIGH);
//     while(nema17_pos > 0){
//       if(nema17_pos >= 540){
//         runSteps(NEMA17, 540);
//         nema17_pos = nema17_pos - 540;
//       } else {
//         runSteps(NEMA17, nema17_pos);
//         nema17_pos = 0;
//       }
//     }
//     //reset to move forwards
//     digitalWrite(NEMA17_DIR_PIN,LOW);
//   }

//   void displaySteps(int steps) {
//       Serial.println("*********");
//       Serial.print("Number of Steps: ");
//       // for (int i = 0; i < value.length(); i++)
//         Serial.print(steps);

//       Serial.println();
//       Serial.println("*********");
//   }

//   void onWrite(BLECharacteristic *pCharacteristic) {
//     std::string value = pCharacteristic->getValue();

//     if (value.length() > 0) {
//             // Reset function
//       if(value.compare("RESET") == 0){
//         Serial.println("Reset Requested");
//         pCharacteristic->setValue("Reset Request Successful");
//       }
//       // run a specific number of steps
//       else if(value[0] == 'f') {
//         std::string stepString;
//         int steps; 

//         for (int i = 1; i < value.length(); i++)
//           stepString[i-1] = value[i];

//         steps = stoi(stepString);
      
//         //stoi changes string to integer
//         runSteps(NEMA11, steps);
//         runSteps(NEMA17, steps);
//         pCharacteristic->setValue("Ran both motors for " + value + " Steps");
//       } 

//       else if(value[0] == 'r') {
//         std::string stepString;
//         int steps; 

//         for (int i = 1; i < value.length(); i++)
//           stepString[i-1] = value[i];

//         steps = stoi(stepString);
      
//         //stoi changes string to integer
//         runSteps(NEMA11, steps);
//         runSteps(NEMA17, steps);
//         pCharacteristic->setValue("Ran both motors for " + value + " Steps");
//       } 
//  // else its an equation: 
//       else {
//         Serial.println("Equation is: ");
//         for (int i = 0; i < value.length(); i++)
//           Serial.print(value[i]);
//         pCharacteristic->setValue("Equation received is: " + value);
        


//       }
//       //should call toBase();

//       //notify tells the phone that charactersitic changed
//       pCharacteristic->notify();
//     }
//   }
// };

// void setup()

// {
//   Serial.begin(115200);
//   delay(2000);
  
//   pinMode(NEMA17_STEP_PIN,OUTPUT); 
//   pinMode(NEMA17_DIR_PIN,OUTPUT);
//   pinMode(NEMA17_SLEEP_PIN,OUTPUT);

   
//   pinMode(NEMA11_STEP_PIN,OUTPUT); 
//   pinMode(NEMA11_DIRPIN,OUTPUT);
//   pinMode(NEMA11_SLEEP_PIN,OUTPUT);  

//   pinMode(M_EN,OUTPUT);
//   pinMode(PS_EN,OUTPUT);

//   //enPin and sleepPins are negated, i.e driver is enabled when enPin is low, and motors sleep when LOW
//   digitalWrite(M_EN,LOW);
//   digitalWrite(NEMA17_SLEEP_PIN,HIGH);
//   digitalWrite(NEMA11_SLEEP_PIN,HIGH);

//   Wheel wheel;

//   //Bluetooth device configs
//   BLEDevice::init("BIT_ESP32");
//   BLEServer *pServer = BLEDevice::createServer();
//   BLEService *pService = pServer->createService(SERVICE_UUID);
//   BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                          CHARACTERISTIC_UUID,
//                                          BLECharacteristic::PROPERTY_READ |
//                                          BLECharacteristic::PROPERTY_WRITE |
//                                          BLECharacteristic::PROPERTY_NOTIFY |
//                                          BLECharacteristic::PROPERTY_INDICATE
//                                        );
//   pCharacteristic->setCallbacks(new MyCallbacks());
//   pCharacteristic->setValue("Hello Ian");
//   pService->start();

//   //Advertise the bluetooth service
//   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//   pAdvertising->addServiceUUID(SERVICE_UUID);
//   pAdvertising->setScanResponse(true);
//   pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
//   pAdvertising->setMinPreferred(0x12);
//   BLEDevice::startAdvertising();

//   // runSteps(NEMA17, 200);
//   // Serial.write("Ran the nema 17");
//   changeDirection(NEMA11, LOW);
//   changeDirection(NEMA17, LOW);


// }


// void loop()

// {


// }

// void changeDirection(int motor, int state) {
//   if(motor == NEMA11) {
//     digitalWrite(NEMA11_DIRPIN,state); 
//   } else if(motor == NEMA17) {
//     digitalWrite(NEMA17_DIR_PIN,state); 
//   }
// }

// // void runSteps(int motor, int numSteps) {

// //   if(motor == NEMA11) {

// //     // Makes 200 pulses for making one full cycle rotation
// //     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
// //     for(int x = 0; x < numSteps; x++) {
// //       digitalWrite(NEMA11_STEP_PIN,HIGH); 
// //       delayMicroseconds(500000); 

// //       digitalWrite(NEMA11_STEP_PIN,LOW); 
// //       delayMicroseconds(500000); 
// //     }
// //     delay(1000); // One second delay

// //   } else if(motor == NEMA17) {
    
// //     // Makes 200 pulses for making one full cycle rotation
// //     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
// //     for(int x = 0; x < numSteps; x++) {
// //       digitalWrite(NEMA17_STEP_PIN,HIGH); 
// //       delayMicroseconds(1000); 

// //       digitalWrite(NEMA17_STEP_PIN,LOW); 
// //       delayMicroseconds(1000); 
// //     }
// //     delay(1000); // One second delay

// //   }

// // }


// //***********************NOTES***********************//
// //HIGH MAKES motor move backwards - to base
// // LOW makes motor move forwards
// //to change between 1 character and the other on the driving gear you need 20 full steps - nema 11 
// //to change between 1 wheel and the other on the linear actuator you need 665 full steps - NEMA17

// //***********************Change Log***********************//
// //Added toBase() and global variable nema17_pos
// //    - Variable increments every time the NEMA17 poart of runSteps is called
// //    - toBase() changes direction and decreases by that number of steps
