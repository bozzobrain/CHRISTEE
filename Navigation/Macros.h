//macros.h
//3-2-14 STACEE can walk in circles (   almost :)  )
//3-25-14 STACEE can do forward/mine/backward/dump

//Macro_commands
#define MACRO_SNIPPIT 4
#define ENCODER_SNIPPIT 6
#define ACTUATOR_SNIPPIT 7

        bool flipflop=true;
        
#define TRAVERSE_FORWARD      0
#define DROPPING_BUCKET       1
#define DIGGING_FORWARD       2
#define DIGGING_REVERSE       3
#define TRAVERSE_BACKWARD     4
#define DUMPING_BUCKET        5
#define BUCKET_TO_DRIVE	      6

#define DRIVE_INCREMENT       500
#define DRIVE_INC_NUM_FORWARD 1  //600
#define DRIVE_INC_NUM_REVERSE 1  //550
#define DRIVE_DIG_FORWARD     50
#define DRIVE_DIG_REVERSE     75
#define ACTUATOR_DUMP_ANGLE   91
#define GYRO_CORRECTION_ANGLE 4

int AUTO_STATE=6;

inline void initMacroSystem()
{
  continuable = true;
  sendLEDstate(AUTO);  
  Navigation.ToSend(LAST_BOARD_ADDRESS_RECEIVE, NAVIGATION_ADDRESS);
  Navigation.ToSend(MACRO_COMMAND_SEND, stored_macro_command);
  Navigation.sendData(CONTROL_ADDRESS);
  //
  //  Navigation.ToSend(MACRO_COMMAND_SEND, stored_macro_command);
  //  Navigation.sendData(CONTROL_ADDRESS);
  //  Navigation.ToSend(MACRO_COMMAND_SEND, stored_macro_command);
  //  Navigation.sendData(CONTROL_ADDRESS);

  if (stored_macro_command == 1)
  {
    while (stored_macro_command != 0)
    {
      straightPathMineDump();
    }
  }
  else
  {
    //    Serial.print("Heard Macro: ");
    //    Serial.println(macro_command);
    //    Serial.print("Macro_sub_command: ");
    //    Serial.println(macro_sub_command);
    switch (stored_macro_command)
    {
      case 2:
        runEncoderDistanceEvenly(1000);
        break;
      case 3:
        runEncoderDistanceEvenly(-1000);
        break;
      case MACRO_SNIPPIT:
        switch (macro_sub_command)
        {
          case 1:
              wipeGyro();   
                flipflop=true;
            while(stored_macro_command!=0)
            {
              static Timers redoTimer(2500),mildDelayTimer(250);
              static int counter=0;
              if(redoTimer.timerDone()||((counter>0)&&mildDelayTimer.timerDone()))
              {
                //FLIP FLOP SAYS DO FORWARD THEN BACKWARD
                if(flipflop)
                {
                  //GOING TO DO 5 ITERATIONS OF THIS PART (250 cm) 
                  if(counter<5)
                  {
                    //Go forward 50 cm
                    newEncoders((signed long)50);
                    //CHECK GYRO FOR SHIFTED ANGLE
                    if(abs(macroAngle>4)){
                      //IF CORRECTION IS REQUIRED DO IT
                      doTurn(-macroAngle);
                    }
                    counter++;
                  }
                  else
                  {
                    flipflop=false;
                    counter=0;
                  }
                }
                else
                {        
                  //GOING TO DO 5 ITERATIONS OF THIS PART (250 cm)    
                  if(counter<5)
                  {
                    //Go backward 50 cm
                    newEncoders((signed long)-50);
                    //CHECK GYRO FOR SHIFTED ANGLE
                    if(abs(macroAngle>4)){
                      //IF CORRECTION IS REQUIRED DO IT
                      doTurn(-macroAngle);
                    }
                    counter++;
                  }
                  else
                  {
                    flipflop=true;
                    counter=0;
                  }
                }
                redoTimer.resetTimer();
              }
              else
              {
                macroCommunicationsUpdate();
              }
            }
            break;
          case 2:
              wipeGyro();   
                 flipflop=true;
            while(stored_macro_command!=0)
            {
              static Timers redoTimer(2500), mildDelayTimer(250);
              static int counter=0;
              if(redoTimer.timerDone()||((counter>0)&&mildDelayTimer.timerDone()))
              {
                //FLIP FLOP SAYS DO FORWARD THEN BACKWARD
                if(flipflop)
                {
                  //GOING TO DO 5 ITERATIONS OF THIS PART (250 cm) 
                  if(counter<6)
                  {
                    //Go forward 50 cm
                    newEncoders((signed long)50);
                    //CHECK GYRO FOR SHIFTED ANGLE
                    if(abs(macroAngle)>4){
                      //IF CORRECTION IS REQUIRED DO IT
                      doTurn(-macroAngle);
                    }
                    counter++;
                    mildDelayTimer.resetTimer();
                  }
                  else
                  {
                    sendActuatorPositionDig(0);
                    delay(150);
                    newEncoders((signed long) 50);
                    delay(150);       
                 
                    newEncoders((signed long) -50);
                    delay(150);                    
                    sendActuatorPositionFeedback(BUCKET_DRIVE_ANGLE_SET);
                    flipflop=false;
                    counter=0;
                  }
                }
                else
                {        
                  //GOING TO DO 5 ITERATIONS OF THIS PART (250 cm)    
                  if(counter<6)
                  {
                    //Go backward 50 cm
                    newEncoders((signed long)-50);
                    //CHECK GYRO FOR SHIFTED ANGLE
                    if(abs(macroAngle)>4){
                      //IF CORRECTION IS REQUIRED DO IT
                      doTurn(-macroAngle);
                    }
                    counter++;
                    mildDelayTimer.resetTimer();
                  }
                  else
                  {
                    sendActuatorPositionFeedback(90);
                    delay(1500);                    
                    sendActuatorPositionFeedback(BUCKET_DRIVE_ANGLE_SET);
                    delay(150);                    
                    flipflop=true;
                    counter=0;
                  }
                }
                redoTimer.resetTimer();
              }
              else
              {
                macroCommunicationsUpdate();
              }
            }
            break;
          case 3:          
              wipeGyro();   
              AUTO_STATE=6;
              sendActuatorPositionFeedback(BUCKET_DRIVE_ANGLE_SET);
              while(stored_macro_command!=0)
              {	
              	static Timers subDelayTimer(75), dumpTimer(6000);
                static int counter=0;
              	switch(AUTO_STATE)
              	{	
              		case TRAVERSE_FORWARD:
              			counter=0;
              			while((counter<DRIVE_INC_NUM_FORWARD)  && (stored_macro_command!=0))
              			{
              				newEncoders((signed long) 500);
              				//RESET TIMER
              				subDelayTimer.resetTimer();
              				//WAIT UNTIL ITS FINISHED WHILE CHECKING COMMS
              				while(!subDelayTimer.timerDone() && (stored_macro_command!=0)) 
              				{
              					macroCommunicationsUpdate();
              					delay(2);
              				}
              			    
              				//CHECK GYRO FOR SHIFTED ANGLE
//              				if(abs(macroAngle)>GYRO_CORRECTION_ANGLE){
//              				  //IF CORRECTION IS REQUIRED DO IT
//              				  
//              				  doTurn(-grabIntegerSign(macroAngle)*(abs(macroAngle+1)));
//              				}
              				counter++;
              			}
              			AUTO_STATE++;
              			break;
              		case DROPPING_BUCKET:
              			//Lower bucket, moving forward when bucket gets low
              			sendActuatorPositionDig(0);			
              			AUTO_STATE++;
              			break;
              		case DIGGING_FORWARD:
              			//Drive forward with bucket in the dirt
              			newEncoders((signed long) DRIVE_DIG_FORWARD);			
              			AUTO_STATE++;
              			break;
              		case DIGGING_REVERSE:
              			//Free bucket by backing up
              			newEncoders((signed long) -DRIVE_DIG_REVERSE);	
              			//Lift the bucket
              			sendActuatorPositionFeedback(BUCKET_DRIVE_ANGLE_SET);		
              			AUTO_STATE++;
              			break;
              		case TRAVERSE_BACKWARD:
              			counter=0;
              			while((counter<DRIVE_INC_NUM_REVERSE) && (stored_macro_command!=0))
              			{
              				newEncoders((signed long) -475);
              				//RESET TIMER
              				subDelayTimer.resetTimer();
              				//WAIT UNTIL ITS FINISHED WHILE CHECKING COMMS
              				while(!subDelayTimer.timerDone()&&(stored_macro_command!=0)) 
              				{
              					macroCommunicationsUpdate();
              					delay(2);
              				}
              			    
              				
              				counter++;
              			}
              			AUTO_STATE++;
              			break;
              		case DUMPING_BUCKET:
              			
                          sendActuatorPositionFeedback(ACTUATOR_DUMP_ANGLE);
              			dumpTimer.resetTimer();
              			while(!dumpTimer.timerDone() && (stored_macro_command!=0))
              			{
              				macroCommunicationsUpdate();
              				delay(5);			
              			}
              			AUTO_STATE++;
              			break;
              		case BUCKET_TO_DRIVE:
              			sendActuatorPositionFeedback(BUCKET_DRIVE_ANGLE_SET);			
              			AUTO_STATE=TRAVERSE_FORWARD;
              			break;
              	
              	}
              	
              	macroCommunicationsUpdate();
              }
            break;
          case 4:
            orientWithWii();
            break;
          case 5:
            wiiCameraLocalize(HORIZONTAL_BEACON, 67 , LEFT_CAMERA);
            break;
          case 6:
            fullRoutine();
            break;
          case 7:
            fullDigRoutine();
            break;
          case 8:
            bucketMovementDig();
            break;
          case 9:
            bucketMovementDump();
            break;
          case 10:
            fiftyForwardFiftyBackward();
            break;
          case 11:
            squareRoutine();
            break;
          case 12:
            orientWithWii();
            break;
            //CW
          case 13:
            doTurn(90);
            break;
          case 14:
            doTurn(-90);
            break;
          case 15:
            runEncoderDistanceEvenly(1000);
            break;
        }
        break;
      case 5:
        wipeEncoders();
        initMPUFilters();
        break;
      case ENCODER_SNIPPIT:
        flipflop=true;
        while(stored_macro_command!=0)
        {
          static Timers redoTimer(2500);
          
          if(redoTimer.timerDone())
          {
            if(flipflop)
            {
            newEncoders((signed long)macro_sub_command);
            flipflop=false;
            }
            else
            {            
            newEncoders((signed long)-macro_sub_command);
            flipflop=true;
            }
            redoTimer.resetTimer();
          }
          else
          {
            macroCommunicationsUpdate();
          }
        }
        break;
      case 7:
        if (macro_sub_command >= 0 && macro_sub_command <= 90)
          sendActuatorPosition(macro_sub_command);
        else if (macro_sub_command == 254)
          sendActuatorPosition(75);
        else if (macro_sub_command == 253)
          sendActuatorPosition(5);
        break;
      case 8:
         if(macro_sub_command<180)
          wiiCameraLocalize(HORIZONTAL_BEACON, macro_sub_command, LEFT_CAMERA);
         else
          wiiCameraLocalize(HORIZONTAL_BEACON, macro_sub_command-180, RIGHT_CAMERA);
        break;
      case 9:
        
         if(macro_sub_command<180)
          wiiCameraLocalize(VERTICAL_BEACON, macro_sub_command, LEFT_CAMERA);
          else
          wiiCameraLocalize(VERTICAL_BEACON, macro_sub_command-180, RIGHT_CAMERA);
        break;
        
    }
  }
  terminateMacroSystem();
  sendLEDstate(MANUAL);
}

inline bool straightPathMineDump()
{

}

inline bool orientWithWii()
{
  //Line up with target
  if (!wiiCameraFindTwoCamera(VERTICAL_BEACON) ) {
    if (wiiCameraLocalize(HORIZONTAL_BEACON, 67, LEFT_CAMERA)) {
      //This means only the horizontal is visible
      
            runEncoderDistanceEvenly(-80);
            doTurn(90);
    }
  }
  else {
    //This means you can see vertical beacon... may also the horizontal
    wiiCameraLocalize(VERTICAL_BEACON,112,RIGHT_CAMERA);
    
            runEncoderDistanceEvenly(-80);
            doTurn(-90);
  }



  return stored_macro_command != 0;
}

void fullRoutine()
{
  while (stored_macro_command != 0)
  {
    orientWithWii();
    //Move bucket and drive
    sendActuatorPosition(BUCKET_DRIVE_ANGLE_SET);
    runEncoderDistanceEvenly(TRAVERSE_DRIVE_DISTANCE);
    //Move bucket dig
    sendActuatorPositionDig(BUCKET_DIG_ANGLE_SET);
    //Scoop
    runEncoderDistanceEvenly(DIG_DRIVE_DISTANCE);
    //Toss back
    sendActuatorPosition(BUCKET_DRIVE_ANGLE_SET + 20);
    //Drive Postition
    sendActuatorPosition(BUCKET_DRIVE_ANGLE_SET);
    runEncoderDistanceEvenly(-TRAVERSE_DRIVE_DISTANCE);
    //Dump
    sendActuatorPosition(BUCKET_DUMP_ANGLE_SET);
    delay(DELAY_BUCKET_EMPTY);
    //Back to drive
    sendActuatorPosition(BUCKET_DRIVE_ANGLE_SET);
  }
}


void driveDigDistance()
{
  runEncoderDistanceEvenly(DIG_DRIVE_DISTANCE);
}



void fiftyForwardFiftyBackward()
{
  while (stored_macro_command != 0)
  {
    runEncoderDistanceEvenly(500);
    runEncoderDistanceEvenly(-500);
  }
}

void squareRoutine()
{
  while (stored_macro_command != 0)
  {
    runEncoderDistanceEvenly(500);
    doTurn(90);
  }
}

//***********LOW LEVEL ROBOT METHODS (USED INTERNALLY MOSTLY)***********
//Robot motors ALL Stop
inline void allStop()
{
  sendMotorCommand(255, 255, 255);
}
//When motor board recieves 255's it appears to lock up,
// this method assures they are unstuck (as one may assume :) )
inline void motor_unStick()
{
  sendMotorCommand(0, 0, 255);
}

