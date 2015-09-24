/*
 * setting.h
 *
 *  Created on: 3 avr. 2015
 *      Author: Colin
 */

#ifndef SETTING_H_
#define SETTING_H_
/* dependencies */
#include "config/module_id.h"

/* Error codes */
#define SETTING_MODULE_E_SUCCESS	0
#define SETTING_MODULE_E_ERROR	MAKE_ERROR(SETTIING_MODULE_ID, 1)

/* memory address */

/* structure of settings */
typedef struct
{
	char calibration_enabled;
	char color_sensor_enabled;
	int x_finish_maze;
	int y_finish_maze;
	unsigned int sleep_delay_s;
	unsigned long threshold_color;
	unsigned long threshold_greater;
}settings;
settings zhonxSettings;
int settingsInit (void);
void halt(void);
#endif /* SETTING_H_ */
