
#include <xc.h>
#include <stdbool.h>
#include "encoderCalculator.h"
#include "gyro.h"
#define ON         0
#define OFF        1
#define INDICATOR1 LATEbits.LATE5
#define INDICATOR2 LATEbits.LATE6
#define INDICATOR3 LATGbits.LATG7
#define INDICATOR4 LATGbits.LATG8
#define WATCHDOG   LATEbits.LATE7


#define DISTANCE_PER_PULSE 0.69 //1.045
union jointhem{
    long joined;
    struct{
        
        unsigned int low;
        unsigned int high;
    }endian;
}_16_to_32;

bool RightStopFlag = true, LeftStopFlag = true;
unsigned long int SpeedRight, SpeedLeft;
long navEncoderRight, navEncoderLeft;
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
    if (encoderTime > 50) {
        //INDICATOR3^=1;
        encoderTime = 0;
//        if (!EncoderRight) {
//            SpeedRight = 0;
//            RightStopFlag = true;
//        }
//        if (!EncoderLeft) {
//            SpeedLeft = 0;
//            LeftStopFlag = true;
//        }
        sendEncoderValues();
    }
//    if (SpeedCalcRight == true) {
//        SpeedCalcRight = false;
//        RightSpeedCalculation();
//    }
//
//    if (SpeedCalcLeft == true) {
//        SpeedCalcRight = false;
//        LeftSpeedCalculation();
//    }
}
int prescalerRight=0;
void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void) {
    //INDICATOR1=ON;
    
    if(prescalerRight>7){
        if(PORTBbits.RB0)
        {
            navEncoderRight++;
            EncoderRight++;
        }
        else
         {
           navEncoderRight--;
            EncoderRight--;  
         }   
        prescalerRight=0;
    }
    else
    {
     prescalerRight++;   
    }
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

int prescalerLeft=0;
void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(void) {

    // INDICATOR2=ON;
    if(prescalerLeft>7){
        if(!PORTBbits.RB1)
        {
           navEncoderLeft++;
           EncoderLeft++;
        }
        else
        {
           navEncoderLeft--;
           EncoderLeft--;  
        }
        prescalerLeft=0;
    }
    else
    {
     prescalerLeft++;   
    }
    
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
    if(navEncoderRight>0)
    {
        INDICATOR1=1;
    }
    else
    {
        INDICATOR1=0;
        
    }
    if(navEncoderLeft>0)
    {
        INDICATOR2=1;        
    }
    else
    {        
        INDICATOR2=0;
    }
    ToSend(LAST_BOARD_ADDRESS_RECEIVE,   PIC_ADDRESS);
    _16_to_32.joined=navEncoderRight*DISTANCE_PER_PULSE;
    ToSend(ENCODER_R_L_NAVIGATION, _16_to_32.endian.low);
    ToSend(ENCODER_R_H_NAVIGATION, _16_to_32.endian.high);
    
    _16_to_32.joined=navEncoderLeft*DISTANCE_PER_PULSE;
    ToSend(ENCODER_L_H_NAVIGATION, _16_to_32.endian.high);
    ToSend(ENCODER_L_L_NAVIGATION, _16_to_32.endian.low);
    
    //ToSend(GYRO_Z_ANGLE, angleZ[0]);
    //ToSend(GYRO_IMPACT, getImpactStatus());
    //navEncoderLeft=0;
    //navEncoderRight=0;
    //ToSend(ENCODER_SPEED_R_NAVIGATION, SpeedRight);
    //ToSend(ENCODER_SPEED_L_NAVIGATION, SpeedLeft);
    sendData(NAVIGATION_ADDRESS);
//    
//    ToSend(LAST_BOARD_ADDRESS_RECEIVE,   PIC_ADDRESS);
//    _16_to_32.joined=navEncoderRight*DISTANCE_PER_PULSE;
//    ToSend(ENCODER_R_L_CONTROL, _16_to_32.endian.low);
//    ToSend(ENCODER_R_H_CONTROL, _16_to_32.endian.high);
//    
//    _16_to_32.joined=navEncoderLeft*DISTANCE_PER_PULSE;
      ToSend(ENCODER_L_H_CONTROL, angleX[0]);//_16_to_32.endian.high);
      ToSend(ENCODER_L_L_CONTROL, angleY[0]);//_16_to_32.endian.low);
      ToSend(ENCODER_SPEED_R_CONTROL,angleZ[0]);// SpeedRight);
      ToSend(ENCODER_SPEED_L_CONTROL,getImpactStatus());// SpeedLeft);
//    sendData(CONTROL_ADDRESS);
}