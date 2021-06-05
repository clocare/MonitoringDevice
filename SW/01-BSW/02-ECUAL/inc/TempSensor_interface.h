/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#ifndef TEMP_SENSOR_INTERFACE_H
#define TEMP_SENSOR_INTERFACE_H


/* public functions prototypes */

/******************************************************************************
* Description : initializing ADC1 and tempreture sensor pin
* Parameters  : void
* Return type : void
******************************************************************************/
void TempSensor_voidInit(void);


/******************************************************************************
* Description : calculating the sensor's reading
* Parameters  : void
* Return type : celcuis temp reading (f32)
******************************************************************************/
float32 TempSensor_f32TempRead(void);

/******************************************************************************
* Description : calculating the sensor's reading average
* Parameters  : void
* Return type : average reading (u8)
******************************************************************************/
// u8 TempSensor_u8CalcAvg(void);


#endif 


















