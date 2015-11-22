/* 
 * File:   initialize.h
 * Author: Igor
 *
 * Created on April 18, 2014, 5:53 PM
 */

#ifndef INITIALIZE_H
#define	INITIALIZE_H

extern struct toSend {
    int Front;
    int Left;
    int Rignt;
    int Back;
    int ShortLeft;
    int ShortRight;
    unsigned int EncoderRight;
    unsigned int EncoderLeft;
};

extern struct toSend toAuto;

extern struct toRecieve {
    int config;
    int status;
    int command;
};

extern struct toRecieve fromAuto;


extern __eds__ unsigned int BufferDMAtwoA[MAX_CHNUM+1][SAMP_BUF_SIZE] __attribute__((eds,space(dma),aligned(512)));
//extern unsigned int BufferDMAtwoB[11] __attribute__((space(dma)));

extern struct datBufs {
    unsigned int buf[DAT_BUF_SIZE];
    int head;
};

extern struct datBufs BufLongBack;
extern struct datBufs BufLongRight;
extern struct datBufs BufLongLeft;
extern struct datBufs BufLongFront;
extern struct datBufs BufMediumBack;
extern struct datBufs BufMediumLeft;
extern struct datBufs BufMediumRight;
extern struct datBufs BufMediumFrontLeft;
extern struct datBufs BufMediumFrontRight;
extern struct datBufs BufShortRight;
extern struct datBufs BufShortLeft;



void inicialize(void);
void clock(void);
void InOut(void);
void PowerUP(void);
void comms(void);
void DmaTwo(void);
void ADC(void);
void initTimer(void);
void initBuffers(void);
void inputCapture(void);

#endif	/* INITIALIZE_H */

