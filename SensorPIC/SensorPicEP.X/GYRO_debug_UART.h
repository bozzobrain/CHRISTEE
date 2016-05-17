#ifndef GYRO_debug_UART_H
#define	GYRO_debug_UART_H


#define Transmition_Buffer_Size 200
#define SPACE               32
#define CARRIAGE_RETURN     13
#define NEGATION            45
#define BAUD_RATE ((60000000/(115200 * 16)) - 1)





void UartInit(void);
void MessageParsser(int,int,int);
int absol(int); 
void maxedHead();
void isNegative(int *);
void loadHundreds(int *);
void loadTens(int *);
void loadUnits(int *);



#endif	/* GYRO_debug_UART_H */

