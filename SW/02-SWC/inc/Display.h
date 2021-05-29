/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 21/5/2021                                  */
/* Version: 1.0                                        */
/* File   : Display.h				                           */
/*******************************************************/ 
#ifndef     DISPLAY_H
#define     DISPLAY_H

#include "Observer.h"

void Display_init(void);

void Display_CurrentSensorsState(void);

void Display_NoCard(void);

void Display_UpdateSensors(ObserverReadingsType Readings);

void Display_Emeregency(void);

#endif
