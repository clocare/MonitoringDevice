
#include "TFT_interface.h"

#include "Display.h"

/************************************************************************/
/*                         Global constants                             */
/************************************************************************/
#define TEXT_COLOR						0x4f3c
#define BACKGROUND_COLOR			0x0000
/************************************************************************/
/*                         Global variables                             */
/************************************************************************/
typedef struct ObserverReadingsType
{
	uint8 Temp;
	uint8 HeartRate;
	uint8 SPO2; 
	uint8 dummy; 
}ObserverReadingsType;
static struct ObserverReadingsType ObserverReadings = {0};

/************************************************************************/
/*														Local Functions														*/
/************************************************************************/
static void Display_NoCard(void);
static void Display_Readings(void);
static void AppendIntegerToString(uint8 * string, uint16 integer);
static void AppendChar(uint8 * string, uint8 charToAppend);

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
void Display_DispCurrentSensorsState(void)
{
	uint8  CloCareHeader[] = "CloCare Monitor";
	TFT_voidFillColor(BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 40 , CloCareHeader , TEXT_COLOR , BACKGROUND_COLOR);
	Display_Readings();
	return;
}

/**
 * @name : Display_mainTask
 * @param:  
 * Non Reentrant
 * Sync
 * Service for Displaying current sensors' state.
 **/
void Display_mainTask(void)
{
	
		while(1);
	
}
	
/************************************************************************/	
/*									Local Functions	Definitions													*/
/************************************************************************/	
static void Display_NoCard(void)
{
	uint8  CloCareHeader[] = "CloCare Monitor";
	uint8  NoCard[] = "No Card Present  ";
	TFT_voidFillColor(BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 40 , CloCareHeader , TEXT_COLOR , BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 60 , NoCard , TEXT_COLOR , BACKGROUND_COLOR);
	return ; 
}

static void Display_Readings(void)
{
	uint8 CardPresent[] = "Card is present  ";
	uint8 tempSensor[80]= "Temp: ";
	uint8 SPO2Sensor[80]= "SPO2: ";
	uint8 HeartRateSensor[80]= "HeartRate: ";
	
	AppendIntegerToString(tempSensor , ObserverReadings.Temp);
	AppendChar(tempSensor , 'C');
	AppendIntegerToString(SPO2Sensor , ObserverReadings.SPO2);
	AppendChar(SPO2Sensor , '%');
	AppendIntegerToString(HeartRateSensor , ObserverReadings.HeartRate);
	AppendChar(HeartRateSensor , 'b');
	AppendChar(HeartRateSensor , 'p');
	AppendChar(HeartRateSensor , 'm');
	
	TFT_voidPrintText(10  , 60 , CardPresent , TEXT_COLOR , BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 70 , tempSensor , TEXT_COLOR , BACKGROUND_COLOR);
	TFT_voidPrintText(10  , 80 , SPO2Sensor , TEXT_COLOR , BACKGROUND_COLOR);	
	TFT_voidPrintText(10  , 90 , HeartRateSensor , TEXT_COLOR , BACKGROUND_COLOR);
	

}
	

static void AppendIntegerToString(uint8 * string, uint16 integer) {
	while (*(string++) != '\0')
		;
	*string--;
	uint8 temp[20] = { 0 };
	uint8 * ptr = temp;

	if (integer == 0) {
		*string++ = '0';
		*string++ = '0';
	} else {

		while (integer) {
			*ptr = ((integer % 10) + '0');
			ptr++;
			integer /= 10;
		}

		if (ptr <= temp + 1) {
			*string++ = '0';
		}

		while ((ptr--) != temp) {
			*string++ = *ptr;
		}
	}
	*string = '\0';
}

static void AppendChar(uint8 * string, uint8 charToAppend)
{
	while (*(string++) != '\0')
		;
	*string--;
	*string++ = charToAppend;
	*string = '\0';
}
