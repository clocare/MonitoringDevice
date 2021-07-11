/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 27/5/2021                                  */
/* Version: 1.0                                        */
/* File   : Observer.h			                           */
/*******************************************************/ 
#ifndef     OBSERVER_H
#define     OBSERVER_H

#define TEMP_MIN							34U
#define TEMP_MAX							50U
#define TEMP_CRITICAL_MIN			39U

#define HEARTRATE_MIN						20U
#define HEARTRATE_MAX						190U
#define HEARTRATE_CRITICAL_MIN	60U

#define SPO2_MIN								50U
#define SPO2_MAX								120U
#define SPO2_CRITICAL_MIN				75U

#define MAX_ERROR_COUNT						10
#define MAX_EMER_COUNTS						10
#define AVG_BUFFER_LENGTH								10
typedef struct ObserverReadingsType
{
	float32 Temp;
	sint32 HeartRate;
	sint32 SPO2; 
	uint8 dummyVariable; 	// used for allignment 
}ObserverReadingsType;

void Observer_init(void);

uint8 Observer_GetCurrentTemp(ObserverReadingsType * ObserverReadings);


#endif
