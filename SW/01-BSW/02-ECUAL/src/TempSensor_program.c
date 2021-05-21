/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "ADC1_interface.h"
#include "GPIO_interface.h"

#include "TempSensor_interface.h"
#include "TempSensor_private.h"
#include "TempSensor_config.h"


void TempSensor_voidInit(void)
{
	/* initializing ADC1 */
	ADC1_voidInit();
	
	/* Configuring RCC */
	
	/* configuring sensor's pin */

}

uint8 TempSensor_u8TempRead(void)
{	
	uint8 TempRead = 0 ;

		/*	Read Sensor */
	return TempRead;

}

uint8 TempSensor_u8CalcAvg(void)
{
	uint8 avgRet = 0 ;
	/*	Calculate Avg	*/
	return avgRet;
}
