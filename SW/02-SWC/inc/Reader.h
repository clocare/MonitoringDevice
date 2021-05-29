/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 21/5/2021                                  */
/* Version: 1.0                                        */
/* File   : Reader.h				                           */
/*******************************************************/ 

#ifndef READER_H
#define READER_H

/************************************************************************/
/*                         Global constants                             */
/************************************************************************/
#define ID_WRITE_LEN		16U			// national id len 
#define ID_READ_LEN			18U
#define ID_BLOCK				04U			// Block in tag memory

/************************************************************************/
/*                          Type definitions                            */
/************************************************************************/
typedef struct nationalID_Type
{
	uint8 id1[ID_WRITE_LEN];		// last byte is 0
	uint8 id2[ID_READ_LEN];			// last byte is 0
}nationalID_Type;	

/************************************************************************/
/*                        Functions APIs	                              */
/************************************************************************/

void Reader_init(void);

boolean Reader_isNewCardPresent(void);

boolean Reader_SetId(nationalID_Type Id);

nationalID_Type Reader_GetId (void);


#endif
