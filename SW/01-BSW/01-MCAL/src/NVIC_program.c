/*******************************************************/ 
/* Author: Nourhan Mansour                             */
/* Date  : 21/8/2020                                   */
/* Vesion: 1.0                                         */
/* File  : NVIC_program.c                              */
/*******************************************************/ 

// 1- include Libraries 
#include "BIT_MATH.h"
#include "STD_TYPES.h"

// 2- include interface file of needed lower layers

// 3- include driver files
#include "NVIC_interface.h"
#include "NVIC_private.h"
#include "NVIC_config.h"


void NVIC_voidInitNVIC (void){
    // 0x05FA0000  PASS CODE 
    SCB_AIRCR = 0x05FA0000 | ( INTERRUPT_GROUPS << 8 );

}
void NVIC_voidEnableInterrupt(t_Interrupt_ID cpy_interrupt_id){
    if(cpy_interrupt_id < 32){
        SET_BIT(NVIC_ISER0 , cpy_interrupt_id);     //Set interrupt enable
    }
    else if (cpy_interrupt_id < 64){
        cpy_interrupt_id -=32;
        SET_BIT(NVIC_ISER1 , cpy_interrupt_id);     //Set interrupt enable
    }
}
void NVIC_voidDisableInterrupt(t_Interrupt_ID cpy_interrupt_id){
    if(cpy_interrupt_id < 32){
        SET_BIT(NVIC_ICER0 , cpy_interrupt_id);     //Clear interrupt Enable
    }
    else if (cpy_interrupt_id < 64){
        cpy_interrupt_id -=32;
        SET_BIT(NVIC_ICER1 , cpy_interrupt_id);     //Clear interrupt Enable
    }
}
void NVIC_voidSetPendingFlag(t_Interrupt_ID cpy_interrupt_id){
    if(cpy_interrupt_id < 32){
        SET_BIT(NVIC_ISPR0 , cpy_interrupt_id); 
    }
    else if (cpy_interrupt_id < 64){
        cpy_interrupt_id -=32;
        SET_BIT(NVIC_ISPR1 , cpy_interrupt_id);     
    }
}
void NVIC_voidClearPendingFlag(t_Interrupt_ID cpy_interrupt_id){
    if(cpy_interrupt_id < 32){
        SET_BIT(NVIC_ICPR0 , cpy_interrupt_id);
    }
    else if (cpy_interrupt_id < 64){
        cpy_interrupt_id -=32;
        SET_BIT(NVIC_ICPR1 , cpy_interrupt_id);     
    }
}
uint8 NVIC_u8GetActiveFlag(t_Interrupt_ID cpy_interrupt_id){
    uint8 ret = 0;
    
    if(cpy_interrupt_id < 32){
        ret = GET_BIT(NVIC_IABR0 , cpy_interrupt_id);
    }
    else if (cpy_interrupt_id < 64){
        cpy_interrupt_id -=32;
        ret = GET_BIT(NVIC_IABR1 , cpy_interrupt_id);     
    }

    return ret;
}

void NVIC_voidSetInterruptPiriority(t_Interrupt_ID cpy_interrupt_id , uint8 cpy_Group_Piriority , uint8 cpy_Sub_piriority){
    /*
        Selection groups may take values 3 ~ 7
        Case of 3 all bits are group bits no sub bits and no shift needed 
        Case of 7 no bits are group bits and all 4 bits are sub bits. All group bits must be shifted by 4 (7-3)
        Equation: Subtract the first group value(3) from the selected group option to get the shift value 
                  of the group bits then OR with sub group bits 
        Note: NVIC_IPR registers are byte addressable 
              The least 4 bits are reserved and values must be written the the MSB 
              AKA : shift value by 4 
    */
    
    uint8 local_shiftValue = INTERRUPT_GROUPS - GROUP16_SUB0 ;     

    NVIC_IPR[cpy_interrupt_id] = ( (cpy_Group_Piriority<< local_shiftValue) | cpy_Sub_piriority) << 4 ; 
}