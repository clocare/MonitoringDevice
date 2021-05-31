// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "StringManipulation.h"

// 2- include interface file of needed lower layers
#include "TFT_interface.h"

// 3- include interface files
#include "Observer.h"
#include "Display.h"


/************************************************************************/
/*                         Global constants                             */
/************************************************************************/
#define TEXT_COLOR						0x4f3c
#define BACKGROUND_COLOR			0x0000
#define READINGS_COLOR				0xffff
/************************************************************************/
/*                         Global variables                             */
/************************************************************************/

static struct ObserverReadingsType ObserverReadings = {0};

/************************************************************************/
/*														Local Functions														*/
/************************************************************************/
static void Display_Readings(void);
static boolean IsEmergency(void);
/************************************************************************/
/*                         APIS definitions                             */
/************************************************************************/

/**
 * @name : Display_init
 * @param:  
 * Non Reentrant
 * Sync
 * Service for TFT initialization.
 **/
void Display_init(void)
{
	TFT_voidInit();
  Display_NoCard();
	return ; 
}

/**
 * @name : Display_DispCurrentState
 * @param:  
 * Non Reentrant
 * Sync
 * Service for Displaying current sensors' state.
 **/
void Display_CurrentSensorsState(void)
{
	Display_Readings();
	if(FALSE == IsEmergency())
	{	
		TFT_voidPrintText(10  , 110 , "                             " , BACKGROUND_COLOR , BACKGROUND_COLOR);
	}
	return;
}

void Display_NoCard(void)
{
	uint8  CloCareHeader[] = "CloCare Monitor";
	uint8  NoCard[] = "No Card Present  ";
	TFT_voidFillColor(BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 40 , CloCareHeader , TEXT_COLOR , BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 60 , NoCard , TEXT_COLOR , BACKGROUND_COLOR);
	return ; 
}
	
void Display_UpdateSensors(ObserverReadingsType Readings)
{
	ObserverReadings.HeartRate = Readings.HeartRate;
	ObserverReadings.SPO2 = Readings.SPO2;
	ObserverReadings.Temp = Readings.Temp;
}

void Display_Emeregency(void)
{
	TFT_voidPrintText(10  , 110 , "Emergency message sent" , TEXT_COLOR , BACKGROUND_COLOR);
}
/************************************************************************/	
/*									Local Functions	Definitions													*/
/************************************************************************/	

static void Display_Readings(void)
{
	uint8 CardPresent[] = "Card is present  ";
	uint8 tempSensor[80]= "Temp: ";
	uint8 SPO2Sensor[80]= "SPO2: ";
	uint8 HeartRateSensor[80]= "HeartRate:";
	
	if ( ObserverReadings.Temp > TEMP_MIN && ObserverReadings.Temp < TEMP_MAX )
	{
		AppendFloatToString(tempSensor , ObserverReadings.Temp);
		AppendString(tempSensor , " C  ");
	}
	else 
	{
		AppendString(tempSensor , " NAN    ");
	}
	
	if (ObserverReadings.SPO2 > SPO2_MIN && ObserverReadings.SPO2 < SPO2_MAX)
	{
		AppendIntegerToString(SPO2Sensor , ObserverReadings.SPO2);
		AppendString(SPO2Sensor , " %  ");
	}
	else 
	{
		AppendString(SPO2Sensor , " NAN    ");
	}
	
	if (ObserverReadings.HeartRate > HEARTRATE_MIN && ObserverReadings.HeartRate < HEARTRATE_MAX)
	{
		AppendIntegerToString(HeartRateSensor , ObserverReadings.HeartRate);
		AppendString(HeartRateSensor , " bpm ");
	}
	else 
	{
		AppendString(HeartRateSensor , " NAN    ");
	}
	
	TFT_voidPrintText(10  , 60 , CardPresent , READINGS_COLOR , BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 70 , tempSensor , READINGS_COLOR , BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 80 , SPO2Sensor , READINGS_COLOR , BACKGROUND_COLOR);	
	TFT_voidPrintText(10  , 90 , HeartRateSensor , READINGS_COLOR , BACKGROUND_COLOR);
	
}

static boolean IsEmergency(void)
{
	boolean ret = FALSE; 
	if ( (ObserverReadings.HeartRate <= HEARTRATE_CRITICAL_MIN && ObserverReadings.HeartRate > HEARTRATE_MIN ) 
					|| ( ObserverReadings.SPO2 <= SPO2_CRITICAL_MIN  && ObserverReadings.SPO2 > SPO2_MIN )
					|| ( ObserverReadings.Temp >= TEMP_CRITICAL_MIN && ObserverReadings.Temp < TEMP_MAX)
				)
	{
		ret = TRUE;
	}
	return ret; 
}


