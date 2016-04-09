/* 
 * File:   encoderCalculator.h
 * Author: User
 *
 * Created on May 12, 2015, 2:43 PM
 */

#ifndef ENCODERCALCULATOR_H
#define	ENCODERCALCULATOR_H

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

//TO NAV
#define ENCODER_R_L_NAVIGATION       5
#define ENCODER_R_H_NAVIGATION      23
#define ENCODER_L_L_NAVIGATION       6
#define ENCODER_L_H_NAVIGATION      24
#define ENCODER_SPEED_R_NAVIGATION  7
#define ENCODER_SPEED_L_NAVIGATION  8

#define ENCODER_SPEED_R_CONTROL  11
#define ENCODER_SPEED_L_CONTROL  12
#define ENCODER_R_CONTROL     	 13
#define ENCODER_L_CONTROL        14


    extern void delay(int ms);
extern void sendData(unsigned char whereToSend);
extern void ToSend(const unsigned char where, const unsigned int what);

void sendEncoderValues();
void updateEncoders();
volatile int encoderTime=-255;
extern bool SpeedCalcRight;
extern bool SpeedCalcLeft;

bool SpeedCalcRight;
bool SpeedCalcLeft;


long EncoderRight;
long EncoderLeft;
unsigned int EncoderRightStorage;
unsigned int EncoderLeftStorage;

unsigned int EncoderCurrentTimeRight;
unsigned int EncoderCurrentTimeLeft;
unsigned int EncoderPastTimeRight;
unsigned int EncoderPastTimeLeft;
volatile unsigned int rolloverRight;
volatile unsigned int rolloverLeft;
unsigned int rolloverPastLeft;
unsigned int rolloverPastRight;

#ifdef	__cplusplus
}
#endif

#endif	/* ENCODERCALCULATOR_H */

