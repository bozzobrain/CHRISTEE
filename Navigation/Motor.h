//Control the motor signal via feedback from the encoders

#define motorKp 1
#define motorKi 0
#define motorKd 0

PID motorOutput(0, motorKp, motorKi, motorKd, 2);
PID motorOutputL(0, motorKp, motorKi, motorKd, 2);
PID motorOutputR(0, motorKp, motorKi, motorKd, 2);
PID simpleMotorOutput(0, 0.95, 0, 0, 2);

#define TEST_LIMIT_MOTOR_MAG 20
void simpleMotorDistanceLRDiffCommand(signed long commandedSpeed, signed long commandedDistance);

//Pass it a value in cm (commandedDistance
void simpleMotorDistanceCommand(signed long commandedDistance)
{
  
//        Serial.print("EMacro commanded distance: ");
//        Serial.println(commandedDistance);
  //If current target internally is not equal to the received command tell the PID where to go
  if(commandedDistance!=(signed long)simpleMotorOutput.returnTarget())
  {
    simpleMotorOutput.clearSystem();
    simpleMotorOutput.updateTarget((float)commandedDistance);
  }
  static int counter=0;
  //for 10 we should be calculating a decision
  if(counter<10)
  {
   
      simpleMotorOutput.updateOutput((macroEncoderL+macroEncoderR)/2.0);
   
    counter++;
  }
  //every 10 we should make a decision
  else
  {
    //Send the resulting output of the PID output error to a LR differential command method
  
    simpleMotorDistanceLRDiffCommand(constrain(simpleMotorOutput.updateOutput((macroEncoderL+macroEncoderR)/2.0),-TEST_LIMIT_MOTOR_MAG,TEST_LIMIT_MOTOR_MAG), commandedDistance);

    counter=0;
  }
  
}

int grabIntegerSign(signed long i)
{
 if(i>=0)
  return 1;
 else
  return -1; 
}

float grabFloatSign(float i)
{
 if(i>=0)
  return 1;
 else
  return -1; 
  
  
}


#define distanceForOneTreadOperation 10
//Meters a speed input into a variable turning capable differential, allows for equal distance as we go on both treads
//    WILL act as the development for diff driving with new encoders.
void simpleMotorDistanceLRDiffCommand(signed long commandedSpeed, signed long commandedDistance)
{
  
//        Serial.print("EMacro commanded speed: ");
//        Serial.println(commandedSpeed);

  //If the left and right ENCODERS are at the same value
  if(macroEncoderL==macroEncoderR)
  {
    
     sendMotorCommand(commandedSpeed,commandedSpeed); 
  }
  else if(abs(macroEncoderL)>abs(macroEncoderR))
  {
    //calculate the magnitude of the difference
    float diff=abs(macroEncoderL)-abs(macroEncoderR);
    
    //scale it for Left versus right speed command... difference is 0-1 for reduction of tread speed
    if(diff<distanceForOneTreadOperation)  //if the difference is less than distance offset that will drive the tread to one side full
        diff = ((distanceForOneTreadOperation-diff)/distanceForOneTreadOperation);  //As the difference approaches the full one sided drive, the difference approaches commandedSpeed
    else 
        diff = 0;
     
    //Create a commanded speed that is not an average of distances, but an average of the errors
    float newCommandedSpeed= ((abs(macroEncoderR-commandedDistance)+abs(macroEncoderL-commandedDistance))/3.75);
    
    if(abs(commandedSpeed)>=5)
    {
      newCommandedSpeed=abs(commandedSpeed);
    }
   
    //Calculate the errors, to be used for direction of motor command
    int errorRight  = commandedDistance-macroEncoderR;
    int errorLeft   = commandedDistance-macroEncoderL;
    
    //Make new motor command values from the new speed and the direction calculated
    int rightCommand= grabIntegerSign(errorRight) * (newCommandedSpeed);
    int leftCommand = grabIntegerSign(errorLeft)  * (newCommandedSpeed*diff);
    
//    Serial.print("NewCommandedSpeed: ");
//    Serial.print(newCommandedSpeed);
//    Serial.print(", difference: ");
//    Serial.println(diff);
    
    sendMotorCommand(leftCommand,rightCommand);
  }
  else
  {
    //calculate the magnitude of the difference
    float diff=abs(macroEncoderR)-abs(macroEncoderL);
     
    //scale it for Left versus right speed command... difference is 0-1 for reduction of tread speed
    if(diff<distanceForOneTreadOperation)  //if the difference is less than distance offset that will drive the tread to one side full
        diff = ((distanceForOneTreadOperation-diff) / distanceForOneTreadOperation);  //As the difference approaches the full one sided drive, the difference approaches commandedSpeed
    else 
        diff = 0;
    
    //Create a commanded speed that is not an average of distances, but an average of the errors
    float newCommandedSpeed = ((abs(macroEncoderR-commandedDistance)+abs(macroEncoderL-commandedDistance))/3.75);  
    
    if(abs(commandedSpeed)>=5)
    {
      newCommandedSpeed=abs(commandedSpeed);
    }
    
    //Calculate the errors, to be used for direction of motor command
    int errorRight  = commandedDistance-macroEncoderR;
    int errorLeft   = commandedDistance-macroEncoderL;
    
    //Make new motor command values from the new speed and the direction calculated
    int rightCommand = grabIntegerSign(errorRight) * (newCommandedSpeed*diff);
    int leftCommand  = grabIntegerSign(errorLeft)  * (newCommandedSpeed);
    
//    Serial.print("NewCommandedSpeed: ");
//    Serial.print(newCommandedSpeed);
//    Serial.print(", difference: ");
//    Serial.println(diff);
    
    sendMotorCommand(leftCommand,rightCommand);     
  }
}



//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!



void motorOutputControlLeft(int speedRun)
{
  motorOutputL.updateTarget(abs(speedRun));
  int mag;
  if (speedRun > 0)
    mag = constrain(abs(motorOutput.updateOutput(encoderSpeedL)), motorLowForward, motorHighForward);
  else
    mag = constrain(abs(motorOutput.updateOutput(encoderSpeedL)), motorLowBackward, motorHighBackward);

  if (speedRun > 0)
    sendMotorCommand(mag, 0);
  else
    sendMotorCommand(-mag, 0);
}

void motorOutputControlRight(int speedRun)
{
  motorOutputR.updateTarget(abs(speedRun));
  int mag;
  if (speedRun > 0)
    mag = constrain(abs(motorOutput.updateOutput(encoderSpeedR)), motorLowForward, motorHighForward);
  else
    mag = constrain(abs(motorOutput.updateOutput(encoderSpeedR)), motorLowBackward, motorHighBackward);

  if (speedRun > 0)
    sendMotorCommand(0, mag);
  else
    sendMotorCommand(0, -mag);
}



void motorOutputControlCorrect(int speedRun, int eL, int eR)
{
  motorOutput.updateTarget(abs(speedRun));
  int mag;
  if (speedRun > 0)
    mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2.0)), motorLowForward, motorHighForward);
  else
    mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2.0)), motorLowBackward, motorHighBackward);
  int magL, magR;
  if (eL > eR)
  {
    magL = mag - 6;
    magR = mag + 2;
  }
  else
  {
    magL = mag + 2;
    magR = mag - 6;
  }

  if (speedRun > 0)
    sendMotorCommand(magL, magR);
  else
    sendMotorCommand(-magL, -magR);
}
//Encoder feedback to control speed
//speed is a positive or negative number representing a control speed
void motorOutputControl(int speedRun)
{
  motorOutput.updateTarget(abs(speedRun));
  int mag;
  if (speedRun > 0)
    mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowForward, motorHighForward);
  else
    mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowBackward, motorHighBackward);
  if (speedRun > 0)
    sendMotorCommand(mag, mag);
  else
    sendMotorCommand(-mag, -mag);

}

void motorOutputControlCorrect(int speedRun, int eL, int eR, bool useGyro)
{
  if (!useGyro)
  {
    motorOutput.updateTarget(abs(speedRun));
    int mag;
    if (speedRun > 0)
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowForward, motorHighForward);
    else
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowBackward, motorHighBackward);

    int magL, magR;
    if (eL > eR)
    {
      magL = mag - 6;
      magR = mag + 2;
    }
    else
    {
      magL = mag + 2;
      magR = mag - 6;
    }

    if (speedRun > 0)
      sendMotorCommand(magL, magR);
    else
      sendMotorCommand(-magL, -magR);



  }
  else
  {
    motorOutput.updateTarget(abs(speedRun));
    int mag;
    if (speedRun > 0)
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowForward, motorHighForward);
    else
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowBackward, motorHighBackward);

    int magL = mag, magR = mag;
    if (speedRun > 0)
    {
      if ((eL > eR) && (macroAngle > 0))
      {
        magL = mag - 7;
        magR = mag + 2;
      }
      else if ((eL < eR) && (macroAngle < 0))
      {
        magL = mag + 2;
        magR = mag - 7;
      }
    }
    else
    {
      if ((eL > eR) && (macroAngle > 0))
      {
        magL = mag - 7;
        magR = mag + 2;
      }
      else if ((eL < eR) && (macroAngle < 0))
      {
        magL = mag + 2;
        magR = mag - 7;
      }
    }


    if (speedRun > 0)
      sendMotorCommand(magL, magR);
    else
      sendMotorCommand(-magL, -magR);
//    Serial.print(lM);
//    Serial.print(",");
//    Serial.print(rM);
//    Serial.print(",");
//    Serial.print(speedRun);
//    Serial.print(",");
//    Serial.print(encoderSpeedL);
//    Serial.print(",");
//    Serial.print(encoderSpeedR);
//    Serial.print(",");
//    motorOutput.verboseCalc();
  }
}








//Encoder feedback to control speed
//speed is a positive or negative number representing a control speed
void motorOutputControlDig(int speedRun)
{
  motorOutput.updateTarget(abs(speedRun));
  int mag;
  if (speedRun > 0)
    mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowDig, motorHighDig);
  else
    mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowDig, motorHighDig);
  if (speedRun > 0)
    sendMotorCommand(mag, mag);
  else
    sendMotorCommand(-mag, -mag);

}

void motorOutputControlCorrectDig(int speedRun, int eL, int eR, bool useGyro)
{
  if (!useGyro)
  {
    motorOutput.updateTarget(abs(speedRun));
    int mag;
    if (speedRun > 0)
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowDig, motorHighDig);
    else
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowDig, motorHighDig);

    int magL, magR;
    if (eL > eR)
    {
      magL = mag - 6;
      magR = mag + 2;
    }
    else
    {
      magL = mag + 2;
      magR = mag - 6;
    }

    if (speedRun > 0)
      sendMotorCommand(magL, magR);
    else
      sendMotorCommand(-magL, -magR);


  }
  else
  {
    motorOutput.updateTarget(abs(speedRun));
    int mag;
    if (speedRun > 0)
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowDig, motorHighDig);
    else
      mag = constrain(abs(motorOutput.updateOutput((encoderSpeedL + encoderSpeedR) / 2)), motorLowDig, motorHighDig);

    int magL = mag, magR = mag;
    if (speedRun > 0)
    {
      if ((eL > eR) && (macroAngle < 0))
      {
        magL = mag - 6;
        magR = mag + 2;
      }
      else if ((eL < eR) && (macroAngle > 0))
      {
        magL = mag + 2;
        magR = mag - 6;
      }
    }
    else
    {
      if ((eL > eR) && (macroAngle > 0))
      {
        magL = mag - 6;
        magR = mag + 2;
      }
      else if ((eL < eR) && (macroAngle < 0))
      {
        magL = mag + 2;
        magR = mag - 6;
      }
    }


    if (speedRun > 0)
      sendMotorCommand(magL, magR);
    else
      sendMotorCommand(-magL, -magR);

  }
}

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

//OLD STUFFF!!!

