/* 
 * File:   main.h
 * Author: Igor
 *
 * Created on March 26, 2015, 5:39 PM
 */

#ifndef MAIN_H
#define	MAIN_H

const int wiiUpdateFrequency=200;

#define ON         0
#define OFF        1
#define INDICATOR1 LATEbits.LATE5
#define INDICATOR2 LATEbits.LATE6
#define INDICATOR3 LATGbits.LATG7
#define INDICATOR4 LATGbits.LATG8
#define WATCHDOG   LATEbits.LATE7

#define HORIZONTAL_BEACON       0
#define RIGHT_BEACON            0
#define VERTICAL_BEACON         1
#define LEFT_BEACON             1

#define LEFT_CAMERA             0
#define RIGHT_CAMERA            1


    //ADDRESSING FOR ROBOT
#define CONTROL_ADDRESS              5
#define NAVIGATION_ADDRESS           4
#define PIC_ADDRESS                  1
#define MOTOR_ADDRESS                6
#define LED_ADDRESS                  2
#define POWER_ADDRESS	             3


//Receive stuff
#define LAST_BOARD_ADDRESS_RECEIVE   0
//FROM NAV
#define ROBOT_MOVING		     1
#define WII_SUBSYSTEM_MODE           2    //MODES INCLUDE: TRIG=0, LEFT_BEACON_ANGLES=1, RIGHT_BEACON_ANGLES=2, CAMERA_OVERRIDE_BEACON_ANGLES=3
#define WII_LEFT_CAMERA_MODE         3    //MODES 3 and 4 only used for WII_SUBSYSTEM_MODE 3
#define WII_RIGHT_CAMERA_MODE        4            //0=LEFT_BEACON 1=RIGHT_BEACON
//Control override servo --requires mode to be servo override=5
#define SERVO_OVERRIDE_LEFT          5
#define SERVO_OVERRIDE_RIGHT         6

//Send stuff
//TO NAV
#define ENCODER_R_L_NAVIGATION      5
#define ENCODER_R_H_NAVIGATION      23
#define ENCODER_L_L_NAVIGATION        6
#define ENCODER_L_H_NAVIGATION      24
#define ENCODER_SPEED_R_NAVIGATION  7
#define ENCODER_SPEED_L_NAVIGATION  8
//To Control
#define ENCODER_R_CONTROL     	 11
#define ENCODER_L_CONTROL        12
#define ENCODER_SPEED_R_CONTROL  13
#define ENCODER_SPEED_L_CONTROL  14




typedef enum WII_state{
    TRIG=0, LEFT_BEACON_ANGLES=1, RIGHT_BEACON_ANGLES=2, CAMERA_OVERRIDE_BEACON_ANGLES=3, CONTINUOUS_AQUISITION=4,  SERVO_OVERRIDE_ANGLE=5, MAXENUMS=6
}states;
int rightAngleOverride, leftAngleOverride;
volatile unsigned int wiiTime = 0;
int receiveArray[20];
int constrain(int value, int lowBound, int highBound);
//exetern from initialize c file
extern void initialize(void);
//Extern from the Wii camera c file
extern void initCamera(int cameraNumber);
extern void I2ConeReset(void);

extern void I2CtwoReset(void);
extern void doOverrideBeaconAcquisition(int cLBeacon, int cRBeacon);
extern void doHorizontalBeaconAcquisition();
extern void doVerticalBeaconAcquisition();
extern void doXYAcquisition();
extern void doContinuousAcquisition();

extern void resetWiiBeaconStates();
extern int cameraReady(int cameraNumber);
extern void readCamera(int cameraNumber);
//Extern from the fast transfer c file
extern void sendData(unsigned char whereToSend);
extern void ToSend(const unsigned char where, const unsigned int what);
extern bool receiveData();
//PWM SERVO OVERRIDE


//extern from the encoderCalculator c file
extern void updateEncoders();
#endif	/* MAIN_H */

