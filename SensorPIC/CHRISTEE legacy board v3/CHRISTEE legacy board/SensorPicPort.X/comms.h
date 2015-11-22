/* 
 * File:   comms.h
 * Author: Igor
 *
 * Created on May 7, 2014, 3:03 PM
 */

#ifndef COMMS_H
#define	COMMS_H

void UpdateData(void);
void UpdateComms(void);
unsigned int Distance(unsigned int pulses);
unsigned int Speed(unsigned int time);

extern struct toSend {
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

extern struct toSend toAuto;

extern struct toRecieve {
    int state;
    int status;
    int command;
};

extern struct pastBufs {
    unsigned int buf[PAST_BUF_SIZE];
    int head;
};

extern struct pastBufs Front;
extern struct pastBufs Back;
extern struct pastBufs Left;
extern struct pastBufs Right;

extern struct toRecieve fromAuto;

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


// State storage
extern int State;
extern int SubState;
extern int StateReceived;

extern unsigned int EncoderRight;
extern unsigned int EncoderLeft;
extern unsigned int EncoderRightStorage;
extern unsigned int EncoderLeftStorage;

extern unsigned int EncoderCurrentTimeRight;
extern unsigned int EncoderCurrentTimeLeft;
extern unsigned int EncoderPastTimeRight;
extern unsigned int EncoderPastTimeLeft;

extern unsigned int SpeedRight;
extern unsigned int SpeedLeft;

#endif	/* COMMS_H */

