/* 
 * File:   interrupt_handler.h
 * Author: Igor
 *
 * Created on March 28, 2015, 11:36 AM
 */

#ifndef INTERRUPT_HANDLER_H
#define	INTERRUPT_HANDLER_H

extern unsigned int time;
extern unsigned int encoderTime;
bool SpeedCalcRight;
bool SpeedCalcLeft;


unsigned int EncoderRight;
unsigned int EncoderLeft;
unsigned int EncoderRightStorage;
unsigned int EncoderLeftStorage;

unsigned int EncoderCurrentTimeRight;
unsigned int EncoderCurrentTimeLeft;
unsigned int EncoderPastTimeRight;
unsigned int EncoderPastTimeLeft;

#endif	/* INTERRUPT_HANDLER_H */

