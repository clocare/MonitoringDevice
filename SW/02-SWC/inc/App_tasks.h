/************************************************************************/ 
/* Author : Nourhan Mansour                            									*/
/* Date   : 27/5/2021                                  									*/
/* Version: 1.0                                        									*/
/* File   : App_tasks.h	 		                   									        */
/************************************************************************/
#ifndef APP_TASKS_H
#define APP_TASKS_H

/************************************************************************/
/*                        		Constants                                 */
/************************************************************************/
#define NEW_CARD_PRESENT_NOTIFICATION					0x01
#define CARD_DETACHED_NOTIFICATION						0x02
#define EMERGENCY_MSG_NOTIFICATION						0x04
#define CARD_IS_PRESENT_NOTIFICATION					0x08


/* Task priorities. */
#define Observer_PRIORITY							( tskIDLE_PRIORITY + 2 )
#define SPO2_PRIORITY									( tskIDLE_PRIORITY + 3 )
#define Reader_PRIORITY								( tskIDLE_PRIORITY + 2 )
#define Display_PRIORITY							( tskIDLE_PRIORITY + 2 )
#define GateWay_PRIORITY							( tskIDLE_PRIORITY + 3 )
#define Emergency_PRIORITY						( tskIDLE_PRIORITY + 2 )

/* Task Stack Size */
#define OBSERVER_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 1 )
#define SPO2_TASK_STACK_SIZE					( configMINIMAL_STACK_SIZE + 1 )
#define READER_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE + 1 )
#define DISPLAY_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE + 2 )
#define GATEWAY_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE + 0 )
#define EMERGENCY_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 0 )

/************************************************************************/
/*													APIS																				*/
/************************************************************************/
void Observer_mainTask(void * pvParameters);

void Reader_MainTask(void * pvParameters);

void Display_mainTask(void * pvParameters);

void GateWay_mainTask(void * pvParameters);

void Emergency_mainTask(void * pvParameters);

void SPO2_Algorithm_mainTask(void * pvParameters);

#endif
