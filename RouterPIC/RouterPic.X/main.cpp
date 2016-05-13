/* 
 * File:   main.cpp
 * Author: Igor
 *
 * Created on August 3, 2014, 7:09 AM
 */


#include <xc.h>
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include "constants.h"
#include "main.h"
#include "initialize.h"
#include "queue.h"
#include "FastTransfer.h"
#include "variables.h"

// PIC32MX575F512H Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
// USERID = No Setting
#pragma config FSRSSEL = PRIORITY_7     // SRS Select (SRS Priority 7)
#pragma config FCANIO = OFF             // CAN I/O Pin Select (Alternate CAN I/O)
#pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
#pragma config FVBUSONIO = OFF          // USB VBUS ON Selection (Controlled by Port Function)

// DEVCFG2
#pragma config FPLLIDIV = DIV_6         // PLL Input Divider (6x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_6         // USB PLL Input Divider (6x Divider)
#pragma config UPLLEN = ON              // USB PLL Enable (Enabled)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG = ON               // Background Debugger Enable (Debugger is enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (ICE EMUC1/EMUD1 pins shared with PGC1/PGD1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)


using namespace std;

int main(void)
{
    // starts: uart, dma, settings ...
    initialize();
    //PIN DIRECTION SETUP
    ERROR_1_TRIS    = OUTPUT;
    STATUS_1_TRIS   = OUTPUT;
    STATUS_2_TRIS   = OUTPUT;
    STATUS_3_TRIS   = OUTPUT;
    WATCHDOG_TRIS   = OUTPUT;

    //INIT PIN STATES
    ERROR_1         = ERROR_1_OFF_STATE;
    STATUS_1        = STATUS_1_OFF_STATE;
    STATUS_2        = STATUS_2_OFF_STATE;
    STATUS_3        = STATUS_3_OFF_STATE;

    //    volatile unsigned int *check = &DCH1CON;
    //    unsigned char * check2;
    //    check2 = dma_one_array;
    //    volatile unsigned int *doublecheck = check;
    //    *doublecheck |= _DCH1CON_CHEN_MASK;

    while (1)
    {
        // checks the incoming buffers and parses data
        receive_one.receiveData();
        receive_two.receiveData();
        receive_three.receiveData();
        receive_four.receiveData();
        if(receive_five.receiveData())
        {
            WATCHDOG ^= 1;
        }
        receive_six.receiveData();


        // checks the queue and sends available data
        DMA_one.queue_send();
        DMA_two.queue_send();
        DMA_three.queue_send();
        DMA_four.queue_send();
        DMA_five.queue_send();
        DMA_six.queue_send();
        
        if(DMA_one.fullError()||DMA_two.fullError()||DMA_three.fullError()||DMA_four.fullError()||DMA_five.fullError()||DMA_six.fullError()){
            STATUS_2=STATUS_2_ON_STATE;
        }
        
        STATUS_3 ^= 1;

    }

    return 0;
}

// this handles errors that the software cannot resolve
// it sets a trap capturing the error that sent it here

static enum feild
{
    EXCEP_IRQ = 0,      // interrupt
    EXCEP_AdEL = 4,     // address error exception (load or ifetch)
    EXCEP_AdES,         // address error exception (store)
    EXCEP_IBE,          // bus error (ifetch)
    EXCEP_DBE,          // bus error (load/store)
    EXCEP_Sys,          // syscall
    EXCEP_Bp,           // breakpoint
    EXCEP_RI,           // reserved instruction
    EXCEP_CpU,          // coprocessor unusable
    EXCEP_Overflow,     // arithmetic overflow
    EXCEP_Trap,         // trap (possible divide by zero)
    EXCEP_IS1 = 16,     // implementation specfic 1
    EXCEP_CEU,          // CorExtend Unuseable
    EXCEP_C2E           // coprocessor 2
} _excep_code;
static unsigned int _epc_code;
static unsigned int _excep_addr;
// this function overrides the normal _weak_ generic handler

void _general_exception_handler(void)
{
    asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));
    _excep_code = (feild) ((_excep_code & 0x0000007C) >> 2);
    WATCHDOG = 1;
    switch (_excep_code)
    {
        case EXCEP_IRQ:  
            STATUS_1=STATUS_1_ON_STATE;
            break;
        case EXCEP_AdEL:  
            STATUS_2=STATUS_2_ON_STATE;
            break;
        case EXCEP_AdES:
            
            break;
        case EXCEP_IBE:  
            
            break;
        case EXCEP_DBE:  
            
            break;
        case EXCEP_Sys:  
            
            break;
        case EXCEP_Bp:    
            
            break;
        case EXCEP_RI:    
            
            break;
        case EXCEP_CpU:   
            
            break;
        case EXCEP_Overflow:  
            
            STATUS_3=STATUS_3_ON_STATE;
            break;
        case EXCEP_Trap: 
            
            ERROR_1 = ERROR_1_ON_STATE; // turn on error led
            break;
        case EXCEP_IS1:  
            
            break;
        case EXCEP_CEU:  
            
            break;
        case EXCEP_C2E: 
            
            break;
            //....
        
    }
    while (1)
    {
        // Examine _excep_code to identify the type of exception
        // Examine _excep_addr to find the address that caused the exception
    }
}