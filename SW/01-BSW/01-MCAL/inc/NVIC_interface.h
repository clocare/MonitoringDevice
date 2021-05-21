/*******************************************************/ 
/* Author: Nourhan Mansour                             */
/* Date  : 21/8/2020                                   */
/* Vesion: 1.0                                         */
/* File  : NVIC_interface.h                            */
/*******************************************************/ 
#ifndef NVIC_INTERFACE_H
#define NVIC_INTERFACE_H

typedef enum {
    NVIC_WWDG = 0, 
    NVIC_PVD,
    NVIC_TAMPER,
    NVIC_RTC,
    NVIC_FLASH,
    NVIC_RCC,
    NVIC_EXTI0,
    NVIC_EXTI1,
    NVIC_EXTI2,
    NVIC_EXTI3,
    NVIC_EXTI4,
    NVIC_DMA1_Channel1,
    NVIC_DMA1_Channel2,
    NVIC_DMA1_Channel3,
    NVIC_DMA1_Channel4,
    NVIC_DMA1_Channel5,
    NVIC_DMA1_Channel6,
    NVIC_DMA1_Channel7,
    NVIC_ADC1_2,
    NVIC_USB_HP_CAN_TX,
    NVIC_USB_LP_CAN_RX,
    NVIC_CAN_RX1,
    NVIC_CAN_SCE,
    NVIC_EXTI9_5,
    NVIC_TIM1_BRK,
    NVIC_TIM1_UP,
    NVIC_TIM1_TRG_COM,
    NVIC_TIM1_CC,
    NVIC_TIM2,
    NVIC_TIM3,
    NVIC_TIM4,
    NVIC_I2C1_EV,
    NVIC_I2C1_ER,
    NVIC_I2C2_EV,
    NVIC_I2C2_ER,
    NVIC_SPI1,
    NVIC_SPI2,
    NVIC_USART1,
    NVIC_USART2,
    NVIC_USART3,
    NVIC_EXTI15_10,
    NVIC_RTCAlarm,
    NVIC_USBWakeup,
    NVIC_TIM8_BRK,
    NVIC_TIM8_UP,
    NVIC_TIM8_TRG_COM,
    NVIC_TIM8_CC,
    NVIC_ADC3,
    NVIC_FSMC,
    NVIC_SDIO,
    NVIC_TIM5,
    NVIC_SPI3,
    NVIC_UART4,
    NVIC_UART5,
    NVIC_TIM6,
    NVIC_TIM7,
    NVIC_DMA2_Channel1,
    NVIC_DMA2_Channel2,
    NVIC_DMA2_Channel3,
    NVIC_DMA2_Channel4_5,

}t_Interrupt_ID;

/**
 * Function: NVIC_voidInitNVIC 
 * Description: Set interrupt groups set based on the groups configutrations in the config file
 * Parameters: No
 * Return: void
 **/
void NVIC_voidInitNVIC (void);

/**
 * Function: NVIC_voidEnableInterrupt 
 * Description: Enable interrupt with id  cpy_interrupt_id ONLY 
 *              it doesn't control Prephiral interrupt control or GIE 
 * Parameters: interrupt ID
 * Return: void
 **/
void NVIC_voidEnableInterrupt(t_Interrupt_ID cpy_interrupt_id);

/**
 * Function: NVIC_voidDisableInterrupt 
 * Description: Disable interrupt with id  cpy_interrupt_id ONLY 
 *              it doesn't control Prephiral interrupt control or GIE 
 * Parameters: interrupt ID
 * Return: void
 **/
void NVIC_voidDisableInterrupt(t_Interrupt_ID cpy_interrupt_id);

/**
 * Function: NVIC_voidSetPendingFlag 
 * Description: Set pending interrupt flag with SW. No HW needed 
 * Parameters: interrupt ID
 * Return: void
 **/
void NVIC_voidSetPendingFlag(t_Interrupt_ID cpy_interrupt_id);


/**
 * Function: NVIC_voidClearPendingFlag 
 * Description: Clear pending interrupt flag with SW. No HW needed  
 * Parameters: interrupt ID
 * Return: void
 **/
void NVIC_voidClearPendingFlag(t_Interrupt_ID cpy_interrupt_id);

/**
 * Function: NVIC_u8GetActiveFlag 
 * Description: Read the active flag status of a specific interrupt
 *              Active flag indicate the interrupt is being processed. 
 * Parameters: interrupt ID
 * Return: Active flag statuse 
 *          1: Raised 
 *          0: No active
 **/
uint8 NVIC_u8GetActiveFlag(t_Interrupt_ID cpy_interrupt_id);


/**
 * Function: NVIC_voidSetInterruptPiriority 
 * 
 * Description: Dependning on the divion of groups and sub Groups,
 *              Each interrupt take a group piriorty and sub group piriority 
 *              Interrupts within the same group cannot be nested 
 *              Interrupts within the same group which happened simultanusly, 
 *              will be allowed to excute in order of their subgroup piriority 
 *              If two or more interrupts have the same SW piriority, they will be served 
 *              with respect to their HW piriority.
 * 
 * Parameters: interrupt ID , Group ID , Sub Group ID
 * 
 * Note: Values of the Group pitriority and sub piriority mus be consistenet with the Interrupt groups 
 *       specified in the conofig file.
 *  
 * EX:  INTERRUPT_GROUPS = GROUP2_SUB8
 *      cpy_Group_Piriority value: 0 ~ 1
 *      cpy_Sub_piriority value: 0 ~ 7
 * 
 * Return: void
 **/
void NVIC_voidSetInterruptPiriority(t_Interrupt_ID cpy_interrupt_id , uint8 cpy_Group_Piriority , uint8 cpy_Sub_piriority);

#endif // End Of File
