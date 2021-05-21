/*********************************************************************************/
/* Author    : Shimo	                                                         */
/* Version   : V01                                                               */
/* Date      : 				                                                     */
/*********************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "ADC1_interface.h"
#include "ADC1_private.h"
#include "ADC1_config.h"


void ADC1_voidInit(void)
{
	/* IMP: set the reg value to 0 before configuring */
	
	/* Configuring CR1 */

	/* Configuring CR2 */

	/* Configuring SMPR2 */

	/* Configuring SQR1 */

	/* Configuring SQR3 */
	
	/* Turning on ADC1 from CR2	*/

}

void ADC1_voidSetThresholds(uint16 Cpy_u16LowThreshold, uint16 Cpy_u16HighThreshold)
{
	(ADC1 -> HTR) = Cpy_u16HighThreshold;
	(ADC1 -> LTR) = Cpy_u16LowThreshold ;
}

uint16 ADC1_u16GetRegularVal(void)
{
	return (uint16)(ADC1 -> DR);
}

