
#include <xc.h>
#include <stdbool.h>
#include "encoderCalculator.h"

#define ON         0
#define OFF        1
#define INDICATOR1 LATEbits.LATE5
#define INDICATOR2 LATEbits.LATE6
#define INDICATOR3 LATGbits.LATG7
#define INDICATOR4 LATGbits.LATG8
#define WATCHDOG   LATEbits.LATE7


#define DISTANCE_PER_PULSE 1//4.18

bool RightStopFlag = true, LeftStopFlag = true;
unsigned int SpeedRight, SpeedLeft;
unsigned int navEncoderRight, navEncoderLeft;
void RightSpeedCalculation(void) {
    if ((SpeedRight == 0) || (rolloverPastRight == 0)) {
        rolloverPastRight = 1;
    }
    unsigned long holder = ((unsigned long) EncoderCurrentTimeRight - ((unsigned long) EncoderPastTimeRight) + (65535 * (rolloverPastRight - 1)));
    float actualTime = (float) holder * 0.0000064;
    float actualSpeed = (DISTANCE_PER_PULSE / actualTime) * 100;  //(0.0746 / actualTime) * 100;
    SpeedRight = (SpeedRight + (unsigned int) actualSpeed) / 2;
    EncoderPastTimeRight = EncoderCurrentTimeRight;

}

void LeftSpeedCalculation(void) {
    if ((SpeedLeft == 0) || (rolloverPastLeft == 0)) {
        rolloverPastLeft = 1;
    }
    unsigned long holder = ((unsigned long) EncoderCurrentTimeLeft - ((unsigned long) EncoderPastTimeLeft) + (65535 * (rolloverPastLeft - 1)));
    float actualTime = (float) holder * 0.0000064;
    float actualSpeed = (DISTANCE_PER_PULSE / actualTime) * 100; //(0.0746 / actualTime) * 100;
    SpeedLeft = (SpeedLeft + (unsigned int) actualSpeed) / 2;
    EncoderPastTimeLeft = EncoderCurrentTimeLeft;
}

void updateEncoders() {
    if (encoderTime > 150) {
        //INDICATOR3^=1;
        encoderTime = 0;
        if (!EncoderRight) {
            SpeedRight = 0;
            RightStopFlag = true;
        }
        if (!EncoderLeft) {
            SpeedLeft = 0;
            LeftStopFlag = true;
        }
        sendEncoderValues();
    }
    if (SpeedCalcRight == true) {
        SpeedCalcRight = false;
        RightSpeedCalculation();
    }

    if (SpeedCalcLeft == true) {
        SpeedCalcRight = false;
        LeftSpeedCalculation();
    }
}

void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void) {
    //INDICATOR1=ON;
    navEncoderRight++;
    EncoderRight++;
//    rolloverPastRight = rolloverRight;
//    rolloverRight = 0;
//    TMR3 = 0x00; //reset timer
//    if (RightStopFlag == true) {
//        RightStopFlag = false;
//        EncoderPastTimeRight = IC1BUF;
//    } else {
//        EncoderCurrentTimeRight = IC1BUF;
//        SpeedCalcRight = true;
//    }
    IFS0bits.IC1IF = 0; // Clear IC1 Interrupt Status Flag
}

void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(void) {

    // INDICATOR2=ON;
    navEncoderLeft++;
    EncoderLeft++;
//    rolloverPastLeft = rolloverLeft;
//    rolloverLeft = 0;
//    SpeedCalcLeft = true;
//    TMR4 = 0x00; //reset timer
//    if (LeftStopFlag == true) {
//        LeftStopFlag = false;
//        EncoderPastTimeLeft = IC2BUF;
//    } else {
//        EncoderCurrentTimeLeft = IC2BUF;
//        SpeedCalcLeft = true;
//    }
    IFS0bits.IC2IF = 0; // Clear IC2 Interrupt Status Flag
}

void sendEncoderValues() {

    ToSend(LAST_BOARD_ADDRESS_RECEIVE,   PIC_ADDRESS);
    ToSend(ENCODER_R_NAVIGATION, navEncoderRight*DISTANCE_PER_PULSE);
    ToSend(ENCODER_L_NAVIGATION, navEncoderLeft*DISTANCE_PER_PULSE);
    //navEncoderLeft=0;
    //navEncoderRight=0;
    //ToSend(ENCODER_SPEED_R_NAVIGATION, SpeedRight);
    //ToSend(ENCODER_SPEED_L_NAVIGATION, SpeedLeft);
    sendData(NAVIGATION_ADDRESS);
    ToSend(LAST_BOARD_ADDRESS_RECEIVE,   PIC_ADDRESS);
    ToSend(ENCODER_R_CONTROL, EncoderRight*DISTANCE_PER_PULSE);
    ToSend(ENCODER_L_CONTROL, EncoderLeft*DISTANCE_PER_PULSE);
    //ToSend(ENCODER_SPEED_R_CONTROL, SpeedRight);
    //ToSend(ENCODER_SPEED_L_CONTROL, SpeedLeft);
    sendData(CONTROL_ADDRESS);
}