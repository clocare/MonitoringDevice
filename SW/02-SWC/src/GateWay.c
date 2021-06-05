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
	//ESP_writeSync("Hello.\n");
	return ; 
}

boolean GateWay_WriteSensorsReadings(const ObserverReadingsType readings)
{
	// Send cmd first
	uint8 * cmd = "READS\n";
	ESP_writeSync(cmd);
	uint8 reply[20];
	boolean ret = FALSE; 
	// Construct string of data 
	uint8 Readings[30] = "";
	AppendFloatToString(Readings , readings.Temp);
	AppendChar(Readings , '\n');
	AppendIntegerToString(Readings , readings.SPO2);
	AppendChar(Readings , '\n');
	AppendIntegerToString(Readings , readings.HeartRate);
	AppendChar(Readings , '\n');
	
	// Send data
	ESP_writeSync(Readings);
	ESP_readSync(reply , 4);
	if(reply[0] == 'O' && reply[1] == 'K')
	{
		ret = TRUE;
	}
	return ret; 
}

boolean GateWay_WriteLoginData(const loginData_Type loginData)
{
	uint8 * cmd = "\nLOGIN_PA\n";
	uint8 national_id [14];
	uint8 passWord[14];
	uint8 i =0 ;
	uint8 reply[20];
	boolean ret = FALSE; 
	for(i=0 ; i<14 ; i++)
	{
		national_id[i] = loginData.id_r[i];
		passWord[i] = loginData.Pass_r[i];
	}
	
	ESP_writeSync(cmd);
	ESP_writeSync(national_id);
	ESP_writeSync("\n");
	ESP_writeSync(passWord);
	ESP_writeSync("\n");
	ESP_readSync(reply , 4);

	if(reply[0] == 'O' && reply[1] == 'K')
	{
		ret = TRUE;
	}
	return ret; 
}

void GateWay_SendEmergencyMsg(void)
{
	uint8 * msg = "EMG\n";	
	ESP_writeSync(msg);
	return;
}
