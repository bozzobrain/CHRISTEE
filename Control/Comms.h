//comms.h
FastTransfer robot;
int roboMessage[50];

FastTransfer Bluetooth;
int receiveArray[4];

//COMM ADDRESSES
#define CONTROL_ADDRESS            5
#define NAVIGATION_ADDRESS         4
#define PIC_ADDRESS                1
#define MOTOR_ADDRESS              6
#define LED_ADDRESS                2
#define POWER_ADDRESS	           3


//CONTROL RECEIVE STUFF
#define LAST_BOARD_ADDRESS_RECEIVE   0
//NAVIGATION
#define MACRO_COMMAND_RECEIVE        1
#define MACRO_SUB_COMMAND_RECEIVE    2
#define MACRO_COMPLETE               3
#define GYRO_ANGLE                   4
#define MOTORL                       5
#define MOTORR                       6
//POWER
#define VOLTS                        7
#define MILLIAMPS                    8
#define MILLIAMPHOUR                 9
#define WATTHOURS_POWER_RECEIVE      10
//PIC (MAYBE NAV)
#define ENCODERSPEEDL                11
#define ENCODERSPEEDR                12
#define ENCODER_R_H_PIC_RECEIVE                 13
#define ENCODER_R_L_PIC_RECEIVE                 15
#define ENCODER_L_H_PIC_RECEIVE                 14
#define ENCODER_L_L_PIC_RECEIVE                 16
//MOTOR
#define BUCKET_ANGLE                 15
//Wii Camera Stuff
#define WII_CAMERA_LEFT_ANGLE        16
#define WII_CAMERA_RIGHT_ANGLE       17
#define WII_CAMERA_LEFT_LOCKED       18
#define WII_CAMERA_RIGHT_LOCKED      19
#define WII_CAMERA_LEFT_BEACON       20 
#define WII_CAMERA_RIGHT_BEACON      21


//CONTROL SEND - ROBOT

//MOTOR
#define LEFTMOTORSPEED              1
#define RIGHTMOTORSPEED             2
#define ACTUATORSPEED               3
//NAVIGATION
#define MACRO_COMMAND_SEND          1
#define MACRO_SUB_COMMAND           2
#define MACRO_STOP                  3
//LED

#define LED_STATE_OVERRIDE          1
#define COLOR_PICKER                2
#define LED_UPDATE_FLAG             3
#define BOX_LOCATION                    4

//PIC
#define ROBOT_MOVING                 1
#define WII_SUBSYSTEM_MODE           2    //MODES INCLUDE: TRIG=0, LEFT_BEACON_ANGLES=1, RIGHT_BEACON_ANGLES=2, CAMERA_OVERRIDE_BEACON_ANGLES=3, CONTINUOUS_AQUISITION=4
#define WII_LEFT_CAMERA_MODE         3    //MODES 3 and 4 only used for WII_SUBSYSTEM_MODE 3   
#define WII_RIGHT_CAMERA_MODE        4            //0=LEFT_BEACON 1=RIGHT_BEACON
#define SERVO_OVERRIDE_LEFT          5
#define SERVO_OVERRIDE_RIGHT         6
//WIIMODES
#define WIIMODE_VERTICAL_BEACON      1
#define WIIMODE_HORIZONTAL_BEACON    2
#define WIIMODE_CONTINUOUS           4
#define WIIMODE_SERVO_OVERRIDE       5

void initializeCommunicaton()
{
  //screen init
  Serial3.begin(256000);
  //robot init
  Serial1.begin(115200);
  //bluetooth
  // Serial2.begin(115200);
  //Debug
  //Serial.begin(9600);

  robot.begin(Details(roboMessage), CONTROL_ADDRESS, false, &Serial1);
}

union jointhem{
    int32_t joined;
    struct {      
      uint16_t low;
      uint16_t high;
    }endian;
}_16_to_32;

inline void updateCommunication()
{
  static Timers minimumResponseTimer(15);
  //So if you have heard from the comm after sending -or- if the resend timer timed out and you havent already sent before a safety timeout condition
  if ((heardBack && minimumResponseTimer.timerDone()) || (resendTimer.timerDone() && notSent ))
  {
    sendDataNavigation(internalMacroKeeper);
    if (internalMacroKeeper == 0)
      sendDataMotor(leftMotorSpeed, rightMotorSpeed, actuatorSpeed);
    
    //make a mental note that we have sent
    if (heardBack)  heardBack = false;
    else notSent=false;
    minimumResponseTimer.resetTimer();     
    resendTimer.resetTimer();
    
     
  }
  //control box is always listening to the robot
  //in order to keep track of macro's and keep
  //screen up to date.
  if (robot.receiveData())
  {
    switch (roboMessage[LAST_BOARD_ADDRESS_RECEIVE]) {
      case NAVIGATION_ADDRESS:
        //NAVIGATION_RECEIVE
         digitalWrite(13, !digitalRead(13));
        
       //When you hear from the navigation, reset comms timers
        heardBack = true;
        notSent = true;
        resendTimer.resetTimer();
        minimumResponseTimer.resetTimer();
        commTimer.resetTimer();
        
        
        //if good checksum, store data locally.
        commMacro             = roboMessage[MACRO_COMMAND_RECEIVE];
    
        //Gyro Angle
        gyroAngle             = roboMessage[GYRO_ANGLE];
        //Robot Motor States
        motorL                = roboMessage[MOTORL];
        motorR                = roboMessage[MOTORR];
        break;
      case POWER_ADDRESS:
        //Power systems
        volts                 = roboMessage[VOLTS];
        milliamps             = roboMessage[MILLIAMPS];
        milliamphours         = roboMessage[MILLIAMPHOUR];
        break;
      case MOTOR_ADDRESS:
        bucketAngle           = roboMessage[BUCKET_ANGLE];
        break;
      case PIC_ADDRESS:
        //Encoder Details
        encoderSpeedL    = roboMessage[ENCODERSPEEDL];
        encoderSpeedR    = roboMessage[ENCODERSPEEDR];
        
        //DISTANCE PULSES
      _16_to_32.endian.high=roboMessage[ENCODER_R_H_PIC_RECEIVE];
      _16_to_32.endian.low =roboMessage[ENCODER_R_L_PIC_RECEIVE];
      encoderR                = ( _16_to_32.joined  ) ;    //IMPLIED CM*100 -> IMPLIED CM
      
      _16_to_32.endian.high=roboMessage[ENCODER_L_H_PIC_RECEIVE];
      _16_to_32.endian.low=roboMessage[ENCODER_L_L_PIC_RECEIVE];
      encoderL                =( _16_to_32.joined ) ;
      
        
        //encoderR         = roboMessage[ENCODERRIGHT];
        //encoderL         = roboMessage[ENCODERLEFT];
        leftAngle        = roboMessage[WII_CAMERA_LEFT_ANGLE];
        rightAngle       = roboMessage[WII_CAMERA_RIGHT_ANGLE];
        leftLocked       = roboMessage[WII_CAMERA_LEFT_LOCKED];
        rightLocked      = roboMessage[WII_CAMERA_RIGHT_LOCKED];
        cameraBeacon[LEFT_CAMERA]     = roboMessage[WII_CAMERA_RIGHT_BEACON];
        cameraBeacon[RIGHT_CAMERA]    = roboMessage[WII_CAMERA_LEFT_BEACON];
        break;
    }
    roboMessage[LAST_BOARD_ADDRESS_RECEIVE] = 0; 
  }

  if (internalMacroKeeper == 0)
  {
    commSafety();
  }
}



inline void commSafety()
{
  commTimer.updateTimer();
  if (commTimer.timerDone())
  {
    packetWait();
    //Safety Timer Reset
    commTimer.resetTimer();
    //Must have heard back because we got out of safety
    heardBack = true;
    //Definitely havent sent a timeout resend
    notSent = true;
    //Reset timer to avoid the accidental send
    resendTimer.resetTimer();
  }
}

inline void killMacro()
{
  sendDataNavigation(0, 0);
  internalMacroKeeper = 0;
  internalSubKeeper = 0;
  commTimer.resetTimer();
}

inline void packetWait()
{
  //Wait timer (50 ms)
  static Timers sendTimerWait(50);
  //
  leftMotorSpeed = 0;
  rightMotorSpeed = 0;
  actuatorSpeed = 255;
  float time = millis();
  float lastTime = time;
  pageKeeper = activePage;  //record which page is active (to return to after macro)
  CTS = true;
  //writeObject(FORMMESSAGE, CommError, 1);    //change the page to the active macro page
  sendTimerWait.resetTimer();
  totalTime = 0;
  while (!robot.receiveData())
  {
    
    uptime = 0;
    updateScreen();
    time = millis();
    if ((abs(time - lastTime)) > 1000) {
      totalTime++;
      lastTime = time;
    }
    if (sendTimerWait.timerDone())
    {
      digitalWrite(13, !digitalRead(13));
      sendDataNavigation(internalMacroKeeper);
      sendDataMotor(0, 0, 255);
    }
    if (digitalReadFast(MACRO_BUTTON) == HIGH)
    {
    
      killMacro();
    }
    delayMicroseconds(50);
    CTS = true;
   // writeObject(LEDDIGITS, 1, totalTime);
  }
  
  delay(1);
  //sendDataNavigation(internalMacroKeeper);
  //delay(5);
  //sendDataMotor(leftMotorSpeed, rightMotorSpeed, actuatorSpeed);
  commTimer.resetTimer();
  CTS = true;
  //writeObject(FORMMESSAGE, pageKeeper, 1);    //change the page
  CTS = true;
  uptime = 0;
}


void sendDataNavigation(int _macro_command, int _macro_sub_command)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(MACRO_SUB_COMMAND, _macro_sub_command);
  robot.ToSend(MACRO_COMMAND_SEND, _macro_command);
  robot.sendData(NAVIGATION_ADDRESS);
}

void sendDataNavigation(int _macro_command)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(MACRO_COMMAND_SEND, _macro_command);
  robot.sendData(NAVIGATION_ADDRESS);
}

void sendDataMotor(int leftMotor, int rightMotor)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(LEFTMOTORSPEED  , leftMotor);
  robot.ToSend(RIGHTMOTORSPEED , rightMotor);
  robot.sendData(MOTOR_ADDRESS);
}

void sendDataMotor(int leftMotor, int rightMotor, int actuator)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(LEFTMOTORSPEED  , leftMotor);
  robot.ToSend(RIGHTMOTORSPEED , rightMotor);
  robot.ToSend(ACTUATORSPEED   , actuator);
  robot.sendData(MOTOR_ADDRESS);
}

void sendDataMotor(int actuator)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(ACTUATORSPEED   , actuator);
  robot.sendData(MOTOR_ADDRESS);
}

void sendLEDdata(int stateOverride, int colorPicker)
{
  robot.ToSend(LAST_BOARD_ADDRESS_RECEIVE, CONTROL_ADDRESS);
  robot.ToSend(LED_STATE_OVERRIDE, stateOverride);
  robot.ToSend(COLOR_PICKER, colorPicker);
  robot.ToSend(LED_UPDATE_FLAG, 1);
  robot.sendData(LED_ADDRESS);
}









