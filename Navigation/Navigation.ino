  //Navigation.ino

//MAX/MIN FOR MACRO MOTOR
int motorLowForward   = 0;    //with sand 14 // WITHOUT SAND 8
int motorHighForward  = 10;   //with sand 25 // WITHOUT SAND 15
int motorLowBackward  = 5;    //with sand 11 // WITHOUT SAND 5
int motorHighBackward = 11;   //with sand 20 // WITHOUT SAND 11
int motorLowDig       = 8;   //with sand 18 // WITHOUT SAND 8
int motorHighDig      = 15;   //with sand 30 // WITHOUT SAND 15
int motorLowG         = 13;   //with sand 25  // WITHOUT SAND 13
int motorHighG        = 16;   //with sand 35 // WITHOUT SAND 16

#define BUCKET_DRIVE_ANGLE_SET   40
#define BUCKET_DIG_ANGLE_SET     0
#define BUCKET_DUMP_ANGLE_SET    90
#define BUCKET_DIG_DRIVE_BEGIN_ANGLE 15
#define DIG_DRIVE_DISTANCE       75
#define TRAVERSE_DRIVE_DISTANCE  400

#define DELAY_BUCKET_EMPTY 2500

#include <FastTransfer.h>
#include <EasyTransfer.h>
#include <EasyTransferCRC.h>
#include <Wire.h>
#include <digitalWriteFast.h>
//#include <Average.h>
#include <PID.h>
#include <Timers.h>
#include "Filter.h"
#include "MPU.h"
//include the .h files
#include "Structs.h"
#include "Methods.h"
#include "Variables.h"
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "sethGyro.h"
#include "Comms.h"
#include "Motor.h"
#include "Sensors.h"
#include "Macros_Actuator.h"
#include "Macros_Gyro.h"
#include "Macros_Encoders.h"
#include "Macros_Wii.h"
#include "Macros.h"
#include "Debug.h"




void setup() {
  pinMode(13,OUTPUT);
  initializeCommunications();
  delay(1000);
//  MPU1.MPUinit();
//  MPU2.MPUinit();
//  initMPUFilters();
  initGyroSeth();
 digitalWrite(13,HIGH);
 delay(100);
 digitalWrite(13,LOW);
 delay(100);
 digitalWrite(13,HIGH);
 delay(100);
 digitalWrite(13,LOW);
 delay(100);
 digitalWrite(13,HIGH);
 
}
Timers gyroTimer(10);
void loop() {
  
  static Timers LEDTimer(1000);
  updateComms();
 
  if(MPUTimer.timerDone()) {
    
  logData();
    updateMPU();
  }
  //USBdebug();
  //if(LEDTimer.timerDone()) digitalWrite(13,!digitalRead(13));
}



