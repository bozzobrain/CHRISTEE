//macros_encoders.h

#define encoderKp 1
#define encoderKi 0
#define encoderKd 0


void updateMacroEncoderValueR(signed long increment)
{
  macroEncoderR+=increment;
}

void updateMacroEncoderValueL(signed long increment)
{
  macroEncoderL+=increment;
}

void encoderRun1()
{ 
  while(macro_stop!=1)
  {
    runEncoderDistanceEvenly(50);
    doTurn(90); 
  }
}

void encoderRun2()
{
  wipeEncoders();
  while (((macroEncoderL < 40) || (macroEncoderR < 40)) && (macro_stop != 1))
  {
    static Timers motorSend(50);
    if(MPUTimer.timerDone()) updateMPU();
    if (motorSend.timerDone())
    {
      sendMotorCommand(15,-15);//(leftMotor,rightMotor)
    }
    macroCommunicationsUpdate();
  }
  allStop();
  motor_unStick();
  wipeEncoders();
}

void encoderRun3()
{
  //turns the opposite way 90 degrees hypothetically with small correction at the end to make sure square
  // possibility of using Gyro
  wipeEncoders();
  int inEncoderL = 0;
  int inEncoderR =0;
  static Timers motorSend(50);
  while (((inEncoderL < 20) || (inEncoderR < 20)) && (macro_stop != 1))
  {
    if(MPUTimer.timerDone()) updateMPU();
    if (motorSend.timerDone())
    {      
      sendMotorCommand(-15,15);//(leftMotor,rightMotor)
    }
    inEncoderL+=encoderL;
    inEncoderR+=encoderR;
    wipeEncoders();
    macroCommunicationsUpdate();
  }
  float encoderDif = inEncoderL-inEncoderR;
  //makes sure that the encoders are ending up at the same distance to ensure that robot ends up flush
  while(((encoderDif >3) || (encoderDif<-3)) && (macro_stop != 1))
  {
    if(MPUTimer.timerDone()) updateMPU();
    //    Serial.print("EncoderDiff: ");
    //    Serial.println(encoderDif);
    if(motorSend.timerDone()){
      //if the left encoder is bigger than the right encoder
      if(encoderDif>3)
      {
        //have to continure the turn in the same direction
        sendMotorCommand(-2,10);//(leftMotor,rightMotor)
        inEncoderL-=encoderL;
        inEncoderR+=encoderR;
      }
      //if the right encoder is bigger than the left encoder
      if(encoderDif<3)
      {
        //have to continure the turn in the same direction
        sendMotorCommand(-10,2);//(leftMotor,rightMotor)
        inEncoderL+=encoderL;
        inEncoderR-=encoderR;
      }
      encoderDif = inEncoderL-inEncoderR;
      wipeEncoders();
    }
    macroCommunicationsUpdate();
  }
  allStop();
  motor_unStick();
  wipeEncoders();
}

void encoderRun5()
{
  while(macro_stop!=1)
  {
    runEncoderDistanceEvenly(100);   
    runEncoderDistanceEvenly(-100);
  }
}

void runRightEncoder(int cm)
{
  wipeEncoders();
  //Position PID loop to target an end distance
  PID output(cm, encoderKp, encoderKi, encoderKd, 2);
  PIDTimer.resetTimer();
  while((macroEncoderR<abs(cm)) && (macro_stop != 1))
  {

    if(MPUTimer.timerDone()) updateMPU();
    if(PIDTimer.timerDone())
    {
      motorOutputControlRight(output.updateOutput(macroEncoderR));
      //output.verboseCalc();
    }

    macroCommunicationsUpdate();    
  }

  allStop();
  motor_unStick();
  wipeEncoders();
}

void runLeftEncoder(int cm)
{
  wipeEncoders();
  //Position PID loop to target an end distance
  PID output(cm, encoderKp, encoderKi, encoderKd, 2);
  PIDTimer.resetTimer();
  while(((macroEncoderL<abs(cm))) && (macro_stop != 1))
  {

    if(MPUTimer.timerDone()) updateMPU();
    if(PIDTimer.timerDone())
    {
      motorOutputControlLeft(output.updateOutput(macroEncoderL));
      //output.verboseCalc();
    }

    macroCommunicationsUpdate();    
  }

  allStop();
  motor_unStick();
  wipeEncoders();
}

void runEncoderDistance(int cm)
{
  wipeEncoders();
  //Position PID loop to target an end distance
  PID output(abs(cm), encoderKp, encoderKi, encoderKd, 2);
  PIDTimer.resetTimer();
  while(((macroEncoderL<abs(cm))||(macroEncoderR<abs(cm))) && (macro_stop != 1))
  {

    if(MPUTimer.timerDone()) updateMPU();
    if(PIDTimer.timerDone())
    {
      if(cm>0)
        motorOutputControl(output.updateOutput((macroEncoderL+macroEncoderR)/2));
      else       
        motorOutputControl(-output.updateOutput((macroEncoderL+macroEncoderR)/2));
      //output.verboseCalc();
    }

    macroCommunicationsUpdate();    
  }

  allStop();
  motor_unStick();
  wipeEncoders();
}

#define GYRO true
#define NOGYRO false
void runEncoderDistanceEvenly(float cm)
{
  //SET INTERNAL GYRO ANGLE TO ZERO
  gyroF1.zeroInternalAngle();
  gyroF2.zeroInternalAngle();
  //Internal macroAngle Keeper for this macro
  macroAngle=0;
  //Zero the encoder variables in the comms system
  wipeEncoders();
  
  //Position PID loop to target the distance requested
  PID output(abs(cm), encoderKp, encoderKi, encoderKd, 2);
  //Timer queue up for running
  PIDTimer.resetTimer();
  
   
  do
  {

    
    if(MPUTimer.timerDone()) updateMPU();
    if(PIDTimer.timerDone())
    {
     
	//if the encoders (left to right) are equal
    if(macroEncoderL==macroEncoderR)
    {
	  if(cm>0)
		motorOutputControl(constrain(output.updateOutput(macroEncoderL),0,75));
	  else
		motorOutputControl(-constrain((output.updateOutput(macroEncoderL)),0,75));
	  
    }
    //else the encoders are not even
    else
    {
	  //If requested direction is positive
	  if(cm>0)
	  {
		motorOutputControlCorrect(constrain((output.updateOutput((macroEncoderL+macroEncoderR)/2.0)),0,75),macroEncoderL,macroEncoderR,GYRO);
	  }
	
	//If requested direction is negative
	else
	  {
		motorOutputControlCorrect(-constrain((output.updateOutput((macroEncoderL+macroEncoderR)/2.0)),0,75),macroEncoderL,macroEncoderR,GYRO);
	  }
    }
    
    
     // output.verboseCalc();
    }

    macroCommunicationsUpdate();    
  }
  while(((macroEncoderL<abs(cm))||(macroEncoderR<abs(cm))) && (macro_stop != 1));

  allStop();
  motor_unStick();
  wipeEncoders();
}

#define DEADZONE_ENCODER 50

bool isInRange(signed long checkNum, signed long target, signed long range)
{
  return (((checkNum-range)<target) && ((checkNum+range)>target));
}

void newEncoders(signed long cm)
{
  //SET INTERNAL GYRO ANGLE TO ZERO
  gyroF1.zeroInternalAngle();
  gyroF2.zeroInternalAngle();
  //Internal macroAngle Keeper for this macro
  macroAngle=0;
  //Zero the encoder variables in the comms system
  wipeEncoders();
  
  //Position PID loop to target the distance requested
 // PID output(cm, encoderKp, encoderKi, encoderKd, 2);
  //Timer queue up for running
  PIDTimer.resetTimer();
  Timers CommsDelayTiming(5);
 while( !(isInRange(macroEncoderL,cm,DEADZONE_ENCODER)&&isInRange(macroEncoderR,cm,DEADZONE_ENCODER)) && (macro_stop != 1))
  {
    //if(MPUTimer.timerDone()) updateMPU();
    if(PIDTimer.timerDone())
    {    
        simpleMotorDistanceCommand(cm);
    }
     // output.verboseCalc();
     if(CommsDelayTiming.timerDone())
        macroCommunicationsUpdate();    
  }
  
  //allStop();
  motor_unStick();
  delay(15);
  wipeEncoders();
}




void runEncoderDistanceDiggingly(float cm)
{
  if(cm<0)  cm=cm*0.877;
  gyroF1.zeroInternalAngle();
  gyroF2.zeroInternalAngle();
  
  macroAngle=0;
  wipeEncoders();
  //Position PID loop to target an end distance
  PID output(abs(cm), encoderKp, encoderKi, encoderKd, 2);
  PIDTimer.resetTimer();
  while(((macroEncoderL<abs(cm))||(macroEncoderR<abs(cm))) && (macro_stop != 1))
  {

    
    if(MPUTimer.timerDone()) updateMPU();
    if(PIDTimer.timerDone())
    {
    //Serial.println("IN MACRO");  
      if(macroEncoderL==macroEncoderR)
      {
        if(cm>0)
            motorOutputControlDig(constrain(output.updateOutput(macroEncoderL),0,75));
          else
            motorOutputControlDig(-constrain((output.updateOutput(macroEncoderL)),0,75));
          
      }
      else
      {
        if(cm>0)
          motorOutputControlCorrectDig(constrain((output.updateOutput((macroEncoderL+macroEncoderR)/2.0)),0,75),macroEncoderL,macroEncoderR,GYRO);
        else
          motorOutputControlCorrectDig(-constrain((output.updateOutput((macroEncoderL+macroEncoderR)/2.0)),0,75),macroEncoderL,macroEncoderR,GYRO);
      }
    
    
     // output.verboseCalc();
    }

    macroCommunicationsUpdate();    
  }
  
  allStop();
  motor_unStick();
  //wipeEncoders();
}


void wipeEncoders()
{
  macroEncoderL = 0;
  macroEncoderR = 0;
}

