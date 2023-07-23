#include <Adafruit_MotorShield.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "CharacterWheel.h"
#include "DriverPins.h"
#include "WheelArray.h"
#include "MotorControl.h"


class MyCallbacks: public BLECharacteristicCallbacks {

  public:
    MyCallbacks(MotorControl* controller, WheelArray* wheels) : controller(controller), wheels(wheels) {}

    void onWrite(BLECharacteristic *pCharacteristic) {

      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {

        // Reset function
        if(value.compare("RESET") == 0){
          Serial.println("Reset Requested");
          wheels->reset();
          pCharacteristic->setValue("Reset Request Successful");

        } else if(value.compare("OFF")==0){
          wheels->reset();
          pCharacteristic->setValue("Shutting Down System");
          pCharacteristic->notify();
          delay(200);
          digitalWrite(PS_EN, HIGH);
        } else if(value.compare("ESTOP")==0){
          pCharacteristic->setValue("Turning Power off to the system");
          pCharacteristic->notify();
          delay(200);
          digitalWrite(PS_EN, HIGH);
        }
        // run a specific motor in a specific direction for a specifc num of steps.
        // format: [f/r]nema[11/17][numsteps] : run nema [11/17] [forward/reverse] for [numsteps]
        //
        else if(value[0] == 'f' || value[0] == 'r') {
          char stepString[6];
          char motorchar;
          int steps; 
          
          //get the motor string
          motorchar = value[1];
          
          //get the step string
          int loopnum;
          for (loopnum = 2; loopnum < value.length(); loopnum++)
              stepString[loopnum-2] = value[loopnum];
          stepString[loopnum-2] = '\0';

          //change cstring array to intger
          steps = std::atoi(stepString);

          //NOTE: d refers to driving gear, l refers to the linear actuator
                //f is forward, r is reverse
          if (motorchar == 'd')
          {
            if (value[0] == 'f')
            {
              controller->changeDirection(nema11,forward);
            } else {
              controller->changeDirection(nema11,backward);

            }
            controller->runSteps(nema11, steps);
            pCharacteristic->setValue("Ran nema 11 motor for " + std::to_string(steps) + value[0]);
          } 
          else if (motorchar == 'l') {
            if (value[0] == 'f')
            {
              controller->changeDirection(nema17,forward);
            } else {
              controller->changeDirection(nema17,backward);
            }
            //if(steps < 13250)
            controller->runSteps(nema17, steps);
            pCharacteristic->setValue("Ran nema 17 motor for " + std::to_string(steps) + value[0]);
          }

        } 
        // else its an equation: 
        else {

          Serial.println("Equation is: ");
          for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);
          pCharacteristic->setValue("Equation received is: " + value);

          //TODO: add some checking to make sure that equation is less than 20 characters
          // and that equation has correct symbols.
          wheels->write(value);
          
        }

        //notify tells the phone that charactersitic changed
        pCharacteristic->notify();
      }
    }

    //HELPER FUNCTIONS
    void displaySteps(int steps) {
        Serial.println("*********");
        Serial.print("Number of Steps: ");
        // for (int i = 0; i < value.length(); i++)
          Serial.print(steps);

        Serial.println();
        Serial.println("*********");
    }
  
  private:
    MotorControl* controller;
    WheelArray* wheels;
};

MotorControl controller;
WheelArray wheels;


void setup()

{
  Serial.begin(115200);
  delay(2000);

  //Bluetooth device configs
  BLEDevice::init("BIT_ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks(&controller, &wheels));
  pCharacteristic->setValue("Hello Ian");
  pService->start();

  //Advertise the bluetooth service
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

}


void loop()

{


}

// void runSteps(int motor, int numSteps) {

//   if(motor == nema11) {

//     // Makes 200 pulses for making one full cycle rotation
//     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//     for(int x = 0; x < numSteps; x++) {
//       digitalWrite(nema11_stepPin,HIGH); 
//       delayMicroseconds(500000); 

//       digitalWrite(nema11_stepPin,LOW); 
//       delayMicroseconds(500000); 
//     }
//     delay(1000); // One second delay

//   } else if(motor == nema17) {
    
//     // Makes 200 pulses for making one full cycle rotation
//     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//     for(int x = 0; x < numSteps; x++) {
//       digitalWrite(nema17_stepPin,HIGH); 
//       delayMicroseconds(1000); 

//       digitalWrite(nema17_stepPin,LOW); 
//       delayMicroseconds(1000); 
//     }
//     delay(1000); // One second delay

//   }

// }


//***********************NOTES***********************//
//HIGH MAKES motor move backwards - to base
// LOW makes motor move forwards
//to change between 1 character and the other on the driving gear you need 20 full steps - nema 11 
//to change between 1 wheel and the other on the linear actuator you need 665 full steps - nema17

//***********************Change Log***********************//
//Added toBase() and global variable nema17_pos
//    - Variable increments every time the nema17 poart of runSteps is called
//    - toBase() changes direction and decreases by that number of steps
