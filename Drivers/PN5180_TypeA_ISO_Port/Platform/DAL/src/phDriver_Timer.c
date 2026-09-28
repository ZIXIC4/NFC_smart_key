/**
 * @file phDriver_Timer.c
 * @brief STM32 timer abstraction implementation.
 *
 * This initial implementation uses HAL_Delay for blocking delays.  An
 * asynchronous callback requires a board-specific hardware timer and is
 * therefore rejected until such a timer is supplied.
 */

#include "phDriver.h"
#include "stm32g0xx_hal.h"

static pphDriver_TimerCallBck_t volatile sCallback = NULL;

extern TIM_HandleTypeDef htim2;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        pphDriver_TimerCallBck_t callback;

        /* Make TIM2 operate as a single-shot timer. */
        (void)HAL_TIM_Base_Stop_IT(htim);

        callback = sCallback;
        sCallback = NULL;

        if (callback != NULL)
        {
            callback();
        }
       
    }
}

phStatus_t phDriver_TimerStart(
    phDriver_Timer_Unit_t eTimerUnit,
    uint32_t dwTimePeriod,
    pphDriver_TimerCallBck_t pTimerCallBack)
{
    uint32_t delayMs;
    
    if (dwTimePeriod == 0U)
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    switch (eTimerUnit)
    {
    case PH_DRIVER_TIMER_SECS:
        delayMs = (uint32_t)dwTimePeriod * 1000U;
        break;
    case PH_DRIVER_TIMER_MILLI_SECS:
        delayMs = dwTimePeriod;
        break;
    case PH_DRIVER_TIMER_MICRO_SECS:
        /* HAL_Delay has millisecond resolution; round non-zero values up. */
        delayMs = ((uint32_t)dwTimePeriod + 999U) / 1000U;
        break;
    default:
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    if (delayMs > 5000U)
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }
    
    
    if (pTimerCallBack == NULL)
    {
        HAL_Delay(delayMs);
    }else
    {
        uint32_t delayUs = delayMs * 1000U;
        sCallback = pTimerCallBack;
        HAL_TIM_Base_Stop_IT(&htim2);

        __HAL_TIM_SET_COUNTER(&htim2, 0U);
        __HAL_TIM_SET_AUTORELOAD(&htim2, delayUs - 1U);
        __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

        if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
        {
            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
    }
    
    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
}

phStatus_t phDriver_TimerStop(void)
{
    if (HAL_TIM_Base_Stop_IT(&htim2) != HAL_OK)
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    sCallback = NULL;

    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
    
}
