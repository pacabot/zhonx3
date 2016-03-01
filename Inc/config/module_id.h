/*
 * module_id.h
 *
 *  Created on: 6 avr. 2015
 *      Author: Colin
 */

#ifndef MODULE_ID_H_
#define MODULE_ID_H_

/*driver ID 0xx */

#define BLUETOOTH_DRIVER_MODULE_ID				1
#define CALLBACK_MODULE_ID						2
#define SSD1306_DRIVER_MODULE_ID				3
#define E24LC64_DRIVER_MODULE_ID				4
#define IE512_DRIVER_MODULE_ID					5
#define PCF8574_DRIVER_MODULE_ID				6
#define ADXRS620_DRIVER_MODULE_ID				7
#define LINESENSORS_DRIVER_MODULE_ID			8
#define MOTORS_DRIVER_MODULE_ID					9
#define MULTIMETER_DRIVER_MODULE_ID  			10
#define TELEMETERS_DRIVER_MODULE_ID  			12
#define CALIBRATION_TELEMETERS_DRIVER_MODULE_ID	13
#define TIMESBASE_DRIVER_MODULE_ID				14	//todo : verify if the number 14 is good (old number 13)
#define POWERMANAGMENT_DRIVER_MODULE_ID			15

/* middleware 1xx */

#define SPEED_CONTROL_MODULE_ID					100
#define MAIN_CONTROL_MODULE_ID					101
#define POSITION_CONTROL_MODULE_ID				102
#define TRANSFERT_FUNCTION_MODULE_ID			105
#define WALL_SENSORS_MODULE_ID					106
#define SETTING_MODULE_ID						110
#define RING_BUFFER_MODULE_ID					111
#define CMDLINE_PARSER_MODULE_ID				112

/* application 2xx*/
#define MAZE_SOLVER_MODULE_ID					201
#define STATISTIQUES_MODULE_ID					202

#endif /* MODULE_ID_H_ */
