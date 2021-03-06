/*
 * telemetersCal.c
 *
 *  Created on: 10 avr. 2015
 *      Author: Colin
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

/* Application declarations */
#include "application/statistiques/statistiques.h"

/* Peripheral declarations */
#include "peripherals/display/ssd1306.h"
#include "peripherals/display/smallfonts.h"
#include "peripherals/expander/pcf8574.h"
#include "peripherals/display/ssd1306.h"
#include "peripherals/expander/pcf8574.h"
#include "peripherals/bluetooth/bluetooth.h"
#include "peripherals/motors/motors.h"
#include "peripherals/encoders/ie512.h"
#include "peripherals/eeprom/24lc64.h"
#include "peripherals/flash/flash.h"

/* Middleware declarations */
#include "middleware/math/kalman_filter.h"
#include "middleware/controls/mainControl/mainControl.h"
#include "middleware/moves/basicMoves/basicMoves.h"
#include "middleware/settings/settings.h"

/* Declarations for this module */
#include "peripherals/telemeters/telemeters.h"

#if (MEASURED_DISTANCE)%(TELEMETER_PROFILE_ARRAY_LENGTH) != 0
#error  MEASURED_DISTANCE of cell must be a multiple of NUMBER_OF_CELL
#endif

int wallSensorsCalibrationFront(void)
{
    FRONT_TELEMETERS_PROFILE front_telemeters;
    int i;
    int rv;

    ssd1306ClearScreen(MAIN_AREA);
    ssd1306DrawStringAtLine(0, 1, "Place the robot front", &Font_5x8);
    ssd1306DrawStringAtLine(0, 2, "of wall and press 'RIGHT'", &Font_5x8);
    ssd1306Refresh();

    while (expanderJoyFiltered() != JOY_RIGHT);

    ssd1306ProgressBar(10, 10, 0);
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(0, 1, &Font_5x8, "Calibrating front sensors");
    ssd1306Refresh();

    HAL_Delay(1000);
    mainControlSetFollowType(NO_FOLLOW);

    telemetersStart();

    // take the measures
    basicMoveStraight(-MEASURED_DISTANCE, 15, 15, 200);
    ssd1306Refresh();
    for (i = 0; i < TELEMETER_PROFILE_ARRAY_LENGTH; i++)
    {
        while ((((int) (encoderGetDist(ENCODER_L) + encoderGetDist(ENCODER_R))
                >= -(NUMBER_OF_MILLIMETER_BY_LOOP * 2 * i))) && (hasMoveEnded() != TRUE));

        front_telemeters.left[i] = getTelemeterAvrg(TELEMETER_FL);
        front_telemeters.right[i] = getTelemeterAvrg(TELEMETER_FR);

        ssd1306ClearScreen(MAIN_AREA);

        ssd1306PrintIntAtLine(0, 3, "FL", (uint32_t) getTelemeterAvrg(TELEMETER_FL), &Font_5x8);
        ssd1306PrintIntAtLine(0, 4, "FR", (uint32_t) getTelemeterAvrg(TELEMETER_FR), &Font_5x8);

        ssd1306ProgressBar(10, 10, (i * 100) / TELEMETER_PROFILE_ARRAY_LENGTH);
        ssd1306Refresh();
    }
    while (hasMoveEnded() != TRUE);
    telemetersStop();
    motorsDriverSleep(ON);

    // filter the measure
    kalman_filter_array(front_telemeters.left, TELEMETER_PROFILE_ARRAY_LENGTH);
    kalman_filter_array(front_telemeters.right, TELEMETER_PROFILE_ARRAY_LENGTH);

    /*
     * this four line are for make sure in search for convert in millimeter we don't go outside
     * of the array because we have stronger or smaller value than during the calibration
     */
    front_telemeters.left[0] = 4095;
    front_telemeters.right[0] = 4095;
    front_telemeters.left[TELEMETER_PROFILE_ARRAY_LENGTH] = 0;
    front_telemeters.right[TELEMETER_PROFILE_ARRAY_LENGTH] = 0;

    // save the measures
    bluetoothPrintf("\nfilterd front measures :\n");

    for (int i = 0; i < TELEMETER_PROFILE_ARRAY_LENGTH; i++)
    {
        bluetoothWaitReady();
        bluetoothPrintf("%d|%d|%d\n", i, front_telemeters.left[i], front_telemeters.right[i]);
    }
    bluetoothPrintf("Saving Front telemeters profile into Flash memory...\n");
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(0, 1, &Font_5x8, "save into Flash memory...");
    ssd1306Refresh();

    bluetoothPrintf("Saving Diagonal telemeters profile into Flash memory...\n");
    // Write telemeters profiles in Flash memory
    rv = flash_write(zhonxSettings.h_flash, (unsigned char *) &telemeters_profile->front,
                     (unsigned char *) &front_telemeters, sizeof(FRONT_TELEMETERS_PROFILE));
    if (rv == FLASH_E_SUCCESS)
    {
        bluetoothPrintf("Values saved into Flash Memory\n");
        ssd1306ClearScreen(MAIN_AREA);
        ssd1306PrintfAtLine(0, 1, &Font_5x8, "FLASH memory updated");
        ssd1306Refresh();
    }
    else
    {
        bluetoothPrintf("Failed to write Flash Memory (%d)\n", rv);
        ssd1306ClearScreen(MAIN_AREA);
        ssd1306PrintfAtLine(0, 1, &Font_5x8, "FLASH write error (%d)", rv);
        ssd1306Refresh();
    }
    HAL_Delay(3000);
    return TELEMETERS_DRIVER_E_SUCCESS;
}

int wallSensorsCalibrationDiag(void)
{
    DIAG_TELEMETERS_PROFILE diag_telemeters;
    int i;
    int rv;

    ssd1306ClearScreen(MAIN_AREA);
    ssd1306DrawStringAtLine(0, 1, "Place the robot front", &Font_5x8);
    ssd1306DrawStringAtLine(0, 2, "of wall and press 'RIGHT'", &Font_5x8);
    ssd1306Refresh();

    while (expanderJoyFiltered() != JOY_RIGHT);

    ssd1306ProgressBar(10, 10, 0);
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(0, 1, &Font_5x8, "Calibrating diag sensors");
    ssd1306Refresh();

    HAL_Delay(1000);
    mainControlSetFollowType(NO_FOLLOW);

    double number_of_millimeter_per_loop = sqrt(2 * pow(NUMBER_OF_MILLIMETER_BY_LOOP, 2));
    telemetersStart();

    // take the measures
    basicMoveStraight(-number_of_millimeter_per_loop * TELEMETER_PROFILE_ARRAY_LENGTH, 15, 15, 200);
    for (i = 0; i < TELEMETER_PROFILE_ARRAY_LENGTH; i++)
    {
        while ((((int) (encoderGetDist(ENCODER_L) + encoderGetDist(ENCODER_R))
                >= -(number_of_millimeter_per_loop * 2.00 * (double) i))) && (hasMoveEnded() != TRUE));

        diag_telemeters.left[i] = getTelemeterAvrg(TELEMETER_DL);
        diag_telemeters.right[i] = getTelemeterAvrg(TELEMETER_DR);

        ssd1306ClearScreen(MAIN_AREA);

        ssd1306PrintIntAtLine(0, 3, "DL", (uint32_t) getTelemeterAvrg(TELEMETER_DL), &Font_5x8);
        ssd1306PrintIntAtLine(0, 4, "DR", (uint32_t) getTelemeterAvrg(TELEMETER_DR), &Font_5x8);

        ssd1306ProgressBar(10, 10, (i * 100) / TELEMETER_PROFILE_ARRAY_LENGTH);
        ssd1306Refresh();
    }
    while (hasMoveEnded() != TRUE);
    telemetersStop();
    motorsDriverSleep(ON);

    // filter the measure
    kalman_filter_array(diag_telemeters.left, TELEMETER_PROFILE_ARRAY_LENGTH);
    kalman_filter_array(diag_telemeters.right, TELEMETER_PROFILE_ARRAY_LENGTH);

    /*
     * this four line are for make sure in search for convert in millimeter we don't go outside
     * of the array because we have stronger or smaller value than during the calibration
     */
    diag_telemeters.left[0] = 4095;
    diag_telemeters.right[0] = 4095;
    diag_telemeters.left[TELEMETER_PROFILE_ARRAY_LENGTH] = 0;
    diag_telemeters.right[TELEMETER_PROFILE_ARRAY_LENGTH] = 0;

    // save the measures
    bluetoothPrintf("\nfilterd diag measures :\n");

    for (int i = 0; i < TELEMETER_PROFILE_ARRAY_LENGTH; i++)
    {
        bluetoothWaitReady();
        bluetoothPrintf("%d|%d|%d\n", i, diag_telemeters.left[i], diag_telemeters.right[i]);
    }
    bluetoothPrintf("Saving Front telemeters profile into Flash memory...\n");
    ssd1306ClearScreen(MAIN_AREA);
    ssd1306PrintfAtLine(0, 1, &Font_5x8, "save into Flash memory...");
    ssd1306Refresh();

    bluetoothPrintf("Saving Diagonal telemeters profile into Flash memory...\n");
    // Write telemeters profiles in Flash memory
    rv = flash_write(zhonxSettings.h_flash, (unsigned char *) &telemeters_profile->diag,
                     (unsigned char *) &diag_telemeters, sizeof(DIAG_TELEMETERS_PROFILE));
    if (rv == FLASH_E_SUCCESS)
    {
        bluetoothPrintf("Values saved into Flash Memory\n");
        ssd1306ClearScreen(MAIN_AREA);
        ssd1306PrintfAtLine(0, 1, &Font_5x8, "FLASH memory updated");
        ssd1306Refresh();
    }
    else
    {
        bluetoothPrintf("Failed to write Flash Memory (%d)\n", rv);
        ssd1306ClearScreen(MAIN_AREA);
        ssd1306PrintfAtLine(0, 1, &Font_5x8, "FLASH write error (%d)", rv);
        ssd1306Refresh();
    }
    HAL_Delay(3000);
    return TELEMETERS_DRIVER_E_SUCCESS;
}
