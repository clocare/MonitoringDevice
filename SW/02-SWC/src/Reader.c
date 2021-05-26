
// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// 2- include interface file of needed lower layers
#include "mfrc522.h"

// 3- include interface files
#include "Reader.h"
#include "Observer.h"

MIFARE_Key key;


/************************************************************************/
/*														Local Functions														*/
/************************************************************************/
static boolean Reader_AuthCard(uint8 block);
static boolean Reader_WriteID ( nationalID_Type ID);
static boolean Reader_ReadID (nationalID_Type * ID);
/************************************************************************/
/*                        APIs Definitions                              */
/************************************************************************/

/******************************************
 * @name : Reader_init
 * @param:  
 * Non Reentrant
 * Sync
 * Service to init Reader.
 ******************************************/
void Reader_init(void)
{
	// initialize RFID module
	PCD_Init();
	// initalize keys 
	uint8 i;
	for ( i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

	return;
}

/******************************************
 * @name : Reader_isNewCardPresent
 * @param:  
 * Non Reentrant
 * Sync
 * Service to query RFID reader.
 ******************************************/
boolean Reader_isNewCardPresent(void)
{
	boolean ret = FALSE; 
	if (PICC_IsNewCardPresent())
	{	
		PICC_ReadCardSerial();
		ret = TRUE; 
	}
	return ret; 
}

/******************************************
 * @name : Reader_SetId
 * @param: Id : new ID to set on card
 * Non Reentrant
 * Sync
 * Service to set new ID in card.
 ******************************************/
boolean Reader_SetId(nationalID_Type Id)
{
	volatile boolean ret = FALSE; 
	if (Reader_AuthCard(ID_BLOCK) == TRUE)
	{
		ret = Reader_WriteID(Id);
	}
	return ret;
}

 /******************************************
 * @name : Reader_GetId
 * @param:  
 * @return: Id read from card if success or zero otherwise. 
 * Non Reentrant
 * Sync
 * Service to read card ID. 
 ******************************************/
nationalID_Type Reader_GetId (void)
{
	nationalID_Type ID ;
	volatile nationalID_Type ret_ID = {0};
	uint8 i = 0 ;
	if (Reader_AuthCard(ID_BLOCK) == TRUE)
	{
		if (Reader_ReadID(&ID) == TRUE)
		{
			for (i=0 ; i < ID_READ_LEN ; i++)
				ret_ID.id2[i] = ID.id2[i]; 
		}
	}
	__asm("NOP");
	return ret_ID; 
}

	/******************************************
 * @name : Reader_MainTask
 * @param:
 * @return:
 * Non Reentrant
 * Sync
 * Main periodic task.
 ******************************************/
void Reader_MainTask(void)
{
	while(1);
	return ; 
}
	
/************************************************************************/
/*									Local Functions	definitions													*/
/************************************************************************/
static boolean Reader_AuthCard(uint8 block)
{
	volatile boolean ret = FALSE; 
	Uid uid = PCD_getUid();
	if (PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A , block  ,  &key ,  &uid) == mfrc522_STATUS_OK)
	{
		ret = TRUE;
	}
	return ret; 
}
static boolean Reader_WriteID (nationalID_Type ID)
{
	boolean ret = FALSE; 
	if (PICC_WriteBlock(ID_BLOCK, ID.id1 , &key) == mfrc522_STATUS_OK)
	{
		ret = TRUE;
	}
	return ret; 
}

static boolean Reader_ReadID (nationalID_Type * ID)
{
	volatile boolean ret = FALSE ;
	volatile int status = PICC_ReadBlock(ID_BLOCK, ID->id2  , &key ); 
	if ( status == mfrc522_STATUS_OK)
	{
		ret = TRUE; 
	}
	return ret; 
}

