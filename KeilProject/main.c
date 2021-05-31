/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 21/5/2021                                   */
/* Version: 1.0.0                                       */
/* File   : main.c					                            */
/********************************************************/ 

/***************** Lib *****************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "hw_reg.h"

/***************** MCAL Drivers *****************/
#include "ADC1_interface.h"
#include "GPIO_interface.h"
#include "I2C_config.h"
#include "RCC_interface.h"
#include "SPI1.h"
#include "SPI2.h"
#include "TIMER_interface.h"
#include "NVIC_interface.h"
#include "UART_interface.h"
#include "I2C_interface.h"
#include "ADC1_interface.h"

/***************** ECUAL Drivers *****************/
#include "TFT_interface.h"
#include "ESP8266.h"
#include "mfrc522.h"
#include "TempSensor_interface.h"
#include "MAX30102_interface.h"

/***************** Software components *****************/
#include "Display.h"
#include "Reader.h"
#include "Observer.h"
#include "GateWay.h"
#include "App_tasks.h"

/***************** Free RTOS files *****************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/************************************************************************/
/*										Local Functions 																	*/
/************************************************************************/
static void RCC_clkConfig(void);
static void GPIO_config(void);

/************************************************************************/
/*                      Global variables                                */
/************************************************************************/

TaskHandle_t ObserverHandle = NULL_PTR; 
TaskHandle_t DisplayHandle = NULL_PTR;
TaskHandle_t ReaderHandle = NULL_PTR;
TaskHandle_t GateWayHandle = NULL_PTR;
TaskHandle_t EmergencyHandle = NULL_PTR;
QueueHandle_t xCardIdQueue = NULL_PTR;
QueueHandle_t xSensorsDataumQueue = NULL_PTR;
SemaphoreHandle_t xCardAssignedSem = NULL_PTR;


/**************** MAIN ****************/
int main (void)
{
	/* 	Enable Clock 	*/
	RCC_clkConfig();

	/* 	pins configuration 	*/
	GPIO_config();
	
	/* Init MCAL modules */
	TIMER_voidInit();
  SPI1_voidInit();
	SPI2_voidInit();
	UART_voidInit();
	I2C_voidPeripheralControl(I2C1, I2C_ENABLE);
	I2C_voidInit(I2C1);
	ADC1_voidInit();
	
	/*	Init SWC		*/
	Display_init();
	Reader_init();
	//Observer_init();			// Sensors are not availabale yet
	GateWay_init();
	// CREATE RTOS TASKS 
	xTaskCreate(
                    Observer_mainTask,       			/* Function that implements the task. */
                    "OBSERVER",          					/* Text name for the task. */
                    OBSERVER_TASK_STACK_SIZE,     /* Stack size in words, not bytes. */
                    ( void * ) 1,    							/* Parameter passed into the task. */
                    Observer_PRIORITY,						/* Priority at which the task is created. */
                    &ObserverHandle );      			/* Used to pass out the created task's handle. */

	xTaskCreate(
                    Display_mainTask,       			/* Function that implements the task. */
                    "DISPLAY",          					/* Text name for the task. */
                    DISPLAY_TASK_STACK_SIZE,     	/* Stack size in words, not bytes. */
                    ( void * ) 1,    							/* Parameter passed into the task. */
                    Display_PRIORITY,							/* Priority at which the task is created. */
                    &DisplayHandle );      				/* Used to pass out the created task's handle. */
	xTaskCreate(
                    Reader_MainTask,       				/* Function that implements the task. */
                    "READER",          						/* Text name for the task. */
                    READER_TASK_STACK_SIZE,     	/* Stack size in words, not bytes. */
                    ( void * ) 1,    							/* Parameter passed into the task. */
                    Reader_PRIORITY,							/* Priority at which the task is created. */
                    &ReaderHandle );      				/* Used to pass out the created task's handle. */
	
	xTaskCreate(
                    GateWay_mainTask,       			/* Function that implements the task. */
                    "GATEWAY",          					/* Text name for the task. */
                    GATEWAY_TASK_STACK_SIZE,     	/* Stack size in words, not bytes. */
                    ( void * ) 1,    							/* Parameter passed into the task. */
                    GateWay_PRIORITY,							/* Priority at which the task is created. */
                    &GateWayHandle );      				/* Used to pass out the created task's handle. */
xTaskCreate(
                    Emergency_mainTask,       			/* Function that implements the task. */
                    "EMERGENCY",          					/* Text name for the task. */
                    EMERGENCY_TASK_STACK_SIZE,     	/* Stack size in words, not bytes. */
                    ( void * ) 1,    								/* Parameter passed into the task. */
                    Emergency_PRIORITY,							/* Priority at which the task is created. */
                    &EmergencyHandle );      				/* Used to pass out the created task's handle. */

									
	// Create Queues 
	xCardIdQueue =xQueueCreate(ID_READ_LEN , sizeof(loginData_Type) );
	xSensorsDataumQueue = xQueueCreate(1 , sizeof(ObserverReadingsType));
										
	/* Start the scheduler. */
	vTaskStartScheduler();
	while (1)
	{
	}
	return 0;
}		// End main

/************************************************************************/
/*                RTOS TASKS APIs Definitions                           */
/************************************************************************/

/******************************************
 * @name : Observer_mainTask
 * @param: NONE
 * @return: NONE
 * Non Reentrant
 * Sync
 * Main periodic task.
 ******************************************/
void Observer_mainTask(void * pvParameters)
{
	uint32_t NotifiedVal;
	ObserverReadingsType readings = {37 , HEARTRATE_MIN , SPO2_MIN  , 0};
	volatile uint8 err = 0 ;
	volatile uint8 err_counter = 0;
	
	while(1)
	{
		if ((NotifiedVal & NEW_CARD_PRESENT_NOTIFICATION) == NEW_CARD_PRESENT_NOTIFICATION)
		{
			// err = Observer_GetCurrentReadings(&readings);
			
			/************************* TEST CODE ****************************/
			 if (readings.HeartRate < 180)
					readings.HeartRate+=5;
			if (readings.SPO2 < 120)
					readings.SPO2+=10;
			if (readings.Temp < 250)
					readings.Temp+=5;
			err = 0 ;
			if (readings.Temp < TEMP_MIN ||readings.Temp > TEMP_MAX )
			{
				err++;
			}
			if (readings.SPO2 < SPO2_MIN ||readings.SPO2 > SPO2_MAX )
			{
				err++;
			}
			if (readings.HeartRate < HEARTRATE_MIN ||readings.HeartRate > HEARTRATE_MAX )
			{
				err++;
			}
			/***********************************************************************************/
			
			xQueueOverwrite(xSensorsDataumQueue , &readings );
			/* Check if values are in critical range while still in valid readings range */
			if ( (readings.HeartRate <= HEARTRATE_CRITICAL_MIN && readings.HeartRate > HEARTRATE_MIN ) 
					|| ( readings.SPO2 <= SPO2_CRITICAL_MIN  && readings.SPO2 > SPO2_MIN )
					|| ( readings.Temp >= TEMP_CRITICAL_MIN && readings.Temp < TEMP_MAX)
				)
			{
				xTaskNotify(EmergencyHandle , EMERGENCY_MSG_NOTIFICATION , eSetValueWithOverwrite);
				xTaskNotify(DisplayHandle  , EMERGENCY_MSG_NOTIFICATION  , eSetValueWithOverwrite );
				vTaskDelay(10 / portTICK_PERIOD_MS); // to make sure notification is recieved.  
			}
			else 
			{
				// ok
			}
			
			/* check if all values are invalid */
			if (err > 2)
			{
				err_counter++;
			}
			else 
			{
				err_counter = 0;
			}
			
			
			if (err_counter > MAX_ERROR_COUNT)			// check if all values are invalid for x times in raw. 
			{
				// No patient is assigned , scan a new card. 
				xTaskNotify(DisplayHandle , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );
				xTaskNotify(ReaderHandle  , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );
				xTaskNotify(GateWayHandle , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );

				// Go Idle and wait for Wake up notification
				xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
			}
			else if (err == 0)
			{
				// Update readings 
				xTaskNotify(GateWayHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
				// Update display. 
				xTaskNotify(DisplayHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			}			
			else 
			{
				// Update display. 
				xTaskNotify(DisplayHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			}

			vTaskDelay(300 / portTICK_PERIOD_MS);		// Wait before new reading collection
		}
		else 
		{
			xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
		}
		
	}
}

/******************************************
 * @name : Reader_MainTask
 * @param: NONE
 * @return: NONE
 * Non Reentrant
 * Sync
 * Main periodic task.
 ******************************************/

void Reader_MainTask(void * pvParameters)
{
loginData_Type ID ; 
	uint32_t NotifiedVal = 0; 
	while(1)
	{
		//NotifiedVal = 0x00; 
		if (Reader_isNewCardPresent() == TRUE)
		{
			// Get ID to be used by the GateWay task in local server communication
			ID = Reader_GetLogin();
			xQueueSend(xCardIdQueue , &ID  , portMAX_DELAY);	
			
			// Notify Observer, Display, Gateway, and emergency task to start working 
			xTaskNotify(ObserverHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			xTaskNotify(DisplayHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			
			// Go Idle and wait for card detached notification
			xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
			Reader_init();
		}
		else
		{
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
}

/******************************************
 * @name : Display_mainTask
 * @param:  NONE
	* @return: NONE 
 * Non Reentrant
 * Sync
 * main display periodic task 
 ******************************************/

	void Display_mainTask(void * pvParameters)
{
		uint32_t NotifiedVal  = 0; 
		ObserverReadingsType reading; 
		while(1)
		{

				if ( (NotifiedVal & NEW_CARD_PRESENT_NOTIFICATION) == NEW_CARD_PRESENT_NOTIFICATION)
				{
					xQueuePeek( xSensorsDataumQueue, &( reading ), portMAX_DELAY);
					Display_UpdateSensors(reading);
					Display_CurrentSensorsState();
					// vTaskDelay(500 / portTICK_PERIOD_MS);
				}
				else if ( (NotifiedVal & CARD_DETACHED_NOTIFICATION) == CARD_DETACHED_NOTIFICATION)
				{
					Display_NoCard();
				}
				else if ((NotifiedVal & EMERGENCY_MSG_NOTIFICATION) == EMERGENCY_MSG_NOTIFICATION)
				{
					Display_Emeregency();
				}
				else 
				{
				}
				xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);

		}
		
	
}

void GateWay_mainTask(void * pvParameters)
{
	uint32_t NotifiedVal = 0 ;
	ObserverReadingsType reading; 
	loginData_Type ID ; 

	while(1)
	{
		xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
		if ( (NotifiedVal & NEW_CARD_PRESENT_NOTIFICATION) == NEW_CARD_PRESENT_NOTIFICATION)
		{
			xQueuePeek(xSensorsDataumQueue ,&reading , portMAX_DELAY);
			xQueuePeek(xCardIdQueue ,&ID , portMAX_DELAY);
			// Send over to the Gateway.
			GateWay_WriteLoginData(ID);
			GateWay_WriteSensorsReadings(reading);
		}
		else 
		{
		}
	}
}
	
void Emergency_mainTask(void * pvParameters)
{
	uint32_t NotifiedVal = 0 ;
	loginData_Type ID ; 
	while(1)
	{
		xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
		if ( (NotifiedVal & EMERGENCY_MSG_NOTIFICATION) == EMERGENCY_MSG_NOTIFICATION)
		{
				xQueuePeek(xCardIdQueue ,&ID , portMAX_DELAY);
				GateWay_WriteLoginData(ID);
				GateWay_SendEmergencyMsg();
		}
	}
}
/************************************************************************/	
/*********** 					Local functions definitions			 			*************/
/************************************************************************/
static void RCC_clkConfig(void)	
{
	RCC_voidInitSystemClock();
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_IOPAEN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_IOPBEN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_AFIOEN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_SPI1EN);
	RCC_voidEnableClock(t_APB1, RCC_APB1ENR_SPI2EN);
	RCC_voidEnableClock(t_APB2 , RCC_APB2ENR_USART1EN);
	RCC_voidEnableClock(t_APB1, RCC_APB1ENR_TIM3EN);
	RCC_voidEnableClock(t_APB1, RCC_APB1ENR_I2C1EN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_ADC1EN );
	
	return ; 
}

static void GPIO_config(void)
{
	
	// Allow I/O usage of JTAG pins A15 , B3 , B4
	// AFIO clk must be enabled first. 
	SET_BIT(AFIO->MAPR , 25);

		/** Init TFT pinsout **/
	GPIO_voidInitPortPinDirection(PORTB, PIN4, OUTPUT_PUSH_PULL_10MHZ);		// RESET PIN
	GPIO_voidInitPortPinDirection(PORTB, PIN3, OUTPUT_PUSH_PULL_10MHZ);		// Data Command PIN
	
	GPIO_voidInitPortPinDirection(PORTB, PIN12, OUTPUT_PUSH_PULL_10MHZ);	// Chip Select  PIN
	GPIO_voidInitPortPinDirection(PORTB, PIN13, AF_PUSH_PULL_10MHZ);			// CLK PIN
//GPIO_voidInitPortPinDirection(PORTB, PIN14, INPUT_FLOATING);					// MISO PIN - Not IN USE
	GPIO_voidInitPortPinDirection(PORTB, PIN15, AF_PUSH_PULL_10MHZ);			// MOSI PIN
	GPIO_voidInitPortPinDirection(PORTB, PIN5, OUTPUT_PUSH_PULL_10MHZ);		// LED PIN
	
	GPIO_voidSetPortPinValue(PORTB, PIN5 , STD_HIGH);											// Turn LED on

	//	Init mfrc522 GPIO pins
	// RST O/P PP && SS  O/P PP
	GPIO_voidInitPortPinDirection(PORTA, PIN3 , OUTPUT_PUSH_PULL_10MHZ);	// RESET PIN
	GPIO_voidInitPortPinDirection(PORTA, PIN4, OUTPUT_PUSH_PULL_10MHZ);		// SS PIN
	GPIO_voidInitPortPinDirection(PORTA, PIN5, AF_PUSH_PULL_10MHZ);				// CLK PIN
	GPIO_voidInitPortPinDirection(PORTA, PIN6, INPUT_FLOATING);						// MISO PIN
	GPIO_voidInitPortPinDirection(PORTA, PIN7, AF_PUSH_PULL_10MHZ);				// MOSI PIN
	
	GPIO_voidSetPortPinValue(PORTA, PIN4 , STD_HIGH);
	GPIO_voidSetPortPinValue(PORTA, PIN3 , STD_HIGH);

	// init SPO2 & I2C 
	GPIO_voidInitPortPinDirection(PORTB, PIN6, AF_OPEN_DRAIN_2MHZ);			// SCL PIN
	GPIO_voidInitPortPinDirection(PORTB, PIN7, AF_OPEN_DRAIN_2MHZ);			// SDA PIN
	
	// init UART pins 
	GPIO_voidInitPortPinDirection(PORTA  , PIN9 , AF_PUSH_PULL_50MHZ);  // TX PIN
	GPIO_voidInitPortPinDirection(PORTA , PIN10 , INPUT_FLOATING);			// RX PIN
	
	// GPIO configuration of Temp sensor. 
	GPIO_voidInitPortPinDirection( PORTB , PIN0 , INPUT_ANALOG);				// Temp Sensor PIN 

	return ; 
}
