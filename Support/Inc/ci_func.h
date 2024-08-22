/*
 * ci_func.h
 *
 *  Created on: Aug 22, 2024
 *      Author: user
 */

#ifndef INC_CI_FUNC_H_
#define INC_CI_FUNC_H_


#include <stdint.h>
#include "cmd_interp.h"

extern bool isReportParametersActive;
extern bool isInfwUpdateMode;

extern eCI_RESULT func_debug(void);
extern eCI_RESULT func_versionReport(void);
extern eCI_RESULT func_reset(void);
extern eCI_RESULT func_showAvailableCommands(void);
extern eCI_RESULT func_systemConfiguration(void);

extern uint8_t funcTable( char* token );


#endif /* INC_CI_FUNC_H_ */
