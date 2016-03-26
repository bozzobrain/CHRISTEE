//comms.h
//ADDRESSING FOR ROBOT
#define CONTROL_ADDRESS            5
#define NAVIGATION_ADDRESS         4
#define PIC_ADDRESS                1
#define MOTOR_ADDRESS              6
#define LED_ADDRESS                2
#define POWER_ADDRESS	           3

#define LED_STATE 1
//Communications data details array
int motor[7];
#define LAST_BOARD_ADDRESS_RECEIVE   0
#define LEFTMOTOR       1
#define RIGHTMOTOR      2
#define ACTUATOR        3
#define ACTUATOR_ANGLE_NAVIGATION 4
#define ACTUATOR_ANGLE_CONTROL    15

#define vibrate A4

Timers safetyTimer(1500);
void initializeCommunication()
{  
  Serial.begin(115200);
  
  communicationBoardIn.begin(Details(motor), MOTOR_ADDRESS, false, &Serial);  
}


inline void updateComms()
{
  static Timers actuatorSendTimer(250);
  if(actuatorSendTimer.timerDone())
  {
    communicationBoardIn.ToSend(LAST_BOARD_ADDRESS_RECEIVE, MOTOR_ADDRESS);
     communicationBoardIn.ToSend(ACTUATOR_ANGLE_NAVIGATION,  bucketAngle);
     communicationBoardIn.sendData(NAVIGATION_ADDRESS);
   //  communicationBoardIn.ToSend(LAST_BOARD_ADDRESS_RECEIVE, MOTOR_ADDRESS);
   //  communicationBoardIn.ToSend(ACTUATOR_ANGLE_CONTROL,  bucketAngle);
   //  communicationBoardIn.sendData(CONTROL_ADDRESS);
  }
  
  
  if(communicationBoardIn.receiveData())
  {    
    leftMotorSpeed = motor[LEFTMOTOR];
    rightMotorSpeed = motor[RIGHTMOTOR];    
    actuatorValue=(uint8_t)motor[ACTUATOR]; 
      
    safetyTimer.resetTimer();    
  }
  else
  {
    if (safetyTimer.timerDone())
  {    
//    communicationBoardIn.ToSend(LAST_BOARD_ADDRESS_RECEIVE, MOTOR_ADDRESS);
//    communicationBoardIn.ToSend(LED_STATE,0);
//    communicationBoardIn.sendData(LED_ADDRESS);
    leftMotorSpeed = 255;
    rightMotorSpeed = 255;
    actuatorSpeed = 255;
  }
  }
}


