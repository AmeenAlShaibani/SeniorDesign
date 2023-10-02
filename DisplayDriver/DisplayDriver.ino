#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "CharacterWheel.h"
#include "Configs.h"
#include "WheelsManager.h"
#include "MotorControl.h"

//Callback function that gets called whenever the ERC characteristic is written to.
class MyCallbacks: public BLECharacteristicCallbacks {

  private:
    MotorControl *controller;
    WheelsManager *wheels;

  public:

    //Initializer List for Controller and Wheel Array
    MyCallbacks(MotorControl *Icontroller, WheelsManager *Iwheels) : controller(Icontroller), wheels(Iwheels) {}


    void onWrite(BLECharacteristic *ercCharacteristic) {

      std::string value = ercCharacteristic->getValue();

      if (value.length() > 0) {
        // Reset string
        if(value.compare("RESET") == 0) {
          Serial.println("Reset Requested");
          wheels->reset();
          ercCharacteristic->setValue("Reset Request Successful");

        // Turn Off string
        } else if(value.compare("OFF")==0){
          wheels->reset();
          ercCharacteristic->setValue("Shutting Down System");
          ercCharacteristic->notify();
          delay(BLE_NOTIF_DELAY_ms);
          digitalWrite(PS_EN, HIGH);
        } 
        // HW DEBUGGING TOOL: run a specific motor in a specific direction for a specifc num of steps.
        // format: [f/r][d/l][numsteps] which translates to : " run [Drive/LinearActuator Motor] [forward/reverse] for [numsteps] "
            // d : Drive Gear (Nema 11)
            // l : Linear Actuator (Nema17)
            // f/r : Forward / Backward
            // usage example: fd100: "run the drive gear forward for 100 steps"
            
        
        else if(value[0] == 'f' || value[0] == 'r') {
          char stepString[6]; //the char array that represents the number of steps to move. 
                              //Note: It is a size of 6 since the maximum number of steps you can use for a motor is 12635 as defined in Configs.h
          char motorChar; //char to represent whether we are using NEMA11 or 17
          int steps; 
          
          //get the motor string
          motorChar = value[1];
          
          int loopNum;
          //start after the first 2 characters to grab all numbers used to represent steps
          for (loopNum = 2; loopNum < value.length(); loopNum++)
              stepString[loopNum - 2] = value[loopNum];

          //Add null char to end  make stepString a valid cstring
          stepString[loopNum - 2] = '\0';

          //change cstring array to int
          steps = std::atoi(stepString);

          if (motorChar == 'd')
          {
            if (value[0] == 'f')
            {
              controller->changeDirection(NEMA11,FORWARD);
            } else {
              controller->changeDirection(NEMA11,BACKWARD);
            }
            controller->runSteps(NEMA11, steps);
            ercCharacteristic->setValue("Ran nema 11 motor for " + std::to_string(steps) + value[0]);
          } 
          else if (motorChar == 'l') {
            if (value[0] == 'f')
            {
              controller->changeDirection(NEMA17,FORWARD);
            } else {
              controller->changeDirection(NEMA17,BACKWARD);
            } 
            controller->runSteps(NEMA17, steps);
            ercCharacteristic->setValue("Ran nema 17 motor for " + std::to_string(steps) + value[0]);
          }
        } 
        // else its an equation: 
        else {
          Serial.println("Equation is: ");
          for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);

          ercCharacteristic->setValue("Equation received is: " + value);

          //TODO: add some checking to make sure that equation is less than 20 characters
          // and that equation has correct symbols.
          wheels->write(value);
        }
        //notify tells the phone that charactersitic changed
        ercCharacteristic->notify();
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
};

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  delay(STARTUP_DELAY_ms);

  //Define a reference to the controller, as is usually standard for singleton classes
  //Since it signifies that alias for an existing object, and that no ownership semantics (like deallocation) 
  MotorControl& controller = MotorControl::getInstance();
  WheelsManager wheels;


//Set up the BLE functions

  //Bluetooth device configs
  BLEDevice::init("BIT_ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *ercCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  //Pass a pointer to the controller reference, and a pointer to the wheels for the callback function 
  ercCharacteristic->setCallbacks(new MyCallbacks(&controller, &wheels));
  ercCharacteristic->setValue("ERC"); //Equation Reciever Characteristic
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

//   if(motor == NEMA11) {

//     // Makes 200 pulses for making one full cycle rotation
//     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//     for(int x = 0; x < numSteps; x++) {
//       digitalWrite(NEMA11_STEP_PIN,HIGH); 
//       delayMicroseconds(500000); 

//       digitalWrite(NEMA11_STEP_PIN,LOW); 
//       delayMicroseconds(500000); 
//     }
//     delay(1000); // One second delay

//   } else if(motor == NEMA17) {
    
//     // Makes 200 pulses for making one full cycle rotation
//     // for 1/8 microstep, you need 200*8 pulses for 1 rev. 
//     for(int x = 0; x < numSteps; x++) {
//       digitalWrite(NEMA17_STEP_PIN,HIGH); 
//       delayMicroseconds(1000); 

//       digitalWrite(NEMA17_STEP_PIN,LOW); 
//       delayMicroseconds(1000); 
//     }
//     delay(1000); // One second delay

//   }

// }


//***********************NOTES***********************//
//HIGH MAKES motor move backwards - to base
// LOW makes motor move forwards
//to change between 1 character and the other on the driving gear you need 20 full steps - nema 11 
//to change between 1 wheel and the other on the linear actuator you need 665 full steps - NEMA17

//***********************Change Log***********************//
//Added toBase() and global variable nema17_pos
//    - Variable increments every time the NEMA17 poart of runSteps is called
//    - toBase() changes direction and decreases by that number of steps
