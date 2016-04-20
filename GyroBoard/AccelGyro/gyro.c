/*
 * File:   AccelGyro.c
 * Author: John
 *
 * Created on April 15, 2016, 4:32 PM
 */


#include <xc.h>
#include <stdbool.h>
#include <math.h>
#include "gyro.h"
#include "I2C_API.h"


bool initialize(int address)
{
    calibrate();
    //TODO: setGyroOffset to O and accelOffset to 0
    //TODO: make and is valid function verifying the the gyros are all giving the same reliable data
    //TODO: initializer for the gyro from the mpu sampling sketch
    return 0;
}
void meanGyroAccel()
{
    long iterator = 0;
    const short SAMPLE_SIZE = 1023;    //this number is the result of 2 to the power of 10 which allows a bit shift when finding the average
	
	while (iterator < (SAMPLE_SIZE + 101)){
		
		//Get the raw data values from the gyro and accel
        get_Movement_6();
		//Gyro::getData6();
		//We toss the first 100 reading
		if (iterator > 100 && iterator <= (SAMPLE_SIZE + 100)){
			//adding the the next 1024 raw data values to the variable so they can eventually be averaged 
			ax_average += rawAX;
			ay_average += rawAY;
			az_average += rawAZ;

			gx_average += rawGX;
			gy_average += rawGY;
			gz_average += rawGZ;
			if (iterator == (SAMPLE_SIZE + 100)){
				//Averaging the values....
				ax_average /= SAMPLE_SIZE;
				ay_average /= SAMPLE_SIZE;
				az_average /= SAMPLE_SIZE;
                
				gx_average /= SAMPLE_SIZE;
				gy_average /= SAMPLE_SIZE;
				gz_average /= SAMPLE_SIZE;
			}		
		}
		iterator++;
		while(TMR1 < 1)
        {
        }
        }

}

void calibrate()
{
  	/*We start off with sending a guess offset value to the mpu6050. (the averaged value divided by 8)
	* Then we take another set of averaged raw data values and look to
	* see how close we get to the dead zone(to zero within a set amount).
	*
	*/
	//getmotionData();
	//get_Movement_6();
    short ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
    
    meanGyroAccel();

	//this is divided by 8 to be sure that we don't over shoot the zero by to far
	ax_offset = -ax_average / 8;
	ay_offset = -ay_average / 8;
	az_offset = -az_average / 8;

	gx_offset = -gx_average / 4;
	gy_offset = -gy_average / 4;
	gz_offset = -gz_average / 4;

	while (1){
        //#IMPORTANT
		setAcclOffset(ax_offset, ay_offset, az_offset);
		setGyroOffset(gx_offset, gy_offset, gz_offset);
	
		meanGyroAccel();

		int ifReady = 0;
		if (abs(ax_average) <= acel_deadzone){
			ifReady++;
		}
		else ax_offset -= ax_average / acel_deadzone;

		if (abs(ay_average) <= acel_deadzone){
			ifReady++;
		}
		else ay_offset = ay_offset - ay_average/ acel_deadzone;

		if (abs(16384 - az_average) <= acel_deadzone){
			ifReady++;
		}
		else az_offset = az_offset + (16384 - az_average) / acel_deadzone;

		if (abs(gx_average) <= gyro_deadzone){
			ifReady++;
		}
		else gx_offset -= gx_average / gyro_deadzone;

		if (abs(gy_average) <= gyro_deadzone){
			ifReady++;
		}
		else gy_offset -= gy_average / gyro_deadzone;

		if (abs(gz_average) <= gyro_deadzone){
			ifReady++;
		}
		else gz_offset -= gz_average / gyro_deadzone;
		
		if (ifReady == 6) break;
	}
}


void getAngles()
{
    
	//Finding the accelerometer angle relative to gravity
	float accel_angle_x = atan2(rawAX, rawAZ)*RAD_TO_DEGREE;
	float accel_angle_y = atan2(rawAY, rawAZ)*RAD_TO_DEGREE;

	//Stabilizes the data and zeros it(depending on the accuracy of the initial calibration this may not be necessary(?)) 
	//gyro_axis = (gyroData - (Calibrated Value When Motionless(average of first 20 values:see - Calibrate_Sensor()) / (Sensitivity Scale Factor);
	int gyro_x = (rawGX - gx_average) / 131;
	int gyro_y = ((rawGY - gy_average) / 131) * -1;
	int gyro_z = (rawGZ - gx_average) / 84.5;

	float dt = 0.01;
	//looking at the change in rotation and adds it the last angle value
	float gyro_angle_x = gyro_x*dt + angleX[1];
	float gyro_angle_y = gyro_y*dt + angleY[1];
	float gyro_angle_z = gyro_z*dt + angleZ[1];


	//Low_Pass_Filter/complementary 
	//It is important the the accel_angle_<axis> is mixed on the OPPOSITE Gyro axis. 
	//Because When you rotate around an axis of the gyro you are changing the angle of the accelerometer on the opposite axis.(GyroX to AccelY)(GyroY to AccelX)

	float beta = 0.94;
	angleX[0]= beta*gyro_angle_x + (1.0 - beta)*accel_angle_y;
	angleY[0] = beta*gyro_angle_y + (1.0 - beta)*accel_angle_x;
	angleZ[0] = gyro_angle_z;  //Accelerometer doesn't give z-angle

	angleX[1] = angleX[0];
	angleY[1] = angleY[0];
	angleZ[1] = angleZ[0];

}
void getAcceleration()
{
     //A global variable named frontGrav, sideGrav, upGrav need to be made
    const float alpha = 0.1;
    accelerationX = rawAX;
    accelerationY = rawAY;
    accelerationZ = rawAZ;
    
    //Implementing a simple lowpass filter to remove the gravity vectors
    frontGrav = alpha * frontGrav + (1 - alpha) * accelerationX;
        //Subtracting the gravity from the Accelerometer data
    accelerationX = accelerationX - frontGrav;
    
    sideGrav = alpha * sideGrav + (1 - alpha) * accelerationY;
    accelerationY = accelerationY - sideGrav;
    
    upGrav = alpha * upGrav + (1 - alpha) * accelerationZ;
    accelerationZ = accelerationZ - upGrav;
}

void get_Movement_6()
{
  // read raw accel/gyro measurements from device
 //Changing the values of ax, ay and az according to the orientation of the Accel and gyro chip
    if(ReceiveI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_ACCEL_XOUT_H,(unsigned char *) &myData.reg, 2))
    {
        rawAX = myData.value.gyroInData;
    }
    if(ReceiveI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_ACCEL_YOUT_H,(unsigned char *) &myData.reg, 2))
    {
        rawAY = myData.value.gyroInData;
    }
    if(ReceiveI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_ACCEL_ZOUT_H,(unsigned char *) &myData.reg, 2))
    {
        rawAZ = myData.value.gyroInData;
    }
    if(ReceiveI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_ACCEL_XOUT_H,(unsigned char *) &myData.reg, 2))
    {
        rawAX = myData.value.gyroInData;
    }
    if(ReceiveI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_ACCEL_YOUT_H,(unsigned char *) &myData.reg, 2))
    {
        rawAY = myData.value.gyroInData;
    }
    if(ReceiveI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_ACCEL_ZOUT_H,(unsigned char *) &myData.reg, 2))
    {
        rawAZ = myData.value.gyroInData;
    }
    
    //TODO: Change the values of the axis variable to accommodate the orientation of the chips
    
 switch (forwardAxis) {
      case 0:
        //accelgyro.getMotion6(&rawAX, &ay, &az, &gx, &gy, &gz);

          
          break;
      case 1:
        //accelgyro.getMotion6(&ax, &az, &ay, &gx, &gz, &gy);

        break;
      case 2:
         //accelgyro.getMotion6(&ay, &ax, &az, &gy, &gx, &gz);
        
        break;
      case 3:
         //accelgyro.getMotion6(&ay, &az, &ax, &gy, &gz, &gx);
        
        break;  
      case 4:
         //accelgyro.getMotion6(&az, &ay, &ax, &gz, &gy, &gx);
       
        break; 
      case 5:
         //accelgyro.getMotion6(&az, &ax, &ay, &gz, &gx, &gy);
       
        break;   
     }
  }

void setAcclOffset(int AXoffset, int AYoffset, int AZoffset)
{
    bool XPass = false, YPass = false, ZPass = false;
    int counter = 0;
    while(counter != 2)
    {
        if( SendI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_XA_OFFS_H, (unsigned char *)&AXoffset , 1) && XPass != true)
        {
            XPass = true;
        }
        if( SendI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_XA_OFFS_H, (unsigned char *)&AXoffset , 1) && YPass != true)
        {
            YPass = true;
        }
        if( SendI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_XA_OFFS_H, (unsigned char *)&AXoffset , 1) && ZPass != true)
        {
            ZPass = true;
        }
        counter++; 
        //TODO:IF THIS FAILS MORE THEN 3 time?
        //check if the data was sent and received by the slave not just filled the buffer and the I2c line was open
    }   
   
}
void setGyroOffset(int GXoffset, int GYoffset, int GZoffset)
{
    bool XPass = false, YPass = false, ZPass = false;
    int counter = 0;
    while(counter != 2)
    {
        if( SendI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_XA_OFFS_H, (unsigned char *)&GXoffset , 1) && XPass != true)
        {
            XPass = true;
        }
        if( SendI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_XA_OFFS_H, (unsigned char *)&GYoffset , 1) && YPass != true)
        {
            YPass = true;
        }
        if( SendI2Cone(GYROACCEL_1_ADDRESS_H, MPU6050_RA_XA_OFFS_H, (unsigned char *)&GZoffset , 1) && ZPass != true)
        {
            ZPass = true;
        }
        counter++; 
        //TODO:IF THIS FAILS MORE THEN 3 time?
        //check if the data was sent and received by the slave not just filled the buffer and the I2c line was open
    }   
}

void setFullScaleAccelRange(int)
{
    //TODO:GET A FUNCTION in the I2C library that will send the bits as needed for the full scale ranges
}
void setFullScaleGyroRange(int)
{
    
}

//********************************
//    Miscellaneous Functions    *
//********************************
//Math function that finds the absolute  value of a number   
int abs(int givenNum)    
{
    if (givenNum > 0)
        return givenNum;
    else 
        return -givenNum;
    
}
  
    
