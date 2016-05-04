//#include <helper_3dmath.h>
//#include <MPU6050.h>
//#include <MPU6050_6Axis_MotionApps20.h>
//#include <MPU6050_9Axis_MotionApps41.h>


//#include "motion.h"
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "math.h"
#include "Wire.h"
//#include "Gyro.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro(0x69);
//MPU6050 backUpAccelgyro(0x68);
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
//Gyro myGyro(0X68);
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t ax2, ay2, az2;
int16_t gx2, gy2, gz2;
  /* forwardAxis Value Number scheme(Accelorometer Orientation)
  0 = X is Front, Y is Sides, and Z is Up
  1 = X is Front, Z is Sides, and Y is Up
  2 = Y is Front, X is Sides, and Z is Up
  3 = Y is Front, Z is Sides, and X is Up
  4 = Z is Front, Y is Sides, and X is Up
  5 = Z is Front, X is Sides, and Y is Up
*/ 


short forwardAxis = 0;

void GyroAngle();
void RemoveGravity();
void get_Movement_6();
void calibration();

const double RadToDegree =  57.295779513082320876798154814105;
float frontGrav = 0;
float sideGrav = 0;
float upGrav = 0;

float accelerationX, accelerationY, accelerationZ;

float base_x_gyro,base_y_gyro,base_z_gyro;  
float lastGyroX, lastGyroY, lastGyroZ;
float angle_x,angle_y,angle_z;

int sampleSize = 50;
float ax_average, ay_average, az_average, gx_average, gy_average, gz_average;
float ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

int acel_deadzone=8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int gyro_deadzone=3;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)



void initGyroSeth(void)
{
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
     //Serial.begin(115200);
  
    // initialize device
   // Serial.println("Initializing I2C devices...");

    accelgyro.initialize();
     //Serial.print("hi");

    // verify connection
    //Serial.println("Testing device connections...");
    //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    
    // use the code below to change accel/gyro offset values
   
    accelgyro.setXGyroOffset(0);
    accelgyro.setYGyroOffset(0);
    accelgyro.setZGyroOffset(0);
    accelgyro.setXAccelOffset(0);
    accelgyro.setYAccelOffset(0);
    accelgyro.setZAccelOffset(0);
    
    
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);   //8 can be replaced with 2,4,8,or 16
/*
      backUpAccelgyro.setXGyroOffset(-2208);
    backUpAccelgyro.setYGyroOffset(77);
    backUpAccelgyro.setZGyroOffset(-13);
    backUpAccelgyro.setXAccelOffset(-178);
    backUpAccelgyro.setYAccelOffset(11);
    backUpAccelgyro.setZAccelOffset(1440);
    */
    calibration();
   // Serial.print("h");
 // Serial.println("Calibrating Gyro.");
  //Trash first set of Values
  get_Movement_6();
  const int AverageNumber = 25;
  //Averaging the values of the gyro to find the offset to use in finding the angle of rotation
  for (int i = 0; i < AverageNumber; i++) {
    get_Movement_6();
    //Adding the next 15 data points to base_x_gyro    
    base_x_gyro += gx; 
    base_y_gyro += gy;
    base_z_gyro += gz;
    delay(100);
   // Serial.print(".");
  }
  base_x_gyro /= AverageNumber;
  base_y_gyro /= AverageNumber;
  base_z_gyro /= AverageNumber;

 // myGyro._initialize();
}


void GyroAngle(){

  
  //Finding the accelerometer angle relative to gravity
  float accel_angle_x = atan2(ax,az)*RadToDegree;
  float accel_angle_y = atan2(ay,az)*RadToDegree;

  //Stableizes the data and zeros it
  //gyro_axis = (gyroData - (Calibrated Value When Motionless(average of first 20 values:see - Calibrate_Sensor()) / (Sensitivity Scale Factor);
  int gyro_x = (gx - base_x_gyro) / 131;
  int gyro_y = ((gy - base_y_gyro) / 131) * -1;
  int gyro_z = (gz - base_z_gyro) / 84.5;
  
  float dt = 0.01;
  //looking at the change in rotation and adds it the last angle value
  float gyro_angle_x = gyro_x*dt + lastGyroX;
  float gyro_angle_y = gyro_y*dt + lastGyroY;
  float gyro_angle_z = gyro_z*dt + lastGyroZ;

   //Low_Pass_Filter/complementary 
   //It is important the the accel_angle_<axis> is mixed on the OPPOSITE Gyro axis. 
   //Because When you rotate around an axis of the gyro you are changing the angle of the accelerometer on the opposite axis.(GyroX to AccelY)(GyroY to AccelX)
   
   
//   Serial.print("GYROX: "); Serial.println(gyro_x);
//   Serial.print("GYROY: "); Serial.println(gyro_y);
//   Serial.print("GYROZ: "); Serial.println(gyro_z);
   
   
   float beta = 1;//0.94;
   angle_x = beta*gyro_angle_x; //+ (1.0 - beta)*accel_angle_y;
   angle_y = beta*gyro_angle_y; //+ (1.0 - beta)*accel_angle_x;
   angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle
  

  
  lastGyroX = angle_x;
  lastGyroY = angle_y;
  lastGyroZ = angle_z;

}
  
void RemoveGravity(){
    //A global variable named frontGrav, sideGrav, upGrav need to be made
    const float alpha = 0.1;
    accelerationX = ax;
    accelerationY = ay;
    accelerationZ = az;
    
    //Implementing a simple lowpass filter to remove the gravity vectors
    frontGrav = alpha * frontGrav + (1 - alpha) * accelerationX;
        //Subtracting the gravity from the Accelerometer data
    accelerationX = accelerationX - frontGrav;
    
    sideGrav = alpha * sideGrav + (1 - alpha) * accelerationY;
    accelerationY = accelerationY - sideGrav;
    
    upGrav = alpha * upGrav + (1 - alpha) * accelerationZ;
    accelerationZ = accelerationZ - upGrav;
}

void get_Movement_6(){
  // read raw accel/gyro measurements from device
 //Changing the values of ax, ay and az according to the orientation of the Accel and gyro chip
 switch (forwardAxis) {
      case 0:
          accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
          //front_Vec = ax;
          //side_Vec = ay;
          //up_Vec = az;
          break;
      case 1:
         accelgyro.getMotion6(&ax, &az, &ay, &gx, &gz, &gy);
         //front_Vec = ax;
        // side_Vec = az;
         //up_Vec = ay;
        break;
      case 2:
         accelgyro.getMotion6(&ay, &ax, &az, &gy, &gx, &gz);
         //front_Vec = ay;
         //side_Vec = ax;
         //up_Vec = az;
        break;
      case 3:
         accelgyro.getMotion6(&ay, &az, &ax, &gy, &gz, &gx);
         //front_Vec = ay;
         //side_Vec = az;
         //up_Vec = ax;
        break;  
      case 4:
         accelgyro.getMotion6(&az, &ay, &ax, &gz, &gy, &gx);
         //front_Vec = az;
         //side_Vec = ay;
         //up_Vec = ax;
        break; 
      case 5:
         accelgyro.getMotion6(&az, &ax, &ay, &gz, &gx, &gy);
         //front_Vec = az;
         //side_Vec = ax;
         //up_Vec = ay;
        break;   
     }
  }

void getAverageSensor(){
    long iteration = 0, ax_temp = 0, ay_temp = 0, az_temp = 0, gx_temp = 0, gy_temp = 0, gz_temp = 0;

    while(iteration < (sampleSize + 101)){
        //Serial.println("Sampling");
        //Get the raw data values from the gyro and accel
        get_Movement_6();
        //We toss the first 100 reading
        if ((iteration>100)&&iteration <= (sampleSize + 100)){
            //adding the the next 1000 raw data values to the variable so they can eventually be averaged 
            ax_temp += ax;
            ay_temp += ay;
            az_temp += az;
            gx_temp += gx;
            gz_temp += gz;
            gy_temp += gy;
           
       } 
       
       if (iteration == (sampleSize + 100)){
                //Averageing the values.....
                ax_average = ax_temp/sampleSize;
                ay_average = ay_temp/sampleSize;
                az_average = az_temp/sampleSize;
                gx_average = gx_temp/sampleSize;
                gy_average = gy_temp/sampleSize;
                gz_average = gz_temp/sampleSize;
            }
            iteration++;
            delay(2);
  }
}

void calibration(){
    /*We start off with sending a guess offset value to the mpu6050. (the averaged value divided by 8)
     * Then we take another set of averaged raw data values and look to
     * see how close we get to the dead zone(to zero within a set amount).
     * 
     */

    //this is divided by 8 to be sure that we don't over shoot the zero by to far
    ax_offset = -ax_average/8;
    ay_offset = -ay_average/8;
    az_offset = -az_average/8;

    
    gx_offset = -gx_average/4;
    gy_offset = -gy_average/4;
    gz_offset = -gz_average/4;
   while (1){
     
        // accelgyro.getMotion6(&az, &ax, &ay, &gz, &gx, &gy);
    accelgyro.setXAccelOffset((int16_t)ax_offset);
    delay(5);
    accelgyro.setYAccelOffset((int16_t)ay_offset);
    delay(5);
    accelgyro.setZAccelOffset((int16_t)az_offset);
    delay(5);

    accelgyro.setXGyroOffset((int16_t)gx_offset);
    delay(5);
    accelgyro.setYGyroOffset((int16_t)gy_offset);
    delay(5);
    accelgyro.setZGyroOffset((int16_t)gz_offset);
    delay(5);
    //Serial.println("hi");
    getAverageSensor();
      Serial.println();
      Serial.print("ax_average"); Serial.println(ax_average);
      Serial.print("ay_average"); Serial.println(ay_average);
      Serial.print("az_average"); Serial.println(az_average);
      Serial.print("gx_average"); Serial.println(gx_average);
      Serial.print("gy_average"); Serial.println(gy_average);
      Serial.print("gz_average"); Serial.println(gz_average); Serial.println();
  
    int ifReady = 0;
    if (abs(ax_average) <= acel_deadzone){
      ifReady++;
      Serial.print("ax Ready "); Serial.println(ax_offset);
    }
    else ax_offset -= ax_average / acel_deadzone;

    if (abs(ay_average) <= acel_deadzone){
      ifReady++;
      Serial.print("ay Ready "); Serial.println(ay_offset);
    }
    else ay_offset -= (ay_average/ acel_deadzone);

    if (abs( az_average) <= acel_deadzone){   //16384-2g sensativity
      ifReady++;
      Serial.print("az Ready: "); Serial.println(az_offset);
    }
    else az_offset -= ( az_average / acel_deadzone);

    if (abs(gx_average) <= gyro_deadzone){
      ifReady++;
      Serial.print("gx Ready "); Serial.println(gx_offset);
    }
    else gx_offset -= (gx_average / gyro_deadzone);

    if (abs(gy_average) <= gyro_deadzone){
      ifReady++;
      Serial.print("gy Ready "); Serial.println(gy_offset);
    }
    else gy_offset -= gy_average / gyro_deadzone;

    if (abs(gz_average) <= gyro_deadzone){
      ifReady++;
      Serial.print("gz Ready "); Serial.println(gz_offset);
    }
    else gz_offset -= gz_average / gyro_deadzone;
    Serial.print("Ready: "); Serial.println(ifReady);
    
    
      Serial.print("ax_offset"); Serial.println(ax_offset);
      Serial.print("ay_offset"); Serial.println(ay_offset);
      Serial.print("az_offset"); Serial.println(az_offset);
      Serial.print("gx_offset"); Serial.println(gx_offset);
      Serial.print("gy_offset"); Serial.println(gy_offset);
      Serial.print("gz_offset"); Serial.println(gz_offset);
    
    if (ifReady == 6) {
      Serial.println("DONE!!!!");
      Serial.print("ax_offset"); Serial.println(ax_offset);
      Serial.print("ay_offset"); Serial.println(ay_offset);
      Serial.print("az_offset"); Serial.println(az_offset);
      Serial.print("gx_offset"); Serial.println(gx_offset);
      Serial.print("gy_offset"); Serial.println(gy_offset);
      Serial.print("gz_offset"); Serial.println(gz_offset);
      break;
    }
  }
}

