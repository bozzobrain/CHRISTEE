#include <xc.h>
#include <stdbool.h>
#include <stdlib.h>

#include "GYRO_debug_UART.h"

struct UART_ring_buff {
    unsigned char buf[Transmition_Buffer_Size];
    int head;
    int tail;
    int count;
};
struct UART_ring_buff output_buffer;

bool UartStall = true;
 
void UartInit(void)
{
    //****************************UART*************************
    RPOR0bits.RP65R = 0b000001; //configures the output TX to configurable pin RP65
  
    U1MODEbits.STSEL = 0; // 1-stop bit
    U1MODEbits.PDSEL = 0; // No parity, 8-data bits
    U1MODEbits.ABAUD = 0; // Auto-baud disabled
    U1BRG = BAUD_RATE; // Baud Rate setting for 57600
    U1STAbits.URXISEL = 0b01; // Interrupt after all TX character transmitted
    IFS0bits.U1TXIF = 0; // Clear TX interrupt flag
    IEC0bits.U1TXIE = 1; // Enable TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
    
    //********************************************************
}
void MessageParsser(int inputNumX,int inputNumY,int inputNumZ)
{
    
    
    
//    isNegative(&inputNumX);
//    loadHundreds(&inputNumX);
//    loadTens(&inputNumX);
//    loadUnits(&inputNumX);
//    isNegative(&inputNumY);
//    loadHundreds(&inputNumY);
//    loadTens(&inputNumY);
//    loadUnits(&inputNumY);
    isNegative(&inputNumZ);
    loadHundreds(&inputNumZ);
    loadTens(&inputNumZ);
    loadUnits(&inputNumZ);

    output_buffer.buf[output_buffer.head] = CARRIAGE_RETURN;
    maxedHead();

    if(UartStall == true)
    {
        U1TXREG = output_buffer.buf[output_buffer.tail];
        output_buffer.tail++;
        if(output_buffer.tail >= Transmition_Buffer_Size)output_buffer.tail = 0;
        output_buffer.count--;
        
        UartStall = false;
    }
  
}
void isNegative(int *passNum)
{
    if(*passNum < 0)
    {
        output_buffer.buf[output_buffer.head] = NEGATION;
        output_buffer.head++;
        output_buffer.count++;
        *passNum = absol(*passNum);
        
        //add Negative to the buffer ring
        //then take the abs value
    } 
}
void loadHundreds(int *passNum)
{
    
    int hundreds = 0;
    while(*passNum > 100)    //if the angle is larger then one-Hundred we can find how many hundreds and push the asci value into the ring buffer
    {
        *passNum -= 100;
        hundreds++;
    }
    output_buffer.buf[output_buffer.head] = hundreds + 48;
    maxedHead();
   
    
}
void loadTens(int *passNum)
{
    int tens = 0;
    while(*passNum > 10)    //if the angle is larger then one-Hundred we can find how many hundreds and push the asci value into the ring buffer
    {
        *passNum -= 10;
        tens++;
    }
    output_buffer.buf[output_buffer.head] = tens + 48;
    maxedHead();
    
    
}
void loadUnits(int *passNum)
{
    int units = 0;
    while(*passNum > 1)    //if the angle is larger then one-Hundred we can find how many hundreds and push the asci value into the ring buffer
    {
        *passNum -= 1;
        units++;
    }
    output_buffer.buf[output_buffer.head] = units + 48;
    maxedHead();
    output_buffer.buf[output_buffer.head] = SPACE;
    maxedHead();
    
}
void maxedHead()
{
    output_buffer.head++;
    output_buffer.count++;
    if (output_buffer.head >= Transmition_Buffer_Size)
    {
        output_buffer.head = 0;
    }
   
}



/*
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    if(output_buffer.count > 0)
    {
        U1TXREG = output_buffer.buf[output_buffer.tail];
        output_buffer.tail++;
        if(output_buffer.tail >= Transmition_Buffer_Size)output_buffer.tail = 0;
        output_buffer.count--;
    }
        
    else
    {
        UartStall = true;
    }
    
    IFS0bits.U1TXIF = 0; // Clear TX interrupt flag
  
}
*/

int absol(int givenNum)    
{
    //if you don't know then DON'T TOUCH!!
    if (givenNum > 0)
        return givenNum;
    else 
        return -givenNum;
    
}
  



