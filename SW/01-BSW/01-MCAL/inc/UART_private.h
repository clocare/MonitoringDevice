/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 24/9/2020                                   */
/* Version: 2.0                                         */
/* File   : UART_private.h                              */
/********************************************************/ 
#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

#define UART_NO_OF_CH       3
typedef struct 
{
    volatile uint32 SR;
    volatile uint32 DR;
    volatile uint32 BRR;
    volatile uint32 CR1;
    volatile uint32 CR2;
    volatile uint32 CR3;
    volatile uint32 GTPR;
}t_UART;

volatile t_UART * UARTx [UART_NO_OF_CH] = {
    0x40013800,
    0x40004400,
    0x40004800,
};

uint8 * UART_Recived_Data_buffer    [UART_NO_OF_CH];
uint8 UART_Transmitted_Data_buffer[UART_NO_OF_CH][256];
/********** Registers Bits *******************/ 

#define USART_CR1_SBK           0
#define USART_CR1_RWU           1
#define USART_CR1_RE            2
#define USART_CR1_TE            3
#define USART_CR1_IDLEIE        4
#define USART_CR1_RXNEIE        5
#define USART_CR1_TCIE          6
#define USART_CR1_TXEIE         7
#define USART_CR1_PEIE          8
#define USART_CR1_PS            9
#define USART_CR1_PCE           10
#define USART_CR1_WAKE          11
#define USART_CR1_M             12
#define USART_CR1_UE            13


#define USART_SR_PE             0
#define USART_SR_FE             1
#define USART_SR_NE             2
#define USART_SR_ORE            3
#define USART_SR_IDLE           4
#define USART_SR_RXNE           5
#define USART_SR_TC             6
#define USART_SR_TXE            7
#define USART_SR_LBD            8
#define USART_SR_CTS            9    



#endif // End Of File
