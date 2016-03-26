/* 
 * File:   constants.h
 * Author: Igor
 *
 * Created on August 17, 2014, 7:20 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define SYS_FREQ 80000000 // system frequency
#define DESIRED_BAUD_RATE 57600
#define BAUD_RATE ((SYS_FREQ / (16 * DESIRED_BAUD_RATE) - 1)

#define RING_BUF_SIZE 64  // size of the incoming ring buffer

#define SIZE_OF_DMA_ARRAY 200

#define INPUT                1
#define OUTPUT               0    

#define WATCHDOG              LATDbits.LATD6
#define WATCHDOG_TRIS         TRISDbits.TRISD6
#define WATCHDOG_ON_STATE     0
#define WATCHDOG_OFF_STATE    1

#define ERROR_1              LATEbits.LATE4
#define ERROR_1_TRIS         TRISEbits.TRISE4
#define ERROR_1_ON_STATE     0
#define ERROR_1_OFF_STATE    1

#define STATUS_1             LATEbits.LATE1
#define STATUS_1_TRIS        TRISEbits.TRISE1
#define STATUS_1_ON_STATE    0
#define STATUS_1_OFF_STATE   1

#define STATUS_2             LATEbits.LATE2
#define STATUS_2_TRIS        TRISEbits.TRISE2
#define STATUS_2_ON_STATE    0
#define STATUS_2_OFF_STATE   1

#define STATUS_3             LATEbits.LATE3
#define STATUS_3_TRIS        TRISEbits.TRISE3
#define STATUS_3_ON_STATE    0
#define STATUS_3_OFF_STATE   1

#endif	/* CONSTANTS_H */

