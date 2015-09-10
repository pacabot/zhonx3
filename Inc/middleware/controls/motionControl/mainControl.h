/**************************************************************************/
/*!
    @file    mainControl.h
    @author  PLF (PACABOT)
    @date
    @version  0.0
 */
/**************************************************************************/
#ifndef __MAINCONTROL_H__
#define __MAINCONTROL_H__

#include "config/config.h"
#include "middleware/controls/motionControl/speedControl.h"
#include "middleware/wall_sensors/wall_sensors.h"

/* Module Identifier */
#include "config/module_id.h"

/* Error codes */
#define MAIN_CONTROL_E_SUCCESS  0
#define MAIN_CONTROL_E_ERROR    MAKE_ERROR(MAIN_CONTROL_MODULE_ID, 1)

#define OFFSET_DIST	10.00

/* Types definitions */
enum rotationTypeEnum {CW, CCW};
enum moveTypeEnum {STRAIGHT, CURVE, STOP};

extern double ROTATION_DIAMETER;

typedef struct
{
	char wall_follow_state;
	char line_follow_state;
	char position_state;
	char speed_state;
}control_params_struct;

typedef struct
{
	enum moveTypeEnum moveType;
	walls cellState;
	double initial_position;
}move_params_struct;

extern control_params_struct control_params;
extern move_params_struct move_params;

int mainControlInit(void);
int mainControlLoop(void);
/**
 * @brief compute and start a new movement
 *
 * This function outputs all params for pids controller (speedControl and positionControl).
 *
 * @param
 *
 * angle => angle of rotation in degres (0° for strait move)
 * radius_or_distance => radius in mm of rotate if angle > 0 or distance in mm if angle = 0°
 * speed_rate => speed ratio in percent of max value
 *
 * @retval HAL status
 */
int  move(float angle, float radius_or_distance, float max_speed, float end_speed);
char isEndMove(void);
int  frontCal(float max_speed);
int  rotate180WithCal(enum rotationTypeEnum rotation_type, float max_speed, float end_speed);
int  rotate90WithCal(enum rotationTypeEnum rotation_type, float max_speed, float end_speed);
int  moveCell(unsigned long nb_cell, float max_speed, float end_speed);
int  moveHalfCell_IN(float max_speed, float end_speed);
int  moveHalfCell_OUT(float max_speed, float end_speed);
int  moveEndCell(float max_speed, float end_speed);
int  moveStartCell(float max_speed, float end_speed);
int  moveRotateCW90(float max_speed, float end_speed);
int  moveRotateCCW90(float max_speed, float end_speed);
int  moveUTurn(float speed_rotation, float max_speed, float end_speed);
void mainControlTest(void);
void followWallTest(void);
void followLineTest(void);
void rotateTest(void);
void curveRotate(void);

#endif // __MAINCONTROL_H
