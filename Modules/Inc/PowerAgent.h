/*
 * PowerAgent.h
 *
 *  Created on: Aug 19, 2024
 *      Author: user
 */

#ifndef INC_POWERAGENT_H_
#define INC_POWERAGENT_H_

typedef struct sPowerSorces
{
	bool isUSBConnected;
	bool isBatteryConnected;
	bool isChargingEnabled;
	float batteryVoltageWhileCharging;
	float batteryVoltageWithoutCharging;
}tPowerSources;

extern float measuredVoltages[4];
extern tPowerSources boardPowerSources;

extern void checkPowerSourcesConnection(void);
extern void chargeProcess(void);
extern void readADCValues(void);

#endif /* INC_POWERAGENT_H_ */
