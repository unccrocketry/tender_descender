/*
 DFRobot 10DOF IMU + Teensy 3.5
 Tender Descender Logging Code
 
 Outputs data to the built-in SD card as a .txt file in the following format:
 [height],[pressure],[temperature],[ax],[ay],[az],[roll],[pitch],[yaw]
 
 
 
 
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11, pin 7 on Teensy with audio board
 ** MISO - pin 12
 ** CLK - pin 13, pin 14 on Teensy with audio board
 ** CS - pin 4,  pin 10 on Teensy with audio board
 
 
 This example code is in the public domain.
 	 
 */

#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "DFRobot_BMP280.h"
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
#include <FreeSixIMU.h>


// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20


const int chipSelect = BUILTIN_SDCARD;
DFRobot_BMP280 bmp;
bool tareBit = false;
int startHeight = 0;
FreeSixIMU sixDOF = FreeSixIMU();
volatile bool firstBoot = true; //

double xacc;
double yacc;
double zacc;

void setup()
{
  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  
 // Open serial communications and wait for port to open:
//  Serial.begin(9600);
  while (!bmp.begin()) {  
//    Serial.println("Error initializing BMP280!");
    delay(1000);
  }

  sixDOF.init(); //begin the IMU

  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
//    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
//    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
  }

  accel.setRange(ADXL345_RANGE_16_G); //Set accelerometer range to 16 G's
  sixDOF.init(); //begin the IMU
}

void loop()
{

  float angles[3];
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  if (tareBit == false)
  {
    delay(100);
    startHeight = bmp.readAltitudeValue();
    tareBit = true;
    }

  
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  sixDOF.getEuler(angles);

  int alt = int((bmp.readAltitudeValue()-startHeight)*3.28084);
  float pressure = bmp.readPressureValue();
  float temperature = bmp.readTemperatureValue();
  xacc = event.acceleration.x;
  yacc = event.acceleration.y;
  zacc = event.acceleration.z;

  float roll = angles[0];
  float pitch = angles[1];
  float yaw = angles[2];
  
  // if the file is available, write to it:
  if (dataFile) {

    if (firstBoot==true)
    {
      dataFile.println("------------------Starting To Log-------------------");
      firstBoot = false;
    }

    dataFile.print(alt); dataFile.print(",");
    dataFile.print(pressure); dataFile.print(",");
    dataFile.print(temperature); dataFile.print(",");
    dataFile.print(xacc); dataFile.print(",");
    dataFile.print(yacc); dataFile.print(",");
    dataFile.print(zacc); dataFile.print(",");
    dataFile.print(roll); dataFile.print(",");
    dataFile.print(pitch); dataFile.print(",");
    dataFile.print(yaw); dataFile.println(",");
  
    dataFile.close();
    // print to the serial port too:
  }  
  // if the file isn't open, pop up an error:
  else {} 
  }
