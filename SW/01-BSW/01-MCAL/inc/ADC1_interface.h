/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#ifndef ADC1_INTERFACE_H
#define ADC1_INTERFACE_H

/* Macros */

#define ADC_ENABLE	1
#define ADC_DISABLE	0

/* watchdog channel */
#define ADC_CH0		0

/* Channel */
#define ADC_1_Channel			0

/* CR2 */
#define ADC_RIGHT_ALIGNMENT		0
#define ADC_LEFT_ALIGNMENT		1

/* External event, selected grop */
#define ADC_JSWSTART			0

/* External event, regular group */
#define ADC_SWSTART				0

/* SMPR1, SMPR2 */
#define ADC_1_5_CYCLES			0

/* SQR1, SQR2, SQR3 */
#define ADC_1_CONVERSION		0


/* public functions prototypes */

/******************************************************************************
* Description : initializing the ADC1 peripheral and onfiguring RCC
* Parameters  : void
* Return type : void
******************************************************************************/
void ADC1_voidInit(void);

/******************************************************************************
* Description : setting ADC1 threshold of analog watchdog
* Parameters  : low threshold(uint16), high threshold(uint16)
* Return type : void
******************************************************************************/																
void ADC1_voidSetThresholds(uint16 Copy_u16LowThreshold, uint16 Copy_u16HighThreshold);

/******************************************************************************
* Description : getting ADC1 conversion val
* Parameters  : void
* Return type : low 16 bit of DR register (uint16)
******************************************************************************/
uint16 ADC1_u16GetRegularVal(void);


/*
void ADC1_SetAWDCallBack(void (*CallBack) (void));
void ADC1_SetEOCCallBack(void (*CallBack) (void));

void (*ADC1_voidAWDCallBack)(void);
void (*ADC1_voidEOCCallBack)(void);
*/

#endif 


















