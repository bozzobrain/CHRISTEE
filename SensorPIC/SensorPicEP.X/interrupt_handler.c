#include <xc.h>
#include <stdbool.h>
#include "interrupt_handler.h"

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    time++;
    encoderTime++;
    IFS0bits.T1IF = 0; // clear interrupt flag
}
void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void) {
    EncoderRight++;
    EncoderCurrentTimeRight = IC1BUF;
    SpeedCalcRight = true;
    IFS0bits.IC1IF = 0; // Clear IC1 Interrupt Status Flag
}

void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(void) {
    EncoderLeft++;
    EncoderCurrentTimeLeft = IC2BUF;
    SpeedCalcLeft = true;
    IFS0bits.IC2IF = 0; // Clear IC2 Interrupt Status Flag
}