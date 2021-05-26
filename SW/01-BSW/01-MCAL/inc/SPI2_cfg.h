#/********************************************************/ 
/* Author : Nourhan Mansour	                             */
/* Date   : 8/5/2021	                                   */
/* Version: 1.0.0          	                             */
/* File   : SPI2_config.h    	                           */
/*********************************************************/ 
#ifndef SPI2_CONFIG_H
#define SPI2_CONFIG_H
     

/* Master Slave Selection */
#define SPI_MASTER_MODE         1U
#define SPI_SLAVE_MODE          0U
#define SPI2_MODE            		SPI_MASTER_MODE

/* Clk Polarity & phase  for either master or slave*/
/*
    Logic 0 when Idle = 0
    Logic 1 when Idle = 1
*/
#define SPI_CLK_POL_HIGH				1U
#define SPI_CLK_POL_LOW					0U
#define SPI2_CLK_POL         		SPI_CLK_POL_HIGH

/*
    First data Transition in first clock  = 0
    First data Transition in second clock = 1 
*/
#define SPI_CLK_PHA_HIGH				 1U
#define SPI_CLK_PHA_LOW					 0U
#define SPI2_CLK_PHASE       		 SPI_CLK_PHA_HIGH

/* LSB FIRST = 1 
   MSB FIRST = 0
*/ 
#define SPI_LSB_FIRST           1U
#define SPI_LSB_LAST            0U

#define SPI2_TransferStart			SPI_LSB_LAST

/* 8  Bit Data Frame  = 0 
   16 Bit Data Frame  = 1
*/
#define SPI_8BIT_DFF						0U
#define SPI_16BIT_DFF						1U

#define SPI2_DFF_MODE           SPI_8BIT_DFF 


#if SPI2_MODE == SPI_MASTER_MODE 
        
    #define SPIBAUD_RATE_CLK_DIV2           0
    #define SPIBAUD_RATE_CLK_DIV4           1
    #define SPIBAUD_RATE_CLK_DIV8           2
    #define SPIBAUD_RATE_CLK_DIV16          3
    #define SPIBAUD_RATE_CLK_DIV32          4
    #define SPIBAUD_RATE_CLK_DIV64          5
    #define SPIBAUD_RATE_CLK_DIV128         6
    #define SPIBAUD_RATE_CLK_DIV256         7

    #define SPI2_BAUD_RATE_SET               SPIBAUD_RATE_CLK_DIV2
#endif


#endif // End Of File
