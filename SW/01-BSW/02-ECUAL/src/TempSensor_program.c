/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/
#include "STD_TYPES.h"

#include <stm32f10x_adc.h>
#include <math.h>


#include "TempSensor_interface.h"
#include "TempSensor_private.h"
#include "TempSensor_config.h"


static uint16 Read_ADC(void);


void TempSensor_voidInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
		//ADC1 configuration
		//select independent conversion mode (single)
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		//We will convert single channel only
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		//we will convert one time
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		//select no external triggering
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		//right 12-bit data alignment in ADC data register
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		//single channel conversion
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		//load structure values to control and status registers
		ADC_Init(ADC1, &ADC_InitStructure);
		//ADC1 channel8 configuration
		//and rank=1 which doesn't matter in single mode
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
		//Enable ADC1
		ADC_Cmd(ADC1, ENABLE);
		//Enable ADC1 reset calibration register
		ADC_ResetCalibration(ADC1);
		//Check the end of ADC1 reset calibration register
		while(ADC_GetResetCalibrationStatus(ADC1));
		//Start ADC1 calibration
		ADC_StartCalibration(ADC1);
		//Check the end of ADC1 calibration
		while(ADC_GetCalibrationStatus(ADC1));
}

float32 TempSensor_f32TempRead(void)
{
	uint16 Loc_u16val = 0;
	float32 Loc_f32celsius = 0.0;

	/* reading ADC1 value */
	Loc_u16val = Read_ADC();

	/* calculating R */
	Loc_u16val = (4096 / Loc_u16val) - 1;      					/* (65535/ADC - 1) */
	Loc_u16val = SERIESRESISTOR / Loc_u16val;  						/* 10K / (65535/ADC - 1) */

	/* calculating T */
	Loc_f32celsius = (float32)1.0 * Loc_u16val / THERMISTORNOMINAL;     			/* (R/Ro) */
	Loc_f32celsius = log(Loc_f32celsius);                  			/* ln(R/Ro) */
	Loc_f32celsius /= BCOEFFICIENT;                   				/* 1/B * ln(R/Ro) */
	Loc_f32celsius += (float32)1.0 / (float32)(TEMPERATURENOMINAL + 273.15); 			/* + (1/To) */
	Loc_f32celsius = (float32)1.0 / (float32)Loc_f32celsius;                 			/* Invert */
	Loc_f32celsius -= (float32)273.15;                         				/* convert K temp to C */
	
	return(Loc_f32celsius);
}


static uint16 Read_ADC()
{
	uint16_t adc_val;
	//Start ADC1 Software Conversion
				ADC_SoftwareStartConvCmd(ADC1, ENABLE);
				//wait for conversion complete
				while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
				//read ADC value
				adc_val=ADC_GetConversionValue(ADC1);
				//clear EOC flag
				ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	return adc_val;
}
