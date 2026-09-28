#include "phDriver.h"
#include "stm32g0xx_hal.h"
#include "phDriver_Stm32Pn5180Config.h"

phStatus_t phDriver_PinMap(uint32_t dwPinNumber, void **ppPort, uint16_t *pPin)
{
    GPIO_TypeDef *port = NULL;
    uint16_t pin = 0U;

    switch (dwPinNumber)
    {
    case PHDRIVER_PIN_RESET:
        port = RESET_N_GPIO_Port;
        pin = RESET_N_Pin;
        break;
    case PHDRIVER_PIN_IRQ:
        port = IRQ_GPIO_Port;
        pin = IRQ_Pin;
        break;
    case PHDRIVER_PIN_BUSY:
        port = BUSY_GPIO_Port;
        pin = BUSY_Pin;
        break;
    case PHDRIVER_PIN_DWL:
        port = REQ_GPIO_Port;
        pin = REQ_Pin;
        break;
    case PHDRIVER_PIN_SSEL:
        port = SPI1_NSS_GPIO_Port;
        pin = SPI1_NSS_Pin;
        break;
    default:
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    if ((ppPort == NULL) || (pPin == NULL))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }
    *ppPort = (void *)port;
    *pPin = pin;
    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
}
