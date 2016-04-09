#define acceptableGyroBeaconAngle           5
#define gyroCorrectBeaconCenterMinAngle     4
#define gyroCorrectBeaconCenterMaxAngle    10
#define ROBOT_ANGLE_CORRECT_WIDE           90
#define SERVO_MIN                          0
#define SERVO_MAX                         180
#define OFFSET_CAMERA_REGISTER              3
#define NUMBER_SWEEPS_UNSEEN                2

bool wiiCameraLocalize(int targetBeacon, int targetAngle, int cameraNumber)
{
  int sendBeacon;
  if(targetBeacon==HORIZONTAL_BEACON){
   sendBeacon=HORIZONTAL_SEND; 
  }
  else
  sendBeacon=VERTICAL_SEND;
    Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(WII_SUBSYSTEM_MODE, sendBeacon);
  Navigation.sendData(PIC_ADDRESS);
  bool doneLocalizing = false;
  int sweepCount = numberSweeps[cameraNumber];
  static Timers decisionTimer(300),sendTimerWii(150);
  while (!doneLocalizing && (stored_macro_command != 0))
  {
    sweepCount = numberSweeps[cameraNumber];

    //while the wii camera doesnt see it..
    while ((stored_macro_command != 0) && (!beaconSeen[cameraNumber]))
    {

      if (sendTimerWii.timerDone()) {
        
    Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
        Navigation.ToSend(WII_SUBSYSTEM_MODE, sendBeacon);
        Navigation.sendData(PIC_ADDRESS);
        sendLEDCommand(RED);
      }

      //if we have swept a number of times
      if ((numberSweeps[cameraNumber] > (sweepCount + NUMBER_SWEEPS_UNSEEN)))
      {
        static int turnCount;
        //Reposition for another look
        doTurn(ROBOT_ANGLE_CORRECT_WIDE);
        turnCount++;

        beaconSeen[cameraNumber] = 0;

        //if you have turned too much give up on this beacon...
        if (turnCount > (450 / ROBOT_ANGLE_CORRECT_WIDE))
          return false;
        sweepCount = numberSweeps[cameraNumber];
      }
      //Listen for break and for wii
      macroCommunicationsUpdate();
    }

    //    //Allow to center -- do small adjustments if camera is on edge of capable angle
    //    while (((macro_stop != 1) && (beaconSeen[cameraNumber])) && (!beaconCentered))
    //    {
    //      if (decisionTimer.timerDone())
    //      {
    //        if (beaconAngle[cameraNumber] < SERVO_MIN + 10) {
    //          doTurn(5);
    //        }
    //        else if ((beaconAngle[cameraNumber] > SERVO_MAX - 10)) {
    //          doTurn(-5);
    //        }
    //        decisionTimer.resetTimer();
    //      }
    //      //Listen for break and for wii
    //      macroCommunicationsUpdate();
    //    }


    //Then make angle = what you want;
    while ( (!(((beaconAngle[cameraNumber] - acceptableGyroBeaconAngle) < targetAngle) && ((beaconAngle[cameraNumber] + acceptableGyroBeaconAngle) > targetAngle))) && ((stored_macro_command != 0) && (beaconSeen[cameraNumber]) && (beaconCentered[cameraNumber])) && !doneLocalizing)
    {

      if (decisionTimer.timerDone())
      {
      if (sendTimerWii.timerDone()) 
        sendLEDCommand(BLUE);
        if (beaconAngle[cameraNumber] < targetAngle)
        {
          doTurn(constrain((targetAngle - beaconAngle[cameraNumber]) / 2, gyroCorrectBeaconCenterMinAngle, gyroCorrectBeaconCenterMaxAngle));
        }
        else
        {
          doTurn(constrain((targetAngle - beaconAngle[cameraNumber]) / 2 , -gyroCorrectBeaconCenterMaxAngle, -gyroCorrectBeaconCenterMinAngle));
        }
        beaconSeen[cameraNumber] = 0;
        beaconCentered[cameraNumber] = 0;
        decisionTimer.resetTimer();
      }
      //Listen for break and for wii
      macroCommunicationsUpdate();

      if (( ( (beaconAngle[cameraNumber] - acceptableGyroBeaconAngle) < targetAngle) && ((beaconAngle[cameraNumber] + acceptableGyroBeaconAngle) > targetAngle)))
      {
        doneLocalizing = true;
        return stored_macro_command != 0;
      }
    }

    macroCommunicationsUpdate();
    if (( ( (beaconAngle[cameraNumber] - acceptableGyroBeaconAngle) < targetAngle) && ((beaconAngle[cameraNumber] + acceptableGyroBeaconAngle) > targetAngle)))
    {
      doneLocalizing = true;
      return stored_macro_command != 0;
    }
  }

}

bool wiiCameraFindTwoCamera(int targetBeacon)
{
  
  int sendBeacon;
  if(targetBeacon==HORIZONTAL_BEACON){
   sendBeacon=HORIZONTAL_SEND; 
  }
  else
  sendBeacon=VERTICAL_SEND;
  
    Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(WII_SUBSYSTEM_MODE, sendBeacon);
  Navigation.sendData(PIC_ADDRESS);
  bool doneLocalizing = false;
  int sweepCountLeft = numberSweeps[LEFT_CAMERA], sweepCountRight = numberSweeps[RIGHT_CAMERA];
  static Timers decisionTimer(300), sendTimerWii(100);
  while (!doneLocalizing && (stored_macro_command != 0))
  {
    sweepCountLeft = numberSweeps[LEFT_CAMERA];
    sweepCountRight = numberSweeps[RIGHT_CAMERA];

    //while the wii camera doesnt see it..
    while ((stored_macro_command != 0) && (!beaconSeen[RIGHT_CAMERA] || !beaconSeen[LEFT_CAMERA]))
    {
      if (sendTimerWii.timerDone()) {
        
        Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
        Navigation.ToSend(WII_SUBSYSTEM_MODE, sendBeacon);
        Navigation.sendData(PIC_ADDRESS);
        sendLEDCommand(RED);
      }

      //if we have swept a number of times
      if ((numberSweeps[LEFT_CAMERA] > (sweepCountLeft + NUMBER_SWEEPS_UNSEEN)) || (numberSweeps[RIGHT_CAMERA] > (sweepCountRight + NUMBER_SWEEPS_UNSEEN)))
      {
        static int turnCount;
        //Reposition for another look
        doTurn(ROBOT_ANGLE_CORRECT_WIDE);
        turnCount++;

        //if you have turned too much give up on this beacon...
        if (turnCount > (450 / ROBOT_ANGLE_CORRECT_WIDE))
          return false;
        sweepCountLeft = numberSweeps[LEFT_CAMERA];
        sweepCountRight = numberSweeps[RIGHT_CAMERA];
      }
      //Listen for break and for wii
      macroCommunicationsUpdate();
    }
    return true;
    macroCommunicationsUpdate();

  }

}


