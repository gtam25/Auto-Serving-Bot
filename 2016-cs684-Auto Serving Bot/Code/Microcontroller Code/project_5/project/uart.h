//-------------------------------------------
/*
filename : uart.h
author   : Manjunath.k, Amit
Date	 : 23-10-16
purpose	 : Autonomous serving bot
		   UART header file
*/
//-------------------------------------------
#ifndef UART_H
#define UART_H
#include "global.h"

//-------------------------------------------
//Macros (defines)
//Defines the baud rates used
//-------------------------------------------
#define BAUD 9600
#define BUFF_LEN 700
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

//-------------------------------------------
//Prototypes
//Defined in project.c file
//-------------------------------------------
void uart_start(void);
void uart_sendint(uint8_t data);
void uart_sendint16(uint16_t data);
void uart_sendstr(char *data);
uint8_t uart_get(void);




#endif
