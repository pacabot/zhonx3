/**************************************************************************/
/*!
    @file    TimeBase.c
    @author  PLF (PACABOT)
    @date    03 August 2014
    @version  0.10
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

#include "middleware/settings/settings.h"

/* Peripheral declarations */
#include "peripherals/display/ssd1306.h"
#include "peripherals/display/smallfonts.h"
#include "peripherals/expander/pcf8574.h"
#include "peripherals/tone/tone.h"

/* Middleware declarations */
#include "middleware/controls/pidController/pidController.h"
#include "middleware/display/banner.h"

/* Declarations for this module */
#include "peripherals/times_base/times_base.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

GPIO_InitTypeDef GPIO_InitStruct;

volatile int32_t Blink[3] = {500, 10, 0};

/**
 * @brief  Period elapsed callback in non blocking mode
 * @param  htim : TIM handle
 * @retval None
 */
void timesBaseInit(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	uint32_t uwPrescalerValue = 0;

	/*##-1- Configure the TIM peripheral #######################################*/
	/* -----------------------------------------------------------------------
	    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
	    since APB1 prescaler is different from 1.
	      TIM3CLK = 2 * PCLK1
	      PCLK1 = HCLK / 4
	      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
	    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
	    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	    Prescaler = ((SystemCoreClock /2) /10 KHz) - 1
	      ----------------------------------------------------------------------- */

	/* Compute the prescaler value to have TIM7 counter clock equal to 1 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / (HI_TIME_FREQ * 2));
	htim7.Instance = TIM7;
	htim7.Init.Prescaler =  uwPrescalerValue;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 2-1;
	htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim7);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim7);

	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / (LOW_TIME_FREQ * 100));

	htim6.Instance = TIM6;
	htim6.Init.Prescaler =  uwPrescalerValue;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 100-1;
	HAL_TIM_Base_Init(&htim6);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

	if(HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
	{
		/* Starting Error */
		//	    Error_Handler();
	}

	/*## Configure the TIM peripheral for ADC123 injected trigger ####################*/
	/* -----------------------------------------------------------------------
	 	    Use TIM5 for start Injected conversion on ADC1 (gyro rate).
	 	    Use TIM5 for start Injected conversion on ADC2 (not use).
	 	    Use TIM5 for start Injected conversion on ADC3 (not use).
	 	     ----------------------------------------------------------------------- */

	/* Compute the prescaler value to have TIM5 counter clock equal to 4 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / (GYRO_TIME_FREQ * 100));

	htim5.Instance = TIM5;
	htim5.Init.Prescaler =  uwPrescalerValue;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 100-1;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	//	  htim5.Init.RepetitionCounter = 0x0;
	HAL_TIM_Base_Init(&htim5);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;			//TIM_TRGO_UPDATE see adc.c => ADC1 injected section
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim5);

	/*## Configure the TIM peripheral for ADC23 regular trigger ####################*/
	/* -----------------------------------------------------------------------
	 	    Use TIM2 for start Regular conversion on ADC2 (telemeters, just use in timer base).
	 	    Use TIM2 for start Regular conversion on ADC3 (not use).
	 	     ----------------------------------------------------------------------- */

	/* Compute the prescaler value to have TIM2 counter clock equal to 16 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / (TELEMETERS_TIME_FREQ * 100));

	htim2.Instance = TIM2;
	htim2.Init.Prescaler =  uwPrescalerValue;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 100-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.RepetitionCounter = 0x0;
	HAL_TIM_Base_Init(&htim2);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim2);

	/*## Configure the TIM peripheral for ADC1 regular trigger ####################*/
	/* -----------------------------------------------------------------------
	 	    Use TIM4 for start Regular conversion on ADC1 (vbat, gyro_temp, internal_temps, internal vbat).
	 	     ----------------------------------------------------------------------- */

	/* Compute the prescaler value to have TIM4 counter clock equal to 1 Hz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / (MULTIMMETER_TIME_FREQ * 10000));

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = uwPrescalerValue;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 10000-1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim4);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	HAL_TIM_OC_Init(&htim4);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
	sConfigOC.Pulse = 1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

	HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_4);
}

char timeOut(unsigned char second, int loop_nb)
{
	static int old_tick;
	if (loop_nb == 0)
	{
		old_tick = HAL_GetTick();
	}
	if (((HAL_GetTick() - old_tick) / 1000) < second)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void ledPowerBlink(unsigned int off_time, unsigned int on_time, unsigned int repeat)
{
	Blink[0] = (off_time / 10);
	Blink[1] = (on_time / 10);
	Blink[2] = (repeat / 10);
}

void ledBlink_IT(void)
{
	static int cnt_led = 0;

	if (Blink[0] == 0 && Blink[1] == 0)
	{
		return;
	}

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	cnt_led++;
	if(cnt_led <= (Blink[0]))
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, SET);
	}
	if(cnt_led >= ((Blink[0]+Blink[1])))
		cnt_led = 0;
}

void sleep_mode_IT()//todo move this function
{
#ifdef KILL_WEN_UNUSED
	if((zhonxSettings.sleep_delay_s != 0) && (HAL_GetTick() > (joy_activ_old_time + zhonxSettings.sleep_delay_s*1000)))
	{
		halt();
	}
#endif
}

void highFreq_IT(void)
{
}

void lowFreq_IT(void)
{
	tone_IT();
	banner_IT();
	sleep_mode_IT();
	ledBlink_IT();
}

