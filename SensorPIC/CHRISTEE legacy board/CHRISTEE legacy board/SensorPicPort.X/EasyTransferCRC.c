#include <xc.h>
#include <stdbool.h>
#include <stdlib.h>
#include "constants.h"
#include "EasyTransferCRC.h"
#define FCY 40000000UL
#include <libpic30.h>

void begin(unsigned char * ptrSend, unsigned char lenSend, unsigned char * ptrRecieve, unsigned char lenRecieve) {
    ringBufS_init(&ring_buffer);
    addressToSend = ptrSend;
    addressToRecieve = ptrRecieve;
    sizeToSend = lenSend;
    sizeToRecieve = lenRecieve;


    //    //dynamic creation of rx parsing buffer in RAM
    rx_buffer = malloc(sizeToRecieve * sizeof (char));




    // DMA one to send
    DMA1CON = 0x6001; // One-Shot, Post-Increment, RAM-to-Peripheral, byte size
    DMA1CNT = sizeToSend + 3; // # of DMA requests
    DMA1REQbits.IRQSEL = 0b0001100; // Select UART1 transmitter
    DMA1PAD = (volatile unsigned int) &U1TXREG;
    DMA1STA = __builtin_dmaoffset(BufferDMAoneA);
    IFS0bits.DMA1IF = 0; // Clear DMA Interrupt Flag
    IEC0bits.DMA1IE = 1; // Enable DMA interrupt

    //dma 0 configuration
    DMA0CON = 0x4002; // Continuous, Ping-Pong, Post-Inc., Periph-RAM, byte size
    DMA0CNT = (sizeToRecieve + 3); // # of DMA requests
    DMA0REQbits.IRQSEL = 0b0001011; // Select UART1 receiver
    DMA0PAD = &U1RXREG;//0x0226;//(volatile unsigned int) &U1RXREG; //0x0226
    DMA0STA = __builtin_dmaoffset(BufferDMAzeroA);
    DMA0STB = __builtin_dmaoffset(BufferDMAzeroB);
    IFS0bits.DMA0IF = 0; // Clear DMA interrupt
    IEC0bits.DMA0IE = 1; // Enable DMA interrupt
    DMA0CONbits.CHEN = 1; // Enable DMA channel


    __delay_ms(50);

}

unsigned char CRC8(const unsigned char * data, unsigned char len) {
    unsigned char crc = 0x00;
    while (len--) {
        unsigned char extract = *data++;
        unsigned char tempI;
        for (tempI = 8; tempI; tempI--) {
            unsigned char sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum) {
                crc ^= polynomial;
            }
            extract >>= 1;
        }
    }
    return crc;
}

void sendData() {
    unsigned char CS = CRC8(addressToSend, sizeToSend);
    BufferDMAoneA[0] = (0x06);
    BufferDMAoneA[1] = (0x85);
    BufferDMAoneA[2] = (sizeToSend);

    int i;
    for (i = 0; i < sizeToSend; i++) {
        
        BufferDMAoneA[(i + 3)] = (*(addressToSend + i));
    }

    BufferDMAoneA[(sizeToSend + 3)] = (CS);

    DMA1CONbits.CHEN = 1;
    DMA1REQbits.FORCE = 1; // Manual mode: Kick-start the first transfer


}

bool receiveData() {

    //start off by looking for the header bytes. If they were already found in a previous call, skip it.
    if (rx_len == 0) {
        //this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
        if (ringBufS_size(&ring_buffer) >= 3) {
            //this will block until a 0x06 is found or buffer size becomes less then 3.
            while (ringBufS_get(&ring_buffer) != 0x06) {
                //This will trash any preamble junk in the serial buffer
                //but we need to make sure there is enough in the buffer to process while we trash the rest
                //if the buffer becomes too empty, we will escape and try again on the next call
                if (ringBufS_size(&ring_buffer) < 3)
                    return false;
            }
            if (ringBufS_get(&ring_buffer) == 0x85) {
                rx_len = ringBufS_get(&ring_buffer);
                //make sure the binary structs on both Arduinos are the same size.
                if (rx_len != sizeToRecieve) {
                    rx_len = 0;
                    return false;
                }
            }
        }
    }

    //we get here if we already found the header bytes, the struct size matched what we know, and now we are byte aligned.
    if (rx_len != 0) {
        while (ringBufS_size(&ring_buffer) && rx_array_inx <= rx_len) {
            rx_buffer[rx_array_inx++] = ringBufS_get(&ring_buffer);
        }

        if (rx_len == (rx_array_inx - 1)) {
            //seem to have got whole message
            //last unsigned char is CS
            calc_CS = CRC8(rx_buffer, rx_len);
            



            if (calc_CS == rx_buffer[rx_array_inx - 1]) {//CS good
                memcpy(addressToRecieve, rx_buffer, sizeToRecieve);
                rx_len = 0;
                rx_array_inx = 0;
                return true;
            } else {
                //failed checksum, need to clear this out anyway
                rx_len = 0;
                rx_array_inx = 0;
                return false;
            }
        }
    }


    return false;
}

