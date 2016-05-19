//motor.ino
//created by Igor Vinograd

//Motor control board
#include <FastTransfer.h>
//#include <EasyTransferCRC.h>
#include <Servo.h>
#include <Timers.h>
#include "Variables.h"

#include "Methods.h"

#include "Structs.h"

//Communcications update method/fast transfer calls
#include "Comms.h"

//Motor controller output
#include "Servos.h"

//input systems (actuator/ )
#include "Pins.h"


void setup(){
  initializeLed();
  initializeCommunication();
  initializeServos();
  //initTappers(); 
}

void loop(){
  updateComms();
  updateServos();
  updateLed();
  updateOffset();
  updateAngle();
  //toggleTappers(); 
}

void initTappers() 
{
 pinMode(A4, OUTPUT);  
 digitalWrite(A4, LOW);
 delay(500); 
 digitalWrite(A4, HIGH); 
 delay(500); 
 digitalWrite(A4, LOW); 
}

void toggleTappers () 
{
  
  static Timers timerVibrate(200);
   //if((bucketAngle >= VIBRATING_MOTOR_ON_HIGH)||(bucketAngle<=VIBRATING_MOTOR_ON_LOW)){
     if(timerVibrate.timerDone())
       digitalWrite(A4,!digitalRead(A4));
   //else{
     //digitalWrite(A4,HIGH); 
   //}
 //static int pastTime = 0; 
 //static int currentTime = 0;
 //static boolean state = false; 
 //currentTime = millis(); 
 //if(currentTime > (pastTime + 250))
 //{
  //pastTime = currentTime;
  //digitalWrite(A4, state); 
  //state = !state;   
 //}
}

