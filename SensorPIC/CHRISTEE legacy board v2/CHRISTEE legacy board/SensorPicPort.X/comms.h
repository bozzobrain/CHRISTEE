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

#endif	/* COMMS_H */

