/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 21/5/2021                                   */
/* Version: 1.0.0                                       */
/* File   : main.c					                            */
/********************************************************/ 

/* Lib */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "hw_reg.h"

/* MCAL Drivers */
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

/* ECUAL Drivers */
#include "TFT_interface.h"
#include "mfrc522.h"
#include "TempSensor_interface.h"
#include "MAX30102_interface.h"

/* Software components */
#include "Display.h"
#include "Reader.h"

/*	Local Functions 	*/
static void RCC_clkConfig(void);
static void GPIO_config(void);

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
// 	I2C_voidPeripheralControl(I2C1, I2C_ENABLE);
//	I2C_voidInit(I2C1);
//	ADC1_voidInit();
	
	/* Sensors */
//	TempSensor_voidInit();
//	MAX30102_voidInit();
	
	/*	Init SWC		*/
	Display_init();
	Reader_init();
	
	uint8 idtest[16] = {3 , 1 , 8 , 0 , 1 , 1 , 8 , 1 , 2 , 0 , 1 , 8 , 4 , 5 , 0 , 0 };
	volatile nationalID_Type id = {{3 , 1 , 8 , 0 , 1 , 1 , 8 , 1 , 2 , 0 , 1 , 8 , 4 , 5 , 0 , 0 },
												{0}
												};
	boolean flag = TRUE;
	uint8 i =0 ; 
	while(1)
	{
		
		if (Reader_isNewCardPresent() == TRUE)
		{
			if (Reader_SetId(id) == TRUE)
			{
				TFT_voidPrintText(10  , 60 , "   ID IS SET   " , 0xf4c3 , 0);

				id = Reader_GetId();
				for (i=0 ; i<12  ; i++)
				{
					if  (idtest[i] != id.id2[i])
					{
						flag = FALSE; 
					}
				}
				if (flag == TRUE){
					TFT_voidPrintText(10  , 70 , "   ID Retrieved   " , 0xf4c3 , 0);
					
					TFT_voidPrintText(10 , 80 , "31801181201845" , 0xf4c3 , 0);
			
					
				}
			}
		}
	}
	return 0;
}

/*********** Local functions definitions *************/

static void RCC_clkConfig(void)	
{
	RCC_voidInitSystemClock();
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_IOPAEN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_IOPBEN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_AFIOEN);
	RCC_voidEnableClock(t_APB2, RCC_APB2ENR_SPI1EN);
	RCC_voidEnableClock(t_APB1, RCC_APB1ENR_SPI2EN);
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
//GPIO_voidInitPortPinDirection(PORTB, PIN14, INPUT_FLOATING);					// MISO PIN
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
	
	// GPIO configuration of Temp sensor is handled within the driver. 
	
	return ; 
}
