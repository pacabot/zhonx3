/*
 *  menu.c
 *
 *  Created on: 4 déc. 2014
 *      Author: colin
 */
/* STM32 hal library declarations */
#include "stm32f4xx_hal.h"

/* General declarations */
#include "config/basetypes.h"
#include "config/config.h"
#include "config/errors.h"

#include "stdbool.h"
#include <arm_math.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* Peripheral declarations */
#include "peripherals/display/ssd1306.h"
#include "peripherals/display/smallfonts.h"
#include "peripherals/expander/pcf8574.h"
#include "peripherals/multimeter/multimeter.h"
#include "peripherals/tone/tone.h"
#include "peripherals/bluetooth/bluetooth.h"
#include "peripherals/eeprom/24lc64.h"
#include "peripherals/encoders/ie512.h"
#include "peripherals/gyroscope/adxrs620.h"
#include "peripherals/multimeter/multimeter.h"
#include "peripherals/telemeters/telemeters.h"
#include "peripherals/tone/tone.h"
#include "peripherals/motors/motors.h"
#include "peripherals/lineSensors/lineSensors.h"
#include "peripherals/expander/pcf8574.h"
//#include "peripherals/telemeters/telemetersCal.h"


/* Middleware declarations */
#include "middleware/display/icons.h"
#include "middleware/display/pictures.h"
#include "middleware/settings/settings.h"
#include "middleware/cmdline/cmdline_parser.h"
#include "middleware/controls/mazeControl/wallFollowControl.h"
#include "middleware/wall_sensors/wall_sensors.h"
#include "middleware/moves/mazeMoves/mazeMoves.h"
#include "middleware/moves/mazeMoves/mazeMovesAdvanced.h"
#include "middleware/moves/mazeMoves/spyPost.h"
#include "middleware/moves/mazeMoves/spyWall.h"
#include "middleware/controls/pidController/pidCalculator.h"
#include "middleware/controls/lineFollowerControl/lineFollowControl.h"

/* Application declarations */

#include "application/lineFollower/lineFollower.h"
#include "application/solverMaze/solverMaze.h"
#include "application/solverMaze/robotInterface.h"

/* Declarations for this module */
#include "application/display/menu.h"

typedef enum
{
    void_line,
    function,
    boolean,
    integer,
    preset_value,
    new_menu
}line_on_menue_type;

/*external functions */
extern int setMeddle(void);
extern int setDark(void);


/*
 * to create a new menu you have to create a new variable of type "const menuItem" like this :
 * const menuItem name =
 * {
 * 		"menu name",
 * 		{
 * 			{"line 1 name ",type_of_argument, &(void*) pointeur_on_argument},		// 'type of argument' could be integer for int, or 'l' for long, or menu for  a menu, or function for a function
 * 			{"line 1 name ",type of argument, &(void*) pointeur_on_argument},		// maximum 20 line. if it's not enough you must add in "menu.h". for that you have to modify "MAX_LINE_IN_MENU"
 * 			{NULL,        0,     NULL} 					// the last ligne must be this one, this line will be not print but indispensable. /!\ this line  compte dans les 20 ligne du menu
 * 		}
 * }
 */

const menuItem scan_settings_menu =
{
        "SCAN SETT",         //9 characters max
        {
                {"min speed",           integer,(void*)&zhonxSettings.speeds_scan.min_speed},
                {"max speed tans",      integer,(void*)&zhonxSettings.speeds_scan.max_speed_traslation},
                {"max speed rot",       integer,(void*)&zhonxSettings.speeds_scan.max_speed_rotation},
                {NULL,       0,     NULL}
        }
};

const menuItem run1_settings_menu =
{
        "RUN1 SETING",         //9 characters max
        {
                {"min speed",           integer,(void*)&zhonxSettings.speeds_run1.min_speed},
                {"max speed tans",      integer,(void*)&zhonxSettings.speeds_run1.max_speed_traslation},
                {"max speed rot",       integer,(void*)&zhonxSettings.speeds_run1.max_speed_rotation},
                {"return to start",     boolean,(void*)&zhonxSettings.return_to_start_cell},
                {NULL,       0,     NULL}
        }
};

const menuItem run2_settings_menu =
{
        "RUN2 SETTING",         //9 characters max
        {
                {"min speed",           integer,(void*)&zhonxSettings.speeds_run2.min_speed},
                {"max speed tans",      integer,(void*)&zhonxSettings.speeds_run2.max_speed_traslation},
                {"max speed rot",       integer,(void*)&zhonxSettings.speeds_run2.max_speed_rotation},
                {"return to start",     boolean,(void*)&zhonxSettings.return_to_start_cell},
                {NULL,       0,     NULL}
        }
};

const menuItem maze_menu =
{
        "MAZE",         //9 characters max
        {
                {"New maze",        function,    (void*)maze_solver_new_maze},
                {"scan settings",   new_menu,    (void*)&scan_settings_menu},
                {"Run1",            function,    (void*)startRun1},
                {"Run1 settings",   new_menu,    (void*)&run1_settings_menu},
                {"Run2",            function,    (void*)startRun2},
                {"Run2 settings",   new_menu,    (void*)&run2_settings_menu},
                {"print maze",      function,    (void*)printStoredMaze},
                {"restart explo",   function,    (void*)&restartExplo},
                {"Test maze move",  function,    (void*)test_move_zhonx},
                {NULL,        0,     NULL}
//				{"Calibration", boolean,    (void*)&zhonxSettings.calibration_enabled},
//				{"Color finish",boolean,	(void*)&zhonxSettings.nime_competition},
//				{"X finish",    integer,	(void*)&zhonxSettings.maze_end_coordinate.x},
//				{"Y finish",    integer,	(void*)&zhonxSettings.maze_end_coordinate.y}
		}

};

const menuItem follower_menu =
{
		"LINE FOLL",    //9 characters max
		{
				{"line follower",   function,	(void*)lineFollower},
				{"Calibration",     function,   (void*)floorSensorCalib},
				{"speed",			integer,	(void*)&line_speed},
				{"line length",		integer,	(void*)&line_length},
				{"Set F. KP",       integer,    (void*)&Line_follower_KP},
				{"Set F. KD",       integer,    (void*)&Line_follower_KD},
				{"line sensor test",function,   (void*)test_line_sensors},
				{NULL, 0, NULL}
		}
};

const menuItem telemetersCal =
{
        "TELEMETER",    //9 characters max
        {
                {"Calibration front",   function,    (void*)wallSensorsCalibrationFront},
                {"Calibration diag",    function,    (void*)wallSensorsCalibrationDiag},
                {"Send calib values",   function,    (void *)telemetersGetCalibrationValues},
                {"Set BT baudrate",     preset_value,(void *)&BTpresetBaudRate},
                {NULL, 0, NULL}
        }
};

const menuItem spyPostCal =
{
        "SPYPOST",      //9 characters max
        {
                { "Start calibration",  function, (void*)spyPostCalibration},
                { "Read Calibration.",  function, (void*)spyPostReadCalibration},
                { "SpyPost test.",      function, (void*)spyPostTest},
		{NULL, 0, NULL}
        }
};

const menuItem pidCal =
{
        "PID",          //9 characters max
        {
                { "Gyro Kp critic", function, (void*)pidGyro_GetCriticalPoint},
                { "Enc. Kp critic", function, (void*)pidEncoders_GetCriticalPoint},
                { "Tel. Kp critic", function, (void*)pidTelemeters_GetCriticalPoint},
                {NULL, 0, NULL}
        }
};

const menuItem frontCal =
{
        "FRONT CAL",    //9 characters max
        {
                { "Start calibration",  function, (void*)spyWallFrontDistCal},
                { "SpyWall test",       function, (void*)spyWallFrontTest},
                {NULL, 0, NULL}
        }
};

const menuItem gyroCal =
{
        "GYRO CAL",    //9 characters max
        {
                { "Start calibration",  function, (void*)adxrs620Cal},
                { "Gyro test",          function, (void*)adxrs620Test},
                {NULL, 0, NULL}
        }
};

const menuItem calibration_menu =
{
        "CALIB.",        //9 characters max
        {
                {"Telemeters",      new_menu,  (void*)&telemetersCal},
                {"SpyPost",         new_menu,  (void*)&spyPostCal},
                {"SpyWall",         new_menu,  (void*)&frontCal},
                {"Gyroscope",       new_menu,  (void*)&gyroCal},
                {"PID",             new_menu,  (void*)&pidCal},
                {NULL, 0, NULL}
        }
};

const menuItem tests_menu =
{
		"TESTS",        //9 characters max
		{
				{"Wall sensor",		function, (void*)testWallsSensors},
				{"Bluetooth",		function, (void*)bluetoothTest},
				{"Multimeter",		function, (void*)mulimeterTest},
				{"Display",			function, (void*)ssd1306Test},
				{"Eeprom",			function, (void*)eepromTest},
				{"Encoders",		function, (void*)encoderTest},
				{"Joystick",		function, (void*)joystickTest},
				{"Gyroscope",		function, (void*)adxrs620Test},
				{"Telemeters",		function, (void*)telemetersTest},
				{"Beeper",			function, (void*)toneTest},
				{"Motors",			function, (void*)motorsTest},
				{"Line sensors",	function, (void*)lineSensorsTest},
				{"Expender LEDs",	function, (void*)expenderLedTest},
				{"flash maze",      function, (void*)test_maze_flash},
				{"wall Follow",     function, (void*)wallFollowTest},
				{NULL, 0, NULL}
		}
};

const menuItem control_menu =
{       "CONTROL",      //9 characters max
        {
                { "Move Test 1",        function, (void*)movesTest1 },
                { "Move Test 2",        function, (void*)movesTest2 },
                { "Advanced Move 1",    function, (void*)mazeMoveAdvancedTest},
                {NULL, 0, NULL}
        }
};

const menuItem zhonxNameMenu =
{
		"SET MAME",     //9 characters max
		{
				{"Meddle",  function, setMeddle},
				{"Dark",    function, setDark},
				{NULL, 0, NULL}
		}
};

const menuItem mainMenu =
{
		(char *)CONFIG_ZHONX_INFO_ADDR,
		{
				{"Maze menu",       new_menu,	 (void*)&maze_menu},
				{"Line menu",       new_menu,    (void*)&follower_menu},
				{"Unit tests",      new_menu,    (void*)&tests_menu},
				{"Calibration menu",new_menu,    (void*)&calibration_menu},
				{"Control menu",    new_menu,    (void*)&control_menu},
				{"Zhonx Name",      new_menu,    (void*)&zhonxNameMenu},
				{NULL, 0, NULL}
		}
};

extern I2C_HandleTypeDef hi2c1;

int menu(const menuItem Menu)
{
	encodersInit();
	encodersReset();
    signed char line_screen = 1;
    signed char line_menu = 0;
    // Display main menu
    displayMenu(Menu, line_menu);
    ssd1306InvertArea(0, MARGIN, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT);
    ssd1306Refresh();
    while (true)
    {
        HAL_Delay(1);
        int joystick = expanderJoyFiltered();
        //		killOnLowBattery();
		if (joystick == JOY_LEFT || encoderGetDist(ENCODER_L) > 20)
		{
			return SUCCESS;
		}
		// Joystick down
		if (joystick == JOY_DOWN || encoderGetDist(ENCODER_R) < -20)
		{
			toneItMode(100, 10);
			if (Menu.line[line_menu + 1].name != null)
			{
				line_menu++;
				if (line_screen >= MAX_LINE_SCREEN)
				{
					displayMenu(Menu, line_menu - (line_screen - 1));
					ssd1306InvertArea(0, line_screen * MARGIN,
					HIGHLIGHT_LENGHT,
									  HIGHLIGHT_HEIGHT);
					ssd1306Refresh();
				}
				else
				{
					line_screen++;
					menuHighlightedMove((line_screen - 1) * ROW_HEIGHT + 1, (line_screen) * ROW_HEIGHT);
				}
				encodersReset();
			}
			else
			{
				line_menu = 0;
				line_screen = 1;
				displayMenu(Menu, line_menu - (line_screen - 1));
				ssd1306InvertArea(0, line_screen * MARGIN,
				HIGHLIGHT_LENGHT,
								  HIGHLIGHT_HEIGHT);
				ssd1306Refresh();
			}
			encodersReset();
		}
		if( joystick == JOY_UP || encoderGetDist(ENCODER_R) > 20)
		{
			toneItMode(100, 10);
			if (line_menu > 0)
			{
				line_menu--;
				if (line_screen <= 1)
				{
					displayMenu(Menu, line_menu);
					ssd1306InvertArea(0, MARGIN, HIGHLIGHT_LENGHT,
					HIGHLIGHT_HEIGHT);
					ssd1306Refresh();
				}
				else
				{
					line_screen--;
					menuHighlightedMove((line_screen + 1) * ROW_HEIGHT - 1, (line_screen) * ROW_HEIGHT);
				}
			}
			else
			{
				while (Menu.line[line_menu + 1].name != null)
				{
					line_menu++;
				}
				if (line_menu < MAX_LINE_SCREEN - 1)
				{
					displayMenu(Menu, 0);
					line_screen = line_menu + 1;
				}
				else
				{
					line_screen = MAX_LINE_SCREEN;
					displayMenu(Menu, line_menu - (MAX_LINE_SCREEN - 1));
				}
				ssd1306InvertArea(0, MARGIN * line_screen, HIGHLIGHT_LENGHT,
				HIGHLIGHT_HEIGHT);
				ssd1306Refresh();
			}
			encodersReset();
		}
		if (joystick == JOY_RIGHT || encoderGetDist(ENCODER_L) < -20) // Validate button joystick right
		{
			toneItMode(8000, 20);
			switch (Menu.line[line_menu].type)
			{
				case boolean:
					modifyBoolParam(Menu.line[line_menu].name, (unsigned char*) Menu.line[line_menu].param);
					break;
				case integer:
					modifyLongParam(Menu.line[line_menu].name, (long*) (int*) Menu.line[line_menu].param);
					break;
				case 'l':
					modifyLongParam(Menu.line[line_menu].name, (long*) Menu.line[line_menu].param);
					break;
				case new_menu:
					menu(*(const menuItem*) Menu.line[line_menu].param);
					break;
				case function:
					if (Menu.line[line_menu].param != NULL)
					{
						ssd1306ClearScreen(MAIN_AREA);
						ssd1306Refresh();
						Menu.line[line_menu].param();
					}
					break;
				case preset_value:
					modifyPresetParam(Menu.line[line_menu].name, (presetParam *)Menu.line[line_menu].param);
					break;
				default:
					break;
			}
			HAL_Delay(50);
			displayMenu(Menu, line_menu - (line_screen - 1));
			ssd1306InvertArea(0, MARGIN * line_screen, HIGHLIGHT_LENGHT,
			HIGHLIGHT_HEIGHT);
			ssd1306Refresh();
			encodersReset();
		}
        cmdline_parse();
    }
    return -1;
}

void menuHighlightedMove(unsigned char y, unsigned char max_y)
{
    if (max_y > y)
    {
        //		ssd1306InvertArea(0, y-1, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT*2);
        for (; y <= max_y; y++)
        {
            ssd1306InvertArea(0, y - 1, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT);
            ssd1306InvertArea(0, y, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT);
            if (y % 2) //refresh if pair, increases the refresh speed
                ssd1306Refresh();
            while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
                ;
        }
    }
    else
    {
        //		ssd1306InvertArea(0, y-HIGHLIGHT_HEIGHT+1, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT*2);
        for (; y >= max_y; y--)
        {
            ssd1306InvertArea(0, y + 1, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT);
            ssd1306InvertArea(0, y, HIGHLIGHT_LENGHT, HIGHLIGHT_HEIGHT);
            if (y % 2) //refresh if pair, increases the refresh speed
                ssd1306Refresh();
            while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
                ;
        }
    }
    ssd1306Refresh();
}

void displayMenu(const menuItem menu, int line)
{
    //char str[5];
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306ClearRect(0, 0, 38, 7); //clear title name
    ssd1306DrawString(0, -1, menu.name, &Font_3x6);
    for (int i = 0; i < MAX_LINE_SCREEN; i++)
    {
        if (menu.line[i].name != null)
            ssd1306DrawStringAtLine(0, i, menu.line[line + i].name, &Font_5x8);
        switch (menu.line[line + i].type)
        {
            case boolean:
                if (*((bool*) menu.line[i + line].param) == true)
                    ssd1306DrawStringAtLine(90, i, "yes", &Font_5x8);
                else
                    ssd1306DrawStringAtLine(90, i, "no", &Font_5x8);
                break;
            case integer:
                ssd1306PrintIntAtLine(90, i, " ", *((unsigned int*) menu.line[i + line].param), &Font_3x6);
                break;
            case 'l':
                ssd1306PrintIntAtLine(90, i, " ", *((unsigned long*) menu.line[i + line].param), &Font_3x6);
                break;
            case function:
                ssd1306DrawStringAtLine(110, i, ">", &Font_3x6);
                break;
            case new_menu:
                ssd1306DrawStringAtLine(110, i, "->", &Font_3x6);
                break;
            case preset_value:
                ssd1306PrintIntAtLine(90, i, " ", (long) ((presetParam*) menu.line[i + line].param)->p_value,
                                      &Font_3x6);
                break;
        }
    }
    uint8_t nmbr_item = 0;
    while (menu.line[nmbr_item].name != null)
    {
        nmbr_item++;
    }
    if (nmbr_item > MAX_LINE_SCREEN)
    {
        //int heightOneItem=54/nmbr_item;
        ssd1306DrawRect(123, (54 * line) / nmbr_item + MARGIN, 3, (54 * MAX_LINE_SCREEN) / nmbr_item);
    }
}

int modifyBoolParam(char *param_name, unsigned char *param)
{
	encodersReset();
    char str[4];
    bool param_copy = (bool) *param;

    ssd1306ClearScreen(MAIN_AREA);

    // Write the parameter name
    ssd1306DrawStringAtLine(0, 0, param_name, &Font_5x8);

    if (param_copy == true)
    {
        sprintf(str, "YES");
    }
    else
    {
        sprintf(str, "NO");
    }
    ssd1306DrawStringAtLine(0, 2, str, &Font_8x8);
    ssd1306DrawStringAtLine(0, 3, "PRESS 'RIGHT' TO VALIDATE", &Font_3x6);
    ssd1306DrawStringAtLine(0, 4, "      'LEFT'  TO RETURN", &Font_3x6);
    ssd1306Refresh();

    while (1)
    {
        int joystick = expanderJoyFiltered();
        HAL_Delay(100);
		if (joystick == JOY_LEFT || encoderGetDist(ENCODER_L) > 20)
		{
			return SUCCESS;
		}

		if (joystick == JOY_DOWN || encoderGetDist(ENCODER_R) < -20 || joystick == JOY_UP || encoderGetDist(ENCODER_R) > 20)
		{
			if (param_copy == true)
			{
				param_copy = false;
				sprintf(str, "NO");
			}
			else
			{
				param_copy = true;
				sprintf(str, "YES");
			}
			ssd1306ClearRectAtLine(0, 2, 128);
			ssd1306DrawStringAtLine(0, 2, str, &Font_8x8);
			ssd1306Refresh();
			encodersReset();
		}
		if (joystick == JOY_RIGHT || encoderGetDist(ENCODER_L) < -20)
		{
			*param = param_copy;
			ssd1306ClearScreen(MAIN_AREA);
			ssd1306Refresh();
			return SUCCESS;
			encodersReset();
        }
    }
    return SUCCESS;
}

int modifyLongParam(char *param_name, long *param)
{
	encodersReset();
    int step = 1;
    char str[40];
    long param_copy = *param;
    char column = 0;

    ssd1306ClearScreen(MAIN_AREA);

    // Write the parameter name
    ssd1306ClearRect(0, 0, 38, 7); //clear title name
    ssd1306DrawString(0, -1, param_name, &Font_3x6);

    sprintf(str, "%10i", (int) param_copy);

    ssd1306DrawStringAtLine(0, 1, str, &Font_8x8);
    ssd1306DrawStringAtLine(0, 3, "PRESS 'RIGHT' TO VALIDATE", &Font_3x6);
    ssd1306DrawStringAtLine(0, 4, "      'LEFT'  TO RETURN", &Font_3x6);

    ssd1306DrawStringAtLine((10 - column) * 8, 0, "-", &Font_8x8);
    ssd1306DrawStringAtLine((10 - column) * 8, 2, "-", &Font_8x8);
    ssd1306Refresh();

    while (1)
    {
        int joystick = expanderJoyFiltered();
		if (joystick == JOY_LEFT || encoderGetDist(ENCODER_L) > 20)
		{
                if (column == 10)
                    return SUCCESS;
                else
                {
                    column++;
                    ssd1306ClearRectAtLine(0, 0, 128);
                    ssd1306ClearRectAtLine(0, 2, 128);
                    ssd1306DrawStringAtLine((9 - column) * 9, 0, "-", &Font_8x8);
                    ssd1306DrawStringAtLine((9 - column) * 9, 2, "-", &Font_8x8);
                    ssd1306Refresh();
                }
    			encodersReset();
		}
		if (joystick == JOY_UP || encoderGetDist(ENCODER_R) > 20)
		{
			//param_copy +=1;
			param_copy += (step * pow(10, column));
			sprintf(str, "%10i", (int) param_copy);
			ssd1306ClearRectAtLine(0, 1, 128);
			ssd1306DrawStringAtLine(0, 1, str, &Font_8x8);
			ssd1306Refresh();
			encodersReset();
		}
		if (joystick == JOY_DOWN || encoderGetDist(ENCODER_R) < -20)
		{
			param_copy -= (step * pow(10, column));
			//param_copy -= 1;
			sprintf(str, "%10i", (int) param_copy);
			ssd1306ClearRectAtLine(0, 1, 128);
			ssd1306DrawStringAtLine(0, 1, str, &Font_8x8);
			ssd1306Refresh();
			encodersReset();
		}
		if (joystick == JOY_RIGHT || encoderGetDist(ENCODER_L) < -20)
		{
			if (column == 0)
			{
				*param = param_copy;
				ssd1306Refresh();
				return SUCCESS;
			}
			else
			{
				column--;
				ssd1306ClearRectAtLine(0, 0, 128);
				ssd1306ClearRectAtLine(0, 2, 128);
				ssd1306DrawStringAtLine((9 - column) * 9, 0, "-", &Font_8x8);
				ssd1306DrawStringAtLine((9 - column) * 9, 2, "-", &Font_8x8);
				ssd1306Refresh();
			}
			encodersReset();
        }
    }

    return SUCCESS;
}

int modifyPresetParam(char *param_name, presetParam *param)
{
    char str[40];
    presetParam *preset = param;
    int param_copy = 0;
    int preset_val = 0;
    int *p_preset_val;
    int presetBufferLen = 0;
    int *p_presetBuffer = (int *) (preset->presetBuffer);

    preset_val = *(p_presetBuffer);

    // Check if the current value is present into the preset buffer
    while (preset_val != -0x7FFFFFFF)
    {
        if (preset_val == *((int *) preset->p_value))
        {
            // Current value has been found into preset buffer
            param_copy = preset_val;
            // Keep a pointer toward the value in preset buffer
            p_preset_val = p_presetBuffer;
            // Don't break here, in order to count until end of the buffer
        }
        p_presetBuffer++;
        presetBufferLen++;
        preset_val = *p_presetBuffer;
    }

    // Decrement presetBufferLen by 1, as the last element is not used
    presetBufferLen--;

    if (presetBufferLen <= 0)
    {
        // This should never happen!
        return ERROR;
    }

    if (param_copy == 0)
    {
        // Current value is not into preset buffer
        param_copy = *((int *) preset->p_value);
        p_presetBuffer = (int *) (preset->presetBuffer);
        *((int *) (preset->p_value)) = *p_presetBuffer;
    }
    else
    {
        // Set back the stored pointer
        p_presetBuffer = p_preset_val;
    }

    ssd1306ClearScreen(MAIN_AREA);

    // Write the parameter name
    ssd1306ClearRect(0, 0, 38, 7); //clear title name
    ssd1306DrawString(0, -1, param_name, &Font_3x6);

    // Write parameter's current value
    sprintf(str, "%10i", param_copy);

    ssd1306DrawStringAtLine(0, 1, str, &Font_8x8);
    ssd1306DrawStringAtLine(0, 3, "PRESS 'RIGHT' TO VALIDATE", &Font_3x6);
    ssd1306DrawStringAtLine(0, 4, "      'LEFT'  TO RETURN", &Font_3x6);

    ssd1306DrawStringAtLine(5 * 8, 0, "-", &Font_8x8);
    ssd1306DrawStringAtLine(5 * 8, 2, "-", &Font_8x8);

    ssd1306Refresh();

    while (1)
    {
        int joystick = expanderJoyFiltered();

        switch (joystick)
        {
            case JOY_LEFT:
                // Exit button
                return SUCCESS;

            case JOY_UP:
                p_presetBuffer++;
                if (*p_presetBuffer == -0x7FFFFFFF)
                {
                    // Reset pointer to the first element of preset buffer
                    p_presetBuffer = (int *) (preset->presetBuffer);
                }
                param_copy = *p_presetBuffer;
                sprintf(str, "%10i", (int) param_copy);
                ssd1306ClearRectAtLine(0, 1, 128);
                ssd1306DrawStringAtLine(0, 1, str, &Font_8x8);
                ssd1306Refresh();
                break;

            case JOY_DOWN:
                p_presetBuffer--;
                if (p_presetBuffer == (int *) (preset->presetBuffer))
                {
                    // Set pointer to the last element of the preset buffer
                    p_presetBuffer = &(((int *) preset->presetBuffer)[presetBufferLen]);
                }
                param_copy = *(p_presetBuffer);
                sprintf(str, "%10i", (int) param_copy);
                ssd1306ClearRectAtLine(0, 1, 128);
                ssd1306DrawStringAtLine(0, 1, str, &Font_8x8);
                ssd1306Refresh();
                break;

            case JOY_RIGHT:
                // Validate button
                *((int *) (preset->p_value)) = param_copy;
                if (preset->callback != NULL)
                {
                    // Call the callback function
                    preset->callback(param_copy, NULL);
                    // TODO: Check returned value if needed
                }
                ssd1306Refresh();
                return SUCCESS;

            default:
                break;
        }
    }

    return SUCCESS;
}

void graphMotorSettings(float *acceleration, float *maxSpeed, float *deceleration)
{
    int number_value = 0;
    float* values[3] = { acceleration, maxSpeed, deceleration };
    while (true)
    {
        printGraphMotor(*acceleration, *maxSpeed, *deceleration);
        switch (expanderJoyFiltered())
        {
            case JOY_LEFT:
                if (number_value <= 0)
                {
                    return;
                }
                else
                {
                    number_value--;
                }
                break;
            case JOY_DOWN:
                *(values[number_value]) -= 0.1;
                break;
            case JOY_UP:
                *(values[number_value]) += 0.1;
                break;
            case JOY_RIGHT:
                if (number_value >= 2)
                {
                    return;
                }
                else
                {
                    number_value++;
                }
                break;
            default:
                break;
        }
    }
}
void printGraphMotor(float acceleration, float maxSpeed, float deceleration)
{
    char str[10];
    ssd1306ClearScreen(MAIN_AREA);
    char point1[2] = { (char) ((0 - maxSpeed) / (0 - acceleration)), 64 - (char) maxSpeed };
    char point2[2] = { (char) (128 - (0 - maxSpeed) / (0 - deceleration)), 64 - (char) (maxSpeed) };
    ssd1306DrawLine(0, 64, point1[0], point1[1]);
    ssd1306DrawLine(point1[0], point1[1], point2[0], point2[1]);
    ssd1306DrawLine(point2[0], point1[1], 128, 64);

    sprintf(str, "%.2fM.S^2", acceleration);
    ssd1306DrawString((0 + point1[0]) / 2 + 2, (64 + point1[1]) / 2 + 2, str, &Font_3x6);

    sprintf(str, "%.2fM.S", maxSpeed);
    ssd1306DrawString((point1[0] + point2[0]) / 2, (point1[1] + point2[1]) / 2, str, &Font_3x6);

    sprintf(str, "%.2fM.S^2", deceleration);
    ssd1306DrawString((point2[0] + 128) / 2 - 27, (point2[1] + 64) / 2, str, &Font_3x6);
    ssd1306Refresh();
}
void welcomeDisplay()
{

    ssd1306ClearScreen(MAIN_AREA);
    ssd1306DrawBmp(pacabot_Img, 1, 1, 128, 40);
    ssd1306Refresh();
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
        ;

    for (int i = 0; i <= 100; i += 1)
    {
        ssd1306ProgressBar(10, 35, i);
        ssd1306Refresh();
        HAL_Delay(5);
    }
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306DrawBmp(fiveYears_Img, 1, 1, 128, 54);
    ssd1306Refresh();
}

void powerOffConfirmation()
{
    unsigned char confirm = FALSE;
    modifyBoolParam("TURN POWER OFF ?", &confirm);
    if (confirm == TRUE)
    {
        //modifyBoolParam("SAVE PARAM ?", &confirm);
        if (confirm == TRUE)
        {
//            save_setting();
        }
        halt();
        while (1)
            ;
    }
}
void killOnLowBattery()
{
    //	if(multimeterGetBatVoltage() < (BATTERY_LOWER_VOLTAGE_NO_LOAD)*1000)
    //	{
    //		tone(A2,500);
    //		HAL_Delay(400);
    //		halt();
    //	}
}
