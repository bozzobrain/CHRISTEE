
Timers acquisitionTimer(100);

#define HALFWAY_VALUE 1.89

bool makePerpendicular(bool L_R)
{
  //LEFT CAMERA
  if(L_R)
  {
    //PLUG THIS IN AS THE INITIAL VALUE
    int prevDist=distLeft, currentDist=distLeft;
    //Turn the robot a certain dist...
    doTurn(2);
    
    //load new dist
    currentDist=distLeft;
    //if the distance increased
    if((currentDist-prevDist)>=0)
    {      
      //while it continues to increase
      while(((currentDist-prevDist)>=0) && (stored_macro_command!=0))
      {
        //Small turn
        doTurn(2);       
        //Reset timer
        acquisitionTimer.resetTimer();
        //Wait for camera to reacquire
        while((!acquisitionTimer.timerDone()) && (stored_macro_command!=0))
        {           
          if(MPUTimer.timerDone())     updateMPU();
          macroCommunicationsUpdate();
        }      
        //store old dist
        prevDist=currentDist;
        //Record new dist          
        currentDist=distLeft;
      }
    }
    else
    {
      //load new dist and overright prev dist
      currentDist=distLeft;
      prevDist=distLeft;        
      //while it continues to increase
      while(((currentDist-prevDist)>=0) && (stored_macro_command!=0))
      {
        //Small turn
        doTurn(-2);        
        //Reset timer
        acquisitionTimer.resetTimer();
         //Wait for camera to reacquire
        while((!acquisitionTimer.timerDone()) && (stored_macro_command!=0))
        {           
          if(MPUTimer.timerDone())     updateMPU();
          macroCommunicationsUpdate();
        }
        
        prevDist=currentDist;   
        currentDist=distLeft;  
        
      }      
    }
  }
  
  //-RIGHT CAMERA-
  else
  {    
    //PLUG THIS IN AS THE INITIAL VALUE
    int prevDist=distRight, currentDist=distRight;    
    //Turn the robot a certain dist...
    doTurn(2);    
    //load new dist
    currentDist=distRight;
    //if the distance increased
    if((currentDist-prevDist)>=0)
    {
      //while it continues to increase
      while(((currentDist-prevDist)>=0) && (stored_macro_command!=0))
      {
        //Small turn
        doTurn(2);       
           //Reset timer
        acquisitionTimer.resetTimer();
         //Wait for camera to reacquire
        while((!acquisitionTimer.timerDone()) && (stored_macro_command!=0))
        {           
          if(MPUTimer.timerDone())     updateMPU();
          macroCommunicationsUpdate();
        }
        currentDist=distRight;
        prevDist=currentDist;   
        
      }
    }
    else
    {
        //load new dist and overright prev dist
        currentDist=distRight;
        prevDist=currentDist;
        
       //while it continues to increase
      while(((currentDist-prevDist)>=0) && (stored_macro_command!=0))
      {
        //Small turn
        doTurn(-2);
        //Reset timer
        acquisitionTimer.resetTimer();
        //Wait for camera to reacquire
        while((!acquisitionTimer.timerDone()) && (stored_macro_command!=0))
        {           
          if(MPUTimer.timerDone())     updateMPU();
          macroCommunicationsUpdate();
        }        
        prevDist=currentDist;     
        currentDist=distRight;

      }      
    }
  }
}

bool makeCentered(bool L_R)
{
  int target_angle=0;
    //LEFT CAMERA
  if(L_R)
  {
    //USING THE LEFT CAMERA AS CONST ANGLE
     target_angle=leftAngle;
     //WHILE THE LEFT AND RIGHT ARE NOT SIMILAR TO WITHIN 5 degrees
     while((!isAbout(target_angle,rightAngle,5)) && (stored_macro_command!=0))
     {
       //Move to attempt to align the cameras to same angle value
       newEncoders(-10);
       //Reset timer
       acquisitionTimer.resetTimer();
       //Wait for camera to reacquire
       while((!acquisitionTimer.timerDone()) && (stored_macro_command!=0))
       {           
          if(MPUTimer.timerDone())     updateMPU();
          macroCommunicationsUpdate();
       }   
   
      //MAY WANT THIS TO UPDATE THE ANGLE BUT FOR NOW DO NOT   
        //target_angle=leftAngle;     
     }
  }
  
  //RIGHT CAMERA
  else
  {
     //USING THE RIGHT CAMERA AS CONST ANGLE
     target_angle=rightAngle;  
     //WHILE THE LEFT AND RIGHT ARE NOT SIMILAR TO WITHIN 5 degrees  
     while((!isAbout(target_angle,leftAngle,5)) && (stored_macro_command!=0))
     {
       //Move to attempt to align the cameras to same angle value
         newEncoders(-10);     
        //Reset timer
        acquisitionTimer.resetTimer();
        //Wait for camera to reacquire
        while((!acquisitionTimer.timerDone()) && (stored_macro_command!=0))
        {           
          if(MPUTimer.timerDone())     updateMPU();
          macroCommunicationsUpdate();
        } 
    //MAY WANT THIS TO UPDATE THE ANGLE BUT FOR NOW DO NOT   
        //target_angle=rightAngle; 
     } 
  }  
}
bool angleDataValid()
{  
  return true;
}
bool wiiCameraLocalize()
{
  //MAKE SURE CAMERAS ARE LOCKED
  while((robotSeen==0) && (stored_macro_command!=0))
  {    
        if(MPUTimer.timerDone())     updateMPU();
        macroCommunicationsUpdate();
  }
  
    if(((robotSeen==XY_FOUND || robotSeen==BOTH_CAMERAS) && angleDataValid()) && (stored_macro_command!=0))
    {
   //if X-Y found (both found)
     //if on right (EITHER BY X/Y)
//       if(x_coordinate>HALFWAY_VALUE)
//         makePerpendicular(LEFT_PERPENDICULAR);  //ON RIGHT SIDE
//       else if(x_coordinate<=HALFWAY_VALUE)
//         makePerpendicular(RIGHT_PERPENDICULAR);  //ON LEFT SIDE      
//       
       // or by greatest ANGLE of CAMERA
      if(leftAngle>rightAngle)    //LEFT SIDE
      {        
         makePerpendicular(RIGHT_PERPENDICULAR);  //ON LEFT SIDE     
      }
      else                        //RIGHT_SIDE
      {        
         makePerpendicular(LEFT_PERPENDICULAR);  //ON RIGHT SIDE
      }
   
    }
   
   //else determine left or right camera found robot
   //if left camera     
       //make perpendicular to beacon left
    else if(robotSeen==LEFT_CAMERA_ONLY || robotSeen==LEFT_CAMERA_ONLY_1)
       makePerpendicular(LEFT_PERPENDICULAR);
   
   //if right camera
       //make perpendicular to beacon right
    else if(robotSeen==RIGHT_CAMERA_ONLY  || robotSeen==RIGHT_CAMERA_ONLY_1 )
       makePerpendicular(RIGHT_PERPENDICULAR);
     
   
   
}

bool wiiCameraFindTwoCamera(int targetBeacon)
{
  
 

}


