/* 
 * File:   constants.h
 * Author: Igor
 *
 * Created on April 18, 2014, 9:53 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define RBUF_SIZE    64  //the size of the ring buffer
#define polynomial 0x8C  //polynomial used to calculate crc

//if this is increased any more the datBufs_ave function sum value
//will need increased. right now unsigned int is the sum value
#define DAT_BUF_SIZE 10 //size of data buffers

#define PAST_BUF_SIZE 5 //selection buffer size

#define BAUD_RATE (((40000000/38400)/16)-1)

#define MAX_CHNUM 12
#define CHAN_TO_SCAN_LONG 4
#define CHAN_TO_SCAN_MEDIUM 7
#define SAMP_BUF_SIZE 16

#define MAX_LONG 500
#define MAX_MEDIUM 150
#define MIN_LONG 100

#define TIMER_COUNTS 7812 // timer 1 period register

#define RANGE 30 // var is about range setting

#define WEIGHTED_AVE 0.50// .85  .50  //weighted average constant

#endif	/* CONSTANTS_H */

