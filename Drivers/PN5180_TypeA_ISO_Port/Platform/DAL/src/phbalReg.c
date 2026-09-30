/**
 * @file phbalReg.c
 * @brief STM32 SPI Bus Abstraction Layer implementation skeleton.
 *
 */

#include "phDriver.h"
#include "stm32g0xx_hal.h"

extern SPI_HandleTypeDef hspi1;

/**
 * @brief Initialize the BAL.
 */
phStatus_t phbalReg_Init(
    void * pDataParams,          /**< [In] BAL parameter structure phbalReg_Type_t. */
    uint16_t wSizeOfDataParams   /**< [In] Size of the parameter structure. */
    )
{
    phbalReg_Stm32Spi_DataParams_t *pspi = (phbalReg_Stm32Spi_DataParams_t *) pDataParams;
    pspi -> sBalReg.wId = PH_COMP_BAL | PHBAL_REG_STM32_SPI_ID;
    pspi -> sBalReg.bBalType = PHBAL_REG_TYPE_SPI;
    pspi->hSpi = &hspi1;
    pspi->read_timeout = 100U;
    pspi->write_timeout = 100U;

    if (pspi -> hSpi == NULL)
    {
        return PH_DRIVER_ERROR;
    }   
    

    return PH_ERR_SUCCESS;
}

/**
 * @brief Perform data transmit/receive/exchange on the bus.
 */
phStatus_t phbalReg_Exchange(
    void * pDataParams,         /**< [In] BAL parameter structure. */
    uint16_t wOption,           /**< [In] Option parameter, for future use. */
    uint8_t * pTxBuffer,        /**< [In] Data to transmit. */
    uint16_t wTxLength,         /**< [In] Number of bytes to transmit. */
    uint16_t wRxBufSize,        /**< [In] Receive buffer size / number of bytes to receive. */
    uint8_t * pRxBuffer,        /**< [Out] Received data. */
    uint16_t * pRxLength        /**< [Out] Number of received data bytes. */
    )
{
    phbalReg_Stm32Spi_DataParams_t *pspi = (phbalReg_Stm32Spi_DataParams_t *) pDataParams;

    if (pRxBuffer == NULL && (pTxBuffer != NULL && wTxLength > 0))
    {
        // Transmit 
        if (HAL_SPI_Transmit(pspi -> hSpi, pTxBuffer, wTxLength, pspi -> write_timeout)!= HAL_OK)
        {

            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
    }
    else if (pTxBuffer == NULL && (pRxBuffer != NULL && wRxBufSize > 0))
    {
        // Receive
        if (HAL_SPI_Receive(pspi -> hSpi, pRxBuffer, wRxBufSize, pspi -> read_timeout)!= HAL_OK)
        {
            
            return PH_DRIVER_ERROR | PH_COMP_DRIVER;
        }
    }
    else 
    {
        return PH_DRIVER_ERROR| PH_COMP_DRIVER;;
    }

    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;;
}

/**
 * @brief Set a configuration parameter. This configuration must been done before calling phbalReg_Exchange.
 */
phStatus_t phbalReg_SetConfig(
    void * pDataParams,         /**< [In] BAL parameter structure. */
    uint16_t wConfig,           /**< [In] Configuration identifier. */
    uint32_t dwValue            /**< [In] Configuration value. */
    )
{
    phbalReg_Stm32Spi_DataParams_t *pspi = (phbalReg_Stm32Spi_DataParams_t *) pDataParams;
    if (wConfig == PHBAL_REG_CONFIG_WRITE_TIMEOUT_MS){
        pspi -> write_timeout = (uint8_t) dwValue;
    }
    else if (wConfig == PHBAL_REG_CONFIG_READ_TIMEOUT_MS){
        pspi -> read_timeout = (uint8_t) dwValue;
    }
    else{
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }       
    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
}

/**
 * @brief Get a configuration parameter.
 */
phStatus_t phbalReg_GetConfig(
    void * pDataParams,         /**< [In] BAL parameter structure. */
    uint16_t wConfig,           /**< [In] Configuration identifier. */
    uint32_t * pValue           /**< [Out] Configuration value. */
    )
{
    phbalReg_Stm32Spi_DataParams_t *pspi = (phbalReg_Stm32Spi_DataParams_t *) pDataParams;

    if (wConfig == PHBAL_REG_CONFIG_WRITE_TIMEOUT_MS){
        *pValue = (uint32_t) pspi -> write_timeout;
    }
    else if (wConfig == PHBAL_REG_CONFIG_READ_TIMEOUT_MS){
        *pValue = (uint32_t) pspi -> read_timeout;
    }
    else{
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    return PH_DRIVER_SUCCESS | PH_COMP_DRIVER;
}
