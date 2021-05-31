/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 30/5/2021                                  */
/* Version: 1.0                                        */
/* File   : StrManipulation.h			               */
/*******************************************************/ 
#ifndef     STR_MANIPULATION_H
#define     STR_MANIPULATION_H

void AppendIntegerToString(uint8 * string, uint32 integer);
void AppendFloatToString(uint8 * string, float32 floatValue);
void AppendChar(uint8 * string, uint8 charToAppend);
void AppendString(uint8 * string, uint8 * stringToAppend);

#endif
