/*******************************************************/ 
/* Author: Nourhan Mansour                             */
/* Date  : 21/8/2020                                   */
/* Vesion: 1.0                                         */
/* File  : NVIC_private.h                              */
/*******************************************************/ 
#ifndef NVIC_PRIVATE_H
#define NVIC_PRIVATE_H


/************* Registers Definitions *****************/

    /************ NVIC Registers *************/
#define NVIC_ISER0     *( (volatile uint32 *) 0xE000E100)
#define NVIC_ISER1     *( (volatile uint32 *) 0xE000E104)

#define NVIC_ICER0     *( (volatile uint32 *) 0xE000E180)
#define NVIC_ICER1     *( (volatile uint32 *) 0xE000E184)

#define NVIC_ISPR0     *( (volatile uint32 *) 0xE000E200)
#define NVIC_ISPR1     *( (volatile uint32 *) 0xE000E204)

#define NVIC_ICPR0     *( (volatile uint32 *) 0xE000E280)
#define NVIC_ICPR1     *( (volatile uint32 *) 0xE000E284)

#define NVIC_IABR0     *( (volatile uint32 *) 0xE000E300)
#define NVIC_IABR1     *( (volatile uint32 *) 0xE000E304)

#define NVIC_IPR        ( (volatile uint8 *)  0xE000E400)  //Access as an array of u8s 

    /************* SCB Registers ***************/
#define SCB_CPUID      *( (volatile uint32 *) 0xE000ED00)
#define SCB_ICSR       *( (volatile uint32 *) 0xE000ED04)
#define SCB_VTOR       *( (volatile uint32 *) 0xE000ED08)
#define SCB_AIRCR      *( (volatile uint32 *) 0xE000ED0C)
#define SCB_SCR        *( (volatile uint32 *) 0xE000ED10)
#define SCB_CCR        *( (volatile uint32 *) 0xE000ED14)
#define SCB_SHPR1      *( (volatile uint32 *) 0xE000ED18)
#define SCB_SHPR2      *( (volatile uint32 *) 0xE000ED1C)
#define SCB_SHPR3      *( (volatile uint32 *) 0xE000ED20)

/*****************************************************/

/*************** Bits Config *************************/

    /********** Piriority Groups *************/
typedef enum 
{
    GROUP16_SUB0 =    0b011,
    GROUP8_SUB2  =    0b100,
    GROUP4_SUB4  =    0b101,
    GROUP2_SUB8  =    0b110,
    GROUP0_SUB16 =    0b111,

}t_Piriority_Groups;


#endif  // End Of File