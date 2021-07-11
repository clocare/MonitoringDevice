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
#include "spo2_algorithm.h"
#include "StringManipulation.h"

/***************** MCAL Drivers *****************/
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "SPI1.h"
#include "SPI2.h"
#include "TIMER_interface.h"
#include "NVIC_interface.h"
#include "UART_interface.h"
#include "I2C_interface.h"
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
TaskHandle_t Spo2Handle = NULL_PTR;
TaskHandle_t DisplayHandle = NULL_PTR;
TaskHandle_t ReaderHandle = NULL_PTR;
TaskHandle_t GateWayHandle = NULL_PTR;
TaskHandle_t EmergencyHandle = NULL_PTR;
QueueHandle_t xCardIdQueue = NULL_PTR;
QueueHandle_t Spo2Queue = NULL_PTR;
QueueHandle_t HRQueue = NULL_PTR;
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
	I2C_voidPeripheralControl(I2C1, I2C_ENABLE);
	I2C_voidInit(I2C1);
	TIMER_voidInit();
  SPI1_voidInit();
	SPI2_voidInit();
	UART_voidInit();
	
	/*	Init SWC		*/
	Observer_init();			
	Display_init();
	Reader_init();
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
                    SPO2_Algorithm_mainTask,       			/* Function that implements the task. */
                    "SPO2",          					/* Text name for the task. */
                    SPO2_TASK_STACK_SIZE,     /* Stack size in words, not bytes. */
                    ( void * ) 1,    							/* Parameter passed into the task. */
                    SPO2_PRIORITY,						/* Priority at which the task is created. */
                    &Spo2Handle );      			/* Used to pass out the created task's handle. */

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
	Spo2Queue = xQueueCreate(1 , sizeof(sint32));
	HRQueue = xQueueCreate(1 , sizeof(sint32));
										
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
 * @name : SPO2_Algorithm_mainTask
 * @param: NONE
 * @return: NONE
 * Non Reentrant
 * Sync
 * Main periodic task.
 ******************************************/
void SPO2_Algorithm_mainTask(void * pvParameters)
{
	uint32 irBuffer[100]; //infrared LED sensor data
 	uint32 redBuffer[100];  //red LED sensor data
 	sint32 bufferLength; //data length
 	sint32 spo2; //SPO2 value
 	sint8 validSPO2; //indicator to show if the SPO2 calculation is valid
 	sint32 heartRate; //heart rate value
 	sint8 validHeartRate; //indicator to show if the heart rate calculation is val
	uint32 NotifiedVal = 0;
	volatile uint8 i;	
	bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps
	
	//TickType_t xLastWakeTime;
	const TickType_t xFrequency =  portTICK_PERIOD_MS;

   // Initialise the xLastWakeTime variable with the current time.
   //  xLastWakeTime = xTaskGetTickCount();
	 
		//read the first 100 samples, and determine the signal range
		for ( i = 0 ; i < bufferLength ; i++)
		{
			while (MAX30102_available() == FALSE) //do we have new data?
				MAX30102_check(); //Check the sensor for new data

			redBuffer[i] = MAX30102_getRed();
			irBuffer[i] = MAX30102_getIR();
			MAX30102_nextSample(); //We're finished with this sample so move to next sample
      vTaskDelay( xFrequency );
			//taskYIELD()

		}

		//calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
		maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
		if ( (validHeartRate != 0) )
		{
			xQueueOverwrite(HRQueue , &heartRate );
		}
		if (validSPO2 != 0 )
		{
			xQueueOverwrite(Spo2Queue , &spo2 );
		}
	//	vTaskSuspend(Spo2Handle);
		//Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
		while (1)
		{
			if ( (NotifiedVal & NEW_CARD_PRESENT_NOTIFICATION) == NEW_CARD_PRESENT_NOTIFICATION)
			{		
					//dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
					for ( i = 25; i < 100; i++)
					{
						redBuffer[i - 25] = redBuffer[i];
						irBuffer[i - 25] = irBuffer[i];
					}

					//take 25 sets of samples before calculating the heart rate.
					for ( i = 75; i < 100; i++)
					{
						while (MAX30102_available() == FALSE) //do we have new data?
											MAX30102_check(); //Check the sensor for new data
						redBuffer[i] = MAX30102_getRed();
						irBuffer[i] = MAX30102_getIR();
					//	vTaskDelay( xFrequency );

					}
					//After gathering 25 new samples recalculate HR and SP02
					maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
					
							if ( (validHeartRate != 0) )
							{
									xQueueOverwrite(HRQueue , &heartRate );
							}
							
							if (validSPO2 != 0 )
							{
								xQueueOverwrite(Spo2Queue , &spo2 );
							}
					taskYIELD()
				//	vTaskDelay( xFrequency );
			//		vTaskSuspend(Spo2Handle);
				}

		else 							// Check if no card present
			{
			xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
			}
	}
	return ; 
}

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
	uint32 NotifiedVal = 0;
	ObserverReadingsType readings = {0};
	volatile uint8 err = 0 ;
	volatile uint8 err_counter = 0;
	
	while(1)
	{
		if ((NotifiedVal & NEW_CARD_PRESENT_NOTIFICATION) == NEW_CARD_PRESENT_NOTIFICATION)
		{

			err = Observer_GetCurrentTemp(&readings);
			if ( xQueueReceive (Spo2Queue , & (readings.SPO2) , 100 ) == pdFALSE  )
			{
				readings.SPO2 = 0;
				err++;
			}
			else if ( (readings.SPO2 < SPO2_MIN ) || ( readings.SPO2 > SPO2_MAX ) )
			{
				readings.SPO2 = 0;
				err++;
			}
			else 
			{	
				//ok
			}
			if (xQueueReceive (HRQueue , & (readings.HeartRate) , 100 ) == pdFALSE)
			{
				readings.HeartRate = 0;
				err++;
			}
			else if (readings.HeartRate < HEARTRATE_MIN ||readings.HeartRate > HEARTRATE_MAX )
			{
				readings.HeartRate = 0;
				err++;
			}
			else 
			{	
				//ok
			}
			xQueueOverwrite(xSensorsDataumQueue , &readings );
			/* Check if values are in critical range while still in valid readings range */
			if ( (readings.HeartRate <= HEARTRATE_CRITICAL_MIN && readings.HeartRate > HEARTRATE_MIN ) 
					|| ( readings.SPO2 <= SPO2_CRITICAL_MIN  && readings.SPO2 > SPO2_MIN )
					|| ( readings.Temp >= TEMP_CRITICAL_MIN && readings.Temp < TEMP_MAX)
				)
			{
				xTaskNotify(EmergencyHandle , EMERGENCY_MSG_NOTIFICATION , eSetValueWithOverwrite);
				xTaskNotify(DisplayHandle  , EMERGENCY_MSG_NOTIFICATION  , eSetValueWithOverwrite );
				vTaskDelay(10 * portTICK_PERIOD_MS); // to make sure notification is recieved.  
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
				xTaskNotify(Spo2Handle , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );
				xTaskNotify(DisplayHandle , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );
				xTaskNotify(ReaderHandle  , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );
				xTaskNotify(GateWayHandle , CARD_DETACHED_NOTIFICATION  , eSetValueWithOverwrite );

				// Go Idle and wait for Wake up notification
				xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
			}
			else if (err == 0)
			{
				// Update readings 
				xTaskNotify(GateWayHandle , CARD_IS_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
				// Update display. 
				xTaskNotify(DisplayHandle , CARD_IS_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			}			
			else 
			{
				// Update display. 
				xTaskNotify(DisplayHandle , CARD_IS_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			}
			vTaskDelay(200 * portTICK_PERIOD_MS);		// Wait before new reading collection
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
	loginData_Type ID = {"2981001234908" ,"", "2981001234908" , ""};
	uint32_t NotifiedVal = 0; 
//	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 100 * portTICK_PERIOD_MS;
		// Initialise the xLastWakeTime variable with the current time.
//		xLastWakeTime = xTaskGetTickCount();	

 
	while(1)
	{
		//NotifiedVal = 0x00; 
		if (Reader_isNewCardPresent() == TRUE)
		{
			// Get ID to be used by the GateWay task in local server communication
		//	Reader_SetLoginData(ID);
			ID = Reader_GetLogin();
			xQueueSend(xCardIdQueue , &ID  , portMAX_DELAY);	
			
			// Notify Observer, Display, Gateway, and emergency task to start working 
			xTaskNotify(DisplayHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			xTaskNotify(ObserverHandle, NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );
			vTaskDelay(2 * portTICK_PERIOD_MS);	
			xTaskNotify(GateWayHandle , NEW_CARD_PRESENT_NOTIFICATION  , eSetValueWithOverwrite );		
			vTaskDelay(2 * portTICK_PERIOD_MS);			// Delay to enable Login process to complete.
			xTaskNotify(Spo2Handle , NEW_CARD_PRESENT_NOTIFICATION ,eSetValueWithOverwrite ); 

			// Go Idle and wait for card detached notification
			xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
			Reader_init();
		}
		else
		{
			 vTaskDelay( xFrequency );

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
					Display_CurrentSensorsState();
				}
				else if (((NotifiedVal & CARD_IS_PRESENT_NOTIFICATION) == CARD_IS_PRESENT_NOTIFICATION))
				{
					xQueuePeek( xSensorsDataumQueue, &( reading ), portMAX_DELAY);
					Display_UpdateSensors(reading);
					Display_CurrentSensorsState();
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
	loginData_Type login ; 
	boolean status = 0 ;
	while(1)
	{
		xTaskNotifyWait( 0xffffffff  , 0x00  , &NotifiedVal   , portMAX_DELAY);
		if ( (NotifiedVal & NEW_CARD_PRESENT_NOTIFICATION) == NEW_CARD_PRESENT_NOTIFICATION)
		{
			xQueuePeek(xCardIdQueue ,&login , portMAX_DELAY);
			status = 0; 
			while (status == 0)
			{
				// Keep try to log in 
				status = GateWay_WriteLoginData(login);
				vTaskDelay(100 * portTICK_PERIOD_MS);		// Wait before sending again

			}
		}
		else if ( (NotifiedVal & CARD_IS_PRESENT_NOTIFICATION) == CARD_IS_PRESENT_NOTIFICATION)
		{
				xQueuePeek(xSensorsDataumQueue ,&reading , portMAX_DELAY);
				GateWay_WriteSensorsReadings(reading);
				vTaskDelay(1000 * portTICK_PERIOD_MS);		// Wait before sending again

		}
		else{}
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
		//		GateWay_WriteLoginData(ID);
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
