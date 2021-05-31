// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "StringManipulation.h"

// 2- include interface file of needed lower layers
#include "ESP8266.h"

// 3- include interface files
#include "Observer.h"
#include "Reader.h"
#include "GateWay.h"


void GateWay_init(void)
{
	ESP_init();
	// Send Hi msg 
	ESP_writeSync("Hello.\n");
	return ; 
}

void GateWay_WriteSensorsReadings(const ObserverReadingsType readings)
{
	// Send cmd first
	uint8 cmd [10] = "\ncmd";
	AppendIntegerToString(cmd , SENSORS_READINGS_CMD );	
	AppendChar(cmd , '\n');
	ESP_writeSync(cmd);

	// Construct string of data 
	uint8 Readings[30] = "\nReads:\n";
	AppendIntegerToString(Readings , readings.HeartRate);
	AppendChar(Readings , '\n');
	AppendIntegerToString(Readings , readings.SPO2);
	AppendChar(Readings , '\n');
	AppendFloatToString(Readings , readings.Temp);
	AppendChar(Readings , '\n');
	// Send data
	ESP_writeSync(Readings);
	
	return ; 
}

void GateWay_WriteLoginData(const loginData_Type loginData)
{
	ESP_writeSync("\n");
	ESP_writeSync(LOGIN_DATA_CMD);
	ESP_writeSync("\n");
	ESP_writeSync(loginData.id_r);
	ESP_writeSync("\n");

	return ;
}

void GateWay_SendEmergencyMsg(void)
{
	uint8 msg [20] = "\nEmergency Code\n";	
	ESP_writeSync(msg);
	return;
}
