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

void Observer_GetCurrentReadings(ObserverReadingsType * ObserverReadings)
{
	
	ObserverReadings->Temp = TempSensor_f32TempRead();
	ObserverReadings->SPO2 = MAX30102_u8GetSPO2Value();
	ObserverReadings->HeartRate = MAX30102_u8GetHRValue();
	return ; 
}

void Observer_mainTask(void)
{
	while (1);
	return ; 
}

