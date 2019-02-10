//Tender Descender Code
//UNCC Rocketry 2018-2019
//This program uses a Teensy 3.2 and a DFRobot 10DOF IMU.
//Last Updated: Feb. 09 2019

/*The height is zeroed/tared on power. 
  Once takeoff is detected the program updates the maximum altitude read.
  Once altitude is decreasing, the program sets the "apogee" bit to "true"
  On descent, if the altitude surpasses the threshold height, the program begins to look for sudden changes in the magnitude of the acceleration (all three axes)
  If threshold altitude is surpassed on descent, and an acceleration impulse is detected, both e-matches (e1 and e2), as well as the Teensy's on-board LED are set to HIGH.  */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "DFRobot_BMP280.h"

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

DFRobot_BMP280 bmp;
bool tareBit = false;
bool takeoff = false;
bool apogee = false;
bool timerBool = false;

int timerBoolVal = 60000; //[milliseconds] We want to make sure that at least 60 seconds have passed before we can fire the ematches.


int startHeight = 0;
volatile int maxHeight = 0;


unsigned long startTimer;
unsigned long endTimer;



int e1 = 23;
int e2 = 22;
int led = 13;

elapsedMillis time_elapsed;

double xacc;
double yacc;
double zacc;

void setup()
{
  maxHeight = 0;
  
 /* Initialise the barometer */
  while (!bmp.begin()) {  
    delay(1000);
  }

  /* Initialise the accelerometer */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
  }

  accel.setRange(ADXL345_RANGE_16_G);
  pinMode(led,OUTPUT);
  pinMode(e1, OUTPUT);
  pinMode(e2, OUTPUT);
}
void loop()
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  
  float pressure = bmp.readPressureValue();
  float temperature = bmp.readTemperatureValue();
  xacc = event.acceleration.x;
  yacc = event.acceleration.y;
  zacc = event.acceleration.z;

  /* Zero the height */ 
  if (tareBit == false)
  {   
    delay(100);
    startHeight = bmp.readAltitudeValue();
    tareBit = true;
  }

  /* Tare the altitude, and convert to feet. */
  int alt = int((bmp.readAltitudeValue()-startHeight)*3.28084);

  /*Determine magnitude of acceleration*/
  double acc_mag = sqrt(xacc*xacc+yacc*yacc+zacc*zacc); 


  /*Detect Takeoff*/
  if ((acc_mag > 20)&&(takeoff==false))
  {
    takeoff = true;
    startTimer = millis();
  }

  if ((takeoff == true) && (apogee==false))
  {
    /* Compare current height to the maximum height & update if necessary. */
    alt = int((bmp.readAltitudeValue()-startHeight)*3.28084);
    if (alt > maxHeight+3)
    {
      maxHeight = alt;
    }    
    
  delay(50); //Delay to decrease chances of capturing a spike
    
    /* If current height is less than 10 feet of the maximum recorded height, the rocket has reached apogee and is descending. */
    if ((alt < maxHeight-10) && (apogee == false))
    
    {
      delay(100); //wait one hundred milliseconds and check again to make sure we didn't capture a spike.
      alt = int((bmp.readAltitudeValue()-startHeight)*3.28084);
      if (alt < maxHeight - 10) //
      {
        apogee=true;
      }
    }
  
  }

  if (apogee==true)
  {
    /* As the rocket is descending, if an altitude of less than 20ft is observed, and the magnitude acceleration is greater than 20 and specified time out has happened.
    the two e-matches and the led pins on-board the Teensy 3.2 are set to HIGH. */
    if (timerBool == true)
   {
      alt = int((bmp.readAltitudeValue()-startHeight)*3.28084);
      if ((alt < 20) && (acc_mag > 20))
      {
        digitalWrite(led,HIGH);
        digitalWrite(e1,HIGH);
        digitalWrite(e2,HIGH);
      }
    }
  }

  if (takeoff == true)
  {
    endTimer = millis();   
    if ((endTimer - startTimer) > timerBoolVal)
    {
       timerBool = true;
       }
  }
  }
