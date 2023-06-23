
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "PWM.h"

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
    //angleZ[0]=-100;
    initialize();
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

#define SUCCESS  1
#define FAIL  2
#define PEND  0

//        if (wiiTime > wiiUpdateFrequency)
//        {
//            //INDICATOR1 ^= 1;
//
//            static bool leftReady = true, rightReady = true;
//
//                static int counterLeft=0, counterRight=0;
//            if (cameraReady(LEFT_CAMERA) == SUCCESS)
//            {
//                counterLeft=0;
//                leftReady = true;
//                readCamera(LEFT_CAMERA);
//            }
//            else if (cameraReady(LEFT_CAMERA) == FAIL)
//            {
//                readCamera(LEFT_CAMERA);
//                //INDICATOR4 ^= 1;
//            }
//            else if (cameraReady(LEFT_CAMERA) == PEND)
//            {
//                leftReady = false;
//                //INDICATOR3 ^= 1;
//                counterLeft++;
//                if(counterLeft>2){
//                    counterLeft=0;
//                    I2CtwoReset();
//                }
//            }
//
//            if (cameraReady(RIGHT_CAMERA) == SUCCESS)
//            {
//                counterRight=0;
//                rightReady = true;
//                readCamera(RIGHT_CAMERA);
//            }
//            else if (cameraReady(RIGHT_CAMERA) == FAIL)
//            {
//                readCamera(RIGHT_CAMERA);
//                //INDICATOR4 ^= 1;
//            }
//            else if (cameraReady(RIGHT_CAMERA) == PEND)
//            {
//                counterRight++;
//                rightReady = false;
//                //INDICATOR3 ^= 1;
//                if(counterRight>2){
//                    I2ConeReset();
//                    counterRight=0;
//                }
//            }
//
//            if (leftReady && rightReady)
//            {
//                WATCHDOG ^= 1;
//                switch (currentState)
//                {
//                    case TRIG:
//                        doXYAcquisition();
//                        break;
//                    case LEFT_BEACON_ANGLES:
//                        doVerticalBeaconAcquisition();
//                        break;
//                    case RIGHT_BEACON_ANGLES:
//                        doHorizontalBeaconAcquisition();
//                        break;
//                    case CAMERA_OVERRIDE_BEACON_ANGLES:
//                        doOverrideBeaconAcquisition(leftCameraTarget, rightCameraTarget);
//                        break;
//                    case CONTINUOUS_AQUISITION:
//                        doContinuousAcquisition();
//                        break;
//                    case SERVO_OVERRIDE_ANGLE:
//                        setAngle1(constrain(leftAngleOverride * 10, SERVO_MIN, SERVO_MAX));
//                        setAngle2(constrain(rightAngleOverride * 10, SERVO_MIN, SERVO_MAX));
//                        break;
//                    case MAXENUMS:
//
//                        break;
//                }
//
//                wiiTime = 0;
//                leftReady = false;
//                rightReady = false;
//            }
//
//        }
    }
}

int constrain(int value, int lowBound, int highBound)
{
    if (value < lowBound) value = lowBound;
    if (value > highBound) value = highBound;
    return value;

}