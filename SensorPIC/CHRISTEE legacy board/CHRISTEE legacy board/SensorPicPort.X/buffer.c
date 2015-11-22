#include <xc.h>
#include <string.h>
#include "constants.h"
#include "buffer.h"


    void ringBufS_init(struct ringBufS* _this) {
        /*****
          The following clears:
            -> buf
            -> head
            -> tail
            -> count
          and sets head = tail
         ***/
        memset(_this, 0, sizeof (*_this));
    }

    void ringBufS_put(struct ringBufS* _this, const unsigned char c) {
        if (_this->count < RBUF_SIZE) {
            _this->buf[_this->head] = c;
            _this->head = modulo_inc(_this->head, RBUF_SIZE);
            ++_this->count;
        } else {
            _this->buf[_this->head] = c;
            _this->head = modulo_inc(_this->head, RBUF_SIZE);
            _this->tail = modulo_inc(_this->tail, RBUF_SIZE);

        }
    }

    unsigned char ringBufS_get(struct ringBufS* _this) {
        unsigned char c;
        if (_this->count > 0) {
            c = _this->buf[_this->tail];
            _this->tail = modulo_inc(_this->tail, RBUF_SIZE);
            --_this->count;
        } else {
            c = 0;
        }
        return (c);
    }

    void ringBufS_flush(struct ringBufS* _this, const int clearBuffer) {
        _this->count = 0;
        _this->head = 0;
        _this->tail = 0;
        if (clearBuffer) {
            memset(_this->buf, 0, sizeof (_this->buf));
        }
    }

    int ringBufS_size(struct ringBufS* _this) {
        return (_this->count);
    }

    unsigned int modulo_inc(const unsigned int value, const unsigned int modulus) {
        unsigned int my_value = value + 1;
        if (my_value >= modulus) {
            my_value = 0;
        }
        return (my_value);
    }

    unsigned int modulo_dec(const unsigned int value, const unsigned int modulus) {
        unsigned int my_value = (0 == value) ? (modulus - 1) : (value - 1);
        return (my_value);
    }

    void datBufs_put(struct datBufs* _this, const unsigned int c){
        _this->buf[_this->head] = c;
        _this->head = modulo_inc(_this->head, DAT_BUF_SIZE);
    }

    unsigned int datBufs_get(struct datBufs* _this) {
        if (_this->head == 0){
            return (_this->buf[DAT_BUF_SIZE - 1]);
        }
        else{
        return (_this->buf[_this->head - 1]);}
    }

    unsigned int datBufs_ave (struct datBufs* _this) {
        // increase sum to unsigned long if the DAT_BUF_SIZE gets bigger than 10
        unsigned int sum = 0;
        unsigned int ave;
        int u;
        for (u = 0; u < DAT_BUF_SIZE; u++) {
            sum = sum + _this->buf[u];
        }
        ave = sum / DAT_BUF_SIZE;
        return (ave);
    }

    void datBufs_init (struct datBufs* _this) {

         memset(_this, 0, sizeof (*_this));
    }


     void pastBufs_put(struct pastBufs* _this, const unsigned int c){
        _this->buf[_this->head] = c;
        _this->head = modulo_inc(_this->head, PAST_BUF_SIZE);
    }

    unsigned int pastBufs_get(struct pastBufs* _this) {
        if (_this->head == 0){
            return (_this->buf[PAST_BUF_SIZE - 1]);
        }
        else{
        return (_this->buf[_this->head - 1]);}
    }

    unsigned int pastBufs_ave (struct pastBufs* _this) {
        // increase sum to unsigned long if the DAT_BUF_SIZE gets bigger than 10
        unsigned int sum;
        unsigned int ave;
        int u;
        for (u = 0; u < PAST_BUF_SIZE; u++) {
            sum = sum + _this->buf[u];
        }
        ave = sum / PAST_BUF_SIZE;
        return (ave);
    }