/**************************************************************************/
/*!
 @file     expander.c
 @author  PLF (PACABOT)
 @date
 @version  0.0

 Driver for expander PCF8574
 */
/**************************************************************************/
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
#include "peripherals/bluetooth/bluetooth.h"

/* Middleware declarations */

/* Declarations for this module */
#include "peripherals/expander/pcf8574.h"

#define EXPANDER_I2C_ADDRESS            (0x20 << 1)

#define NORMAL_DELAY_REAPEAT            400
#define FAST_DELAY_REAPEAT              70
#define DONE                            0

/* extern variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
unsigned int joy_activ_old_time;

//Send DATA
static void sendData(uint8_t val)
{
    static uint8_t aTx;
    aTx = val;
    // I2C
    //	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
        return;
//    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)EXPANDER_I2C_ADDRESS, (uint8_t*)&aTxBuffer, 1, 1000);
    HAL_I2C_Master_Transmit_DMA(&hi2c1, (uint16_t)EXPANDER_I2C_ADDRESS, (uint8_t*)&aTx, 1);//, 1000);

}

//get DATA
static char getData(void)
{
    // I2C
    static uint8_t aRx;

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Receive(&hi2c1, (uint16_t)EXPANDER_I2C_ADDRESS, (uint8_t*)&aRx, 1, 1000);

    return aRx;
}

void expanderInit(void)
{
//    sendData(0xFF);
    uint8_t aTxBuffer = 0xFF;
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)EXPANDER_I2C_ADDRESS, (uint8_t*)&aTxBuffer, 1, 1000);
}

void expanderSetbit(char pin, char val)
{
    if (val == 1)
    {
        sendData(getData() | (0x1 << pin));
    }
    else
    {
        sendData(getData() & ~(0x1 << pin));
    }
}

char expanderGetbit(char pin)
{
    return getData();
}

void expanderLedState(char led, char val)
{
    switch (led)
    {
        case 3:
            expanderSetbit(4, reverse_bit(val));
            break;
        case 2:
            expanderSetbit(5, reverse_bit(val));
            break;
        case 1:
            expanderSetbit(6, reverse_bit(val));
            break;
    }
}

void expanderSetLeds(char leds)
{
    sendData((char)(0xFF & (~(leds << 4))));
}

char expanderJoyState(void)
{
    switch ((~getData()) & 0x0F)
    {
        case 4:
            //		bluetoothPrintf("Joystick UP\n");
            return JOY_UP;
            break;
        case 1:
            //		bluetoothPrintf("Joystick DOWN\n");
            return JOY_DOWN;
            break;
        case 8:
            //		bluetoothPrintf("Joystick LEFT\n");
            return JOY_LEFT;
            break;
        case 2:
            //		bluetoothPrintf("Joystick RIGHT\n");
            return JOY_RIGHT;
            break;
        default:
            //bluetoothPrintf("Joystick ERROR\n");
            return 0;
    }
}
char expanderJoyFiltered(void)
{
    char joy_state = expanderJoyState();
    if (antiBounceJoystick2(joy_state) == DONE)
        return joy_state;
    return 0;
}

void antiBounceJoystick(void)
{
    unsigned long int time_base = HAL_GetTick();
    do
    {
        if (expanderJoyState() != 0)
            time_base = HAL_GetTick();
    }
    while (time_base != (HAL_GetTick() - 20));
}

char antiBounceJoystick2(char arrow_type)
{
    static long time_base = 0;
    static char old_arrow_type = 0;
    static char fast_clic = false;
    long time = HAL_GetTick();

    if (arrow_type == JOY_LEFT || arrow_type == JOY_RIGHT) //no repeat for LEFT and RIGHT keys
    {
        if (old_arrow_type != arrow_type)
        {
            old_arrow_type = arrow_type;
            return DONE;
        }
        else
            return -1;
    }
    else if (old_arrow_type != arrow_type)
    {
        joy_activ_old_time = time;
        old_arrow_type = arrow_type;
        time_base = time;
        fast_clic = false;
        return DONE;
    }
    else if ((fast_clic == true) && ((time_base + FAST_DELAY_REAPEAT) < time))
    {
        time_base = time;
        return DONE;
    }
    else if (((time_base + NORMAL_DELAY_REAPEAT) < time) && (fast_clic == false))
    {
        time_base = time;
        fast_clic = true;
        return DONE;
    }

    return -1;
}

void joystickTest(void)
{
    int state;

    while (expanderJoyState() != JOY_LEFT)
    {
        state = expanderJoyState();

        ssd1306ClearScreen(MAIN_AREA);
        ssd1306DrawCircle(60, 20, 3);
        ssd1306DrawCircle(60, 40, 3);
        ssd1306DrawCircle(50, 30, 3);
        ssd1306DrawCircle(70, 30, 3);
        switch (state)
        {
            case JOY_UP:
                ssd1306FillCircle(60, 20, 3);
                break;
            case JOY_DOWN:
                ssd1306FillCircle(60, 40, 3);
                break;
            case JOY_LEFT:
                ssd1306FillCircle(50, 30, 3);
                break;
            case JOY_RIGHT:
                ssd1306FillCircle(70, 30, 3);
                break;
        }
        ssd1306Refresh();
    }
    ssd1306FillCircle(50, 30, 3);
    ssd1306Refresh();
    HAL_Delay(1000);
    antiBounceJoystick();
}

void expenderLedTest()
{
        expanderSetLeds(0b100);
        HAL_Delay(250);
        expanderSetLeds(0b110);
        HAL_Delay(250);
        expanderSetLeds(0b111);
        HAL_Delay(250);

    antiBounceJoystick();
    return;
}
