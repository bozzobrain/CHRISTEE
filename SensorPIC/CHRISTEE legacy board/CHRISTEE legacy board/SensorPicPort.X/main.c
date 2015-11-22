
// DSPIC33FJ128GP804 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include <stdbool.h>
#define FCY 40000000UL
#include <libpic30.h>
#include "constants.h"
#include "main.h"

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRI              // Oscillator Mode (Primary Oscillator (XT, HS, EC))
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)

// FICD
#pragma config ICS = PGD2               // Comm Channel Select (Communicate on PGC2/EMUC2 and PGD2/EMUD2)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

void main(void)
{

    initialize();
    //led light
    LATBbits.LATB11 = 1;
    T1CONbits.TON = 1; // activate timer 1


    while (1)
    {

        //watch dog led light
        LATBbits.LATB11 ^= 1;

        if (receiveData())
        {
            UpdateComms();
        }

        if (Transmit == true)
        {
            Transmit = false;
            UpdateData();
            sendData();
        }

        if (SpeedCalcRight == true)
        {
            SpeedCalcRight = false;
            RightSpeedCalculation();
        }

        if (SpeedCalcLeft == true)
        {
            SpeedCalcRight = false;
            LeftSpeedCalculation();
        }



        //State 0 is for data gathering 
        if (State == 0)
        {
            //SubState 0 checks the long range sensors
            if (SubState == 0)
            {
                if (StateInitialized == false)
                {
                    StateInitialized = true;
                    InitializeLong();

                    PreCalculations();
                }
                if (StateRunning == false)
                {
                    StateRunning = true;
                    StartLong();
                    //TimerFlag = false;

                }
                if (StateComplete == true)
                {
                    //ShutDown();
                    CalculationsLong();
                    StateInitialized = false;
                    StateRunning = false;
                    StateComplete = false;
                    SubState = 1;
                    NewDataAvailable = true;
                    if (State != StateReceived)
                    {
                        State = StateReceived;
                        SubState = 0;
                    }
                }
            }


            //SubState 1 checks the Medium range sensors
            if (SubState == 1)
            {
                if (StateInitialized == false)
                {
                    StateInitialized = true;
                    InitializeMedium();
                }
                if (StateRunning == false)
                {
                    // mediums are always on and don't require a timer
                    StateRunning = true;
                    StartMedium();
                }
                if (StateComplete == true)
                {
                    CalculationsMedium();
                    StateInitialized = false;
                    StateRunning = false;
                    StateComplete = false;
                    NewDataAvailable = true;
                    if (State != StateReceived)
                    {
                        State = StateReceived;
                    }
                    SubState = 0;
                }
            }
        }






        //State 1 is for data gathering and sending the data
        if (State == 1)
        {
            //SubState 0 checks the long range sensors
            if (SubState == 0)
            {
                if (StateInitialized == false)
                {
                    StateInitialized = true;
                    InitializeLong();
                    MainCalculations();
                    NewDataAvailable = true;
                }
                if (StateRunning == false)
                {
                    StateRunning = true;
                    StartLong();
                }
                if (StateComplete == true)
                {
                    //ShutDown();
                    CalculationsLong();
                    StateInitialized = false;
                    StateRunning = false;
                    StateComplete = false;
                    SubState = 1;
                    if (State != StateReceived)
                    {
                        State = StateReceived;
                        SubState = 0;
                    }
                }
            }


            //SubState 1 checks the Medium range sensors
            if (SubState == 1)
            {
                if (StateInitialized == false)
                {
                    StateInitialized = true;
                    InitializeMedium();
                }
                if (StateRunning == false)
                {
                    // mediums are always on and don't require a timer
                    StateRunning = true;
                    StartMedium();
                }
                if (StateComplete == true)
                {
                    CalculationsMedium();
                    StateInitialized = false;
                    StateRunning = false;
                    StateComplete = false;
                    if (State != StateReceived)
                    {
                        State = StateReceived;
                    }
                    SubState = 0;
                }
            }
        }
    }
}
