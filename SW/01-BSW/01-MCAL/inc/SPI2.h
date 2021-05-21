/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 8/5/2021                                    */
/* Version: 1.0.0                                       */
/* File   : SPI2.h 					                            */
/********************************************************/ 
#ifndef SPI2_H
#define SPI2_H

#include "STD_TYPES.h"
#include "SPI2_cfg.h"

void SPI2_voidInit(void);

uint16 SPI2_u8SendRecieveSync(uint16 data);

uint8 SPI2_ReadDR(void);

void SPI2_voidStop(void);

#endif
