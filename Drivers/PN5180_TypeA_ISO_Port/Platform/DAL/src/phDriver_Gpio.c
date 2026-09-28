#include "phDriver.h"
#include "stm32g0xx_hal.h"

static uint32_t phDriver_PinIndex(uint16_t pin)
{
    uint32_t index = 0U;
    while (((uint32_t)pin >> index) > 1U)
    {
        index++;
    }
    return index;
}

static uint32_t phDriver_ReadMode(GPIO_TypeDef *port, uint16_t pin)
{
    uint32_t index = phDriver_PinIndex(pin);
    return (port->MODER >> (index * 2U)) & 0x03U;
}

static uint32_t phDriver_ReadPull(GPIO_TypeDef *port, uint16_t pin)
{
    uint32_t index = phDriver_PinIndex(pin);
    return (port->PUPDR >> (index * 2U)) & 0x03U;
}

static phStatus_t phDriver_CheckPinConfig(GPIO_TypeDef *port, uint16_t pin,
    phDriver_Pin_Func_t ePinFunc, const phDriver_Pin_Config_t *config)
{
    uint32_t mode = phDriver_ReadMode(port, pin);
    uint32_t pull = phDriver_ReadPull(port, pin);
    uint32_t mask = pin;
    uint32_t rising = (EXTI->RTSR1 & mask) != 0U;
    uint32_t falling = (EXTI->FTSR1 & mask) != 0U;

    /* Check whether the GPIO mode differs from the requested input/output function. */
    if ((ePinFunc == PH_DRIVER_PINFUNC_OUTPUT && mode != 1U) ||
        ((ePinFunc == PH_DRIVER_PINFUNC_INPUT || ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT) && mode != 0U))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    /* Check whether the requested pull-up or pull-down setting is missing. */
    if ((config->bPullSelect == PH_DRIVER_PULL_UP && pull != 1U) ||
        (config->bPullSelect == PH_DRIVER_PULL_DOWN && pull != 2U))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    /* Check whether the output latch level differs from the requested logic level. */
    if ((ePinFunc == PH_DRIVER_PINFUNC_OUTPUT) &&
        (((port->ODR & pin) != 0U) != (config->bOutputLogic != 0U)))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    /* Check whether interrupt configuration also needs to be validated. */
    if (ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT)
    {
        /* Check whether the EXTI line's interrupt is masked (disabled). */
        if ((EXTI->IMR1 & mask) == 0U)
        {
            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
        /* Check whether rising-edge-only triggering is requested but not configured. */
        if ((config->eInterruptConfig == PH_DRIVER_INTERRUPT_RISINGEDGE) && (!rising || falling))
        {
            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
        /* Check whether falling-edge-only triggering is requested but not configured. */
        if ((config->eInterruptConfig == PH_DRIVER_INTERRUPT_FALLINGEDGE) && (rising || !falling))
        {
            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
        /* Check whether both edges are requested but either trigger is disabled. */
        if ((config->eInterruptConfig == PH_DRIVER_INTERRUPT_EITHEREDGE) && (!rising || !falling))
        {
            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
    }
    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
}

phStatus_t phDriver_PinConfig(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc,
    phDriver_Pin_Config_t *pPinConfig)
{
    void *port = NULL;
    uint16_t pin = 0U;
    phStatus_t status = phDriver_PinMap(dwPinNumber, &port, &pin);
    if ((status != (PH_DRIVER_SUCCESS | PH_COMP_DRIVER)) || (pPinConfig == NULL))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }
    if ((ePinFunc != PH_DRIVER_PINFUNC_INPUT) &&
        (ePinFunc != PH_DRIVER_PINFUNC_OUTPUT) &&
        (ePinFunc != PH_DRIVER_PINFUNC_INTERRUPT))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }
    return phDriver_CheckPinConfig((GPIO_TypeDef *)port, pin, ePinFunc, pPinConfig);
}

uint8_t phDriver_PinRead(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc)
{
    void *port = NULL;
    uint16_t pin = 0U;
    (void)ePinFunc;

    if (PH_DRIVER_SUCCESS != (phDriver_PinMap(dwPinNumber, &port, &pin) & PH_ERR_MASK))
    {
        return 0U;
    }
    return (HAL_GPIO_ReadPin((GPIO_TypeDef *)port, pin) == GPIO_PIN_SET) ? 1U : 0U;
}

phStatus_t phDriver_IRQPinRead(uint32_t dwPinNumber)
{
    void *port = NULL;
    uint16_t pin = 0U;
    return (phDriver_PinMap(dwPinNumber, &port, &pin) == (PH_DRIVER_SUCCESS | PH_COMP_DRIVER)) ?
        (phStatus_t)phDriver_PinRead(dwPinNumber, PH_DRIVER_PINFUNC_INPUT) : 0U;
}

phStatus_t phDriver_IRQPinPoll(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc,
    phDriver_Interrupt_Config_t eInterruptType)
{
    uint8_t idleLevel;
    if ((eInterruptType != PH_DRIVER_INTERRUPT_RISINGEDGE) &&
        (eInterruptType != PH_DRIVER_INTERRUPT_FALLINGEDGE))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }
    idleLevel = (eInterruptType == PH_DRIVER_INTERRUPT_RISINGEDGE) ? 0U : 1U;
    while (phDriver_PinRead(dwPinNumber, ePinFunc) == idleLevel) { }
    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
}

void phDriver_PinWrite(uint32_t dwPinNumber, uint8_t bValue)
{
    void *port = NULL;
    uint16_t pin = 0U;
    if (phDriver_PinMap(dwPinNumber, &port, &pin) == (PH_DRIVER_SUCCESS | PH_COMP_DRIVER))
    {
        HAL_GPIO_WritePin((GPIO_TypeDef *)port, pin,
            (bValue != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void phDriver_PinClearIntStatus(uint32_t dwPinNumber)
{
    void *port = NULL;
    uint16_t pin = 0U;
    if (phDriver_PinMap(dwPinNumber, &port, &pin) == (PH_DRIVER_SUCCESS | PH_COMP_DRIVER))
    {
        (void)port;
        __HAL_GPIO_EXTI_CLEAR_IT(pin);
    }
}
