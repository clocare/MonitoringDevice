/*******************************************************/ 
/* Author: Nourhan Mansour                             */
/* Date  : 8/8/2020                                    */
/* Vesion: 1.1                                         */
/* File  : RCC_config.h                                */
/*******************************************************/ 
#ifndef     RCC_CONFIG_H
#define     RCC_CONFIG_H


/*  enable Microcontroller output clock for the selected source 
    Values: 
        MCO_NOCLK
        MCO_SYSCLK
        MCO_HSI
        MCO_HSE
        MCO_PLL
*/
#define MICRO_OUTPUT_CLOCK      MCO_NOCLK            

// Clock source definitions 
#define HSI         0
#define HSE_RC      1
#define HSE_XTAL    2

// Clock Source: HSI - HSE_RC - HSE_XTAL 
#define CLOCK_SRC               HSE_XTAL
// Clock PLL: 1:Enable   -   0:Disable
#define CLOCK_PLL              	1

#if CLOCK_SRC == HSI
    /*
        HSI Trim:
        default value is 16  
        +/-1 step = 40KHZ
        Min value = 0
        Max value = 31 
    */
    #define TRIM_VALUE  0
#elif CLOCK_SRC == HSE_RC ||  CLOCK_SRC == HSE_XTAL 
    /**
     * Choose to 1 enable Security , 0 to diable 
    */
    #define CSSON   1
#endif

#if CLOCK_PLL == 1
    /* Options: 
        RCC_PLL_HSI_DIV2
        RCC_PLL_HSE_DIV2
        RCC_PLL_HSE
    */
    #define PLL_SRC     RCC_PLL_HSE
    /* Choose multiplier
        integer value:  2 ~ 16 
        Warning: result clock MUST NOT exceed 72MHZ
    */
    #define PLL_MUL     6
#endif

/* Clocks prescalars */

/* Available options for AHB Clock prescalar are:-
	RCC_AHB_PRESCALAR_DIV_NO 	
	RCC_AHB_PRESCALAR_DIV_2 	
	RCC_AHB_PRESCALAR_DIV_4 	
		...
	RCC_AHB_PRESCALAR_DIV_512
*/
#define RCC_AHB_PRESCALAR_DIV 	RCC_AHB_PRESCALAR_DIV_NO 	

/* Available options for APB1 Clock prescalar are:-
	RCC_APB1_PRESCALAR_DIV_NO
	RCC_APB1_PRESCALAR_DIV_2	
	RCC_APB1_PRESCALAR_DIV_4	
	RCC_APB1_PRESCALAR_DIV_8	
	RCC_APB1_PRESCALAR_DIV_16 						*/
#define RCC_APB1_PRESCALAR_DIV 	RCC_APB1_PRESCALAR_DIV_2

/* Available options for APB2 Clock prescalar are:-
	RCC_APB2_PRESCALAR_DIV_NO
	RCC_APB2_PRESCALAR_DIV_2	
	RCC_APB2_PRESCALAR_DIV_4	
	RCC_APB2_PRESCALAR_DIV_8	
	RCC_APB2_PRESCALAR_DIV_16 						*/
#define RCC_APB2_PRESCALAR_DIV 	RCC_APB2_PRESCALAR_DIV_NO 

#endif  // End Of File
