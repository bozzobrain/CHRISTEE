/* 
 * File:   calculations.h
 * Author: Igor
 *
 * Created on May 4, 2014, 4:37 PM
 */

#ifndef CALCULATIONS_H
#define	CALCULATIONS_H

void CalculationsLong(void);
unsigned int Average(unsigned int * Array, unsigned int size );
unsigned int LongConvertion (unsigned int counts);
unsigned int MediumConvertion (unsigned int counts);
unsigned int ShortConvertion (unsigned int counts);

extern struct datBufs {
    unsigned int buf[DAT_BUF_SIZE];
    int head;
};


// sensor data storage buffers
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

//sort buffers
extern unsigned int SortLongFront[8];
extern unsigned int SortLongLeft[8];
extern unsigned int SortLongRight[8];
extern unsigned int SortLongBack[8];
extern unsigned int SortMediumFrontLeft[8];
extern unsigned int SortMediumFrontRight[8];
extern unsigned int SortMediumLeft[8];
extern unsigned int SortMediumRight[8];
extern unsigned int SortMediumBack[8];
extern unsigned int SortShortLeft[8];
extern unsigned int SortShortRight[8];


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


#endif	/* CALCULATIONS_H */

