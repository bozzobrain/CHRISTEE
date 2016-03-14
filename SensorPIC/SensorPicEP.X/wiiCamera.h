/* 
 * File:   wiiCamera.h
 * Author: User
 *
 * Created on April 26, 2015, 3:25 PM
 */

#ifndef WIICAMERA_H
#define	WIICAMERA_H

#ifdef	__cplusplus
extern "C" {
#endif


    //ADDRESSING FOR ROBOT
#define CONTROL_ADDRESS              5
#define NAVIGATION_ADDRESS           4
#define PIC_ADDRESS                  1
#define MOTOR_ADDRESS                6
#define LED_ADDRESS                  2
#define POWER_ADDRESS	             3

#define LAST_BOARD_ADDRESS_RECEIVE   0
    //SEND TO NAVIGATION
#define ENCODER_R_PIC_RECEIVE        5
#define ENCODER_L_PIC_RECEIVE        6
#define ENCODER_SPEED_R_PIC_RECEIVE  7
#define ENCODER_SPEED_L_PIC_RECEIVE  8
#define WII_BEACON_SEEN_LEFT         9      //Boolean says whether camera is hunting or searching
#define WII_BEACON_SEEN_RIGHT        10
#define WII_NUMBER_SWEEPS_LEFT       11     //Will count up :)
#define WII_LEFT_CAMERA_ANGLE        12    //Returns at all beacon focused
#define WII_RIGHT_CAMERA_ANGLE       13    //^^
#define WII_LEFT_CAMERA_LOCKED       14    //Returns true when beacon is centered in sight of the camera
#define WII_RIGHT_CAMERA_LOCKED      15    //^^
#define WII_X_COORDINATE             16    //Returns during WII_SUBSYSTEM_MODE=TRIG
#define WII_Y_COORDINATE             17    //^^
#define WII_FULL_BEACON_WIDTH_PIXELS 18    //Still working on this, thinking look for (horizontal) or (vertical) or (horizontal -> vertical)
#define WII_CAMERA_LEFT_ANGLE        16
#define WII_CAMERA_RIGHT_ANGLE       17
#define WII_CAMERA_LEFT_LOCKED       18
#define WII_CAMERA_RIGHT_LOCKED      19
#define WII_CAMERA_RIGHT_BEACON      20
#define WII_CAMERA_LEFT_BEACON       21
#define WII_NUMBER_SWEEPS_RIGHT     22

#define HORIZONTAL_BEACON       0
#define RIGHT_BEACON            0
#define VERTICAL_BEACON         1
#define LEFT_BEACON             1

#define LEFT_CAMERA             0
#define RIGHT_CAMERA            1

#define HEIGH_WIDTH_COMPARE     15
#define ACCEPTABLE_CENTER_WIDTH 5//      +/- this number from center     [Low res - 120]  [High res - 5]

#define SERVO_STEP_WIDE         150

#define INVALID                 9999
#define CENTER_PIXEL_RIGHT      512//500//384
#define CENTER_PIXEL_LEFT       400 //384

#define Pi                      3.14159265359

#define DistanceXFromWall       1.0
#define physicalBeaconWidth     1.15
#define physicalCameraWidth     0.40

    // bit flags for blobs
#define BLOB1 0x01
#define BLOB2 0x02
#define BLOB3 0x04
#define BLOB4 0x08

    struct Blob {
        int X;
        int Y;
        int Size;
        unsigned char number;
    };

    struct CameraStruct {
        struct Blob Blob1;
        struct Blob Blob2;
        struct Blob Blob3;
        struct Blob Blob4;
        unsigned char data_buf[16];
        unsigned char result;

    };
    struct CameraStruct Camera[2];

    struct WiiAngles {
        int cameraL_LeftBeacon;
        int cameraL_RightBeacon;
        int cameraR_LeftBeacon;
        int cameraR_RightBeacon;
    };

    struct coordinateData {
        double x;
        double y;
    };



    volatile unsigned int time = 0;
    int numberSweeps[2];
    int rotatingLeft[2] = {0, 0};
    bool beaconSeen[2];
    unsigned int beaconAngle[2][2];
    int beaconCentered[2][2];
    int centerBlob[2][2];


    unsigned int angleSet[2] = {900, 900}; //Servo Control integers

    //------------METHOD DECLARATIONS------------------

    void delay(int ms);
    //---------Externally Executed Method Declarations---
    extern bool SendI2Cone(unsigned char s_address, unsigned char * dat, unsigned char how_much);
    extern bool SendI2Ctwo(unsigned char s_address, unsigned char * dat, unsigned char how_much);
    extern bool ReceiveI2Cone(unsigned char s_address, unsigned char d_address, unsigned char * dat, unsigned char how_much);
    extern bool ReceiveI2Ctwo(unsigned char s_address, unsigned char d_address, unsigned char * dat, unsigned char how_much);
    extern unsigned char StatusI2Cone(void);
    extern unsigned char StatusI2Ctwo(void);
    extern void sendData(unsigned char whereToSend);
    extern void ToSend(const unsigned char where, const unsigned int what);


    //int isAbout(int compareThis, int toThis, int range);
    //unsigned char read(int cameraNumber);
    void initCamera(int cameraNumber); //Turn the camera i2c initialization
    void resetWiiBeaconStates(); //Make camera data refresh, force reaquistion
    void doBeaconAcquisition(int cLBeacon, int cRBeacon); //Generic Operation
    void doOverrideBeaconAcquisition(int cLBeacon, int cRBeacon); //Override pointing both camera at same beacon
    void doHorizontalBeaconAcquisition(); //Both at horizontal
    void doVerticalBeaconAcquisition(); //both at vertical
    void doXYAcquisition(); //do one then the other and do XY map
    void doContinuousAcquisition(); //Do Continuous back and forth aquisition
    bool calculateXY(struct coordinateData * returnable); //Helper do XY calc

    int cameraReady(int cameraNumber); //Ask the i2c to grab data from the Wii
    void readCamera(int cameraNumber); //Parse the I2c return buffer to camera data
    //int vertical, horizontal;
#ifdef	__cplusplus
}
#endif

#endif	/* WIICAMERA_H */

