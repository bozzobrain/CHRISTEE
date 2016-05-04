//macros_gyro.h

int gyroLow = 15;
int gyroHigh = 40;

int gyroCorrectL = 55, sideDeadL = 4;    //1000000000
int gyroCorrectR = 55, sideDeadR = 4;
//PID GYRO TURN
float gyroKp = 2.85   , gyroKi = 0, gyroKd = 1000; //0.5,0,55


//void updateGyros() {
//  angle1 = gyro1.updateGyro();
//  angle2 = gyro2.updateGyro();
//  macroAngle = (angle1 + angle2) / 2;
//}
//-----------------turning helper method.-----------------------
inline void turnHelp(int mag) {
  if (mag > 0)
    mag = constrain(mag, motorLowG, motorHighG);
  else
    mag = constrain(mag, -motorHighG, -motorLowG);

  sendMotorCommand(-mag,mag);
}
//******************GYROSCOPE TURNING MACRO******************
inline bool doTurn(int setAngle)
{
  int magnitude = 0;

  //internalize the number of degrees to turn

  angleSet = setAngle;

  macroAngle=0;    
  //zero macro angle
  //gyroF1.zeroInternalAngle();
  //gyroF2.zeroInternalAngle();
  PID output(setAngle, gyroKp, gyroKi, gyroKd, 2);
  output.clearSystem();
  PIDTimer.resetTimer();
  //while the robot is not0 facing the angle requested

  Timers CommsDelayTiming(5);
  //for (int i = 0; i < 2; i++) {
  while (!((macroAngle < (angleSet + 1)) && (macroAngle > (angleSet - 1))) &&  (stored_macro_command != 0))
  {
    
    if(CommsDelayTiming.timerDone()) macroCommunicationsUpdate();
    //update our internal angle
    if(MPUTimer.timerDone()) updateMPU();
    //update the PID system timer
    if (PIDTimer.timerDone())  //Check if the timer is done
    {
      //update decision making and sending
      turnHelp(output.updateOutput(macroAngle));    
      PIDTimer.resetTimer();
    }
  }

  //freeze motors after complete
  allStop();
  delay(5);
  motor_unStick();

  //zero macro angle
  //gyroF1.zeroInternalAngle();
  //gyroF2.zeroInternalAngle();
  macroAngle=0;

  //END TURNING
  return (stored_macro_command != 0);
}


