//Comms.h

FastTransfer Navigation;
//ADDRESSING FOR ROBOT
#define CONTROL_ADDRESS              5
#define NAVIGATION_ADDRESS           4
#define PIC_ADDRESS                  1
#define MOTOR_ADDRESS                6
#define LED_ADDRESS                  2
#define POWER_ADDRESS	             3

//LED SEND
#define STATE 1
#define COLOR_PICKER 2
#define UPDATE_FLAG 3

#define RED 0b1111100000000000
#define BLUE 0b0000000000011111
#define GREEN 0b0000011111100000

#define MANUAL 3
#define MACRO 2
#define AUTO 2
//NAVIGATION SEND
#define MACRO_COMMAND_SEND           1
#define MACRO_SUB_COMMAND_SEND       2
#define MACRO_COMPLETE               3
#define GYRO                         4
#define LEFT_MOTOR                   5
#define RIGHT_MOTOR                  6


//SEND TO PIC
#define ROBOT_MOVING                 1
#define WII_SUBSYSTEM_MODE           2    //MODES INCLUDE: TRIG=0, LEFT_BEACON_ANGLES(VERT)=1, RIGHT_BEACON_ANGLES(HORIZ)=2, CAMERA_OVERRIDE_BEACON_ANGLES=3, CONTINUOUS_AQUISITION=4
#define WII_LEFT_CAMERA_MODE         3    //MODES 3 and 4 only used for WII_SUBSYSTEM_MODE 3   
#define WII_RIGHT_CAMERA_MODE        4            //0=LEFT_BEACON 1=RIGHT_BEACON

int motor_bucket_angle;
//Motor board SEND Definitions
#define COMMSPEED_MOTOR_SEND         0
#define LEFTMOTOR_MOTOR_SEND         1
#define RIGHTMOTOR_MOTOR_SEND        2
#define ACTUATOR_MOTOR_SEND          3




int navigation_receive[20];

//------------------------------------NAVIGATION RECEIVE------------------------------------------
//CONTROL RECEIVE
#define LAST_BOARD_ADDRESS_RECEIVE   0
#define MACRO_COMMAND_RECEIVE        1
#define MACRO_SUB_COMMAND_RECEIVE    2
#define MACRO_STOP                   3

//MOTOR Receive
#define ACTUATOR_ANGLE               4

//RECEIVE FROM PIC
#define ENCODER_R_PIC_RECEIVE        5
#define ENCODER_L_PIC_RECEIVE        6
#define ENCODER_SPEED_R_PIC_RECEIVE  7
#define ENCODER_SPEED_L_PIC_RECEIVE  8
#define WII_BEACON_SEEN_LEFT         9      //Boolean says whether camera is hunting or searching
#define WII_BEACON_SEEN_RIGHT        10
#define WII_NUMBER_SWEEPS_LEFT       11     //Will count up :)
#define WII_LEFT_CAMERA_ANGLE        12    //Returns at all beacon focused
#define WII_RIGHT_CAMERA_ANGLE       13    //^^
#define WII_LEFT_CAMERA_LOCKED       14    //Returns true when beacon is centered in sight of the camera
#define WII_RIGHT_CAMERA_LOCKED      15    //^^
#define WII_X_COORDINATE             16    //Returns during WII_SUBSYSTEM_MODE=TRIG 
#define WII_Y_COORDINATE             17    //^^
#define WII_FULL_BEACON_WIDTH_PIXELS 18    //Still working on this, thinking look for (horizontal) or (vertical) or (horizontal -> vertical)

#define WII_NUMBER_SWEEPS_RIGHT       22

#define LEFT_CAMERA 0
#define RIGHT_CAMERA 1
#define HORIZONTAL_SEND         2
#define RIGHT_BEACON_SEND       2
#define HORIZONTAL_BEACON       0
#define RIGHT_BEACON            0
#define VERTICAL_SEND           1
#define LEFT_BEACON_SEND        1
#define VERTICAL_BEACON         1
#define LEFT_BEACON             1
//Wii BEACON SENSOR
//int distanceFromCenter;
int beaconAngle[2];
int numberSweeps[2];
bool beaconSeen[2];
bool beaconCentered[2];
int beaconWidth;

//    -------------DEPRECIATED WII CAMERA STUFFFINGS-------------------
//    distanceFromCenter = wii_camera_receive[DISTANCE_FROM_CENTER];
//    beaconAngle = wii_camera_receive[BEACON_ANGLE];
//    numberSweeps = wii_camera_receive[NUMBER_SWEEPS];
//    beaconSeen = (bool)wii_camera_receive[BEACON_SEEN];
//    beaconCentered = (bool)wii_camera_receive[BEACON_CENTERED];
//    beaconWidth = wii_camera_receive[BEACON_WIDTH];

//STARTUP SYSTEMS
inline void initializeCommunications()
{
  // I2C Init for Gyroscope
  Wire.begin();
  // USB Serial
  Serial.begin(115200);
//...........................

  // Sensor PIC direct
  Serial1.begin(57600);
  // Router PIC
  Serial3.begin(57600);
  // Fast Tramsfer Communications
  Navigation.begin(Details(navigation_receive) , NAVIGATION_ADDRESS, false, &Serial3);
  // Send Actuator Stop Command
  sendActuatorCommand(255);
  
}

void prepManualData()
{
  Navigation.ToSend(MACRO_COMPLETE, 0);
  Navigation.ToSend(MACRO_COMMAND_SEND, 0);
  Navigation.ToSend(MACRO_SUB_COMMAND_SEND, 0);
}
void prepAutoData()
{
  Navigation.ToSend(LEFT_MOTOR    , lM);
  Navigation.ToSend(RIGHT_MOTOR   , rM);
  Navigation.ToSend(GYRO          , macroAngle);
}
//MAIN UPDATE FOR COMMUNICATIONS WHILE NOT IN A MACRO
inline void updateComms()
{
  updateFromControlBoard();
  //Data has been received from the Communication Board
  if (readyToSend)
  {
    prepManualData();
    prepAutoData();
    Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
    Navigation.sendData(CONTROL_ADDRESS);
    readyToSend = false;
  }
}

inline void macroCommunicationsUpdate()
{
  
//  static Timers LEDresend(100);
//  if(LEDresend.timerDone()){
//    sendLEDstate(MACRO);
//  }
  if (Navigation.receiveData())
  {
    if ((stored_macro_command != navigation_receive[MACRO_COMMAND_RECEIVE]) || (navigation_receive[MACRO_STOP] == 1))
    {
      stored_macro_command = 0;
      macro_sub_command = 0;
      macro_stop = 1;
      return;
    }
    
    pullDataFromPacket();
    prepAutoData();
    Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
    Navigation.ToSend(MACRO_COMPLETE, 0);
    Navigation.ToSend(MACRO_COMMAND_SEND, stored_macro_command);
    Navigation.sendData(CONTROL_ADDRESS);

    
  }
  else
  {
    commSafety();
  }
}



void updateFromControlBoard()
{
  //Data received from the Communications Board
  if (Navigation.receiveData())
  {
     pullDataFromPacket();
    //If sent a macro command -- do it
    if (stored_macro_command != 0)
    {
      initMacroSystem();
    }  
  }
  else //failed to get fresh packet
  {
    commSafety();
  }
}

void pullDataFromPacket() {
  static float keeper1, keeper2;
  switch ( navigation_receive[LAST_BOARD_ADDRESS_RECEIVE]) {
    case CONTROL_ADDRESS:
        //time stamp activity from communications board and okay a response to comm
      readyToSend = true;
      safetyTimer.resetTimer();

      
      //CONTROL RECEIVE
      stored_macro_command     = navigation_receive[MACRO_COMMAND_RECEIVE];
      macro_sub_command        = navigation_receive[MACRO_SUB_COMMAND_RECEIVE];
      macro_stop               = navigation_receive[MACRO_STOP];
       if (stored_macro_command == 0){
        sendLEDstate(MANUAL);
      }
      break;
     
    case MOTOR_ADDRESS:
      //----------------MOTOR BOARD RECEIVE DATA------------------
      motor_bucket_angle       = navigation_receive[ACTUATOR_ANGLE];
      break;  
    case PIC_ADDRESS:
      //----------------PIC ENCODER DATA---------------------------
      encoderR                += navigation_receive[ENCODER_R_PIC_RECEIVE]   / 100.0;    //IMPLIED CM*100 -> IMPLIED CM
      encoderL                += navigation_receive[ENCODER_L_PIC_RECEIVE]   / 100.0;
      keeper1                  = navigation_receive[ENCODER_SPEED_R_PIC_RECEIVE] / 100.0;//IMPLIED CM/SEC*100 -> IMPLIED CM/SEC
      keeper2                  = navigation_receive[ENCODER_SPEED_L_PIC_RECEIVE] / 100.0;
      encoderSpeedR            = ((encoderSpeedR * 3) + keeper1) / 4.0;
      encoderSpeedL            = ((encoderSpeedL * 3) + keeper2) / 4.0;
      
      //    Serial.print(encoderL);
      //    Serial.print(",");
      //    Serial.print(encoderR);
      //    Serial.print(",");
      //    Serial.print(encoderSpeedL);
      //    Serial.print(",");
      //    Serial.println(encoderSpeedR);

      //--------------WII DATA FROM PIC--------------------------------------
      beaconCentered[LEFT_CAMERA] = navigation_receive[WII_LEFT_CAMERA_LOCKED];
      beaconCentered[RIGHT_CAMERA] = navigation_receive[WII_RIGHT_CAMERA_LOCKED];

      //    if(beaconCentered[LEFT_CAMERA])
      //  {
      beaconAngle[LEFT_CAMERA]    = navigation_receive[WII_LEFT_CAMERA_ANGLE];
      //    navigation_receive[WII_LEFT_CAMERA_LOCKED]=0;
      //  }

      //    if(beaconCentered[RIGHT_CAMERA])
      //   {
      beaconAngle[RIGHT_CAMERA]   = navigation_receive[WII_RIGHT_CAMERA_ANGLE];
      //    navigation_receive[WII_RIGHT_CAMERA_LOCKED]=0;
      //   }
      numberSweeps[LEFT_CAMERA]                = navigation_receive[WII_NUMBER_SWEEPS_LEFT];
      numberSweeps[RIGHT_CAMERA]                = navigation_receive[WII_NUMBER_SWEEPS_RIGHT];
      beaconSeen[LEFT_CAMERA]     = navigation_receive[WII_BEACON_SEEN_LEFT];
      beaconSeen[RIGHT_CAMERA]    = navigation_receive[WII_BEACON_SEEN_RIGHT];
      break;
  }
}

//CHECK IF THE TIMER HAS RUN OUT BETWEEN COMMS UPDATES
inline void commSafety()
{
  if (safetyTimer.timerDone())
  {
    packetWait();
  }
}

//DELAY TIMEOUT OCCURRED METHOD
inline void packetWait()
{
  //sendMotorCommand(0, 0, 255);
  while (navigation_receive[LAST_BOARD_ADDRESS_RECEIVE]!=CONTROL_ADDRESS)
  {
    Navigation.receiveData();
    sendLEDstate(0);
    delay(25);
  }
  readyToSend = true;      //make not we got a good one
  latency.resetTimer();  //delay till send after not received
  safetyTimer.resetTimer(); //safety system reset
}

inline void terminateMacroSystem()
{
  stored_macro_command = 0;
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(MACRO_COMPLETE, 1);
  Navigation.ToSend(MACRO_COMMAND_SEND, stored_macro_command);
  Navigation.sendData(CONTROL_ADDRESS);
  delay(5);
  Navigation.receiveData();    //See if we hear back quickly
  while (navigation_receive[MACRO_COMMAND_RECEIVE] != 0)
  {
    static int sender = 0;
    Navigation.receiveData();

    sender++;
    if (sender >= 5)
    {
      Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
      Navigation.ToSend(MACRO_COMMAND_SEND, 0);
      Navigation.ToSend(MACRO_COMPLETE, 1);
      Navigation.sendData(CONTROL_ADDRESS);
      sender = 0;
    }
    delay(10);
  }
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(MACRO_COMPLETE, 0);
  Navigation.sendData(CONTROL_ADDRESS);
  sendActuatorCommand(255);
  lM = 0;
  rM = 0;
}

//MOTOR COMMAND HELPER COMMUNICATIONS METHODS
void sendMotorCommand(int leftMotor, int rightMotor)
{
  lM = leftMotor;
  rM = rightMotor;
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(LEFTMOTOR_MOTOR_SEND, leftMotor);
  Navigation.ToSend(RIGHTMOTOR_MOTOR_SEND, rightMotor);
  Navigation.sendData(MOTOR_ADDRESS);
}
void sendMotorCommand(int leftMotor, int rightMotor, int actuator)
{
  lM = leftMotor;
  rM = rightMotor;
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(LEFTMOTOR_MOTOR_SEND, leftMotor);
  Navigation.ToSend(RIGHTMOTOR_MOTOR_SEND, rightMotor);
  Navigation.ToSend(ACTUATOR_MOTOR_SEND, actuator);
  Navigation.sendData(MOTOR_ADDRESS);
}
void sendActuatorCommand(int actuator)
{
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(ACTUATOR_MOTOR_SEND, actuator);
  Navigation.sendData(MOTOR_ADDRESS);
}

void sendLEDCommand(int color) {
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(STATE, 6);
  Navigation.ToSend(COLOR_PICKER, color);
  Navigation.sendData(LED_ADDRESS);
}
void sendLEDstate(int state) {

  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(STATE, state);  
  Navigation.ToSend(COLOR_PICKER, 0);
  Navigation.sendData(LED_ADDRESS);
}



