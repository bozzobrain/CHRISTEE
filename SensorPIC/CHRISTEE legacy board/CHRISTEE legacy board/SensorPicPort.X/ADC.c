#include <xc.h>
#include "constants.h"
#include "ADC.h"



void InitializeLong(void){

    //first clears the register and then sets the longs to be scanned
    AD1CSSL = 0;
    AD1CSSLbits.CSS3 = 1; // LongLeft
    AD1CSSLbits.CSS5 = 1; // LongFront
    AD1CSSLbits.CSS6 = 1; // LongBack
    AD1CSSLbits.CSS8 = 1; // LongRight

    //sets dma count
    DMA2CNT = (SAMP_BUF_SIZE * CHAN_TO_SCAN_LONG) - 1;
    AD1CON2bits.SMPI = (CHAN_TO_SCAN_LONG - 1);

//    //powers up the longs
//    LATAbits.LATA9 = 1;
//    LATCbits.LATC4 = 1;
//    LATCbits.LATC5 = 1;
//    LATBbits.LATB9 = 1;
}


void ShutDown(void){

//    //shuts down the longs
//    LATAbits.LATA9 = 0;
//    LATCbits.LATC4 = 0;
//    LATCbits.LATC5 = 0;
//    LATBbits.LATB9 = 0;
}

void InitializeMedium(void){

    //first clears the register and then sets the mediums to be scanned
    AD1CSSL = 0;
    //current config
    AD1CSSLbits.CSS1 = 1; // MediumBack
    AD1CSSLbits.CSS2 = 1; // MediumFrontLeft
    AD1CSSLbits.CSS4 = 1; // MediumLeft
    AD1CSSLbits.CSS7 = 1; // MediumRight
    AD1CSSLbits.CSS9 = 1; // MediumFrontRight

    //config with two shorts
    AD1CSSLbits.CSS10 = 1;  // ShortLeft
    AD1CSSLbits.CSS11 = 1;  // ShortRight


    DMA2CNT = (SAMP_BUF_SIZE * CHAN_TO_SCAN_MEDIUM) - 1;
    AD1CON2bits.SMPI = (CHAN_TO_SCAN_MEDIUM - 1);
}


void StartLong(void){
    DMA2CONbits.CHEN = 1;	// Enable DMA
    AD1CON1bits.ADON = 1; // Turn on the A/D converter
}

void StartMedium(void){
    DMA2CONbits.CHEN = 1;	// Enable DMA
    AD1CON1bits.ADON = 1; // Turn on the A/D converter
}