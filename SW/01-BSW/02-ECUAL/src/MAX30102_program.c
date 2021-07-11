/*
 * MAX30102_program.c
 *
 *  Created on: Apr 25, 2021
 *      Author: Omar
 */

#include "BIT_MATH.h"
#include "STD_TYPES.h"

#include "I2C_interface.h"
#include "TIMER_interface.h"

#include "MAX30102_interface.h"
#include "MAX30102_config.h"
#include "MAX30102_private.h"

#define TIMER_MUL				6
#define MAX_BUFFER		260		// will read at most 32 bytes at a time.
static uint8 activeLEDs;
sense_struct sense;

/*
 * Pseudo-Code Example of Reading Data from FIFO
 * First transaction: Get the FIFO_WR_PTR:
 * START;
 * Send device address + write mode
 * Send address of FIFO_WR_PTR;
 * REPEATED_START;
 * Send device address + read mode
 * Read FIFO_WR_PTR;
 * STOP;
 */

static void Fill_IR_Red(uint8 * Readings, uint32 * IR, uint32 * Red) {
	uint8 IR_Counter = 0;
	uint8 Red_Counter = 0;
	for (uint8 i = 0; i < 192; i++) {
		if ((i % 3) == (i % 6)) {
			Red[Red_Counter] |= (Readings[i] << ((2 - (i % 3)) * 8));
			//Red[Red_Counter] = Readings[i];
			if (i == 0)
				continue;
			if ((i + 1) % 3 == 0)
				Red_Counter++;

		} else {
			IR[IR_Counter] |= (Readings[i] << ((2 - (i % 3)) * 8));
			//IR[IR_Counter] = Readings[i];
			if (i == 0)
				continue;
			if ((i + 1) % 3 == 0)
				IR_Counter++;
		}
	}
}

//Given a register, read it, mask it, and then set the thing
static void bitMask(uint8 reg, uint8 mask, uint8 thing) {
	// Grab current register context
	uint8 originalContents = MAX30102_u8ReadRegister(reg);

	// Zero-out the portions of the register we're interested in
	originalContents = originalContents & mask;

	// Change contents
	MAX30102_voidWriteRegister(reg, originalContents | thing);
}

/********************************************************************************/
/*							APIS												*/
/********************************************************************************/
sint32 MAX30102_u8GetSPO2Value(void)
{
	sint32 spo2=0;
		// do something
	return spo2; 
}

sint32 MAX30102_u8GetHRValue(void)
{
	sint32 HR = 0 ;
	
	return HR; 
}

void MAX30102_shutDown(void) {
	// Put IC into low power mode (datasheet pg. 19)
	// During shutdown the IC will continue to respond to I2C commands but will
	// not update with or take new readings (such as temperature)
	bitMask(MAX30102_MODE_CONFIG, MAX30102_SHUTDOWN_MASK, MAX30102_SHUTDOWN);
}

void MAX30102_wakeUp(void) {
	// Pull IC out of low power mode (datasheet pg. 19)
	bitMask(MAX30102_MODE_CONFIG, MAX30102_SHUTDOWN_MASK, MAX30102_WAKEUP);
}
//Read the FIFO Write Pointer
uint8 MAX30102_getWritePointer(void) {
	return (MAX30102_u8ReadRegister( MAX30102_FIFO_WRIT_EPTR));
}

//Read the FIFO Read Pointer
uint8 MAX30102_getReadPointer(void) {
	return (MAX30102_u8ReadRegister( MAX30102_FIFO_READ_PTR));
}

void MAX30102_clearFIFO(void) {
	MAX30102_voidWriteRegister( MAX30102_FIFO_WRIT_EPTR, 0);
	MAX30102_voidWriteRegister( MAX30102_FIFO_OVER_FLOW, 0);
	MAX30102_voidWriteRegister( MAX30102_FIFO_READ_PTR, 0);
}

void MAX30102_voidWriteRegister(uint8 Cpy_u8Register, uint8 Cpy_u8Value) {

	uint8 Temp_pu8Buffer[2] = { Cpy_u8Register, Cpy_u8Value };
	I2C_voidMasterSendData(I2C1, Temp_pu8Buffer, 2, MAX30102_SLAVE_ADDRESS,
	I2C_NO_REPEAT_S);

}

void MAX30102_voidWriteSeqRegisters(uint8 Cpy_u8StartRegister, uint8 * p_u8Values,
		uint8 Cpy_u8NoOfVals) {
	uint8 Temp_pu8Buffer[10];
	Temp_pu8Buffer[0] = Cpy_u8StartRegister;
	for (uint8 i = 0; i < Cpy_u8NoOfVals; i++) {
		Temp_pu8Buffer[i + 1] = p_u8Values[i];
	}
	I2C_voidMasterSendData(I2C1, Temp_pu8Buffer, (Cpy_u8NoOfVals + 1), MAX30102_SLAVE_ADDRESS, I2C_NO_REPEAT_S);

}

uint8 MAX30102_u8ReadRegister(uint8 Cpy_u8Register) {
	uint8 Temp_u8RegVal;
	I2C_voidMasterSendData(I2C1, &Cpy_u8Register, 1, MAX30102_SLAVE_ADDRESS,
	I2C_REPEAT_S);

	I2C_voidMasterReceiveData(I2C1, &Temp_u8RegVal, 1, MAX30102_SLAVE_ADDRESS);

	return Temp_u8RegVal;
}

void MAX30102_voidReadSeqRegisters(uint8 Cpy_u8StartRegister, uint8 * p_u8Values,
		uint8 Cpy_u8NoOfVals) {

	I2C_voidMasterSendData(I2C1, &Cpy_u8StartRegister, 1,
	MAX30102_SLAVE_ADDRESS, I2C_REPEAT_S);

	I2C_voidMasterReceiveData(I2C1, p_u8Values, Cpy_u8NoOfVals,
	MAX30102_SLAVE_ADDRESS);

}

void MAX30102_voidGetReadings(uint32 * p_u8IR_RxBuffer, uint32 * p_u8Red_RxBuffer) {
	uint8 Readings[192];

	MAX30102_voidWriteRegister(MAX30102_FIFO_READ_PTR, 0);

	MAX30102_voidReadSeqRegisters(MAX30102_FIFO_DATA, Readings, 192);
	Fill_IR_Red(Readings, p_u8IR_RxBuffer, p_u8Red_RxBuffer);

}

void MAX30102_voidInit(void)
{
	uint8 Temp_u8RegVal[3];
	Temp_u8RegVal[0] = (MAX30102_FIFO_ROLLOVER_ENABLE_STATE << MAX30102_FIFO_CONFIG_FIFO_ROLLOVER_EN)
			| ((0x07 & MAX30102_FIFO_SAMPLING_AVERAGE) << MAX30102_FIFO_CONFIG_SMP_AVE);
	Temp_u8RegVal[1] = (MAX30102_MODE_REDIRONLY & MAX30102_MODE_CONTROL)<< MAX30102_MODE_CONFIG_MODE;
	Temp_u8RegVal[2] = ((0x3 & MAX30102_LED_PULSE_WIDTH)<< MAX30102_SPO2_CONFIG_LED_PW)
			| ((0x7 & MAX30102_SPO2_SAMPLING_RATE) << MAX30102_SPO2_CONFIG_SPO2_SR)
			| ((0x3 & MAX30102_ADC_FULL_SCALE) << MAX30102_SPO2_CONFIG_SPO2_ADC_RGE);

	MAX30102_voidWriteSeqRegisters(MAX30102_FIFO_CONFIG, Temp_u8RegVal, 3);

	Temp_u8RegVal[0] = MAX30102_LED1_PULSE_AMPLITUDE;
	Temp_u8RegVal[1] = MAX30102_LED2_PULSE_AMPLITUDE;

	MAX30102_voidWriteSeqRegisters(MAX30102_LED1_PULSE_AMP, Temp_u8RegVal, 2);
	Temp_u8RegVal[0] = ((0x3 & MAX30102_SLOT1)<< MAX30102_MULTILED_CONFIG1__SLOT1)
						| ((0x7 & MAX30102_SLOT2) << MAX30102_MULTILED_CONFIG1__SLOT2);


	MAX30102_voidWriteRegister(MAX30102_MULTILED_CONFIG1, Temp_u8RegVal[0]);
	MAX30102_voidReadSeqRegisters(MAX30102_FIFO_CONFIG, Temp_u8RegVal, 3);
	MAX30102_voidReadSeqRegisters(MAX30102_LED1_PULSE_AMP, Temp_u8RegVal, 2);

	Temp_u8RegVal[0] = MAX30102_u8ReadRegister(MAX30102_MULTILED_CONFIG1);

	activeLEDs = 2; 	// multi mode selected;
}

// Die Temperature
// Returns temp in C
float MAX30102_readTemperature() {

	//DIE_TEMP_RDY interrupt must be enabled
	//See issue 19: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/issues/19

	// Step 1: Config die temperature register to take 1 temperature sample
	MAX30102_voidWriteRegister( MAX30102_DIE_TEMP_CONFIG, 0x01);

	// Poll for bit to clear, reading is then complete
	// Timeout after 100ms
	unsigned long startTime = 0;
	while (startTime < 100) {
		//uint8_t response = readRegister8(_i2caddr, MAX30105_DIETEMPCONFIG); //Original way
		//if ((response & 0x01) == 0) break; //We're done!

		//Check to see if DIE_TEMP_RDY interrupt is set
		uint8 response = MAX30102_u8ReadRegister( MAX30102_INT_STAT2);
		if ((response & MAX30102_INT_DIE_TEMP_RDY_ENABLE) > 0)
			break; //We're done!
		TIMER_voidSetBusyWait(TIM3,TIMER_MUL*1000); //Let's not over burden the I2C bus 1ms delay
		startTime++;
	}
	//TODO How do we want to fail? With what type of error?
	if (startTime >= 100)
		return (-1.0);

	// Step 2: Read die temperature register (integer)
	sint8 tempInt = MAX30102_u8ReadRegister( MAX30102_DIE_TEMP_INT);
	uint8 tempFrac = MAX30102_u8ReadRegister( MAX30102_DIE_TEMP_FRAC); //Causes the clearing of the DIE_TEMP_RDY interrupt

	// Step 3: Calculate temperature (datasheet pg. 23)
	return (float) tempInt + ((float) tempFrac * 0.0625);
}

//
// Data Collection
//

//Tell caller how many samples are available
uint8 MAX30102_available(void) {
	sint8 numberOfSamples = sense.head - sense.tail;
	if (numberOfSamples < 0)
		numberOfSamples += STORAGE_SIZE;

	return (numberOfSamples);
}

//Report the most recent red value
uint32 MAX30102_getRed(void) {
	//Check the sensor for new data for 250ms
	if ( MAX30102_safeCheck(250))
		return (sense.red[sense.head]);
	else
		return (0); //Sensor failed to find new data
}

//Report the most recent IR value
uint32 MAX30102_getIR(void) {
	//Check the sensor for new data for 250ms
	if (MAX30102_safeCheck(250))
		return (sense.IR[sense.head]);
	else
		return (0); //Sensor failed to find new data
}



//Report the next Red value in the FIFO
uint32 MAX30102_getFIFORed(void) {
	return (sense.red[sense.tail]);
}

//Report the next IR value in the FIFO
uint32 MAX30102_getFIFOIR(void) {
	return (sense.IR[sense.tail]);
}


//Advance the tail
void MAX30102_nextSample(void) {
	if ( MAX30102_available()) //Only advance the tail if new data is available
	{
		sense.tail++;
		sense.tail %= STORAGE_SIZE; //Wrap condition
	}
}

//Polls the sensor for new data
//Call regularly
//If new data is available, it updates the head and tail in the main struct
//Returns number of new samples obtained
uint16 MAX30102_check(void) {
	//Read register FIDO_DATA in (3-byte * number of active LED) chunks
	//Until FIFO_RD_PTR = FIFO_WR_PTR

	uint8 readPointer = MAX30102_getReadPointer();
	uint8 writePointer = MAX30102_getWritePointer();

	sint16 numberOfSamples = 0;

	//Do we have new data?
	if (readPointer != writePointer) {
		//Calculate the number of readings we need to get from sensor
		numberOfSamples = writePointer - readPointer;
		if (numberOfSamples < 0)
			numberOfSamples += 32; //Wrap condition

		//We now have the number of readings, now calc bytes to read
		//For this example we are just doing Red and IR (3 bytes each)
		sint16 bytesLeftToRead = numberOfSamples * activeLEDs * 3;

		//Get ready to read a burst of data from the FIFO register
		uint8 reg = MAX30102_FIFO_DATA;
		I2C_voidMasterSendData(I2C1, &reg , 1, MAX30102_SLAVE_ADDRESS, I2C_NO_REPEAT_S);

		//We may need to read as many as 288 bytes so we read in blocks no larger than I2C_BUFFER_LENGTH
		//I2C_BUFFER_LENGTH changes based on the platform. 64 bytes for SAMD21, 32 bytes for Uno.
		//Wire.requestFrom() is limited to BUFFER_LENGTH which is 32 on the Uno
		while (bytesLeftToRead > 0) {
			sint16 toGet = bytesLeftToRead;
			if (toGet > I2C_BUFFER_LENGTH) {
				//If toGet is 32 this is bad because we read 6 bytes (Red+IR * 3 = 6) at a time
				//32 % 6 = 2 left over. We don't want to request 32 bytes, we want to request 30.
				//32 % 9 (Red+IR+GREEN) = 5 left over. We want to request 27.

				//Trim toGet to be a multiple of the samples we need to read
				toGet = I2C_BUFFER_LENGTH - (I2C_BUFFER_LENGTH % (activeLEDs * 3));
			}

			bytesLeftToRead -= toGet;

			//Request toGet number of bytes from sensor
			// _i2cPort->requestFrom(MAX30102_ADDRESS, toGet);
			uint8 recBuffer [MAX_BUFFER];
			uint8 toGetIdx = 0 ;
			I2C_voidMasterReceiveData(I2C1, recBuffer, toGet , MAX30102_SLAVE_ADDRESS);

			while (toGet > 0) {
				sense.head++; //Advance the head of the storage struct
				sense.head %= STORAGE_SIZE; //Wrap condition

				uint8 temp[sizeof(uint32)] = {0}; //Array of 4 bytes that we will convert into long
				uint32 tempLong;

				//Burst read three bytes - RED

				 temp[3] = 0;
				 temp[2] = recBuffer[toGetIdx++];
				 temp[1] = recBuffer[toGetIdx++];
				 temp[0] = recBuffer[toGetIdx++];

				//I2C_voidMasterReceiveData(I2C1, temp, 3 , MAX30102_SLAVE_ADDRESS);

				//Convert array to long
				//memcpy(&tempLong, temp, sizeof(tempLong));
				tempLong = (uint32) (temp[0] << 0) | (uint32) (temp[1] << 8) | (uint32) (temp[2] << 16);
				tempLong &= 0x3FFFF; 				//Zero out all but 18 bits
				sense.red[sense.head] = tempLong; 	//Store this reading into the sense array

				if (activeLEDs > 1) {

				//	I2C_voidMasterReceiveData(I2C1, temp, 3 , MAX30102_SLAVE_ADDRESS);
					 temp[3] = 0;
					 temp[2] = recBuffer[toGetIdx++];
					 temp[1] = recBuffer[toGetIdx++];
					 temp[0] = recBuffer[toGetIdx++];
					//Convert array to long
					//memcpy(&tempLong, temp, sizeof(tempLong));
					tempLong = (uint32) (temp[0] << 0) | (uint32) (temp[1] << 8) | (uint32) (temp[2] << 16);
					tempLong &= 0x3FFFF; //Zero out all but 18 bits
					sense.IR[sense.head] = tempLong;
				}

				toGet -= activeLEDs * 3;
			}

		} //End while (bytesLeftToRead > 0)

	} //End readPtr != writePtr

	return (numberOfSamples); //Let the world know how much new data we found
}

//Check for new data but give up after a certain amount of time
//Returns true if new data was found
//Returns false if new data was not found
boolean MAX30102_safeCheck(uint8 maxTimeToCheck)
{
	uint32 markTime = 0;

	while(1)
	{
		if(markTime > maxTimeToCheck)
			return(FALSE);

		if(MAX30102_check()>0) //We found new data!
			return(TRUE);

		TIMER_voidSetBusyWait(TIM3,TIMER_MUL*1000);
		markTime++;
	}
}
