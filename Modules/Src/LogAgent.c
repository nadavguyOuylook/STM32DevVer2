/*
 * LogAgent.c
 *
 *  Created on: Apr 26, 2024
 *      Author: user
 */


#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "time.h"
#include "LogAgent.h"
#include "usbd_cdc_if.h"
#include "fatfs.h"

char *pointerToBuffer;

char terminalBuffer[TERMINALBUFFERSIZE] = {0};

float free_kb = 0;
float total_kb = 0;

char logsIndexFile[35];
char currentLogFilename[64];
uint8_t FileReadBuffer[1024];
unsigned int BytesWritten = 0;

uint32_t previousLogIndex;
uint32_t lastFileSizeCheck;

void logData(bool printToScreen, bool screenOnly, tColorsForDisplay selectedColor)
{
	static char localBuffer[GENERALBUFFERSIZE] = {0};
	static char screen_buffer[GENERALBUFFERSIZE] = {0};

	memset(localBuffer, 0, GENERALBUFFERSIZE);
	memset(screen_buffer, 0, GENERALBUFFERSIZE);

	uint16_t localStringLength = 0;

	localStringLength = fmin(sizeof(terminalBuffer) * 1.0, strlen(terminalBuffer) * 1.0);
	memcpy(localBuffer, terminalBuffer, localStringLength);

	if (strlen(localBuffer) > 0)
	{
		if (printToScreen)
		{
			uint16_t localLengthToPrint = fmin((float)GENERALBUFFERSIZE, strlen(localBuffer));
			memset(screen_buffer, 0, GENERALBUFFERSIZE);
			if (selectedColor == WHITE)
			{
				strcat(screen_buffer, "\033[39m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == MAGENTA)
			{
				strcat(screen_buffer, "\033[35m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == BLUE)
			{
				strcat(screen_buffer, "\033[34m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == ORANGE)
			{
				strcat(screen_buffer, "\033[33m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == GREEN)
			{
				strcat(screen_buffer, "\033[32m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == RED)
			{
				strcat(screen_buffer, "\033[31m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == BLACK)
			{
				strcat(screen_buffer, "\033[30m");
				memcpy(&screen_buffer[5], localBuffer, localLengthToPrint);
			}
			else if (selectedColor == COLOROVERRIDE)
			{
				strcat(screen_buffer, "");
				memcpy(screen_buffer, localBuffer, localLengthToPrint);
			}
			else
			{
				memcpy(screen_buffer, localBuffer, localLengthToPrint);
			}

			if ( (selectedColor != NOCOLOR) && (localLengthToPrint < GENERALBUFFERSIZE - 5) )
			{
				strcat(screen_buffer, "\033[39m");
				localLengthToPrint = fmin((float)GENERALBUFFERSIZE, strlen(screen_buffer));
//				HAL_UART_Transmit(&terminalUART, (uint8_t *)screen_buffer, localLengthToPrint, 150);
				CDC_Transmit_FS((uint8_t *)screen_buffer, localLengthToPrint);
			}
			else if ((selectedColor != NOCOLOR) && (localLengthToPrint >= 4090))
			{
//				HAL_UART_Transmit(&terminalUART, (uint8_t *)screen_buffer, localLengthToPrint, 150);
//				HAL_UART_Transmit(&terminalUART, (uint8_t *)"\033[39m", 6, 150);
				CDC_Transmit_FS((uint8_t *)screen_buffer, localLengthToPrint);
				CDC_Transmit_FS((uint8_t *)"\033[39m", 6);
			}
			else
			{
//				HAL_UART_Transmit(&terminalUART, (uint8_t *)screen_buffer, localLengthToPrint, 150);
			}


			if (screenOnly)
			{
				memset(localBuffer, 0, sizeof(localBuffer));
				return;
			}
		}
		memset(localBuffer, 0, sizeof(localBuffer));
	}
	else
	{
//		if (ee.DebugLevel == 18)
//		{
//			//			HAL_UART_Transmit(&terminalUART, (uint8_t *)"Buffer size is 0\r\n", 18, 150);
//		}
	}
//	if (ee.DebugLevel == 2)
//	{
//		//	    HAL_UART_Transmit(&terminalUART, (uint8_t *)"logData Rel Sem - 2\r\n", 21, 150);
//	}

}

char *resolvePointerToLogsBuffer(void)
{
	if (true)
	{
		return ((char *)terminalBuffer);
	}
	else
	{
		//TODO: LED Indication?
//		Error_Handler();
	}
	return ((char *)terminalBuffer);
}

bool createNewLogFile(void)
{
    bool localret = false;
    char localIndex[6] = "";
	unsigned int br = 0;

	FRESULT FS_ret = f_open(&USERFile, "Index.txt", FA_READ);
    FS_ret = f_read(&USERFile, &FileReadBuffer, sizeof(FileReadBuffer), &br);

	if (FS_ret != FR_OK)
	{
		do
		{
			HAL_Delay(1);
			FS_ret = f_open(&USERFile, "Index.txt", FA_OPEN_APPEND | FA_WRITE);
		} while ( (FS_ret != FR_OK) );

		FS_ret = f_write(&USERFile, "00000\t", strlen("00000\t"), &BytesWritten);
		FS_ret = f_close(&USERFile);
	}

    if (br == 6)
    {
    	memcpy(localIndex, FileReadBuffer, 5);
    	localIndex[5] = '\0';
    	previousLogIndex = atoi(localIndex);
    }
    else
    {
    	//TODO: what if br is not 6
    	// recreate index, etc.
    	previousLogIndex = 0;
    }
    FS_ret = f_close(&USERFile);
    FS_ret = f_open(&USERFile, "Index.txt", FA_OPEN_EXISTING | FA_WRITE);
    char tempString[15] = "";
    if (previousLogIndex + 1 < MAX_LOG_ID)
    {

    }
    else
    {
    	previousLogIndex = 0;
    }
    sprintf(tempString,"%05ld\t",previousLogIndex+1);
    FS_ret = f_write(&USERFile, tempString, strlen(tempString), &BytesWritten);
    FS_ret = f_close(&USERFile);

    sprintf(currentLogFilename,"LOG_%05ld.CSV", previousLogIndex+1);
    FS_ret = f_open(&USERFile, currentLogFilename, FA_CREATE_ALWAYS | FA_WRITE);

    writeLogHeaders();

    return localret;
}

uint32_t getCurrentLogSize(void)
{
    FILINFO fno1;
    DIR dp1;
    f_opendir(&dp1, "\\");
    f_findfirst(&dp1, &fno1, "\\", "LOG_*");
    while( (f_findnext(&dp1, &fno1) == FR_OK) && (fno1.fname[0] != 0x00) )
    {
    	f_stat("\\", &fno1);
    	if(strcmp(currentLogFilename, fno1.fname) == 0)
    	{
    		return fno1.fsize;
    	}
    }
    f_closedir(&dp1);
    return 0;
}

void closeLogFile(void)
{
	sprintf(resolvePointerToLogsBuffer(), "%s Closing current log file\r\n", CT());
	logData( false, false, WHITE);
	f_close(&USERFile);
}

void monitorLogSize(void)
{
	if (HAL_GetTick() - lastFileSizeCheck > 10000)
	{
		f_sync(&USERFile);
		if (getCurrentLogSize() > MAX_LOG_SIZE)
		{
			closeLogFile();
			createNewLogFile();
		}
		lastFileSizeCheck = HAL_GetTick();
	}
}

void deleteLogs(void)
{
	FILINFO fno1;
	DIR dp1;
	FATFS *getFreeFs;
	DWORD free_clusters, free_sectors, total_sectors;

	f_getfree("\\", &free_clusters, &getFreeFs);
	total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	free_sectors = free_clusters * getFreeFs->csize;
	free_kb = (float)free_sectors*(float)(W25Q128FV_SUBSECTOR_SIZE)/1048576;
	total_kb = (float)total_sectors*(float)(W25Q128FV_SUBSECTOR_SIZE)/1048576;

	f_opendir(&dp1, "\\");
	f_findfirst(&dp1, &fno1, "\\", "LOG_*");

	while( (fno1.fname[0] != 0) && (free_kb <= 10) )
	{
		sprintf(resolvePointerToLogsBuffer(), "%s Deleted log: %s\r\n", CT(), fno1.fname);
		logData( false, false, WHITE);

		f_unlink(&fno1.fname[0]);
		f_findnext(&dp1, &fno1);
		f_getfree("\\", &free_clusters, &getFreeFs);
		total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
		free_sectors = free_clusters * getFreeFs->csize;
		free_kb = (float)free_sectors*(float)(W25Q128FV_SUBSECTOR_SIZE)/1048576;
	}
	f_closedir(&dp1);
	sprintf(resolvePointerToLogsBuffer(), "%s EOD\r\n", CT());
	logData( false, false, WHITE);
}

void writeLogHeaders(void)
{
//	sprintf(terminalBuffer,"LNK, UL-RSSI1, UL-RSSI2, UL-PSRLQ, UL-SNR, DA-Ant, RFMode, UL-TxPWR, DL-RSSI, DL-PSRLQ, DL_SNR");
//	logData(terminalBuffer, true, false, false);
//	sprintf(terminalBuffer,"SMA, State, TriggerMode, Battery[V], Altitude[m], Acceleration[m/Sec^2]");
//	logData(terminalBuffer, true, false, false);
}
