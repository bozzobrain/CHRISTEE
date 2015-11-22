/* 
 * File:   EasyTransferCRC.h
 * Author: Igor
 *
 * Created on April 23, 2014, 7:12 PM
 */

#ifndef EASYTRANSFERCRC_H
#define	EASYTRANSFERCRC_H

extern struct ringBufS {
    unsigned char buf[RBUF_SIZE];
    int head;
    int tail;
    int count;
};

extern struct ringBufS ring_buffer;



unsigned char * addressToSend; //address of struct
unsigned char * addressToRecieve;
unsigned char sizeToSend;
unsigned char sizeToRecieve;
unsigned char *rx_buffer; //address for temporary storage and parsing buffer
unsigned char rx_array_inx; //index for RX parsing buffer
unsigned char rx_len; //RX packet length according to the packet
unsigned char calc_CS; //calculated Chacksum

extern unsigned char BufferDMAzeroA[10] __attribute__((space(dma)));
extern unsigned char BufferDMAzeroB[10] __attribute__((space(dma)));
extern unsigned char BufferDMAoneA[20] __attribute__((space(dma)));



    unsigned char CRC8(const unsigned char * data, unsigned char len);
    void begin(unsigned char * ptrSend, unsigned char lenSend, unsigned char * ptrRecieve, unsigned char lenRecieve);
    void sendData();
    bool receiveData();





#endif	/* EASYTRANSFERCRC_H */

