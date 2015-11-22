#include <xc.h>
#define FCY 40000000UL
#include <libpic30.h>
#include "constants.h"
#include "initialize.h"

void initialize(void)
{

    clock();
    comms();
    InOut();
    PowerUP();
    begin(&toAuto, sizeof (toAuto), &fromAuto, sizeof (fromAuto));
    DmaTwo();
    ADC();
    initTimer();
    initBuffers();
    inputCapture();

}

void clock(void)
{
    // FORC = 80MHz  FOY = 40MHz
    // Setting the clock at 40mips
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD = 30; // M = 32
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    CLKDIVbits.PLLPRE = 3; // N1 = 5
    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1)
    {
    };

}

void comms(void)
{

    RPINR18bits.U1RXR = 0b10111; //configures the input RX to a programable pin RP23
    RPOR11bits.RP22R = 0b00011; //configures the output TX to configurale pin RP22
    LATCbits.LATC6 = 1;
    LATCbits.LATC7 = 1;

    // UART config
    U1MODEbits.STSEL = 0; // 1-stop bit
    U1MODEbits.PDSEL = 0; // No parity, 8-data bits
    U1MODEbits.ABAUD = 0; // Auto-baud disabled
    U1BRG = BAUD_RATE; // Baud Rate setting for 38400
    U1STAbits.UTXISEL0 = 0; // Interrupt after all TX character transmitted
    U1STAbits.UTXISEL1 = 1;
    U1STAbits.URXISEL = 0; // Interrupt after one RX character is received
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
}

void DmaTwo(void)
{
    DMA2CONbits.AMODE = 2; // Configure DMA for Peripheral indirect mode
    DMA2CONbits.MODE = 1; // Configure DMA One-Shot, Ping-Pong modes disabled
    DMA2PAD = (int) &ADC1BUF0;
    //DMA2CNT = set some place else
    DMA2REQ = 13; // Select ADC1 as DMA Request source

    DMA2STA = __builtin_dmaoffset(&BufferDMAtwoA);



    IFS1bits.DMA2IF = 0; //Clear the DMA interrupt flag bit
    IEC1bits.DMA2IE = 1; //Set the DMA interrupt enable bit

    //DMA2CONbits.CHEN=1;	enabled some place else

}

void ADC()
{
    AD1CON1bits.FORM = 0; // Data Output Format: Signed Fraction (Q15 format)
    AD1CON1bits.SSRC = 7; // Sample Clock Source: internal counter ends sampling
    AD1CON1bits.ASAM = 1; // ADC Sample Control: Sampling begins immediately after conversion
    AD1CON1bits.AD12B = 1; // 12-bit ADC operation


    AD1CON2bits.CSCNA = 1; // Scan Input Selections for CH0+ during Sample A bit
    //AD1CON2bits.CHPS = 0; // Converts CH0

    AD1CON3bits.ADRC = 1; // ADC Clock is derived internal RC

    // AD1CON3bits.ADCS = 63; // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*64 = 1.6us (625Khz)
    // ADC Conversion Time for 10-bit Tc=12*Tab = 19.2us

    AD1CON1bits.ADDMABM = 0; // DMA buffers are built in scatter/gather mode
    //AD1CON2bits.SMPI = 0; // increment DMA address after completion of every sample convertion operation
    AD1CON4bits.DMABL = 4; // Each buffer contains 16 words


    IFS0bits.AD1IF = 0; // Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 0; // not Enable A/D interrupt
    //AD1CON1bits.ADON = 1; // Turn on the A/D converter


}

void InOut(void)
{

    //inputs in order 2-12
    TRISAbits.TRISA1 = 1;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    TRISBbits.TRISB15 = 1;
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB13 = 1;

    //outputs in order 2-12
    TRISBbits.TRISB4 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA9 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB12 = 0;
    TRISAbits.TRISA10 = 0;
    TRISAbits.TRISA7 = 0;

    //encoder inputs
    TRISCbits.TRISC8 = 1;
    TRISCbits.TRISC9 = 1;

    // led light
    TRISBbits.TRISB11 = 0;

}

void PowerUP(void)
{
    unsigned long i;
    // power up in order 2-12
    LATBbits.LATB4 = 1;
    for (i = 0; i < 1000000; i++);
    LATAbits.LATA4 = 1;
    for (i = 0; i < 1000000; i++);
    LATAbits.LATA9 = 1; //longs start in the off position
    for (i = 0; i < 1000000; i++);
    LATCbits.LATC3 = 1;
    for (i = 0; i < 1000000; i++);
    LATCbits.LATC4 = 1; //longs start in the off position
    for (i = 0; i < 1000000; i++);
    LATCbits.LATC5 = 1; //longs start in the off position
    for (i = 0; i < 1000000; i++);
    LATBbits.LATB8 = 1;
    for (i = 0; i < 1000000; i++);
    LATBbits.LATB9 = 1; //longs start in the off position
    for (i = 0; i < 1000000; i++);
    LATBbits.LATB12 = 1;
    for (i = 0; i < 1000000; i++);
    LATAbits.LATA10 = 1;
    for (i = 0; i < 1000000; i++);
    LATAbits.LATA7 = 1;
    for (i = 0; i < 1000000; i++);

}

void initTimer(void)
{
    //timer 1
    //50ms timer for starting up the longs
    T1CONbits.TON = 0; //disable timer 1
    T1CONbits.TCS = 0; //internal instruction clock
    T1CONbits.TGATE = 0; // disable gated timer mode
    T1CONbits.TCKPS = 0b11; // 1:256 prescalar
    TMR1 = 0x00; //clar timer register
    PR1 = TIMER_COUNTS; //3906; //timer period
    IPC0bits.T1IP = 0x01; // timer lowest priority level
    IFS0bits.T1IF = 0; // clear timer1 interrupt flag
    IEC0bits.T1IE = 1; // enable timer1 interrupt

    // timer 2
    T2CONbits.TON = 0; //disable timer 2
    T2CONbits.TCS = 0; //internal instruction clock
    T2CONbits.TGATE = 0; //disable gated timer mode
    T2CONbits.TCKPS = 0b11; // 1:256 prescalar
    TMR2 = 0x00; //clar timer register
    PR2 = 500;
    IPC1bits.T2IP = 0x01; // timer lowest priority level
    IFS0bits.T2IF = 0; // clear timer1 interrupt flag
    IEC0bits.T2IE = 0; // disable timer1 interrupt
    T2CONbits.TON = 1; //enable timer 2

    // timer 4
    T4CONbits.TON = 0; //disable timer 4
    T4CONbits.TCS = 0; //internal instruction clock
    T4CONbits.TGATE = 0; //disable gated timer mode
    T4CONbits.TCKPS = 0b11; // 1:256 prescalar
    TMR4 = 0x00; //clar timer register
    PR4 = 3907; //25mS
    IPC6bits.T4IP = 0x01; // timer lowest priority level
    IFS1bits.T4IF = 0; // clear timer1 interrupt flag
    IEC1bits.T4IE = 0; // disable timer4 interrupt
    T4CONbits.TON = 1; //enable timer 4


}

void initBuffers(void)
{
    datBufs_init(&BufLongBack);
    datBufs_init(&BufLongRight);
    datBufs_init(&BufLongLeft);
    datBufs_init(&BufLongFront);
    datBufs_init(&BufMediumBack);
    datBufs_init(&BufMediumLeft);
    datBufs_init(&BufMediumRight);
    datBufs_init(&BufMediumFrontLeft);
    datBufs_init(&BufMediumFrontRight);
    datBufs_init(&BufShortRight);
    datBufs_init(&BufShortLeft);
}

void inputCapture(void)
{
    // Initialize the Input Capture Module
    IC1CONbits.ICM = 0b00; // Disable Input Capture 1 module
    IC1CONbits.ICTMR = 1; // Select Timer2 as the IC1 Time base
    IC1CONbits.ICI = 0b00; // Interrupt on every capture event
    IC1CONbits.ICM = 0b010; // Generate capture event on every Falling edge

    // Enable Capture Interrupt And Timer2
    IPC0bits.IC1IP = 1; // Setup IC1 interrupt priority level
    IFS0bits.IC1IF = 0; // Clear IC1 Interrupt Status Flag
    IEC0bits.IC1IE = 1; // Enable IC1

    //pin remap
    RPINR7bits.IC1R = 24; // right tread

    // Initialize the Input Capture Module
    IC2CONbits.ICM = 0b00; // Disable Input Capture 2 module
    IC2CONbits.ICTMR = 1; // Select Timer2 as the IC2 Time base
    IC2CONbits.ICI = 0b00; // Interrupt on every capture event
    IC2CONbits.ICM = 0b010; // Generate capture event on every Falling edge


    // Enable Capture Interrupt And Timer2
    IPC1bits.IC2IP = 1; // Setup IC2 interrupt priority level
    IFS0bits.IC2IF = 0; // Clear IC2 Interrupt Status Flag
    IEC0bits.IC2IE = 1; // Enable IC2

    //pin remap
    RPINR7bits.IC2R = 25; // left tread
}