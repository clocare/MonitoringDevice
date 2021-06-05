/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 30/5/2021                                  */
/* Version: 1.0                                        */
/* File   : GATEWAY.h			                             */
/*******************************************************/ 
#ifndef     GATEWAY_H
#define     GATEWAY_H

#define LOGIN_DATA_CMD					0x00
#define SENSORS_READINGS_CMD		0x01
#define EMERGENCY_MSG_CMD				0x02

void GateWay_init(void);

boolean GateWay_WriteSensorsReadings(const ObserverReadingsType readings);

boolean GateWay_WriteLoginData(const loginData_Type loginData);

void GateWay_SendEmergencyMsg(void);
#endif