/* 
 * File:   interupt_handler.h
 * Author: Igor
 *
 * Created on March 8, 2014, 9:55 AM
 */

#ifndef INTERUPT_HANDLER_H
#define	INTERUPT_HANDLER_H

extern struct ringBufS {
    unsigned char buf[RBUF_SIZE];
    int head;
    int tail;
    int count;
};

extern struct ringBufS ring_buffer;

extern unsigned char sizeToRecieve;
extern unsigned char BufferDMAzeroA[10] __attribute__((space(dma)));
extern unsigned char BufferDMAzeroB[10] __attribute__((space(dma)));


// interrupt function prototypes
void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void);
void __attribute__((interrupt,no_auto_psv)) _DMA0Interrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2ErrInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void);


extern __eds__ unsigned int BufferDMAtwoA[MAX_CHNUM+1][SAMP_BUF_SIZE] __attribute__((eds,space(dma),aligned(512)));
//extern unsigned int BufferDMAtwoB[11] __attribute__((space(dma)));


extern unsigned int LongFront[16];
extern unsigned int LongLeft[16];
extern unsigned int LongRight[16];
extern unsigned int LongBack[16];
extern unsigned int MediumFrontLeft[16];
extern unsigned int MediumFrontRight[16];
extern unsigned int MediumLeft[16];
extern unsigned int MediumRight[16];
extern unsigned int MediumBack[16];
extern unsigned int ShortLeft[16];
extern unsigned int ShortRight[16];


extern int State;
extern int SubState;
extern int StateReceived;
extern bool StateComplete;
extern bool Transmit;
extern bool SpeedCalcRight;
extern bool SpeedCalcLeft;


extern unsigned int EncoderRight;
extern unsigned int EncoderLeft;
extern unsigned int EncoderRightStorage;
extern unsigned int EncoderLeftStorage;

extern unsigned int EncoderCurrentTimeRight;
extern unsigned int EncoderCurrentTimeLeft;
extern unsigned int EncoderPastTimeRight;
extern unsigned int EncoderPastTimeLeft;

#endif	/* INTERUPT_HANDLER_H */

