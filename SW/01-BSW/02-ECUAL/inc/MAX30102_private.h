/*
 * MAX30102_private.h
 *
 *  Created on: Apr 25, 2021
 *      Author: Omar
 */

#ifndef MAX30102_PRIVATE_H_
#define MAX30102_PRIVATE_H_

/*  Slave Address	*/
#define MAX30102_SLAVE_ADDRESS		0x57

/*  Status Registers	*/
#define MAX30102_INT_STAT1 			0x00
#define MAX30102_INT_STAT2 			0x01
#define MAX30102_INT_ENABLE1 		0x02
#define MAX30102_INT_ENABLE2 		0x03

/*  FIFO Registers	*/
#define MAX30102_FIFO_WRIT_EPTR  	0x04
#define MAX30102_FIFO_OVER_FLOW  	0x05
#define MAX30102_FIFO_READ_PTR  	0x06
#define MAX30102_FIFO_DATA 			0x07

/*  Configuration Registers	*/
#define MAX30102_FIFO_CONFIG  		0x08
#define MAX30102_MODE_CONFIG  		0x09
#define MAX30102_SPO2_CONFIG  		0x0A
#define MAX30102_LED1_PULSE_AMP  	0x0C
#define MAX30102_LED2_PULSE_AMP  	0x0D
#define MAX30102_MULTILED_CONFIG1  	0x11
#define MAX30102_MULTILED_CONFIG2  	0x12

/*	Die Temperature Registers	*/
#define MAX30102_DIE_TEMP_INT  		0x1F
#define MAX30102_DIE_TEMP_FRAC  	0x20
#define MAX30102_DIE_TEMP_CONFIG  	0x21

/*	Part ID Registers	*/
#define MAX30102_REVISION_ID  		0xFE
#define MAX30102_PART_ID  			0xFF    // Should always be 0x15.

/* Interrupt Status 1 (Same for Interrupt Enable 1 except for PWR_RDY) Bits Macros (or starting bits for multi-bits areas) */
#define MAX30102_INT_STAT1_PWR_RDY						0
#define MAX30102_INT_STAT1_ALC_OVF						5
#define MAX30102_INT_STAT1_PPG_RDY						6
#define MAX30102_INT_STAT1_A_FULL						7

/* Interrupt Status 2 (Same for Interrupt Enable 2) Bits Macros (or starting bits for multi-bits areas) */
#define MAX30102_INT_STAT2_DIE_TEMP_RDY					1

/* FIFO Configuration Bits Macros (or starting bits for multi-bits areas) */
#define MAX30102_FIFO_CONFIG_FIFO_A_FULL				0
#define MAX30102_FIFO_CONFIG_FIFO_ROLLOVER_EN			4
#define MAX30102_FIFO_CONFIG_SMP_AVE					5

/* Mode Configuration Bits Macros (or starting bits for multi-bits areas) */
#define MAX30102_MODE_CONFIG_MODE						0
#define MAX30102_MODE_CONFIG_RESET						6
#define MAX30102_MODE_CONFIG_SHDN						7

/* SpO2 Configuration Bits Macros (or starting bits for multi-bits areas) */
#define MAX30102_SPO2_CONFIG_LED_PW						0
#define MAX30102_SPO2_CONFIG_SPO2_SR					2
#define MAX30102_SPO2_CONFIG_SPO2_ADC_RGE				5

/* Multi-LED Mode Control Register Bits Macros (or starting bits for multi-bits areas) */
#define MAX30102_MULTILED_CONFIG1__SLOT1				0
#define MAX30102_MULTILED_CONFIG1__SLOT2				4

// MAX30102 Commands
// Interrupt configuration (pg 13, 14)
#define  MAX30102_INT_A_FULL_MASK 		    (~0b10000000)
#define  MAX30102_INT_A_FULL_ENABLE  	    0x80
#define  MAX30102_INT_A_FULL_DISABLE  	    0x00
#define  MAX30102_INT_DATA_RDY_MASK         (~0b01000000)
#define  MAX30102_INT_DATA_RDY_ENABLE       0x40
#define  MAX30102_INT_DATA_RDY_DISABLE      0x00
#define  MAX30102_INT_ALC_OVF_MASK          (~0b00100000)
#define  MAX30102_INT_ALC_OVF_ENABLE 		0x20
#define  MAX30102_INT_ALC_OVF_DISABLE 		0x00
#define  MAX30102_INT_PROX_INT_MASK 		(~0b00010000)
#define  MAX30102_INT_PROX_INT_ENABLE 		(0x10)
#define  MAX30102_INT_PROX_INT_DISABLE		(0x00)
#define  MAX30102_INT_DIE_TEMP_RDY_MASK 	(~0b00000010)
#define  MAX30102_INT_DIE_TEMP_RDY_ENABLE  	0x02
#define  MAX30102_INT_DIE_TEMP_RDY_DISABLE  0x00
#define  MAX30102_SAMPLEAVG_MASK 			(~0b11100000)
#define  MAX30102_SAMPLEAVG_1  	0x00
#define  MAX30102_SAMPLEAVG_2  	0x20
#define  MAX30102_SAMPLEAVG_4  	0x40
#define  MAX30102_SAMPLEAVG_8  	0x60
#define  MAX30102_SAMPLEAVG_16  	0x80
#define  MAX30102_SAMPLEAVG_32  	0xA0
#define  MAX30102_ROLLOVER_MASK  	0xEF
#define  MAX30102_ROLLOVER_ENABLE  0x10
#define  MAX30102_ROLLOVER_DISABLE  0x00
#define  MAX30102_A_FULL_MASK  	0xF0
#define  MAX30102_SHUTDOWN_MASK  	0x7F
#define  MAX30102_SHUTDOWN  		0x80
#define  MAX30102_WAKEUP 			0x00
#define  MAX30102_RESET_MASK  		0xBF
#define  MAX30102_RESET  			0x40
#define  MAX30102_MODE_MASK  		0xF8
#define  MAX30102_MODE_REDONLY  	0x02
#define  MAX30102_MODE_REDIRONLY  	0x03
#define  MAX30102_MODE_MULTILED  	0x07

#define  MAX30102_ADCRANGE_MASK  	0x9F
#define  MAX30102_ADCRANGE_2048  	0x00
#define  MAX30102_ADCRANGE_4096  	0x20
#define  MAX30102_ADCRANGE_8192  	0x40
#define  MAX30102_ADCRANGE_16384  	0x60
#define  MAX30102_SAMPLERATE_MASK  	0xE3
#define  MAX30102_SAMPLERATE_50  	0x00
#define  MAX30102_SAMPLERATE_100  	0x04
#define  MAX30102_SAMPLERATE_200  	0x08
#define  MAX30102_SAMPLERATE_400  	0x0C
#define  MAX30102_SAMPLERATE_800  	0x10
#define  MAX30102_SAMPLERATE_1000  	0x14
#define  MAX30102_SAMPLERATE_1600  0x18
#define  MAX30102_SAMPLERATE_3200  0x1C
#define  MAX30102_PULSEWIDTH_MASK  0xFC
#define  MAX30102_PULSEWIDTH_69  	0x00
#define  MAX30102_PULSEWIDTH_118  	0x01
#define  MAX30102_PULSEWIDTH_215  	0x02
#define  MAX30102_PULSEWIDTH_411  	0x03
#define  MAX30102_SLOT1_MASK  		0xF8
#define  MAX30102_SLOT2_MASK  		0x8F
#define  MAX30102_SLOT3_MASK  		0xF8
#define  MAX30102_SLOT4_MASK  		0x8F
#define  SLOT_NONE  				0x00
#define  SLOT_RED_LED  			0x01
#define  SLOT_IR_LED  				0x02
#define  SLOT_GREEN_LED  			0x03
#define  SLOT_NONE_PILOT  			0x04
#define  SLOT_RED_PILOT 			0x05
#define  SLOT_IR_PILOT  			0x06
#define  SLOT_GREEN_PILOT  		0x07
#define  MAX_30105_EXPECTEDPARTID  0x15
#endif /* MAX30102_PRIVATE_H_ */
