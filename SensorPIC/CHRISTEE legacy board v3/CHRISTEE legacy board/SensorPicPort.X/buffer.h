/* 
 * File:   buffer.h
 * Author: Igor
 *
 * Created on March 5, 2014, 6:09 PM
 */

#ifndef BUFFER_H
#define	BUFFER_H

extern struct ringBufS {
    unsigned char buf[RBUF_SIZE];
    int head;
    int tail;
    int count;
};

extern struct ringBufS ring_buffer;

extern struct datBufs {
    unsigned int buf[DAT_BUF_SIZE];
    int head;
};

extern struct pastBufs {
    unsigned int buf[PAST_BUF_SIZE];
    int head;
};



unsigned int modulo_inc(const unsigned int value, const unsigned int modulus);
unsigned int modulo_dec(const unsigned int value, const unsigned int modulus);
void ringBufS_init(struct ringBufS* _this);
int ringBufS_size(struct ringBufS* _this);
unsigned char ringBufS_get(struct ringBufS* _this);
void ringBufS_put(struct ringBufS* _this, const unsigned char c);
void ringBufS_flush(struct ringBufS* _this, const int clearBuffer);
unsigned int datBufs_ave (struct datBufs* _this);
unsigned int datBufs_get(struct datBufs* _this);
void datBufs_put(struct datBufs* _this, const unsigned int c);
void datBufs_init (struct datBufs* _this);
#endif	/* BUFFER_H */

