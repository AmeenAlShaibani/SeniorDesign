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


// //refers to the current position of the nema17
// int nema17_pos = 0;

// class MyCallbacks: public BLECharacteristicCallbacks {

//   const int nema17 = 177;
//   const int nema11 = 111;

//   void runSteps(int motor, int numSteps) {

//     if(motor == nema11) {
//       // Makes 200 pulses for making one full cycle rotation
//       // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//       for(int x = 0; x < numSteps; x++) {
//         digitalWrite(nema11_stepPin,HIGH); 
//         delayMicroseconds(4600); 

//         digitalWrite(nema11_stepPin,LOW); 
//         delayMicroseconds(4600); 
//       }
//       delay(500); // One second delay

//     } 
//     else if(motor == nema17) {
//       //Add numSteps to global nema17 position
//       nema17_pos += numSteps;

//       // Makes 200 pulses for making one full cycle rotation
//       // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//       for(int x = 0; x < numSteps; x++) {
//         digitalWrite(nema17_stepPin,HIGH); 
//         delayMicroseconds(1000); 

//         digitalWrite(nema17_stepPin,LOW); 
//         delayMicroseconds(1000); 
//       }
//       delay(500); // One second delay
    
//     }

//   }

//   void toBase() { //resets the nema 17 motor to 0
//   // TODO reset all wheels to the null character using nema 11
//   // will need to change the while loop to increment the nema17 in 665 step intervals

//     //Set to move backwards
//     digitalWrite(nema17_dirPin, HIGH);
//     while(nema17_pos > 0){
//       if(nema17_pos >= 540){
//         runSteps(nema17, 540);
//         nema17_pos = nema17_pos - 540;
//       } else {
//         runSteps(nema17, nema17_pos);
//         nema17_pos = 0;
//       }
//     }
//     //reset to move forwards
//     digitalWrite(nema17_dirPin,LOW);
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
//         runSteps(nema11, steps);
//         runSteps(nema17, steps);
//         pCharacteristic->setValue("Ran both motors for " + value + " Steps");
//       } 

//       else if(value[0] == 'r') {
//         std::string stepString;
//         int steps; 

//         for (int i = 1; i < value.length(); i++)
//           stepString[i-1] = value[i];

//         steps = stoi(stepString);
      
//         //stoi changes string to integer
//         runSteps(nema11, steps);
//         runSteps(nema17, steps);
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
  
//   pinMode(nema17_stepPin,OUTPUT); 
//   pinMode(nema17_dirPin,OUTPUT);
//   pinMode(nema17_sleepPin,OUTPUT);

   
//   pinMode(nema11_stepPin,OUTPUT); 
//   pinMode(nema11_dirPin,OUTPUT);
//   pinMode(nema11_sleepPin,OUTPUT);  

//   pinMode(M_EN,OUTPUT);
//   pinMode(PS_EN,OUTPUT);

//   //enPin and sleepPins are negated, i.e driver is enabled when enPin is low, and motors sleep when LOW
//   digitalWrite(M_EN,LOW);
//   digitalWrite(nema17_sleepPin,HIGH);
//   digitalWrite(nema11_sleepPin,HIGH);

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

//   // runSteps(nema17, 200);
//   // Serial.write("Ran the nema 17");
//   changeDirection(nema11, LOW);
//   changeDirection(nema17, LOW);


// }


// void loop()

// {


// }

// void changeDirection(int motor, int state) {
//   if(motor == nema11) {
//     digitalWrite(nema11_dirPin,state); 
//   } else if(motor == nema17) {
//     digitalWrite(nema17_dirPin,state); 
//   }
// }

// // void runSteps(int motor, int numSteps) {

// //   if(motor == nema11) {

// //     // Makes 200 pulses for making one full cycle rotation
// //     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
// //     for(int x = 0; x < numSteps; x++) {
// //       digitalWrite(nema11_stepPin,HIGH); 
// //       delayMicroseconds(500000); 

// //       digitalWrite(nema11_stepPin,LOW); 
// //       delayMicroseconds(500000); 
// //     }
// //     delay(1000); // One second delay

// //   } else if(motor == nema17) {
    
// //     // Makes 200 pulses for making one full cycle rotation
// //     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
// //     for(int x = 0; x < numSteps; x++) {
// //       digitalWrite(nema17_stepPin,HIGH); 
// //       delayMicroseconds(1000); 

// //       digitalWrite(nema17_stepPin,LOW); 
// //       delayMicroseconds(1000); 
// //     }
// //     delay(1000); // One second delay

// //   }

// // }


// //***********************NOTES***********************//
// //HIGH MAKES motor move backwards - to base
// // LOW makes motor move forwards
// //to change between 1 character and the other on the driving gear you need 20 full steps - nema 11 
// //to change between 1 wheel and the other on the linear actuator you need 665 full steps - nema17

// //***********************Change Log***********************//
// //Added toBase() and global variable nema17_pos
// //    - Variable increments every time the nema17 poart of runSteps is called
// //    - toBase() changes direction and decreases by that number of steps
