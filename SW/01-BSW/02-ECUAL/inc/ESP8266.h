/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 22/5/2021                                  */
/* Version: 1.0                                        */
/* File   : ESP8266.h			                             */
/* Note		: This driver implementation is for NodeMcu	 */
/*					board v1.0 using UART 										 */
/*******************************************************/ 
#ifndef     ESP8266_H
#define     ESP8266_H

void ESP_init(void);

void ESP_writeSync (uint8 * DataBuffer);

void ESP_readSync (uint8 * DataBuffer  , uint8 DataLength);

#endif
