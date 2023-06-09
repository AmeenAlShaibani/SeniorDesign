#include <SoftwareSerial.h>


SoftwareSerial HM10(2, 3); // RX = 2, TX = 3

char appData;  

String inData = "";

void setup()

{

  Serial.begin(9600);

  Serial.println("HM10 serial started at 9600");

  HM10.begin(9600); // set HM10 serial at 9600 baud rate

}


void loop()

{

  HM10.listen();  // listen the HM10 port

  while (HM10.available() > 0) {   // if HM10 sends something then read

    appData = HM10.read();

    inData = String(appData);  // save the data in string format

    Serial.write(appData);

  }


 

  if (Serial.available()) {           // Read user input if available.

    delay(10);

    HM10.write(Serial.read());

  }

}

