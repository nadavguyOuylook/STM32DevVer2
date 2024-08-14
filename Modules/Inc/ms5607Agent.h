/*
 * ms5607Agent.h
 *
 *  Created on: Apr 26, 2024
 *      Author: user
 */

#ifndef INC_MS5607AGENT_H_
#define INC_MS5607AGENT_H_

#define P_0 1013.25

typedef struct sBAROMEMSDATA
{
		float air_pressure_out;
		float temperature_out;
		float altitude_out;
		float verticalVelocity;
} tBAROMEMSDATA;

typedef struct sBARODATA
{
		//Log written time
		//    uint32_t LastBaroMeassurementSystick;
		//    uint32_t LastPressureMeassurementSystick;
		//    uint32_t LastTemperatureMeassurementSystick;

		//Update / Data received
		tBAROMEMSDATA rawData;
		tBAROMEMSDATA filteredData;

		float LastMeassuredBaro;
		float LastMeassuredPressure;
		float LastMeassuredTemperature;

		//    float Filtered_Velocity;
		float Delta_Height_Average;
		float Start_Height_Average;
		float start_height;

		bool isNewBaroDataAvailable;
		bool needToWriteLog;
		bool isFaultySensor;

} tBARODATA;

typedef struct sMS5607Unit
{

	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	SPI_HandleTypeDef hspi;

	uint8_t SPITX[1];
	uint8_t SPIRX2Byte[2];
	uint8_t SPIRX3Byte[3];
	uint16_t Coeff1;
	uint16_t Coeff2;
	uint16_t Coeff3;
	uint16_t Coeff4;
	uint16_t Coeff5;
	uint16_t Coeff6;
	uint16_t MSCRC;
	bool isCmdSet;
	bool isPressureLastCmd;
	uint32_t LastTempMeasurement;
	uint32_t LastPressureMeasurement;
	uint32_t LastCommandSent;
	uint32_t RawTemp;
	uint32_t RawPressure;

	int32_t dT;
	int32_t TEMP;
	int64_t OFF;
	int64_t SNES;
	int32_t P;

	tBARODATA Data;
} tMS5607Unit;

extern int32_t TEMP;
extern int32_t P;

extern tMS5607Unit onBoardUnit;
extern tMS5607Unit smallBoardUnit;

extern void initMS56XXOutputStruct(tBARODATA *dataToFilter);
extern void MS56XXReset(tMS5607Unit *ms56Unit);
extern uint16_t MS56XXReadProm(tMS5607Unit *ms56Unit, uint8_t address);
extern void MS56XXInit(tMS5607Unit *ms56Unit);
extern void MS56XXSendCmd(tMS5607Unit *ms56Unit, uint8_t Cmd);
extern uint32_t MS56XXRead3Bytes(tMS5607Unit *ms56Unit, uint8_t address);
extern void MS56XXCyclicRead(tMS5607Unit *ms56Unit);
bool GetAltitudeAndTemp(tMS5607Unit *ms56Unit);
extern void altitudeFromMeasurements(tMS5607Unit *ms56Unit);
extern void ms5607ChipUnSelected(tMS5607Unit *ms56Unit);
extern void initMS56XXUnit(tMS5607Unit *unitToInitialize, GPIO_TypeDef* GPIOPortToSet, uint16_t PinToSet, SPI_HandleTypeDef hspiToSet);



#endif /* INC_MS5607AGENT_H_ */
