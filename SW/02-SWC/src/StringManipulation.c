
#include "STD_TYPES.h"
#include "StringManipulation.h"

 void AppendIntegerToString(uint8 * string, uint32 integer) {
	while (*(string++) != '\0')
		;
	*string--;
	uint8 temp[20] = { 0 };
	uint8 * ptr = temp;

	if (integer == 0) {
		*string++ = '0';
		*string++ = '0';
	} else {

		while (integer) {
			*ptr = ((integer % 10) + '0');
			ptr++;
			integer /= 10;
		}

		if (ptr <= temp + 1) {
			*string++ = '0';
		}

		while ((ptr--) != temp) {
			*string++ = *ptr;
		}
	}
	*string = '\0';
}

 void AppendChar(uint8 * string, uint8 charToAppend)
{
	while (*(string++) != '\0')
		;
	*string--;
	*string++ = charToAppend;
	*string = '\0';
	return ; 
}

 void AppendString(uint8 * string, uint8 * stringToAppend)
{
	while (*(stringToAppend) != '\0')
	{
		AppendChar(string , *stringToAppend);
		stringToAppend++;
	}	
	return ; 
}
 void AppendFloatToString(uint8 * string, float32 floatValue)
{
	uint32 Mantissa = (uint32)(floatValue);
	uint32 exp = (uint32) ( (float32)floatValue - (Mantissa*1.0) * 100 );
	AppendIntegerToString(string , Mantissa);
	AppendChar (string , '.');
	AppendIntegerToString(string , exp);
}