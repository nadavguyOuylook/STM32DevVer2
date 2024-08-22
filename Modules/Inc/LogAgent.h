/*
 * LogAgent.h
 *
 *  Created on: Apr 26, 2024
 *      Author: user
 */

#ifndef INC_LOGAGENT_H_
#define INC_LOGAGENT_H_

#include "time.h"

#define GENERALBUFFERSIZE 1024
#define TERMINALBUFFERSIZE 512

typedef enum colorsForDisplay {GREEN, BLUE, WHITE, RED, BLACK, MAGENTA, COLOROVERRIDE, ORANGE, NOCOLOR} tColorsForDisplay; //Green - if tested True, BLUE - info, RED - Error, MAGENTA - Debug

extern float free_kb;
extern float total_kb;

extern uint32_t previousLogIndex;
extern uint32_t lastFileSizeCheck;
//extern char terminalBuffer[TERMINALBUFFERSIZE];

extern void logData(bool printToScreen, bool screenOnly, tColorsForDisplay selectedColor);
extern char *resolvePointerToLogsBuffer(void);



#define MAX_LOG_SIZE (100000)
#define MAX_LOG_ID 	 (99999)

extern char logsIndexFile[35];
extern char currentLogFilename[64];
extern uint8_t FileReadBuffer[1024];

extern bool createNewLogFile(void);

extern uint32_t getCurrentLogSize(void);
extern void monitorLogSize(void);

extern void writeLogHeaders(void);
extern void deleteLogs(void);
extern void closeLogFile(void);

#endif /* INC_LOGAGENT_H_ */
