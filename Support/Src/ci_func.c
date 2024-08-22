/*
 * ci_func.c
 *
 *  Created on: Aug 22, 2024
 *      Author: user
 */



#include "main.h"
#include "usart.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "fatfs.h"
#include "ff.h"



bool isReportParametersActive = false;
bool isInfwUpdateMode = false;

eCI_RESULT func_debug(void)
{
//	if (!sessionUnlocked)
//	{
//		return (CI_COMMAND_ERROR);
//	}
//	if (get_param_count() > 0)
//	{
//		ee.debugLevel = get_param_int(0);
//		ee_save1();
//		sprintf(terminalBuffer,"Debug level set to: %d",ee.debugLevel);
//		logData(terminalBuffer, false, false, false);
//	}
	return (CI_OK);
}

eCI_RESULT func_versionReport(void)
{
//	sprintf(terminalBuffer,"RC Firmware Version: %2.2f, BuildID: %2.2f, Configuration: %2.2f \3",fwVersion, buildID, ee.configuration);
//	logData(terminalBuffer, false, true, false);
	return (CI_OK);
}

eCI_RESULT func_reset(void)
{
	NVIC_SystemReset();
	return (CI_OK);
}

eCI_RESULT func_showAvailableCommands(void)
{
	return (CI_OK);
}

eCI_RESULT func_systemConfiguration(void)
{
//	if (!sessionUnlocked)
//	{
//		return (CI_COMMAND_ERROR);
//	}
//	printConfiguration(true);
	return (CI_OK);
}

eCI_RESULT func_screenOrientation(void)
{
////	if (!sessionUnlocked)
////	{
////		return (CI_COMMAND_ERROR);
////	}
////	if (get_param_count() > 0)
////	{
////		if ( (get_param_int(0) <= 1) && (get_param_int(0) >= 0) )
////		{
////			ee.screenOreintation = get_param_int(0);
////			ee_save1();
////			screenInit();
////			setFullDisplay();
////		}
////
////		if (ee.screenOreintation == 0)
////		{
////			logData("Screen orientation set to Landscape", false, false, false);
////		}
////		else
////		{
////			logData("Screen orientation set to Portrait", false, false, false);
////		}
//	}
	return (CI_OK);
}

eCI_RESULT func_setDateTime(void)
{
//	int year = 0;
//	int month = 0;
//	int day = 0;
//	int hour = 0;
//	int minute = 0;
//	int second = 0;
//
//	if (!sessionUnlocked)
//	{
//		return (CI_COMMAND_ERROR);
//	}
//	if (get_param_count() > 0)
//	{
//		sprintf(terminalBuffer, "Received Date & Time:\'%s\'", get_param_str(0));
//		logData(terminalBuffer, false, false, false);
//
//		sscanf(get_param_str(0), "%02d/%02d/%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &second);
//
//		if ( (year >= 0) && (year <= 99) && (hour >= 0) && (hour < 24) &&
//				(minute >= 0) && (minute < 60) && (day > 0) && (day < 32) && (month > 0) && (month < 13) && (second >= 0) && (second < 60) )
//		{
//			configurationMessageCounter++;
//
//			sTime.Hours = hour;
//			sTime.Minutes = minute;
//			sTime.Seconds = second;
//
//			sDate.Month = month;
//			sDate.Date = day;
//			sDate.Year = year;
//
//			shouldAddTimeToConfigurationMessage = true;
//			sendSafeAirConfigurationMessage(shouldAddTimeToConfigurationMessage);
//		}
//		else
//		{
//			sprintf(terminalBuffer, "Wrong date time:\'%s\'",get_param_str(0));
//			logData(terminalBuffer, false, false, false);
//		}
//	}
//
	return (CI_OK);
}

eCI_RESULT func_version(void)
{
	sprintf(resolvePointerToLogsBuffer(), "%s version test\r\n", CT());
	logData( true, true, WHITE);
	return (CI_OK);
}

eCI_RESULT func_importFile(void)
{
//	if (!sessionUnlocked)
//	{
//		return (CI_COMMAND_ERROR);
//	}
//	char *res = 0;
//	//	  uint16_t slen = 0;
//	bool _endFile = false;
//	//	  char *fn = get_param_str(0);
//	FIL fileToRead;
//	//	  f_close(&fileToRead);
//	char fullFileName[64] = "";
//	sprintf(fullFileName, "%s",get_param_str(0));
//	if (f_open(&fileToRead, fullFileName, FA_READ) == FR_OK)
//	{
//		while (!_endFile)
//		{
//			char lTerminalBuffer[4096] = {0};
//			res = f_gets(lTerminalBuffer, 4096, &fileToRead);
//			if (res != 0x00)
//			{
//				logData(lTerminalBuffer, false, true, true);
//			}
//			else
//			{
//				_endFile = true;
//				for (int i = 0 ; i < 3 ; i++)
//				{
//					logData("RC EOF", false, true, true);
//					HAL_Delay(10);
//				}
//			}
//		}
//	}
//	f_close(&fileToRead);
	return (CI_OK);
}

eCI_RESULT func_dir(void)
{
    FILINFO fno1;
    DIR dp1;
    f_opendir(&dp1, "\\");
    f_findfirst(&dp1, &fno1, "\\", "*");

    while( (fno1.fname[0] != 0) )
    {
    	if ( (fno1.fsize == 0xffffffff) || ((fno1.fsize == 0x0)) )
    	{
    		f_unlink(fno1.fname);
    	}
    	else
    	{
    		sprintf(resolvePointerToLogsBuffer(),"%s\t %lu",fno1.fname, (long int)fno1.fsize);
    		logData(true, true, WHITE);
    	}
    	f_findnext(&dp1, &fno1);
    }
    f_closedir(&dp1);
//    logData("dir EOD", false, true, true);
	return (CI_OK);
}

eCI_RESULT func_fmt(void)
{
//	if (!sessionUnlocked)
//	{
//		return (CI_COMMAND_ERROR);
//	}
	sprintf(resolvePointerToLogsBuffer(), "\r\n%s!Formatting Flash...restart required!\n\r", CT());
	logData(true, true, WHITE);
	f_sync(&USERFile);
	f_close(&USERFile);

	BSP_QSPI_Erase_Chip();
	fileSystemInit();
	createNewLogFile();
	return (CI_OK);
}


typedef struct
{
    char *string;
    eCI_RESULT (*func)(void);
} functionsList;

functionsList cases [] =
{
		{ "ver2"	, func_versionReport },
		{ "dbg"	, func_debug },
		{ "ee?"	, func_systemConfiguration },
		{ "rst" , func_reset },
		{ "help", func_showAvailableCommands },
		{ "scor", func_screenOrientation },
		{ "ver", func_version },
		{ "dir", func_dir },
		{ "dtm", func_setDateTime}
};

uint8_t funcTable( char* token )
{
	for( functionsList* pCase = cases; pCase != cases + sizeof( cases ) / sizeof( cases[0] ); pCase++ )
	{
		if( 0 == strcmp( pCase->string, token ) )
		{
			(*pCase->func)();
			return (1);
			break;
		}
	}
	return (0);
}
