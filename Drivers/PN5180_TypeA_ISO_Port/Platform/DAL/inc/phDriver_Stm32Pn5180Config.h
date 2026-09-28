#ifndef PHDRIVER_STM32_PN5180_CONFIG_H
#define PHDRIVER_STM32_PN5180_CONFIG_H

/* STM32/PN5180 board configuration for the phDriver abstraction layer. */


#include "main.h"

/* Logical IDs passed as dwPinNumber; phDriver maps them to STM32 GPIOs. */
#define PHDRIVER_PIN_RESET   0x01U
#define PHDRIVER_PIN_IRQ     0x02U
#define PHDRIVER_PIN_BUSY    0x03U
#define PHDRIVER_PIN_DWL     0x04U
#define PHDRIVER_PIN_SSEL    0x05U

/* Board pull-selection aliases used by the NXP DAL semantic configuration.
    these definitions configure board-specific pins, pulls, reset levels, and IRQ behavior 
*/
#define PULL_DOWN                      0U
#define PULL_UP                        1U
#define PHDRIVER_PIN_RESET_PULL_CFG    PULL_UP
#define PHDRIVER_PIN_NSS_PULL_CFG      PULL_UP
#define PHDRIVER_PIN_IRQ_PULL_CFG      PULL_DOWN
#define PHDRIVER_PIN_BUSY_PULL_CFG     PULL_DOWN
#define PHDRIVER_PIN_DWL_PULL_CFG      PULL_DOWN

#define PH_DRIVER_SET_HIGH           1U
#define PH_DRIVER_SET_LOW            0U
#define RESET_POWERUP_LEVEL          PH_DRIVER_SET_HIGH
#define RESET_POWERDOWN_LEVEL        PH_DRIVER_SET_LOW
#define PIN_IRQ_TRIGGER_TYPE         PH_DRIVER_INTERRUPT_RISINGEDGE


#endif /* PHDRIVER_STM32_PN5180_CONFIG_H */
