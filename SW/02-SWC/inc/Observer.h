/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 27/5/2021                                  */
/* Version: 1.0                                        */
/* File   : Observer.h			                           */
/*******************************************************/ 
#ifndef     OBSERVER_H
#define     OBSERVER_H

typedef struct ObserverReadingsType
{
	float32 Temp;
	uint8 HeartRate;
	uint8 SPO2; 
	uint8 dummyVariable; 	// used for allignment 
}ObserverReadingsType;

void Observer_init(void);

void Observer_GetCurrentReadings(ObserverReadingsType * ObserverReadings);

void Observer_mainTask(void);

#endif
