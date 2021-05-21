/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#ifndef TEMP_SENSOR_CONFIG_H
#define TEMP_SENSOR_CONFIG_H


/* options:  			INPUT_ANALOG
 						INPUT_FLOATING				
 						INPUT_PULL_UP_DOWN */
#define INPUT_TYPE		INPUT_ANALOG


/* sensor PORT	*/
/* Options :	GPIOA
 	 	 	 	GPIOB
 	 	 	 	GPIOC   */
#define TEMP_SENSOR_PORT 	GPIOA

/* sensor PIN			  */
/* Options :	PIN0 -> PIN15 */
#define TEMP_SENSOR_PIN  	PIN0

#define TEMP_SAMPLES		10



#endif 
