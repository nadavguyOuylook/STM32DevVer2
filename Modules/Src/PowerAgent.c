/*
 * PowerAgent.c
 *
 *  Created on: Aug 19, 2024
 *      Author: user
 */
#include "stm32f7xx_hal.h"
#include "stdint.h"
#include "adc.h"
#include "PowerAgent.h"
#include "usbd_def.h"
#include "usbd_conf.h"
#include "usb_device.h"

float measuredVoltages[4] = {0};
tPowerSources boardPowerSources = {0};
void checkPowerSourcesConnection(void)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET)
	{
		// USB cable is connected
		boardPowerSources.isUSBConnected = true;
	}
	else
	{
		// USB cable is not connected
		boardPowerSources.isUSBConnected = false;
	}

	if (!boardPowerSources.isChargingEnabled)
	{
		if (measuredVoltages[0] >= 3)
		{
			boardPowerSources.isBatteryConnected = true;
		}
		else
		{
			boardPowerSources.isBatteryConnected = false;
		}
	}
}

void chargeProcess(void)
{
	if ( (boardPowerSources.isUSBConnected) && (boardPowerSources.batteryVoltageWithoutCharging <= 3.9) && (boardPowerSources.isBatteryConnected) && (!boardPowerSources.isChargingEnabled) )
	{
		//Start Charging
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
		boardPowerSources.isChargingEnabled = true;
	}
//	else if ((boardPowerSources.isUSBConnected) && (boardPowerSources.batteryVoltageWhileCharging >= 4.15) && (boardPowerSources.isBatteryConnected) && (boardPowerSources.isChargingEnabled))
//	{
//		//Stop Charging
//		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
//	}
	if (boardPowerSources.isChargingEnabled)
	{
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
			boardPowerSources.isChargingEnabled = false;
		}
		else
		{
			boardPowerSources.isChargingEnabled = true;
		}
	}
}

void readADCValues(void)
{
	HAL_ADC_Start(&hadc1);
	adcValues[0] = HAL_ADC_GetValue(&hadc1);
	measuredVoltages[0] = adcValues[0] * 2 * 3.3 / 4096.0;

	if (boardPowerSources.isChargingEnabled)
	{
		boardPowerSources.batteryVoltageWhileCharging = measuredVoltages[0];
	}
	else
	{
		boardPowerSources.batteryVoltageWithoutCharging = measuredVoltages[0];
	}


}
