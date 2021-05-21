/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#ifndef TEMP_SENSOR_INTERFACE_H
#define TEMP_SENSOR_INTERFACE_H


/* public functions prototypes */

/******************************************************************************
* Description : initializing the tempreture sensor
* Parameters  : void
* Return type : void
******************************************************************************/
void TempSensor_voidInit(void);


/******************************************************************************
* Description : calculating the sensor's reading
* Parameters  : void
* Return type : reading (uint8)
******************************************************************************/
uint8 TempSensor_u8TempRead(void);

/******************************************************************************
* Description : calculating the sensor's reading average
* Parameters  : void
* Return type : average reading (uint8)
******************************************************************************/
uint8 TempSensor_u8CalcAvg(void);


#endif 


















