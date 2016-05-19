
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
//#include "interrupt_handler.h"
#include "PWM.h"
#include "I2C_API_GYRO.h"
#include "UART_handler.h"
//#include "GYRO_debug_UART.h"
#include "gyro.h"

bool GYRO_DEBUG = false;
static int cycleCounter = 0;
bool RUN_DATAFUNCTION = false;
int rightAngleOverride, leftAngleOverride;
volatile unsigned int wiiTime = 0;
int receiveArray[20];
int constrain(int value, int lowBound, int highBound);

void indicatorsOFF()


{
    INDICATOR1 = OFF;
    INDICATOR2 = OFF;
    INDICATOR3 = OFF;
    INDICATOR4 = OFF;
}

void debugLEDs(int debug_point)
{
    debug_point++;
    indicatorsOFF();
    if (debug_point % 8 != 0) LATGbits.LATG8;
    if (debug_point % 4 != 0) LATGbits.LATG7;
    if (debug_point % 2 != 0) LATEbits.LATE5;
    if (debug_point % 1 != 0) LATEbits.LATE6;
}
void sendOverrideAngles();

enum WII_state currentState = CONTINUOUS_AQUISITION;

int leftCameraTarget, rightCameraTarget;

void delay(int ms);

int main(void)
{
    initialize();
    GYRO_DEBUG = false;      //If set to true will run the UART outputting angle info at a 115200 baud-rate
    InitI2Cone();           //initializes the I2c
    
   // if(GYRO_DEBUG) UartInit();  //initializes the UART if debug mode is on
   
    
   // SetSleepEnabled(false); 
    GYRO_initialize(0x69);           //initializing the gyros(SETTING THE SLEEP BITS)
    if(GYRO_DEBUG) UART_init();  //initializes the UART if debug mode is on
      
    get_BaseMotion();           //getting the average values from the gyro and accel
    //initCamera(0);
    //initCamera(1);
    delay(50);
    while (1)
    {


        updateEncoders();
        while (receiveData())
        {
            if (currentState != receiveArray[WII_SUBSYSTEM_MODE])
            {
                currentState = receiveArray[WII_SUBSYSTEM_MODE];
                resetWiiBeaconStates();
            }
            leftCameraTarget = receiveArray[WII_LEFT_CAMERA_MODE];
            rightCameraTarget = receiveArray[WII_RIGHT_CAMERA_MODE];
            //            if (receiveArray[ROBOT_MOVING] != 0) {
            //                receiveArray[ROBOT_MOVING] = 0;
            //                resetWiiBeaconStates();
            //            }
            leftAngleOverride = receiveArray[SERVO_OVERRIDE_LEFT];
            rightAngleOverride = receiveArray[SERVO_OVERRIDE_RIGHT];
        }
        
        
        if(RUN_DATAFUNCTION)    //flagged when the the timer interrupt is triggered
        {
            get_Movement_6();       //getting the data points from the GYRO
            getAcceleration();      //Calculating the acceleration vevtors
            getAngles();            //Calculating the angles on the z, y and x axis
            RUN_DATAFUNCTION = false;   //clearing the flag
        }
        
        if(GYRO_DEBUG == true)
        {
            if(cycleCounter >= 4 )//sending data through the uart every third count of the timer interrupt
            {
                MessageParsser(angleX[0], angleY[0], angleZ[0]);
                cycleCounter = 0;       //reseting the iterator
            }       
        }

#define SUCCESS  1
#define FAIL  2
#define PEND  0


    }
}
void setRUN_DATAFUNCTION(bool status)
{
    RUN_DATAFUNCTION = status;
}
void setCycleCounter(void)
{
    cycleCounter++;
}
bool getGYRO_DEBUG(void)
{
    return GYRO_DEBUG;
}

int constrain(int value, int lowBound, int highBound)
{
    if (value < lowBound) value = lowBound;
    if (value > highBound) value = highBound;
    return value;

}