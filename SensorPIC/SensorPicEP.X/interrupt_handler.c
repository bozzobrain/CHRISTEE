#include <xc.h>
#include <stdbool.h>
#include "interrupt_handler.h"

#define ON         0
#define OFF        1
#define INDICATOR1 LATEbits.LATE5
#define INDICATOR2 LATEbits.LATE6
#define INDICATOR3 LATGbits.LATG7
#define INDICATOR4 LATGbits.LATG8
#define WATCHDOG   LATEbits.LATE7
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    //INDICATOR3=ON;
    time++;
    if(wiiTime<=200) wiiTime++;
    encoderTime++;

    IFS0bits.T1IF = 0; // clear interrupt flag
    //INDICATOR3=OFF;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{

    rolloverRight++;
    rolloverLeft++;
    IFS0bits.T2IF = 0; // clear timer interrupt flag
}
void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void)
{
    setRUN_DATAFUNCTION(true);        //setting the flag that runs the critical functions in main
    if(getGYRO_DEBUG()) setCycleCounter();  //increments the counter for the uart debug
    LATGbits.LATG7 ^= 1;
    IFS1bits.T4IF = 0;                //Clears the interrupt flag    
}
