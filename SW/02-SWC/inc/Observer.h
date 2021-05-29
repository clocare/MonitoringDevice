/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 27/5/2021                                  */
/* Version: 1.0                                        */
/* File   : Observer.h			                           */
/*******************************************************/ 
#ifndef     OBSERVER_H
#define     OBSERVER_H

#define TEMP_MIN							20U
#define TEMP_MAX							50U
#define TEMP_CRITICAL_MIN			39U

#define HEARTRATE_MIN						30U
#define HEARTRATE_MAX						200U
#define HEARTRATE_CRITICAL_MIN	60U

#define SPO2_MIN						40U
#define SPO2_MAX						101U
#define SPO2_CRITICAL_MIN		90U


typedef struct ObserverReadingsType
{
	float32 Temp;
	uint8 HeartRate;
	uint8 SPO2; 
	uint8 dummyVariable; 	// used for allignment 
}ObserverReadingsType;

void Observer_init(void);

uint8 Observer_GetCurrentReadings(ObserverReadingsType * ObserverReadings);


#endif
