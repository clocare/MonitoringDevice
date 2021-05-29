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
	TempSensor_voidInit();
	MAX30102_voidInit();
	
	return ; 
}

uint8 Observer_GetCurrentReadings(ObserverReadingsType * ObserverReadings)
{
	uint8 err = 0 ;
	ObserverReadings->Temp = TempSensor_f32TempRead();
	
	ObserverReadings->SPO2 = MAX30102_u8GetSPO2Value();
	
	ObserverReadings->HeartRate = MAX30102_u8GetHRValue();
	if (ObserverReadings->Temp < TEMP_MIN ||ObserverReadings->Temp > TEMP_MAX )
	{
		err++;
	}
	if (ObserverReadings->SPO2 < SPO2_MIN ||ObserverReadings->SPO2 > SPO2_MAX )
	{
		err++;
	}
	if (ObserverReadings->HeartRate < HEARTRATE_MIN ||ObserverReadings->HeartRate > HEARTRATE_MAX )
	{
		err++;
	}
	return err; 
}



