/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 22/5/2021                                  */
/* Version: 1.0                                        */
/* File   : ESP8266.c			                             */
/*******************************************************/ 

// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// 2- include interface file of needed lower layers
#include "UART_interface.h"

// 3- include interface files. 
#include "ESP8266.h"
#include "ESP8266_cfg.h"

/************************************************************************/
/*                         APIS definitions                             */
/************************************************************************/
void ESP_init(void)
{
	// UART driver initialized in main. 
	// No special initialization required. 
	return ; 
}

void ESP_writeSync (uint8 * DataBuffer)
{
	if (DataBuffer != NULL_PTR)
	{
		UART_voidTransmitSync(ESP_UART_CHANNEL , DataBuffer);
	}
	else 
	{
		// report error passing parameters. 
	}
	
	return ;
}

void ESP_readSync (uint8 * DataBuffer  , uint8 DataLength)
{
	uint8 idx = 0 ; 
	if (DataBuffer != NULL_PTR)
	{
		for (idx =0  ; idx<DataLength ; idx++)
		{
				DataBuffer[idx] = UART_u8RecieveSync(ESP_UART_CHANNEL);
		}
	}
	else 
	{
		// report error passing parameters. 
	}
	return; 
}
