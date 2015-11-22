/* 
 * File:   main.h
 * Author: Igor
 *
 * Created on April 18, 2014, 5:50 PM
 */

#ifndef MAIN_H
#define	MAIN_H


struct ringBufS {
    unsigned char buf[RBUF_SIZE];
    int head;
    int tail;
    int count;
};

struct ringBufS ring_buffer;

struct datBufs {
    unsigned int buf[DAT_BUF_SIZE];
    int head;
};

struct datBufs BufLongBack;
struct datBufs BufLongRight;
struct datBufs BufLongLeft;
struct datBufs BufLongFront;
struct datBufs BufMediumBack;
struct datBufs BufMediumLeft;
struct datBufs BufMediumRight;
struct datBufs BufMediumFrontLeft;
struct datBufs BufMediumFrontRight;
struct datBufs BufShortRight;
struct datBufs BufShortLeft;

struct pastBufs {
    unsigned int buf[PAST_BUF_SIZE];
    int head;
};

struct pastBufs Front;
struct pastBufs Back;
struct pastBufs Left;
struct pastBufs Right;


struct toSend {
    int Front;
    int Left;
    int Rignt;
    int Back;
    int ShortLeft;
    int ShortRight;
    unsigned int DistanceRight;
    unsigned int DistanceLeft;
    unsigned int SpeedRight;
    unsigned int SpeedLeft;
};

struct toSend toAuto;

struct toRecieve {
    int state;
    int status;
    int command;
};

struct toRecieve fromAuto;

//dma buffer declare
// dma zero for receive
unsigned char BufferDMAzeroA[10] __attribute__((space(dma)));
unsigned char BufferDMAzeroB[10] __attribute__((space(dma)));
//dma one for transmit
unsigned char BufferDMAoneA[20] __attribute__((space(dma)));

// dma two for adc
// Number of locations for ADC buffer = 13 (AN0 to AN13) x 16 = 208 words
// Align the buffer to 256 words or 512 bytes. This is needed for peripheral indirect mode
__eds__ unsigned int BufferDMAtwoA[MAX_CHNUM+1][SAMP_BUF_SIZE] __attribute__((eds,space(dma),aligned(512)));
//unsigned int BufferDMAtwoB[11] __attribute__((space(dma)));




//                                     LongFront
//           MediumFrontLeft                               MediumFrontRight
//                                   ____________
//                                   |          |
//                                    \        /
//                                |--- \      /---|
//LongLeft                        |     \    /    |                 LongRight
//MediumLeft                      |      \  /     |                 MediumRight
//                                |               |
//                                |               |
//                                |_______________|
//
//            ShortLeft                                    ShortRight
//                                    MediumBack
//                                     LongBack


// sensor data storage buffers
unsigned int LongFront[16];
unsigned int LongLeft[16];
unsigned int LongRight[16];
unsigned int LongBack[16];
unsigned int MediumFrontLeft[16];
unsigned int MediumFrontRight[16];
unsigned int MediumLeft[16];
unsigned int MediumRight[16];
unsigned int MediumBack[16];
unsigned int ShortLeft[16];
unsigned int ShortRight[16];

//sort buffers
unsigned int SortLongFront[8];
unsigned int SortLongLeft[8];
unsigned int SortLongRight[8];
unsigned int SortLongBack[8];
unsigned int SortMediumFrontLeft[8];
unsigned int SortMediumFrontRight[8];
unsigned int SortMediumLeft[8];
unsigned int SortMediumRight[8];
unsigned int SortMediumBack[8];
unsigned int SortShortLeft[8];
unsigned int SortShortRight[8];


// State storage
int State = 1;
int SubState = 0;
int StateReceived = 1;

bool StateInitialized = false;
bool StateRunning = false;
bool Transmit = false;
bool StateComplete = false;
bool NewDataAvailable = false;
bool SpeedCalcRight = false;
bool SpeedCalcLeft = false;

unsigned int EncoderRight = 0;
unsigned int EncoderLeft = 0;
unsigned int EncoderRightStorage;
unsigned int EncoderLeftStorage;

unsigned int EncoderCurrentTimeRight;
unsigned int EncoderCurrentTimeLeft;
unsigned int EncoderPastTimeRight = 0;
unsigned int EncoderPastTimeLeft = 0;

unsigned int SpeedRight = 0;
unsigned int SpeedLeft = 0;


#endif	/* MAIN_H */

