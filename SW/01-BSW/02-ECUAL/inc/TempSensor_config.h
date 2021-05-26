/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#ifndef TEMP_SENSOR_CONFIG_H
#define TEMP_SENSOR_CONFIG_H


/* options:  	INPUT_ANALOG
							INPUT_FLOATING				
							INPUT_PULL_UP_DOWN 
*/
#define INPUT_TYPE		INPUT_ANALOG

/* write in for GPIOx, PINx */
#define TEMP_SENSOR_PORT 	PORTB 
#define TEMP_SENSOR_PIN		PIN0

#define TEMP_SAMPLES		10

/* the value of the voltage devidor resistor */
#define SERIESRESISTOR 10000

/* resistance at 25 degrees C */
#define THERMISTORNOMINAL 10000

/* temp. for nominal resistance (almost always 25 C) */
#define TEMPERATURENOMINAL 25

/* The beta coefficient of the thermistor (usually 3000-4000) */
#define BCOEFFICIENT 3950

#endif 
