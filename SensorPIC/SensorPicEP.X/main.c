
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"

//




enum WII_state currentState = CONTINUOUS_AQUISITION;

int leftCameraTarget, rightCameraTarget;

void delay(int ms);

int main(void) {
    initialize();
    initCamera(0);
    initCamera(1);
    delay(50);
    while (1) {

        if (encoderTime > 25) {
            //INDICATOR3^=1;
            encoderTime = 0;
            sendEncoderValues();
        }
        if (SpeedCalcRight == true) {
            SpeedCalcRight = false;
            //RightSpeedCalculation();
        }

        if (SpeedCalcLeft == true) {
            SpeedCalcRight = false;
            //LeftSpeedCalculation();
        }

//        if(EncoderLeft>0)
//            INDICATOR1=ON;
//        if(EncoderRight>0)
//            INDICATOR2=ON;
        if (receiveData()) {
            if (currentState != receiveArray[WII_SUBSYSTEM_MODE]) {
                currentState = receiveArray[WII_SUBSYSTEM_MODE];
                resetWiiBeaconStates();
            }
            leftCameraTarget = receiveArray[WII_LEFT_CAMERA_MODE];
            rightCameraTarget = receiveArray[WII_RIGHT_CAMERA_MODE];
            if (receiveArray[ROBOT_MOVING] != 0) {
                receiveArray[ROBOT_MOVING] = 0;
                resetWiiBeaconStates();
            }
        }
        switch (currentState) {
            case TRIG:
                doXYAcquisition();
                break;
            case LEFT_BEACON_ANGLES:
                doVerticalBeaconAcquisition();
                break;
            case RIGHT_BEACON_ANGLES:
                doHorizontalBeaconAcquisition();
                break;
            case CAMERA_OVERRIDE_BEACON_ANGLES:
                doOverrideBeaconAcquisition(leftCameraTarget, rightCameraTarget);
                break;
            case CONTINUOUS_AQUISITION:
                doContinuousAcquisition();
                break;
            case MAXENUMS:

                break;
        }

    }
}

void sendEncoderValues() {
    ToSend(ENCODER_R_NAVIGATION, EncoderRight);
    ToSend(ENCODER_L_NAVIGATION, EncoderLeft);
    ToSend(ENCODER_SPEED_R_NAVIGATION, EncoderRight);
    ToSend(ENCODER_SPEED_L_NAVIGATION, EncoderLeft);
    sendData(NAVIGATION_ADDRESS);
    ToSend(ENCODER_R_CONTROL, EncoderRight);
    ToSend(ENCODER_L_CONTROL, EncoderLeft);
    ToSend(ENCODER_SPEED_R_CONTROL, EncoderRight);
    ToSend(ENCODER_SPEED_L_CONTROL, EncoderLeft);
    sendData(CONTROL_ADDRESS);
}