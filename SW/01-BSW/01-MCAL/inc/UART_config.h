/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 24/9/2020                                   */
/* Version: 2.0                                         */
/* File   : UART_config.h                               */
/********************************************************/ 
#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#define CLK_HZ        8000000

typedef enum {
    BPS_4800,           
    BPS_9600,           
    BPS_19200,          
    BPS_57600,          
    BPS_115200,          
}BAUDRATE_t;

typedef enum {
    No_Parity,
    Even_Parity, 
    Odd_Parity,
}PARITY_t;

typedef enum {
    UART_Enable, 
    UART_Disable,   
}UART_authorize;

typedef enum {
    TRANSMIT_ONLY,
    RECIEVE_ONLY,
    TRANSMIT_RECIEVE,
}TRANSIMISSION_MODE;
typedef struct  
{
    UART_authorize      uart_mode;
    BAUDRATE_t          baudrate;
    uint32              clk_MHZ;
    PARITY_t            parity;
    TRANSIMISSION_MODE  transmitionMode;
}UART_CONFIG;


/****** UART configuration ********/
#define MAX_UART_CH     3

UART_CONFIG user_configuration_t [MAX_UART_CH] = {
    {UART_Enable  , BPS_9600 , CLK_HZ , No_Parity , TRANSMIT_RECIEVE},
    {UART_Disable , BPS_9600 , CLK_HZ , No_Parity , TRANSMIT_RECIEVE},
    {UART_Disable , BPS_115200 , CLK_HZ , No_Parity , TRANSMIT_RECIEVE},
};
#endif // End Of File
