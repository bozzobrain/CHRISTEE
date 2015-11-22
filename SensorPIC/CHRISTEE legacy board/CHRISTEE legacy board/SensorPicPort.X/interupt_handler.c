#include <xc.h>
#include "constants.h"
#include <stdbool.h>
#include "interupt_handler.h"

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void) {

    if (SubState == 0) {
        MoveADCSamples(&BufferDMAtwoA[6][0], &LongBack);
        MoveADCSamples(&BufferDMAtwoA[8][0], &LongRight);
        MoveADCSamples(&BufferDMAtwoA[5][0], &LongFront);
        MoveADCSamples(&BufferDMAtwoA[3][0], &LongLeft);


    }

    if (SubState == 1) {
        MoveADCSamples(&BufferDMAtwoA[1][0], &MediumBack);
        MoveADCSamples(&BufferDMAtwoA[7][0], &MediumRight);
        MoveADCSamples(&BufferDMAtwoA[9][0], &MediumFrontRight);
        MoveADCSamples(&BufferDMAtwoA[2][0], &MediumFrontLeft);
        MoveADCSamples(&BufferDMAtwoA[4][0], &MediumLeft);
        MoveADCSamples(&BufferDMAtwoA[11][0], &ShortRight);
        MoveADCSamples(&BufferDMAtwoA[10][0], &ShortLeft);
    }


    IFS1bits.DMA2IF = 0; // Clear DMA interrupt flag        
    StateComplete = true;
    AD1CON1bits.ADON = 0; // Turn off the A/D converter
    DMA2CONbits.CHEN = 0; // Disable DMA




}

void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void) {
    IFS0bits.DMA1IF = 0; // Clear DMA interrupt flag
}

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void) {
    static unsigned int BufferCountZero = 0; // Keep record of which buffer
    // contains RX Data

    if (BufferCountZero == 0) {
        int k;
        for (k = 0; k < (sizeToRecieve + 4); k++) {
            ringBufS_put(&ring_buffer, BufferDMAzeroA[k]);
        }
    } else {
        int c;
        for (c = 0; c < (sizeToRecieve + 4); c++) {
            ringBufS_put(&ring_buffer, BufferDMAzeroB[c]);
        }
    }
    BufferCountZero ^= 1;
    IFS0bits.DMA0IF = 0; // Clear the DMA0 Interrupt Flag
}

void __attribute__((interrupt, no_auto_psv)) _U2ErrInterrupt(void) {
    IFS4bits.U2EIF = 0; // Clear the UART2 Error Interrupt Flag
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    Transmit = true;
    IFS0bits.T1IF = 0; // clear timer interrupt flag
}


//void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
////    static int rightHolder = 0, leftHolder = 0, rightCounter = 0, leftCounter = 0;
////
////    //RC9 left tread
////    //RC8 right tread
////
////    if (rightHolder != PORTCbits.RC8) {
////        rightCounter++;
////    } else { rightCounter = 0;}
////
////    if (leftHolder != PORTCbits.RC9) {
////        leftCounter++;
////    } else { leftCounter = 0;}
////
////    if (rightCounter > 2) {
////        EncoderRight++;
////        rightHolder = PORTCbits.RC8;
////        rightCounter = 0;
////    }
////
////    if (leftCounter > 2) {
////        EncoderLeft++;
////        leftHolder = PORTCbits.RC9;
////        leftCounter = 0;
////    }
//
//
//    IFS0bits.T2IF = 0; // clear timer interrupt flag
//}


void __attribute__((__interrupt__)) _IC1Interrupt(void) {
    EncoderRight++;
    EncoderCurrentTimeRight = IC1BUF;
    SpeedCalcRight = true;
    IFS0bits.IC1IF = 0; // Clear IC1 Interrupt Status Flag
}

void __attribute__((__interrupt__)) _IC2Interrupt(void) {
    EncoderLeft++;
    EncoderCurrentTimeLeft = IC2BUF;
    SpeedCalcLeft = true;
    IFS0bits.IC2IF = 0; // Clear IC2 Interrupt Status Flag
}