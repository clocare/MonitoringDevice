// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// 2- include interface file of needed lower layers
#include "TempSensor_interface.h"
#include "MAX30102_interface.h"

// 3- include interface files
#include "Observer.h"

void Observer_init(void)
{
	/* init sensors modules */	
	MAX30102_voidInit();
	TempSensor_voidInit();
	
	return ; 
}

uint8 Observer_GetCurrentTemp(ObserverReadingsType * ObserverReadings)
{
	uint8 err = 0 ;
	ObserverReadings->Temp = TempSensor_f32TempRead();	
	if (ObserverReadings->Temp < TEMP_MIN ||ObserverReadings->Temp > TEMP_MAX )
	{
		err++;
	}

	return err; 
}



