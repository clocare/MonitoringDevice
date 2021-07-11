/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 24/9/2020                                   */
/* Version: 2.0                                         */
/* File   : UART_program.c                              */
/********************************************************/ 

// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// 2- include interface file of needed lower layers

// 3- include driver files
#include "UART_interface.h"
#include "UART_config.h"
#include "UART_private.h"


static uint8 Transmit_idx=0;
static uint8 Receive_idx=0;

void UART_voidInit(void)
{
    uint8 i = 0 ;
    for (i=0 ; i< MAX_UART_CH ; i++){
        if (user_configuration_t[i].uart_mode == UART_Enable)
        {
            // Baud Rate selection
           	if (user_configuration_t[i].baudrate == BPS_4800){
                UARTx[i] -> BRR  = 0x4143;
            }else if (user_configuration_t[i].baudrate == BPS_9600){
                UARTx[i] -> BRR  = 0x341;
            }else if (user_configuration_t[i].baudrate == BPS_19200){
                UARTx[i] -> BRR  = 0x1A1;
            } else if (user_configuration_t[i].baudrate == BPS_57600){
                UARTx[i] -> BRR  = 0x8B;
            }else if (user_configuration_t[i].baudrate == BPS_115200){
                UARTx[i] -> BRR  = 0x1A0;		// THIS VALUE is calculated for 6 * 8 MHZ clock
            }

            // Clear configuration bits 
            UARTx[i] -> CR1 = 0 ;

            // Enable TX - RX 
            if (user_configuration_t[i].transmitionMode == TRANSMIT_ONLY){
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_TE);
            }else if (user_configuration_t[i].transmitionMode == RECIEVE_ONLY){
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_RE); 
            }else if (user_configuration_t[i].transmitionMode == TRANSMIT_RECIEVE) {
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_TE);
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_RE);
            }

            // Parity Configuration 
            if (user_configuration_t[i].parity == Even_Parity){
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_PCE);
                CLR_BIT(UARTx[i] -> CR1 , USART_CR1_PS);

            }else if (user_configuration_t[i].parity == Odd_Parity)
            {
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_PCE);
                SET_BIT(UARTx[i] -> CR1 , USART_CR1_PS);
            }

            // Enable UART
            SET_BIT (UARTx[i] -> CR1 , USART_CR1_UE);

            // Clear Status Register 
            UARTx[i] -> SR = 0 ;
        }
    }    
}

void UART_voidTransmitSync(UART_CH cpy_ch , uint8 cpy_arr[] )
{    
    uint16 i=0;
    while (cpy_arr[i] != '\0'){
        UARTx[cpy_ch] -> DR =  cpy_arr[i];
        while (GET_BIT (UARTx[cpy_ch] -> SR , USART_SR_TC) == 0);
        CLR_BIT (UARTx[cpy_ch] -> SR , USART_SR_TC);
        i++;
    }
}

uint8 UART_u8RecieveSync(UART_CH cpy_ch )
{
    uint8 local_RecievedData = 0 ;
    uint16 local_timeout = 0;
    while (GET_BIT (UARTx[cpy_ch] -> SR , USART_SR_RXNE) == 0)
    {   
        if(local_timeout++ > 60000){
            local_RecievedData = 255; 
            break;
        }

    }
    CLR_BIT(UARTx[cpy_ch] -> SR , USART_SR_RXNE);
    if (local_RecievedData != 255 ){
        local_RecievedData = 0xFF & UARTx[cpy_ch] -> DR;
    }

    return local_RecievedData;
}

void UART_voidTransmitAsync(UART_CH cpy_ch , uint8 cpy_data_to_transmit  )
{
	SET_BIT(UARTx[cpy_ch] -> CR1 , USART_CR1_TXEIE);
    UART_Transmitted_Data_buffer[cpy_ch][Transmit_idx] = cpy_data_to_transmit;
    Transmit_idx++;
}

void UART_u8RecieveAsync(UART_CH cpy_ch , uint8 data_buffer[])
{
	SET_BIT(UARTx[cpy_ch] -> CR1 , USART_CR1_RXNEIE);
    UART_Recived_Data_buffer[cpy_ch] = data_buffer;
}

/**************************************************************************8
void USART1_IRQHandler(void)
{
    UART_CH cpy_UART_ch = UART1_CH;
    if (GET_BIT(UARTx[cpy_UART_ch] -> SR , USART_SR_RXNE ) == 1){     // New Data recieved
        Recived_Data_buffer[cpy_UART_ch][Receive_idx] = (0xFF & UARTx[cpy_UART_ch] -> DR);
        Receive_idx++; 
    }
    if (GET_BIT(UARTx[cpy_UART_ch] -> SR , USART_SR_TXE ) == 1){      //  Transmit Line ready to send data
        if(Transmit_idx > 0 ){
            UARTx[cpy_UART_ch] -> DR =  Transmitted_Data_buffer[cpy_UART_ch][--Transmit_idx];
            while (GET_BIT (UARTx[cpy_UART_ch] -> SR , USART_SR_TC) == 0);

        }
    }
    // Clear Status Register
    UARTx[cpy_UART_ch] -> SR = 0 ;
    NVIC_voidClearPendingFlag(USART1);
}


void USART2_IRQHandler(void)
{
    UART_CH cpy_UART_ch = UART2_CH;
    if (GET_BIT(UARTx[cpy_UART_ch] -> SR , USART_SR_RXNE ) == 1){     // New Data recieved
        Recived_Data_buffer[cpy_UART_ch][Receive_idx] = (0xFF & UARTx[cpy_UART_ch] -> DR);
        Receive_idx++; 
    }
    if (GET_BIT(UARTx[cpy_UART_ch] -> SR , USART_SR_TXE ) == 1){      //  Transmit Line ready to send data
        if(Transmit_idx > 0 ){
            UARTx[cpy_UART_ch] -> DR =  Transmitted_Data_buffer[cpy_UART_ch][--Transmit_idx];
            while (GET_BIT (UARTx[cpy_UART_ch] -> SR , USART_SR_TC) == 0);

        }
    }
    // Clear Status Register
    UARTx[cpy_UART_ch] -> SR = 0 ;
    NVIC_voidClearPendingFlag(USART1);

    
}


void USART3_IRQHandler(void)
{
    UART_CH cpy_UART_ch = UART3_CH;
    if (GET_BIT(UARTx[cpy_UART_ch] -> SR , USART_SR_RXNE ) == 1){     // New Data recieved
        Recived_Data_buffer[cpy_UART_ch][Receive_idx] = (0xFF & UARTx[cpy_UART_ch] -> DR);
        Receive_idx++; 
    }
    if (GET_BIT(UARTx[cpy_UART_ch] -> SR , USART_SR_TXE ) == 1){      //  Transmit Line ready to send data
        if(Transmit_idx > 0 ){
            UARTx[cpy_UART_ch] -> DR =  Transmitted_Data_buffer[cpy_UART_ch][--Transmit_idx];
            while (GET_BIT (UARTx[cpy_UART_ch] -> SR , USART_SR_TC) == 0);

        }
    }
    // Clear Status Register
    UARTx[cpy_UART_ch] -> SR = 0 ;
    NVIC_voidClearPendingFlag(USART1);
    
}
**********************************************************/
