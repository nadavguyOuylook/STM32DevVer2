/*
 * ms5607Agent.c
 *
 *  Created on: Apr 26, 2024
 *      Author: user
 */

#include "main.h"
#include "spi.h"

uint8_t SPITX[1] = {0};
uint8_t SPIRX2Byte[2] = {0};
uint8_t SPIRX3Byte[3] = {0};
uint16_t Coeff1 = 0;
uint16_t Coeff2 = 0;
uint16_t Coeff3 = 0;
uint16_t Coeff4 = 0;
uint16_t Coeff5 = 0;
uint16_t Coeff6 = 0;
uint16_t MSCRC = 0;
bool isCmdSet = false;
bool isPressureLastCmd = false;
uint32_t LastTempMeasurement = 0;
uint32_t LastPressureMeasurement = 0;
uint32_t LastCommandSent = 0;
uint32_t RawTemp = 0;
uint32_t RawPressure = 0;

int32_t dT = 0;
int32_t TEMP = 0;
int64_t OFF = 0;
int64_t SNES = 0;
int32_t P = 0;

tBARODATA *pointerToDataToFilter = {0};

tMS5607Unit onBoardUnit = {0};
tMS5607Unit smallBoardUnit = {0};

void initMS56XXOutputStruct(tBARODATA *dataToFilter)
{
	pointerToDataToFilter = dataToFilter;
}

void initMS56XXUnit(tMS5607Unit *unitToInitialize, GPIO_TypeDef* GPIOPortToSet, uint16_t PinToSet, SPI_HandleTypeDef hspiToSet)
{

	unitToInitialize->GPIO_Pin = PinToSet;
	unitToInitialize->GPIOx = GPIOPortToSet;
	unitToInitialize->hspi = hspiToSet;
}

void ms5607ChipSelected(tMS5607Unit *ms56Unit)
{
	HAL_GPIO_WritePin(ms56Unit->GPIOx, ms56Unit->GPIO_Pin, GPIO_PIN_RESET);
}

void ms5607ChipUnSelected(tMS5607Unit *ms56Unit)
{
	HAL_GPIO_WritePin(ms56Unit->GPIOx, ms56Unit->GPIO_Pin, GPIO_PIN_SET);
}

void MS56XXReset(tMS5607Unit *ms56Unit)
{
	ms5607ChipSelected(ms56Unit);
	ms56Unit->SPITX[0] = 0x1E;
	HAL_SPI_Transmit(&ms56Unit->hspi, &ms56Unit->SPITX[0], 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&ms56Unit->hspi) == HAL_SPI_STATE_BUSY);
	ms5607ChipUnSelected(ms56Unit);
}

uint16_t MS56XXReadProm(tMS5607Unit *ms56Unit, uint8_t address)
{
	ms56Unit->SPITX[0] = address;
	ms5607ChipSelected(ms56Unit);
	HAL_SPI_Transmit(&ms56Unit->hspi, &ms56Unit->SPITX[0], 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&ms56Unit->hspi) == HAL_SPI_STATE_BUSY);
	HAL_SPI_Receive(&ms56Unit->hspi, &ms56Unit->SPIRX2Byte[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&ms56Unit->hspi, &ms56Unit->SPIRX2Byte[1], 1, HAL_MAX_DELAY);
	ms5607ChipUnSelected(ms56Unit);
	return ((ms56Unit->SPIRX2Byte[0]<<8) + ms56Unit->SPIRX2Byte[1]);
}

void MS56XXInit(tMS5607Unit *ms56Unit)
{
	MS56XXReset(ms56Unit);
	HAL_Delay(50);
	MS56XXReadProm(ms56Unit, 0xA0);
	ms56Unit->Coeff1 = MS56XXReadProm(ms56Unit, 0xA2);
	ms56Unit->Coeff2 = MS56XXReadProm(ms56Unit, 0xA4);
	ms56Unit->Coeff3 = MS56XXReadProm(ms56Unit, 0xA6);
	ms56Unit->Coeff4 = MS56XXReadProm(ms56Unit, 0xA8);
	ms56Unit->Coeff5 = MS56XXReadProm(ms56Unit, 0xAA);
	ms56Unit->Coeff6 = MS56XXReadProm(ms56Unit, 0xAC);
	ms56Unit->MSCRC = MS56XXReadProm(ms56Unit, 0xAE);

	MS56XXSendCmd(ms56Unit, 0x58);
	HAL_Delay(9);
	ms56Unit->RawTemp = MS56XXRead3Bytes(ms56Unit, 0);

	MS56XXSendCmd(ms56Unit, 0x48);
	HAL_Delay(9);
	ms56Unit->RawPressure = MS56XXRead3Bytes(ms56Unit, 0);
	GetAltitudeAndTemp(ms56Unit);
}

void MS56XXSendCmd(tMS5607Unit *ms56Unit, uint8_t Cmd)
{
	ms5607ChipSelected(ms56Unit);
	ms56Unit->SPITX[0] = Cmd;
	HAL_SPI_Transmit(&ms56Unit->hspi, &ms56Unit->SPITX[0], 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&ms56Unit->hspi) == HAL_SPI_STATE_BUSY);
	ms5607ChipUnSelected(ms56Unit);
	isCmdSet = true;
}

uint32_t MS56XXRead3Bytes(tMS5607Unit *ms56Unit, uint8_t address)
{
	ms56Unit->SPITX[0] = 0x00;
	ms5607ChipSelected(ms56Unit);
	HAL_SPI_Transmit(&ms56Unit->hspi, &ms56Unit->SPITX[0], 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(&ms56Unit->hspi) == HAL_SPI_STATE_BUSY);
	HAL_SPI_Receive(&ms56Unit->hspi, &ms56Unit->SPIRX3Byte[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&ms56Unit->hspi, &ms56Unit->SPIRX3Byte[1], 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&ms56Unit->hspi, &ms56Unit->SPIRX3Byte[2], 1, HAL_MAX_DELAY);
	ms5607ChipUnSelected(ms56Unit);
	ms56Unit->isCmdSet = false;
	return ((ms56Unit->SPIRX3Byte[0] << 16) + (ms56Unit->SPIRX3Byte[1] << 8 ) + (ms56Unit->SPIRX3Byte[0]));
}

bool GetAltitudeAndTemp(tMS5607Unit *ms56Unit)
{
	ms56Unit->dT = (double)ms56Unit->RawTemp - (double)ms56Unit->Coeff5 * 256.0;
	ms56Unit->TEMP = 2000.0 + ms56Unit->dT * (double)ms56Unit->Coeff6 / 8388608.0;
	ms56Unit->OFF = (double)ms56Unit->Coeff2 * 131072.0 + (double)ms56Unit->Coeff4 * (double)ms56Unit->dT / 64.0;
	ms56Unit->SNES = (double)ms56Unit->Coeff1 * 65536.0 + ((double)ms56Unit->Coeff3 * (double)ms56Unit->dT) / 128.0;
	ms56Unit->P = (((double)ms56Unit->RawPressure * (double)ms56Unit->SNES) / 2097152.0 - (double)ms56Unit->OFF) / 32768.0;

	if ( (!isFloatEquals(ms56Unit->Data.rawData.temperature_out, ms56Unit->TEMP, 0.01) )
			|| (!isFloatEquals(ms56Unit->Data.rawData.air_pressure_out, ms56Unit->P * 100 / 100.0, 0.01)) )
	{
		ms56Unit->Data.rawData.temperature_out = ms56Unit->TEMP;
		ms56Unit->Data.rawData.air_pressure_out = ms56Unit->P / 100.0;
		return (true);
	}
	return (false);
}

void MS56XXCyclicRead(tMS5607Unit *ms56Unit)
{
	if ( (HAL_GetTick() - ms56Unit->LastTempMeasurement) > 1000 )
	{
		if (!ms56Unit->isCmdSet)
		{
			MS56XXSendCmd(ms56Unit, 0x58);
			ms56Unit->isCmdSet = true;
			ms56Unit->Data.isNewBaroDataAvailable = false;
			ms56Unit->isPressureLastCmd = false;
			ms56Unit->LastCommandSent = HAL_GetTick();
		}
	}

	if ((HAL_GetTick() - ms56Unit->LastPressureMeasurement) > 20)
	{
		if (!ms56Unit->isCmdSet)
		{
			MS56XXSendCmd(ms56Unit, 0x48);
			ms56Unit->isCmdSet = true;
			ms56Unit->Data.isNewBaroDataAvailable = false;
			ms56Unit->isPressureLastCmd = true;
			ms56Unit->LastCommandSent = HAL_GetTick();
		}
	}

	if ( (HAL_GetTick() - ms56Unit->LastCommandSent) > 9 )
	{
		if (ms56Unit->isCmdSet)
		{
			if (!ms56Unit->isPressureLastCmd)
			{
				ms56Unit->RawTemp = MS56XXRead3Bytes(ms56Unit, 0);
				ms56Unit->LastTempMeasurement = HAL_GetTick();
			}
			else
			{
				ms56Unit->RawPressure = MS56XXRead3Bytes(ms56Unit, 0);
				ms56Unit->LastPressureMeasurement = HAL_GetTick();
			}
			ms56Unit->isCmdSet = false;
			ms56Unit->Data.isNewBaroDataAvailable = GetAltitudeAndTemp(ms56Unit);
		}
	}
}

void altitudeFromMeasurements(tMS5607Unit *ms56Unit)
{

	ms56Unit->Data.filteredData.altitude_out = (((float)pow(((float)P_0 / (float)ms56Unit->Data.filteredData.air_pressure_out), (float)1.0f / 5.257f) - 1.0f) * ((float)(ms56Unit->Data.filteredData.temperature_out / 100.0f) + 273.15f)) / 0.0065f;

}
