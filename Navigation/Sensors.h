//Sensors.h

float lastGyroAngle=0;
void wipeGyro(){
  //lastGyroValue=0;
  macroAngle=0;
}

void updateMPU()
{
  // if(gyroTimer.timerDone())
  //{
     get_Movement_6();

     RemoveGravity();
	
     GyroAngle(); 
    
  //Serial.print("x"); Serial.println(angle_x); 
//  Serial.print("y"); Serial.println(angle_y);
//  Serial.print("z"); Serial.println(angle_z);
  macroAngle+=angle_x-lastGyroAngle;
  lastGyroAngle=angle_x;
  //Serial.print("LastGAngle: "); Serial.println(lastGyroAngle); 
  //Serial.print("macroAngle: "); Serial.println(macroAngle); Serial.println();
 
//  Serial.print("a");Serial.println(accelerationX);
//  
//  Serial.print("b");Serial.println(accelerationY);
//  Serial.print("c");Serial.println(accelerationZ); 
  //static float testAngle=0;
  //testAngle+=angle_y;
  //Serial.print("test angle: "); Serial.println(testAngle); Serial.println();
 // }
//  MPU1.readGyroY();
//  MPU2.readGyroY();
//  
//  angle1=gyroF1.filterRawGyro(MPU1._yGyro,14.2);
//  angle2=gyroF2.filterRawGyro(MPU2._yGyro,14.2);  
//  macroAngle=(angle1+angle2)/2;
  
//  velocity1=accelF1.filterRawAccel(MPU1._zAccel,4095);
//  velocity2=accelF2.filterRawAccel(MPU2._zAccel,4095);
  
//  now=millis();
//  //Calculate the position of accel values
//  travelD1+=velocity1*((now-last)/1000.0);
//  travelD2+=velocity2*((now-last)/1000.0);
//  last=now;
//  
//  totalTravelD=(travelD1+travelD2)/2;
}

void initMPUFilters()
{
    //accelF1.wipeVelocity();
    //accelF2.wipeVelocity();
//    macroAngle=0;
//  for (int k=0;k<50;k++)
//      {
//        MPU1.readGyroY();
//        sampleSet1[k]=MPU1._yGyro;
//        MPU2.readGyroY();
//        sampleSet2[k]=MPU2._yGyro;
//        delay(20);
//      }
//      gyroF1.calibrateFilter(sampleSet1);    
//      gyroF2.calibrateFilter(sampleSet2);
//      for (int k=0;k<100;k++)
//      {
//        sampleSet[k]=MPU1._zAccel;
//        delay(20);
//      }
//      accelF1.calibrateFilter(sampleSet);
//      for (int k=0;k<100;k++)
//      {
//        sampleSet[k]=MPU2._zAccel;
//        delay(20);
//      }      
//      accelF2.calibrateFilter(sampleSet);
//      
//      velocity1=0;
//      velocity2=0;
//      travelD1=0;
//      travelD2=0;
}




