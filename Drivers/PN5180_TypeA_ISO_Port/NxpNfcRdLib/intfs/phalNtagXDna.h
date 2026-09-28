/*----------------------------------------------------------------------------*/
/* Copyright 2025 NXP                                                         */
/*                                                                            */
/* NXP Confidential. This software is owned or controlled by NXP and may only */
/* be used strictly in accordance with the applicable license terms.          */
/* By expressly accepting such terms or by downloading, installing,           */
/* activating and/or otherwise using the software, you are agreeing that you  */
/* have read, and that you agree to comply with and are bound by, such        */
/* license terms. If you do not agree to be bound by the applicable license   */
/* terms, then you may not retain, install, activate or otherwise use the     */
/* software.                                                                  */
/*----------------------------------------------------------------------------*/

/** \file
 * Generic NATG X DNA Application Component of Reader Library Framework.
 * $Author: NXP $
 * $Revision: $ (v07.14.00)
 * $Date: $
 *
 */

#ifndef PHALNTAGXDNA_H
#define PHALNTAGXDNA_H

#include <ph_Status.h>
#include <ph_TypeDefs.h>
#include <ph_RefDefs.h>

#include <phpalMifare.h>
#include <phhalHw.h>

#ifdef NXPBUILD__PH_CRYPTOSYM
#include <phCryptoSym.h>
#endif /* NXPBUILD__PH_CRYPTOSYM */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHAL_NTAGXDNA
/**
 * \addtogroup phalNtagXDna_CommonDefs
 * @{
 */

/**
 * \defgroup phalNtagXDna_Defines_BufferSize Buffer Size
 * @{
 */
#ifdef NXPBUILD__PH_CRYPTOSYM
#define PHAL_NTAGXDNA_SIZE_TI                                                               4U  /**< Size of Transaction Identifier. */
#define PHAL_NTAGXDNA_SIZE_NONCE                                                            13U /**< Size of CCM Nonce. */
#define PHAL_NTAGXDNA_SIZE_CAPS                                                             6U  /**< Size of PD / PCD capabilities. */
#define PHAL_NTAGXDNA_DIV_INPUT_LEN_MAX                                                     31U /**< Maximum Diversification input length. */
#endif /* NXPBUILD__PH_CRYPTOSYM */
#define PHAL_NTAGXDNA_CMD_BUFFER_SIZE_MINIMUM                                            256U   /**< Minimum size for allocating the command buffer during initializing.
                                                                                                 *   Based on the platform, the value should not go below 128 to avoid issues.
                                                                                                 */
#define PHAL_NTAGXDNA_PRS_BUFFER_SIZE_MINIMUM                                            512U   /**< Minimum size for allocating the response / SM processing buffer during initializing.
                                                                                                 *   Based on the platform, the value should not go below 256 to avoid issues.
                                                                                                 */
#define PHAL_NTAGXDNA_MAX_NATIVE_DATA_LEN                                                 64U   /**< Maximum length of data that can be passed to Pal ISO14443-4 layer for Application Chaining. */
/**
 * end of group phalNtagXDna_Defines_BufferSize
 * @}
 */

/**
 * end of group phalNtagXDna_CommonDefs
 * @}
 */
#endif /* NXPBUILD__PHAL_NTAGXDNA */

/***************************************************************************************************************************************/
/* Software DataParams and Initialization Interface.                                                                                   */
/***************************************************************************************************************************************/
#ifdef NXPBUILD__PHAL_NTAGXDNA_SW

/**
 * \defgroup phalNtagXDna_Sw Component : Software
 * \brief Software implementation of the NTAG X DNA commands. Here the NTAG X DNA commands are framed and
 * exchanged to tag.
 * @{
 */

#define PHAL_NTAGXDNA_SW_ID                                                             0x01U   /**< ID for Software NTAG X DNA layer. */

/** \brief Data structure for NTAG X DNA Software layer implementation  */
typedef struct
{
    uint16_t wId;                                                                               /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                                                                /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;                                                                 /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParamsASym;                                                               /**< Pointer to the parameter structure of the ASymmetric Crypto component. */
    void * pCryptoDataParamsEnc;                                                                /**< Pointer to the parameter structure of the Symmetric Crypto layer for encryption. */
    void * pCryptoDataParamsMac;                                                                /**< Pointer to the parameter structure of the Symmetric Crypto layer for MACing. */
    void * pCryptoRngDataParams;                                                                /**< Pointer to the parameter structure of the Crypto layer for Random number generation. */

    uint8_t * pCmdBuf;                                                                          /**< Pointer to global buffer for processing the command. */
    uint16_t wCmdBufSize;                                                                       /**< Size of global command buffer. */
    uint16_t wCmdBufLen;                                                                        /**< Length of bytes available in command buffer (\ref pCmdBuf) for processing. */
    uint16_t wCmdBufOffset;                                                                     /**< Command Buffer offset while performing crypto or exchange operations. */
    uint8_t * pPrsBuf;                                                                          /**< Pointer to global buffer for processing the response / secure messaging information. */
    uint16_t wPrsBufSize;                                                                       /**< Size of global response / secure messaging information buffer. */
    uint16_t wPrsBufLen;                                                                        /**< Length of bytes available in response / secure messaging information buffer (\ref pPrsBuf) for processing. */
    uint16_t wPrsBufOffset;                                                                     /**< Processing Buffer offset while performing crypto or exchange operations. */
    uint8_t aAid[3];                                                                            /**< Aid of the currently selected application */
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t aSesAuthENCKey[PH_CRYPTOSYM_AES256_KEY_SIZE];                                       /**< Authentication Encryption key for the session. */
    uint8_t aSesAuthMACKey[PH_CRYPTOSYM_AES256_KEY_SIZE];                                       /**< Authentication MAC key for the session. */
    uint8_t aSesNonce[PHAL_NTAGXDNA_SIZE_NONCE];                                                /**< Authentication Nonce for the session. */
    uint8_t aTi[PHAL_NTAGXDNA_SIZE_TI];                                                         /**< Transaction Identifier of the current transaction. */
#endif /* NXPBUILD__PH_CRYPTOSYM */
    uint16_t wCmdCtr;                                                                           /**< Command count within transaction. */
    uint16_t wAdditionalInfo;                                                                   /**< Specific error codes for NTAG X DNA generic errors or To get the response length of some commands. */
    uint8_t bAuthState;                                                                         /**< Authenticate Command used. One of the below values will be updated
                                                                                                 *      - \ref PHAL_NTAGXDNA_NOT_AUTHENTICATED "Not Authenticated" \n
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *      - \ref PHAL_NTAGXDNA_EV2_AUTHENTICATED "EV2 Authenticated" \n
                                                                                                 *      - \ref PHAL_NTAGXDNA_ECC_AUTHENTICATED "ECC Authenticated"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
    uint8_t bKeyNo;                                                                             /**< Key number against which this authentication is done */
    uint16_t wKeyType;                                                                          /**< Key Type being used for Authentication. */
    uint8_t bPICCDataComplete;                                                                  /**< Flag to Indicate tag data Status. Indicates the following.
                                                                                                 *      - Tag Data is complete but there is still more data that needs to be provided to user.
                                                                                                 *      - Tag Data is complete and there is no data to be given to user, but last encrypted chunk
                                                                                                 *        needs to be verified.
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t bHasMACProcessed;                                                                   /**< Flag to indicate if MAC verification is complete for response or MAC computation is complete for Command.
                                                                                                 *   This flag will also be used to indicate the status of initial MAC processing.
                                                                                                 */
    uint8_t bIsENCPending;                                                                      /**< Flag to indicate if Encryption of Data is complete or still pending. */
#endif /* NXPBUILD__PH_CRYPTOSYM */
    uint8_t bShortLenApdu;                                                                      /**< Parameter for force set Short Length APDU in case of BIG ISO read. */
} phalNtagXDna_Sw_DataParams_t;

/**
 * \brief Initialization API for NTAG X DNA software component.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS
 *          - If the input size doest not match the DataParams size of this component.
 *          - If any of the DataParams are null.
 * \retval #PH_ERR_INVALID_PARAMETER If the buffers are null.
 * \retval #PH_ERR_PARAMETER_SIZE
 *          - If the buffer size is less than minimum.
 *          - If command buffer (\b wCmdBufSize) is higher than processing buffer (\b wPrsBufSize).
 */
phStatus_t phalNtagXDna_Sw_Init(
        phalNtagXDna_Sw_DataParams_t * pDataParams,                                             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wSizeOfDataParams,                                                             /**< [In] Specifies the size of the data parameter structure */
        void * pPalMifareDataParams,                                                            /**< [In] Pointer to a palMifare component context. */
        void * pKeyStoreDataParams,                                                             /**< [In] Pointer to Key Store data parameters. */
        void * pCryptoDataParamsASym,                                                           /**< [In] Pointer to the parameter structure of the ASymmetric Crypto component. */
        void * pCryptoDataParamsEnc,                                                            /**< [In] Pointer to Symmetric Crypto component context for encryption. */
        void * pCryptoDataParamsMac,                                                            /**< [In] Pointer to Symmetric Crypto component context for MACing. */
        void * pCryptoRngDataParams,                                                            /**< [In] Pointer to a CryptoRng component context. */
        uint8_t * pCmdBuf,                                                                      /**< [In] Pointer to global buffer for processing the command. */
        uint16_t wCmdBufSize,                                                                   /**< [In] Size of global command buffer. Should be >= \ref PHAL_NTAGXDNA_CMD_BUFFER_SIZE_MINIMUM
                                                                                                 *        "Minimum Command Buffer Size"
                                                                                                 */
        uint8_t * pPrsBuf,                                                                      /**< [In] Pointer to global buffer for processing the response / secure messaging information. */
        uint16_t wPrsBufSize                                                                    /**< [In] Size of global response / secure messaging buffer.
                                                                                                 *        Should be >= \ref PHAL_NTAGXDNA_PRS_BUFFER_SIZE_MINIMUM "Minimum Processing Buffer Size"
                                                                                                 */
    );

/**
 * \brief Initialization API for NTAG X DNA software component.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If any of the DataParams are null.
 * \retval XXXX                         Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_Sw_DeInit(
        phalNtagXDna_Sw_DataParams_t * pDataParams                                              /**< [In] Pointer to this layer's parameter structure. */
    );

/**
 * end of group phalNtagXDna_Sw
 * @}
 */
#endif /* NXPBUILD__PHAL_NTAGXDNA_SW */

/***************************************************************************************************************************************/
/* NTAG X DNA Generic interface declarations.                                                                                          */
/***************************************************************************************************************************************/
#ifdef NXPBUILD__PHAL_NTAGXDNA

/** \addtogroup phalNtagXDna
 * \brief This is only a wrapper layer to abstract the different NTAG X DNA implementations.
 * With this wrapper it is possible to support more than one NTAG X DNA implementation
 * in parallel, by adapting this wrapper.
 *
 * \note
 *      - Below are the initialization(s) supported by this component.
 *          - \ref phalNtagXDna_Sw_Init "Software Initialization"
 *      - This component requires CCM cryptography for \ref phalNtagXDna_ISOGeneralAuthenticate "SIGMA-I Authentication". For that, during
 *        \ref phalNtagXDna_Sw_Init "Software Initialization" call, \ref phCryptoSym_mBedTLS "mBedTLS" component of \ref phCryptoSym "CryptoSym"
 *        be used for \ref phalNtagXDna_Sw_DataParams_t.pCryptoDataParamsEnc "Encryption", \ref phalNtagXDna_Sw_DataParams_t.pCryptoDataParamsMac
 *        "MACing" contexts. For \ref phalNtagXDna_Sw_DataParams_t.pCryptoRngDataParams "Random Number Generator" context use \ref phCryptoRng_mBedTLS
 *        "mBedTLS" component of \ref phCryptoRng "CryptoRng".
 *      - The AL component uses two internal buffers for processing. One is named as \ref phalNtagXDna_Sw_DataParams_t.pCmdBuf "Command Buffer" and
 *        another as \ref phalNtagXDna_Sw_DataParams_t.pPrsBuf "Processing Buffer".
 *      - Memory for these internal buffers needs to be passed by the user during initialization.
 *      - The internal buffer size should not be less than minimum. Minimum sizes are,
 *          - \ref phalNtagXDna_Sw_DataParams_t.pCmdBuf "Command Buffer", the \b wCmdBufSize should be >= \ref PHAL_NTAGXDNA_CMD_BUFFER_SIZE_MINIMUM
 *            "Minimum Command Buffer Size"
 *          - \ref phalNtagXDna_Sw_DataParams_t.pPrsBuf "Processing Buffer", the \b wPrsBufSize should be >= \ref PHAL_NTAGXDNA_PRS_BUFFER_SIZE_MINIMUM
 *            "Minimum Processing Buffer Size"
 *           - If the sizes are less than minimum, \ref PH_ERR_PARAMETER_SIZE "Parameter Size" error will be returned.
 *      - Processing buffer size should not be less than Command buffer size else \ref PH_ERR_PARAMETER_SIZE "Parameter Size" error will be returned.
 *      - After completion of the application, call below interfaces to clear all the internal buffers, its sizes, dependent components
 *        like CryptoSym and CryptASym etc...
 *          - \ref phalNtagXDna_Sw_DeInit "De-Initialize Software Component"
 *      - It's must to Initialize the component again after calling De-Initialization.
 *      - During Initialization of HAL component, make sure the transmit and response buffer size are not less than the maximum data that will be received.
 *      - Call \ref phalNtagXDna_GetConfig "GetConfig" with \ref PHAL_NTAGXDNA_ADDITIONAL_INFO "Additional Information" as Configuration identifier
 *        when any of the interface returns \ref PHAL_NTAGXDNA_ERR_DF_GEN_ERROR "General Failure" or \ref PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR
 *        "ISO7816 General Failure".
 *
 * @{
 */

/**
 * \defgroup phalNtagXDna_ErrorCode ErrorCodes
 * \brief Error Codes received from tag and the equivalent Reader Library error codes.
 * @{
 */

/**
 * \defgroup phalNtagXDna_ErrorCodes_Tag ErrorCodes_Tag
 * \brief The actual error codes received from tag. These codes are for reference purpose only.
 * All the interfaces will return codes mentioned in \ref phalNtagXDna_ErrorCodes_Mapped "Mapped Codes" and not these codes.
 * @{
 */

/**
 * \defgroup phalNtagXDna_ErrorCodes_Tag_Native Native
 * \brief NTAG X DNA Native Response Codes.
 * @{
 */
#define PHAL_NTAGXDNA_RESP_OPERATION_OK                                                 0x00U   /**< NTAGXDNA Response - Successful operation.
                                                                                                 *   Equivalent mapped error code will be #PH_ERR_SUCCESS.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_OK                                                           0x90U   /**< NTAGXDNA Response - Successful operation.
                                                                                                 *   Equivalent mapped error code will be #PH_ERR_SUCCESS.
                                                                                                 */

#define PHAL_NTAGXDNA_RESP_ADDITIONAL_FRAME                                             0xAFU   /**< NTAGXDNA Response - Additional data frame is expected to be sent.
                                                                                                 *   Equivalent mapped error code will be #PH_ERR_SUCCESS_CHAINING.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_OUT_OF_EEPROM_ERROR                                          0x0EU   /**< NTAGXDNA Response - Insufficient NV-Memory.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_OUT_OF_EEPROM.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_ILLEGAL_COMMAND_CODE                                         0x1CU   /**< NTAGXDNA Response - Command code not supported.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_DF_GEN_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_INTEGRITY_ERROR                                              0x1EU   /**< NTAGXDNA Response - CRC or MAC does not match data padding bytes not valid.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_PICC_CRYPTO.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_WEAK_FIELD                                                   0x1FU   /**< NTAGXDNA Response - Field strength not sufficient to enable power harvesting
                                                                                                 *   for the targeted current/voltage level.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_WEAK_FIELD_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_PAD_VOLTAGE_UNRELIABLE                                       0x2FU   /**< NTAGXDNA Response - When enabling power harvesting, pad voltage is too low. Pads are
                                                                                                 *   not sufficiently supplied.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_PAD_VOLTAGE_UNRELIABLE_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_NO_SUCH_KEY                                                  0x40U   /**< NTAGXDNA Response - Invalid key number specified.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_NO_SUCH_KEY.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_LENGTH_ERROR                                                 0x7EU   /**< NTAGXDNA Response - Length of command string invalid.
                                                                                                 *   Equivalent mapped error code will be #PH_ERR_LENGTH_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_PERMISSION_DENIED                                            0x9DU   /**< NTAGXDNA Response - Current configuration/status does not allow the requested command.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_PERMISSION_DENIED.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_PARAMETER_ERROR                                              0x9EU   /**< NTAGXDNA Response - Value of Parameter invalid.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_PARAMETER_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_AUTHENTICATION_ERROR                                         0xAEU   /**< NTAGXDNA Response - Current authentication status does not allow the requested Command.
                                                                                                 *   Equivalent mapped error code will be #PH_ERR_AUTH_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_BOUNDARY_ERROR                                               0xBEU   /**< NTAGXDNA Response - Attempt to write beyond the file boundary as set during creation.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_BOUNDARY_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_COMMAND_ABORTED                                              0xCAU   /**< NTAGXDNA Response - Previous Command not fully completed. Not all frames were requested or
                                                                                                 *   provided by the PCD. Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_COMMAND_ABORTED.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_CERT_ERROR                                                   0xCEU   /**< NTAGXDNA Response - Reader certificate or CertAccessRights related error.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_CERTIFICATE_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_DUPLICATE                                                    0xDEU   /**< NTAGXDNA Response - File/Application with same number already exists.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_DUPLICATE.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_MEMORY_ERROR                                                 0xEEU   /**< NTAGXDNA Response - Could not complete NV-Write operation due to loss of power.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_DF_GEN_ERROR.
                                                                                                 */
#define PHAL_NTAGXDNA_RESP_FILE_NOT_FOUND                                               0xF0U   /**< NTAGXDNA Response - Specified file number does not exist.
                                                                                                 *   Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_FILE_NOT_FOUND.
                                                                                                 */
/**
 * end of group phalNtagXDna_ErrorCodes_Tag_Native
 * @}
 */

/**
 * \defgroup phalNtagXDna_ErrorCodes_Tag_ISO7816 ISO7816
 * \brief NTAG X DNA Native ISO 7816 Response Codes.
 *
 * \note
 *  - Equivalent mapped error code for #PHAL_NTAGXDNA_ISO7816_9000 will be #PH_ERR_SUCCESS.
 *  - Equivalent mapped error code will be #PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR for others.
 *    To know the exact tag error codes, call \ref phalNtagXDna_GetConfig "GetConfig" with
 *    \ref PHAL_NTAGXDNA_ADDITIONAL_INFO "Additional Information" as Configuration identifier.
 * @{
 */
#define PHAL_NTAGXDNA_ISO7816_9000                                                      0x9000U /**< NTAGXDNA ISO7816 Response - Correct execution. */
#define PHAL_NTAGXDNA_ISO7816_ERR_6300                                                  0x6300U /**< NTAGXDNA ISO7816 Response - Verification of host signature failed. */
#define PHAL_NTAGXDNA_ISO7816_ERR_6700                                                  0x6700U /**< NTAGXDNA ISO7816 Response - Wrong or inconsistent APDU length. */
#define PHAL_NTAGXDNA_ISO7816_ERR_6982                                                  0x6982U /**< NTAGXDNA ISO7816 Response - Multiple descriptions, refer below
                                                                                                 *      - Security status not satisfied: no access allowed as Read and ReadWrite
                                                                                                 *        access rights are different from 0xE and SDMFileRead (if SDM enabled)
                                                                                                 *        access right is set to 0xF
                                                                                                 *      - Security status not satisfied: SDMReadCtr overflow
                                                                                                 *      - Security status not satisfied: Targeted file cannot be read in VCState.NotAuthenticated
                                                                                                 *        as the related SDMReadCtr is equal or bigger than its SDMRead CtrLimit
                                                                                                 *      - Security status not satisfied: only free write with Write or ReadWrite equal to 0xE
                                                                                                 *        is allowed
                                                                                                 *      - Security status not satisfied: AuthenticatedAES not allowed
                                                                                                 *      - Security status not satisfied: AuthenticatedECC not allowed
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6984                                                  0x6984U /**< NTAGXDNA ISO7816 Response - ECC-based Card-Unilateral Authentication disabled
                                                                                                 *   via the key policy of the targeted key.
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6985                                                  0x6985U /**< NTAGXDNA ISO7816 Response - Multiple descriptions, refer below
                                                                                                 *      - File empty
                                                                                                 *      - Access conditions not satisfied
                                                                                                 *      - Wrapped chained command or multiple pass command ongoing
                                                                                                 *      - Not supported at PICC level
                                                                                                 *      - Key usage counter enabled and limit reached
                                                                                                 *      - Protocol option requested is not supported
                                                                                                 *      - Originality Check with key 0x1 at PICC level disabled due to enhanced
                                                                                                 *        privacy configuration
                                                                                                 *      - Current state different from VCState.NotAuthenticated
                                                                                                 *      - ECC-based Card-Unilateral Authentication disabled over NFC interface
                                                                                                 *      - ECC-based Card-Unilateral Authentication disabled over I2C interface
                                                                                                 *      - KeyUsageCtrLimit enabled for targeted key has been reached
                                                                                                 *      - No file selected
                                                                                                 *      - Attempt to write beyond the file boundary as set during creation
                                                                                                 *      - Targeted file with ISO FileID 0xEF01 at PICC level, while Originality Check
                                                                                                 *        is disabled
                                                                                                 *      - Trying to readSDMSIG while the KeyUsageCtrLimit of the targeted key entry is
                                                                                                 *        enabled and reached
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6987                                                  0x6987U /**< NTAGXDNA ISO7816 Response - Multiple descriptions, refer below
                                                                                                 *      - Expected DO missing
                                                                                                 *      - Unexpected DO received.
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6988                                                  0x6988U /**< NTAGXDNA ISO7816 Response - Multiple descriptions, refer below
                                                                                                 *      - Invalid host ephemeral public key
                                                                                                 *      - Host message decryption failed
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6A80                                                  0x6A80U /**< NTAGXDNA ISO7816 Response - Invalid command data format. */
#define PHAL_NTAGXDNA_ISO7816_ERR_6A82                                                  0x6A82U /**< NTAGXDNA ISO7816 Response - Multiple descriptions, refer below
                                                                                                 *      - Certificate level requested is invalid or certificate has already been requested
                                                                                                 *      - File not found
                                                                                                 *      - Application or file not found, currently selected application remains selected
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6A86                                                  0x6A86U /**< NTAGXDNA ISO7816 Response - Multiple descriptions, refer below
                                                                                                 *      - Wrong parameters P1 and or P2.
                                                                                                 *      - Wrong parameter P1: different from 0x00
                                                                                                 *      - Wrong parameter P2: RFU bits set
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6A87                                                  0x6A87U /**< NTAGXDNA ISO7816 Response - Wrong parameter LC inconsistent with P1-P2. */
#define PHAL_NTAGXDNA_ISO7816_ERR_6A88                                                  0x6A88U /**< NTAGXDNA ISO7816 Response - Wrong parameter P2: Key targeted by PrivKeyNo
                                                                                                 *   does not exist.
                                                                                                 */
#define PHAL_NTAGXDNA_ISO7816_ERR_6C00                                                  0x6C00U /**< NTAGXDNA ISO7816 Response - Wrong Le: expected length insufficient for response data. */
#define PHAL_NTAGXDNA_ISO7816_ERR_6E00                                                  0x6E00U /**< NTAGXDNA ISO7816 Response - Wrong Class byte. */
/**
 * end of group phalNtagXDna_ErrorCodes_Tag_ISO7816
 * @}
 */

/**
 * end of group phalNtagXDna_ErrorCodes_Tag
 * @}
 */

/**
 * \defgroup phalNtagXDna_ErrorCodes_Mapped ErrorCodes_Mapped
 * \brief The error codes from Library with respect to tag Error codes.
 * @{
 */
#define PHAL_NTAGXDNA_ERR_OUT_OF_EEPROM                             (PH_ERR_CUSTOM_BEGIN + 1U)  /**< NTAGXDNA Custom error code - Insufficient NV-Memory.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_OUT_OF_EEPROM_ERROR error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_NO_SUCH_KEY                               (PH_ERR_CUSTOM_BEGIN + 2U)  /**< NTAGXDNA Custom error code - Invalid key number specified.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_NO_SUCH_KEY error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_PERMISSION_DENIED                         (PH_ERR_CUSTOM_BEGIN + 3U)  /**< NTAGXDNA Custom error code - Current configuration/wStatus does not allow the requested command.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_PERMISSION_DENIED error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_BOUNDARY_ERROR                            (PH_ERR_CUSTOM_BEGIN + 4U)  /**< NTAGXDNA Custom error code - Attempt to read/write data from/to beyond the files/record's limits.
                                                                                                 *   This error represents PICC's #PHAL_NTAGXDNA_RESP_BOUNDARY_ERROR error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_COMMAND_ABORTED                           (PH_ERR_CUSTOM_BEGIN + 5U)  /**< NTAGXDNA Custom error code - Previous command not fully completed. Not all frames were requested or
                                                                                                 *   provided by the PCD. This error represents tag's #PHAL_NTAGXDNA_RESP_COMMAND_ABORTED error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_DUPLICATE                                 (PH_ERR_CUSTOM_BEGIN + 6U)  /**< NTAGXDNA Custom error code - File/Application with same number already exists.
                                                                                                 *   This error represents PICC's #PHAL_NTAGXDNA_RESP_DUPLICATE error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_FILE_NOT_FOUND                            (PH_ERR_CUSTOM_BEGIN + 7U)  /**< NTAGXDNA Custom error code - Specified file number does not exist.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_FILE_NOT_FOUND error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_PICC_CRYPTO                               (PH_ERR_CUSTOM_BEGIN + 8U)  /**< NTAGXDNA Custom error code - Crypto error returned by tag.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_INTEGRITY_ERROR error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_PARAMETER_ERROR                           (PH_ERR_CUSTOM_BEGIN + 9U)  /**< NTAGXDNA Custom error code - Parameter value error returned by tag.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_PARAMETER_ERROR error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_DF_GEN_ERROR                              (PH_ERR_CUSTOM_BEGIN + 10U) /**< NTAGXDNA Custom error code - NTAG X DNA Generic error.
                                                                                                 *   Refer \ref phalNtagXDna_GetConfig "GetConfig" with \ref PHAL_NTAGXDNA_ADDITIONAL_INFO
                                                                                                 *   "Additional Information" as config option to get the exact error code. This code
                                                                                                 *   will be returned for the below tag Error codes.
                                                                                                 *      - #PHAL_NTAGXDNA_RESP_MEMORY_ERROR
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR                         (PH_ERR_CUSTOM_BEGIN + 11U) /**< NTAGXDNA Custom error code - NTAG X DNA ISO 7816 Generic error.
                                                                                                 *   Refer \ref phalNtagXDna_GetConfig "GetConfig" with \ref PHAL_NTAGXDNA_ADDITIONAL_INFO
                                                                                                 *   "Additional Information" as config option to get the exact error code. This code
                                                                                                 *   will be returned for the below tag Error codes.
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6300
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6700
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6982
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6984
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6985
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6987
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6988
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6A80
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6A82
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6A86
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6A87
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6A88
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6C00
                                                                                                 *      - #PHAL_NTAGXDNA_ISO7816_ERR_6E00
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_CERTIFICATE_ERROR                         (PH_ERR_CUSTOM_BEGIN + 12U) /**< NTAGXDNA Custom error code - Reader certificate or CertAccessRights related error.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_CERT_ERROR error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_WEAK_FIELD_ERROR                          (PH_ERR_CUSTOM_BEGIN + 13U) /**< NTAGXDNA Custom error code - Field strength not sufficient to enable power
                                                                                                 *   harvesting for the targeted current/voltage level.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_WEAK_FIELD error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_PAD_VOLTAGE_UNRELIABLE_ERROR              (PH_ERR_CUSTOM_BEGIN + 14U) /**< NTAGXDNA Custom error code - When enabling power harvesting, pad voltage is too low.
                                                                                                 *   Pads are not sufficiently supplied.
                                                                                                 *   This error represents tag's #PHAL_NTAGXDNA_RESP_PAD_VOLTAGE_UNRELIABLE error.
                                                                                                 */
#define PHAL_NTAGXDNA_ERR_DEVICE_SIGNATURE_ERROR                    (PH_ERR_CUSTOM_BEGIN + 15U) /**< NTAGXDNA Custom error code - Device Signature Verification failure */
#define PHAL_NTAGXDNA_ERR_DEVICE_LEAF_CERTIFICATE_HASH_ERROR        (PH_ERR_CUSTOM_BEGIN + 16U) /**< NTAGXDNA Custom error code - Device End-Leaf Certificate Hash Verification failure */
#define PHAL_NTAGXDNA_ERR_SIGNATURE_VERIFICATION_FAILURE            (PH_ERR_CUSTOM_BEGIN + 17U) /**< NTAGXDNA Custom error code - Signature Verification Failure */
/**
 * end of group phalNtagXDna_ErrorCodes_Mapped
 * @}
 */

/**
 * end of group phalNtagXDna_ErrorCode
 * @}
 */

/**
 * \defgroup phalNtagXDna_CommonDefs Defines
 * @{
 */

/**
 * \defgroup phalNtagXDna_Defines_ComModes CommunicationModes
 * \brief The communication mode to be used for Command / Response.
 * @{
 */
#define PHAL_NTAGXDNA_COMMUNICATION_PLAIN                                               0x00U   /**< Plain mode of communication. The Command / Response will be is plain format. */
#define PHAL_NTAGXDNA_COMMUNICATION_PLAIN_20                                            0x20U   /**< Plain mode of communication. The Command / Response will be is plain format. */
#ifdef NXPBUILD__PH_CRYPTOSYM
#define PHAL_NTAGXDNA_COMMUNICATION_MAC                                                 0x10U   /**< MAC mode of communication. The Command / Response will have MAC appended at last. */
#define PHAL_NTAGXDNA_COMMUNICATION_FULL                                                0x30U   /**< Enciphered mode of communication. The Command Data / Response Data will be encrypted
                                                                                                 *   and will have MAC appended at last.
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
/**
 * end of group phalNtagXDna_Defines_ComModes
 * @}
 */

/**
 * \defgroup phalNtagXDna_Defines_TargetCurve TargetCurve
 * \brief Target Action options to be used with below mentioned interfaces
 *  - \ref phalNtagXDna_ISOInternalAuthenticate "ISOInternal Authenticate"
 *  - \ref phalNtagXDna_ManageKeyPair "Manage Key-Pair".
 *  - \ref phalNtagXDna_ManageCARootKey "Manage CA Root Key".
 *  - \ref phalNtagXDna_CryptoRequestECCVerify "CryptoRequest ECC Verify".
 * @{
 */
#define PHAL_NTAGXDNA_TARGET_CURVE_ID_NIST_P256                                         0x0CU   /**< Option for NIST P-256 Curve ID. */
#define PHAL_NTAGXDNA_TARGET_CURVE_ID_BRAINPOOL_P256R1                                  0x0DU   /**< Option for BrainPool P-256 R1 Curve ID. */
/**
 * end of group phalNtagXDna_Defines_TargetCurve
 * @}
 */

/**
 * end of group phalNtagXDna_CommonDefs
 * @}
 */

/* NTAG X DNA secure messaging related commands ---------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_SecureMessaging Commands_SecureMessaging
 * \brief Describes about the NTAG X DNA Secure Messaging related commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_SecureMessaging_Defines Defines
 * \brief Macro Definitions for Secure Messaging commands
 * @{
 */

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \defgroup phalNtagXDna_SecureMessaging_Defines_CertDepth Certificate_Depth
 * \brief Option to indicate the Certificate Depth to be used with
 * \ref phalNtagXDna_ISOGeneralAuthenticate "SIGMA-I Authentication"
 * @{
 */
#define PHAL_NTAGXDNA_CERT_DEPTH_END_LEAF_ONLY                                          0x00U   /**< Option to indicate Certificate depth as End-Leaf Only.
                                                                                                 *   Also called as Depth Zero.
                                                                                                 */
#define PHAL_NTAGXDNA_CERT_DEPTH_END_LEAF_PARENT                                        0x01U   /**< Option to indicate Certificate depth as End_Leaf and Parent.
                                                                                                 *   Also called as Depth One.
                                                                                                 */
#define PHAL_NTAGXDNA_CERT_DEPTH_END_LEAF_PARENT_GRAND_PARENT                           0x02U   /**< Option to indicate Certificate depth as End_Leaf, Parent and
                                                                                                 *   Grand-Parent. Also called as Depth Two.
                                                                                                 */
/**
 * end of group phalNtagXDna_SecureMessaging_Defines_CertDepth
 * @}
 */

/**
 * \defgroup phalNtagXDna_SecureMessaging_Defines_KeySize KeySize
 * \brief Option to indicate the Session KeySize to be used with
 * \ref phalNtagXDna_ISOGeneralAuthenticate "SIGMA-I Authentication"
 * @{
 */
#define PHAL_NTAGXDNA_SESSION_KEY_SIZE_AES128                                           0x01U   /**< Option to indicate Session Key Size as AES128. */
#define PHAL_NTAGXDNA_SESSION_KEY_SIZE_AES256                                           0x02U   /**< Option to indicate Session Key Size as AES256. */
/**
 * end of group phalNtagXDna_SecureMessaging_Defines_CertCompressionType
 * @}
 */

/**
 * \defgroup phalNtagXDna_SecureMessaging_Defines_AuthState AuthState
 * \brief Different type of Authenticate states.
 * @{
 */
#define PHAL_NTAGXDNA_NOT_AUTHENTICATED                                                 0xFFU   /**< No authentication. */
#define PHAL_NTAGXDNA_EV2_AUTHENTICATED                                                 0x01U   /**< Authentication Available with EV2 mode. */
#define PHAL_NTAGXDNA_ECC_AUTHENTICATED                                                 0x02U   /**< Authentication Available with ECC mode. */
/**
 * end of group phalNtagXDna_SecureMessaging_Defines_AuthState
 * @}
 */

/**
 * \defgroup phalNtagXDna_SecureMessaging_Defines_Diverfication Diversification
 * \brief Diversification options to be used with \ref phalNtagXDna_AuthenticateEv2 "Authenticate EV2" interface
 * @{
 */
#define PHAL_NTAGXDNA_NO_DIVERSIFICATION                                                  0xFFFFU /**< No diversification. */
#define PHAL_NTAGXDNA_DIV_METHOD_ENCR                             PH_CRYPTOSYM_DIV_MODE_DESFIRE   /**< Encryption based method of diversification. */
#define PHAL_NTAGXDNA_DIV_METHOD_CMAC                         PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS   /**< CMAC based method of diversification. */
/**
 * end of group phalNtagXDna_SecureMessaging_Defines_Diverfication
 * @}
 */

/**
 * \defgroup phalNtagXDna_SecureMessaging_Defines_AuthType AuthType
 * \brief Authentication options to be used with the below mentioned interfaces.
 *  - \ref phalNtagXDna_AuthenticateEv2 "Authenticate EV2"
 * @{
 */
#define PHAL_NTAGXDNA_AUTH_FIRST                                                          0x01U   /**< Option to perform \ref phalNtagXDna_AuthenticateEv2 "First Authentication". */
#define PHAL_NTAGXDNA_AUTH_NONFIRST                                                       0x00U   /**< Option to perform \ref phalNtagXDna_AuthenticateEv2 "NonFirst Authentication". */
 /**
 * end of group phalNtagXDna_SecureMessaging_Defines_AuthType
 * @}
 */

/**
 * end of group phalNtagXDna_SecureMessaging_Defines
 * @}
 */

/**
 * \defgroup phalNtagXDna_SecureMessaging_Cert_Structure Certificate_Structure
 * \brief Structure for NTAG X DNA Host / Device Certificates to be used with the below mentioned interfaces.
 *  - \ref phalNtagXDna_ISOGeneralAuthenticate "SIGMA-I Authentication"
 * @{
 */
typedef struct
{
    uint8_t * pEndLeaf;                                                                         /**< Pointer to global buffer for Host / Device End-Leaf certificate.
                                                                                                 *   Buffer should be pre-allocated by user and the size should be larger
                                                                                                 *   or equal to the certificate size. Should include 7F 21 (UnCompressed)
                                                                                                 *   or 7F 22 (Compressed) notation mentioned in DataSheet followed by
                                                                                                 *   Length of Certificate in BER-TLV notation and the actual certificate
                                                                                                 *   information.
                                                                                                 */
    uint16_t wEndLeaf_Len;                                                                      /**< Length of bytes available in \b pEndLeaf buffer. */

    uint8_t * pParent;                                                                          /**< Pointer to global buffer for Host / Device Parent certificate.
                                                                                                 *   Buffer should be pre-allocated by user and the size should be larger
                                                                                                 *   or equal to the certificate size. Should include 7F 21 (UnCompressed)
                                                                                                 *   or 7F 22 (Compressed) notation mentioned in DataSheet followed by
                                                                                                 *   Length of Certificate in BER-TLV notation and the actual certificate
                                                                                                 *   information.
                                                                                                 */
    uint16_t wParent_Len;                                                                       /**< Length of bytes available in \b pParent buffer. */

    uint8_t * pGrandParent;                                                                     /**< Pointer to global buffer for Host / Device Grand-Parent certificate.
                                                                                                 *   Buffer should be pre-allocated by user and the size should be larger
                                                                                                 *   or equal to the certificate size. Should include 7F 21 (UnCompressed)
                                                                                                 *   or 7F 22 (Compressed) notation mentioned in DataSheet followed by
                                                                                                 *   Length of Certificate in BER-TLV notation and the actual certificate
                                                                                                 *   information.
                                                                                                 */
    uint16_t wGrandParent_Len;                                                                  /**< Length of bytes available in \b pGrandParent buffer. */
} phalNtagXDna_Cert;

/**
 * end of group phalNtagXDna_SecureMessaging_Cert_Structure
 * @}
 */

/**
 * \brief Performs ASymmetric Mutual SIGMA-I authentication. This interfaces performs Cmd.ISOGeneralAuthenticate with
 * Host acts as Initiator or Responder mode based on the information provided in \b bIsHostResponder parameter.
 *  - During ISOGeneralAuthenticate, Ephemeral Key Pair is generated by the Library.
 *  - Device End-Leaf Certificate hash and Signature are verified by the Library internally.
 *  - Exchange of Abort Tag is not supported by the library.
 *  - Interface provides received Device Certificates. The certificate validation is not carried by Library
 *    and shall be taken up by the user.
 *  - If \b bDeviceCert_Cached == \ref PH_ON "On", means user has to provide Device End-Leaf certificate only.
 *    This will be used for Certificate Hash and Signature Verification received from Device.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS                                          Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS                              If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                                                - If the buffers are null.
 *                                                                - Parameter Values not supported for Authentication.
 *                                                                - If \b bDeviceCert_Cached == \ref PH_ON "On" and Device End-Leaf Certificate is not
 *                                                                  available. i.e pDeviceCert->wEndLeaf_Len = 0
 * \retval #PH_ERR_PROTOCOL_ERROR                                   If information received is not proper.
 * \retval #PH_ERR_ABORTED                                          If Device aborts the authentication.
 * \retval #PHAL_NTAGXDNA_ERR_DEVICE_SIGNATURE_ERROR                Verification of Device Signature combination failed.
 * \retval #PHAL_NTAGXDNA_ERR_DEVICE_LEAF_CERTIFICATE_HASH_ERROR    Verification of Device End-Leaf Certificate Hash failed.
 * \retval XXXX
 *                                                                - Depending on status codes return by PICC.
 *                                                                - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ISOGeneralAuthenticate(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bDeviceCert_Cached,                                                             /**< [In] Whether or not the device certificates are cached.
                                                                                                 *        If cached, user need to provide End-Leaf certificate for
                                                                                                 *        Certificate Hash and Signature verification.
                                                                                                 *          - \ref PH_OFF "Device Certificate(s) are not cached"
                                                                                                 *          - \ref PH_ON "Device Certificate(s) are cached"
                                                                                                 */
        uint8_t bIsHostResponder,                                                               /**< [In] Flag to indicate whether Host is Initiator or responder.
                                                                                                 *        Will be one of the following values
                                                                                                 *          - \ref PH_OFF "Host as Initiator"
                                                                                                 *          - \ref PH_ON "Host as Responder"
                                                                                                 */
        uint8_t bCertRepoID,                                                                    /**< [In] Certificate repository Id to use to execute the protocol.
                                                                                                 *        Supported values are 0x00 - 0x07
                                                                                                 */
        uint8_t bCert_Depth,                                                                    /**< [In] Certificate Depth to exchange. Will be one of the following
                                                                                                 *          - \ref PHAL_NTAGXDNA_CERT_DEPTH_END_LEAF_ONLY "End-Leaf Only"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CERT_DEPTH_END_LEAF_PARENT "End-Leaf and Parent"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CERT_DEPTH_END_LEAF_PARENT_GRAND_PARENT
                                                                                                 *            "End-Leaf, Parent and Grand-Parent"
                                                                                                 */
        uint8_t bKeySize,                                                                       /**< [In] Type of Session Key Size to use. Will be one of the following
                                                                                                 *          - \ref PHAL_NTAGXDNA_SESSION_KEY_SIZE_AES128 "Session KeySize as AES128"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SESSION_KEY_SIZE_AES256 "Session KeySize as AES256"
                                                                                                 */
        uint16_t wPriv_KeyNo,                                                                   /**< [In] Key number in KeyStore to use, known as Initiator's / Responder's Private Key.
                                                                                                 *        This will be used to sign the message that will be exchanged, basically sk_init if Host is
                                                                                                 *        Initiator or sk_resp if Host is responder.
                                                                                                 *        Will be used for one of the following ECC Signature generation
                                                                                                 *          - Init_ECC_Sig = ECDSA-Sign(sk_init , 0x02 || keySize_init || keySize_resp || yP ||
                                                                                                 *            xP || AES_CMAC(K_m1, 0x02'|| leaf_cert_hash))
                                                                                                 *          - Resp_ECC_Sig = ECDSA-Sign(sk_resp, 0x01 || keySize_init || keySize_resp || xP || yP ||
                                                                                                 *            AES-CMAC(K_m1, 0x01 || leaf_cert _hash))
                                                                                                 */
        uint16_t wPriv_KeyPos,                                                                  /**< [In] Key position in KeyStore to use. The position where \b wPriv_KeyNo is stored. */
        uint16_t wPub_KeyNo,                                                                    /**< [In] Key number in KeyStore to use, known as Initiator's / Responder's Public Key.
                                                                                                 *        This will be used to verify the signature that was received, basically Init_ECC_Sig if
                                                                                                 *        Host is Responder or Resp_ECC_Sig if Host is Initiator.
                                                                                                 */
        uint16_t wPub_KeyPos,                                                                   /**< [In] Key position in KeyStore to use. The position where \b wPub_KeyNo is stored. */
        uint8_t * pExpRspLen,                                                                   /**< [In] Length of expected response from Device.
                                                                                                 *          - This parameter is for exchanging the LE information.
                                                                                                 *          - If NULL is provided, then the expected Response length will be taken
                                                                                                 *            as 0x00 (1 byte) by default.
                                                                                                 *          - Possible values are NULL, Array consisting of 1 byte or 2 bytes.
                                                                                                 */
        uint8_t bExpRspLen,                                                                     /**< [In] Length of bytes available in \b pExpRspLen buffer. */
        phalNtagXDna_Cert stHostCert,                                                           /**< [In] Structure holding the Host certificate(s) that will be exchanged to device. The certificate should
                                                                                                 *        be filled based on \arg bCert_Depth "Certificate Depth" information.
                                                                                                 */
        phalNtagXDna_Cert * pDeviceCert                                                         /**< [In, Out] Structure holding the Device certificate(s) received. The certificate should be filled based on
                                                                                                 *        \arg bCert_Depth "Certificate Depth" and information.
                                                                                                 *      - [In]: Provide End-Leaf Certificate if \b bDeviceCert_Cached == \ref PH_ON "On"
                                                                                                 *      - [Out]: Certificates will be returned if \b bDeviceCert_Cached == \ref PH_OFF "Off"
                                                                                                 */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Performs Asymmetric Card-Unilateral Authentication. The following operations are performed using this interface.
 * - Ephemeral Key Pair (E.Pub.A) is generated by the Library.
 * - The Signature (Sig.B) received as part of response is verified using the Key Number provided in \b wKeyNo_PubKeyB parameter.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffers are null.
 * \retval #PH_ERR_PROTOCOL_ERROR       If Tag information is not proper for AuthDOHdr, RndB and Signature.
 * \retval #PH_ERR_VERIFICATION_FAILED  Verification of Message / Signature combination failed.
 * \retval XXXX
 *                                      - Depending on status codes return by PICC.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ISOInternalAuthenticate(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bPrivKeyNo,                                                                     /**< [In] Private Key number for signing the response.
                                                                                                 *          - At PICC level, only one key is supported which is 0x01.
                                                                                                 *          - At Application level, up to five keys are supported.
                                                                                                 */
        uint8_t bCurveID,                                                                       /**< [In] The targeted curve for the public key provided in \b pPubBKey parameter.
                                                                                                 *        Should be one of the below values.
                                                                                                 *           - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_NIST_P256 "P-256"
                                                                                                 *           - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_BRAINPOOL_P256R1 "BP-256"
                                                                                                 */
        uint8_t * pPubBKey,                                                                     /**< [In] Public Key (Pub.B) to be used for verification. */
        uint16_t wPubBKeyLen,                                                                   /**< [In] Length of bytes available in \b pPubBKey buffer. */
        uint8_t * pOptsA,                                                                       /**< [In] Complete PCD Options in TLV format.
                                                                                                 *        NULL in case of Optional scenario
                                                                                                 */
        uint8_t bOptsALen,                                                                      /**< [In] Length of bytes available in \b pOptsA buffer.
                                                                                                 *        Zero in case of Optional scenario.
                                                                                                 */
        uint8_t * pExpRspLen,                                                                   /**< [In] Length of expected response from Device.
                                                                                                 *          - This parameter is for exchanging the LE information.
                                                                                                 *          - If NULL is provided, then the expected Response length will be
                                                                                                 *            taken as 0x00 (1 byte) by default or 2 bytes based on LC.
                                                                                                 *          - Possible values are NULL, Array consisting of 1 byte or 2 bytes.
                                                                                                 */
        uint8_t bExpRspLen                                                                      /**< [In] Length of bytes available in \b pExpRspLen buffer. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Performs an Ev2 First or Non First Authentication depending upon \b bFirstAuth Parameter. This will be using the AES128 or AES256
 * keys and will generate and verify the contents based on generic AES algorithm. The Random number generation will be performed by the library.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid AuthType (\b bFirstAuth), Invalid Diversification Options (\b wOption).
 *                                      - If Diversification is higher than \ref PHAL_NTAGXDNA_DIV_INPUT_LEN_MAX
 *                                        "Maximum Diversification Length".
 * \retval #PH_ERR_KEY                  KeyType is not of AES128.
 * \retval #PH_ERR_PROTOCOL_ERROR       If Response length is not of AES128 Block Size (Single or multiple).
 * \retval #PH_ERR_AUTH_ERROR           If Received RndA do not match.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_AuthenticateEv2(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFirstAuth,                                                                     /**< [In] Authentication type to perform. One of the below options.
                                                                                                 *          - \ref PHAL_NTAGXDNA_AUTH_FIRST "First Authentication"
                                                                                                 *          - \ref PHAL_NTAGXDNA_AUTH_NONFIRST "Following Authentication"
                                                                                                 */
        uint16_t wOption,                                                                       /**< [In] One of the below options.
                                                                                                 *          - \ref PHAL_NTAGXDNA_NO_DIVERSIFICATION "No Diversification"
                                                                                                 *          - \ref PHAL_NTAGXDNA_DIV_METHOD_ENCR "Encryption Method of Diversification"
                                                                                                 *          - \ref PHAL_NTAGXDNA_DIV_METHOD_CMAC "CMAC Method of Diversification"
                                                                                                 */
        uint16_t wKeyNo,                                                                        /**< [In] Key number in KeyStore of software. */
        uint16_t wKeyVer,                                                                       /**< [In] Key version in the key store of software. */
        uint8_t bKeyNoCard,                                                                     /**< [In] Key number on card. Valid values are 0x00 - 0x04 */
        uint8_t * pDivInput,                                                                    /**< [In] Diversification input to be used for diversifying the key. Can be NULL. */
        uint8_t bDivLen,                                                                        /**< [In] Length of bytes available in \b pDivInput buffer. */
        uint8_t * pPcdCapsIn,                                                                   /**< [In] PCD Capabilities. Upto 6 bytes. */
        uint8_t bLenPcdCapsIn,                                                                  /**< [In] Length of PcdCapsIn. Always zero for NonFirst authentication. */
        uint8_t * pPcdCapsOut,                                                                  /**< [Out] PCD Capabilities. The size of the buffer should be 6 bytes long. */
        uint8_t * pPdCapsOut                                                                    /**< [Out] PD Capabilities. The size of the buffer should be 6 bytes long. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Performs ProcessSM Apply.
 * The ProcessSM_Apply is used for applying secure messaging to a command before sending it to the target
 * device.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffers are null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ProcessSMApply(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommMode,                                                                      /**< [In] Indicates the protection mode to be applied
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode (0x00)"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN_20 "Plain Mode (0x20)"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOffset,                                                                        /**< [In] Index of the first byte of CmdData in Data field.
                                                                                                 *        Only applicable when \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_FULL
                                                                                                 *        "Full Mode"
                                                                                                 */
        uint8_t bCmdCtrIncr,                                                                    /**< [In] Command counter increment value. Only applicable when \b bCommMode is
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode (0x00)"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN_20 "Plain Mode (0x20)"
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Plain data to protect. One of the following
                                                                                                 *          - NULL if \b bCommMode is
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode (0x00)"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN_20 "Plain Mode (0x20)"
                                                                                                 *          - The plain data to be protected if \b bCommMode is
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bDataLen,                                                                       /**< [In] Length of bytes available in \b pData buffer. */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the following,
                                                                                                 *          - NULL if \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN
                                                                                                 *            "Plain Mode (0x00)" or \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN_20
                                                                                                 *            "Plain Mode (0x20)"
                                                                                                 *          - 8 bytes MAC if \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_MAC
                                                                                                 *            "MAC Mode"
                                                                                                 *          - Encrypted data + MAC if \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_FULL
                                                                                                 *            "Full Mode"
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Performs ProcessSM Remove.
 * The ProcessSM_Remove is used for removing and validating secure messaging from a response received from
 * a target device.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffers are null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ProcessSMRemove(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bCommMode,                                                                      /**< [In] Indicates the protection mode to be applied
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Data from which protection to be removed. One of the following
                                                                                                 *          - If \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC"
                                                                                                 *            then, input data will be RC [|| RespData] || MAC
                                                                                                 *          - If \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "FULL"
                                                                                                 *            then, input data will be RC [|| Encrypted RespData] || MAC
                                                                                                 */
        uint8_t bDataLen,                                                                       /**< [In] Length of bytes available in \b pData buffer. */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the following,
                                                                                                 *          - If \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC"
                                                                                                 *            then, there will be no response.
                                                                                                 *          - If \b bCommMode is \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "FULL"
                                                                                                 *            then, Plain data will be returned.
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * end of group phalNtagXDna_SecureMessaging
 * @}
 */

/* NTAG X DNA Memory and Configuration management commands ----------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_MemoryConfiguration Commands_MemoryConfiguration
 * \brief Describes about the NTAG X DNA Memory and Configuration Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_MemoryConfiguration_Defines Defines
 * \brief Macro Definitions for Memory and Configuration Management commands.
 * @{
 */

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \defgroup phalNtagXDna_MemoryConfiguration_Defines_SetConfig SetConfiguration
 * \brief Set Configuration options to be used with \ref phalNtagXDna_SetConfiguration "SetConfiguration" interface.
 * @{
 */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_tag_CONFIGURATION                               0x00U   /**< Option for updating the tag Configuration. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_ATS_UPDATE                                      0x02U   /**< Option for updating the ATS. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_SAK_UPDATE                                      0x03U   /**< Option for updating the SAK. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_SM_CONFIGURATION                                0x04U   /**< Option for updating the Secure Messaging. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_CAPABILITY_DATA                                 0x05U   /**< Option for updating the Capability Data. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_ATQA_UPDATE                                     0x0CU   /**< Option for updating the ATQA information. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_SILENT_MODE                                     0x0DU   /**< Option for updating the Silent Mode configuration. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_ENHANCED_PRIVACY                                0x0EU   /**< Option for updating the Enhanced Privacy configuration. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_NFC_MANAGEMENT                                  0x0FU   /**< Option for updating the NFC Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_I2C_MANAGEMENT                                  0x10U   /**< Option for updating the I2C Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_GPIO_MANAGEMENT                                 0x11U   /**< Option for updating the GPIO Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_ECC_KEY_MANAGEMENT                              0x12U   /**< Option for updating the ECC Key Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_CERTIFICATE_MANAGEMENT                          0x13U   /**< Option for updating the Certificate Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_WATCHDOG_TIMER_MANAGEMENT                       0x14U   /**< Option for updating the Watchdog Timer Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_CRYPTO_API_MANAGEMENT                           0x15U   /**< Option for updating the Crypto API Management. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_AUTH_CTR_LIMIT                                  0x16U   /**< Option for updating the Authentication Counter and Limit Configuration. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_HALT_WAKEUP                                     0x17U   /**< Option for updating the HALT and Wake-Up Configuration. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_DEFERRED_CONFIGURATION                          0xFEU   /**< Option for updating the Deferred Configurations. */
#define PHAL_NTAGXDNA_SET_CONFIGURATION_LOCK_CONFIGURATION                              0xFFU   /**< Option for updating the Lock Configurations. */
/**
 * end of group phalNtagXDna_MemoryConfiguration_Defines_SetConfig
 * @}
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \defgroup phalNtagXDna_MemoryConfiguration_Defines_GetVersion GetVersion
 * \brief Options to be used with \ref phalNtagXDna_GetVersion "GetVersion" interface.
 * @{
 */
#define PHAL_NTAGXDNA_GET_VERSION_EXCLUDE_FAB_ID                                        0x00U   /**< Option for not exchanging the Option information in \ref phalNtagXDna_GetVersion
                                                                                                 *   "GetVersion" command. If used, the FabID will not be available in the response.
                                                                                                 */
#define PHAL_NTAGXDNA_GET_VERSION_RETURN_FAB_ID                                         0x01U   /**< Option for exchanging the Option information in \ref phalNtagXDna_GetVersion
                                                                                                 *   "GetVersion" command to retrieve the FabID information. If used, the FabID
                                                                                                 *   will be available in the response.
                                                                                                 */
 /**
 * end of group phalNtagXDna_MemoryConfiguration_Defines_GetVersion
 * @}
 */

/**
 * end of group phalNtagXDna_MemoryConfiguration_Defines
 * @}
 */

/**
 * \brief Returns free memory available on the tag
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_FreeMem(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t ** ppMemInfo,                                                                   /**< [Out] Current free memory available. Response Will be of 3 bytes with LSB first. \n
                                                                                                 *              If the free memory available is 7592 bytes, then
                                                                                                 *              7592 in Hex will be 0x001F10 \n
                                                                                                 *              \b ppMemInfo will contain 10 1F 00.
                                                                                                 */
        uint16_t * pMemInfoLen                                                                  /**< [Out] Length of bytes available in \b ppMemInfo buffer. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Performs SetConfiguration to configures several aspects of the card or the application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_SetConfiguration(
        void *pDataParams,                                                                      /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Configuration Option to configure. Refer supported
                                                                                                 *        \ref phalNtagXDna_MemoryConfiguration_Defines_SetConfig
                                                                                                 *        "Set Configurations" options.
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Data for the option specified.*/
        uint8_t bDataLen                                                                        /**< [In] Length of bytes available in \b pData buffer. */
    );

/**
 * \brief Performs GetConfiguration to retrieve configuration aspects of the card or the application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffer is null.
 *                                      - For Invalid ExchangeOption (\b bExchangeOption) value.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetConfiguration(
        void *pDataParams,                                                                      /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Configuration Option to configure. Refer supported
                                                                                                 *        \ref phalNtagXDna_MemoryConfiguration_Defines_SetConfig
                                                                                                 *        "Set Configurations" options. All the options are
                                                                                                 *        supported except option 0x01.
                                                                                                 */
        uint8_t bExchangeOption,                                                                /**< [In] Whether or not to exchange Option (\b bOption) information to tag.
                                                                                                 *          - \ref PH_OFF "OFF": Do not exchange Option information to tag.
                                                                                                 *          - \ref PH_ON "ON"  : Exchange Option information to tag.
                                                                                                 */
        uint8_t ** ppData,                                                                      /**< [In] One of the following information.
                                                                                                 *          - If \b bExchangeOption = \ref PH_OFF "OFF" then, Manufacturer Data.
                                                                                                 *            will be returned.
                                                                                                 *          - If \b bExchangeOption = \ref PH_ON "ON" then, information
                                                                                                 *            based on the Option data will be returned.
                                                                                                 */
        uint16_t * pDataLen                                                                     /**< [In] Length of bytes available in \b ppData buffer. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Performs ActivateConfiguration to abolish a deferred configuration.
 * ActivateConfiguration provides the list of deferred configurations for which the deferral can be ended.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ActivateConfiguration(
        void *pDataParams,                                                                      /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bConfCount,                                                                     /**< [In] Number Of Configurations to be Activated (N). */
        uint8_t * pConfigList,                                                                  /**< [In] List of configurations to be activated (with size N * 2). List
                                                                                                 *        must hold one or more of following values.
                                                                                                 *          - Activate SetConfiguration 0x01 (RandomID)
                                                                                                 *          - Activate SetConfiguration 0x0D (Silent Mode)
                                                                                                 *          - Activate SetConfiguration 0x11 (TagTamper boot measurements)
                                                                                                 *          - Activate ChangeFileSettings SDM encryptions
                                                                                                 */
        uint8_t bConfigList_Len                                                                 /**< [In] Length of bytes available in \b pConfigList buffer. */
    );

/**
 * \brief Returns manufacturing related data of the tag.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetVersion(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Option information to be exchanged. Will be one of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_GET_VERSION_EXCLUDE_FAB_ID "Do Not Return FabID":
                                                                                                 *            Option byte is not exchanged to tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_GET_VERSION_RETURN_FAB_ID "Return FabID":
                                                                                                 *            Option byte is exchanged to tag.
                                                                                                 */
        uint8_t ** ppVersion,                                                                   /**< [Out] Returns the complete version information of the tag.
                                                                                                 *         The information includes,
                                                                                                 *              - Hardware Information
                                                                                                 *              - Software Information
                                                                                                 *              - Production Related Information
                                                                                                 *                  - Will have FabID based on \b bOption information.
                                                                                                 */
        uint16_t * pVerLen                                                                      /**< [Out] Length of bytes available in \b ppVersion buffer. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Returns the Unique Identifier of the tag.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetCardUID(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t ** ppUid,                                                                       /**< [Out] Returns the complete UID information of the tag. */
        uint16_t * pUidLen                                                                      /**< [Out] Length of bytes available in \b ppUid buffer. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * end of group phalNtagXDna_MemoryConfiguration
 * @}
 */

/* NATG X DNA Symmetric Key management commands ---------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_Symm_KeyManagement Commands_SymmetricKeyManagement
 * \brief Describes about the NTAG X DNA Symmetric Key Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_Symm_KeyManagement_Defines Defines
 * \brief Macro Definitions for Symmetric Key Management commands.
 * @{
 */

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \defgroup phalNtagXDna_Symm_KeyManagement_Defines_ChangeKey ChangeKey
 * \brief Diversification options to be used with \ref phalNtagXDna_ChangeKey "ChangeKey" interface.
 * @{
 */
#define PHAL_NTAGXDNA_CHGKEY_NO_DIVERSIFICATION                                         0xFFFFU /**< No diversification. */
#define PHAL_NTAGXDNA_CHGKEY_DIV_NEW_KEY                                                0x0002U /**< Indicating diversification of new key is required. */
#define PHAL_NTAGXDNA_CHGKEY_DIV_OLD_KEY                                                0x0004U /**< Indicating diversification of old key is required. */
#define PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_ENCR                    PH_CRYPTOSYM_DIV_MODE_DESFIRE   /**< Indicating key diversification method based on ENCR. */
#define PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_CMAC                PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS   /**< Indicating key diversification method based on CMAC. */
/**
 * end of group phalNtagXDna_Symm_KeyManagement_Defines_ChangeKey
 * @}
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \defgroup phalNtagXDna_Symm_KeyManagement_Defines_KeySetting KeySetting
 * \brief Key Settings options to be used with \ref phalNtagXDna_GetKeySettings "Get Key Settings" interface.
 * @{
 */
#define PHAL_NTAGXDNA_KEY_SETTING_UNKNOWN                                               0xFFU   /**< Option for Unknown Key settings option. */
#define PHAL_NTAGXDNA_KEY_SETTING_EXCLUDE_OPTION                                        0xFEU   /**< Option for not exchanging option information. */
#define PHAL_NTAGXDNA_KEY_SETTING_CRYPTOREQUEST_KEY_METADATA                            0x00U   /**< Option for retrieval of CryptoRequest Keys meta-data. */
#define PHAL_NTAGXDNA_KEY_SETTING_ECC_PRIVATE_KEY_METADATA                              0x01U   /**< Option for retrieval of ECC Private Key meta-data. */
#define PHAL_NTAGXDNA_KEY_SETTING_CA_ROOT_KEY_METADATA                                  0x02U   /**< Option for retrieval of CA Root Key meta-data. */
/**
 * end of group phalNtagXDna_Symm_KeyManagement_Defines_KeySetting
 * @}
 */

/**
 * \defgroup phalNtagXDna_Symm_KeyManagement_Defines_KeyPolicy KeyPolicy
 * \brief KeyPolicy Options to be used with \ref phalNtagXDna_ChangeKey "Change Key" interface.
 * @{
 */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_DISABLED                                           0x0000U /**< Option for Key Policy as disabled. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_MAC_VERIFY                                         0x0001U /**< Option for Key Policy as CMAC verification. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_MAC_SIGN                                           0x0002U /**< Option for Key Policy as CMAC generation. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_ECB_CBC_DECRYPT                                    0x0004U /**< Option for Key Policy as ECB / CBC Decryption. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_ECB_CBC_ENCRYPT                                    0x0008U /**< Option for Key Policy as ECB / CBC Encryption. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_CCM_GCM_DECRYPT_VERIFY                             0x0010U /**< Option for Key Policy as GCM, CCM Decryption or Verification. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_CCM_GCM_ENCRYPT_SIGN                               0x0020U /**< Option for Key Policy as GCM, CCM Encryption or Signing. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_CCM_GCM_ENCRYPT_SIGN_INTERNAL_NONCE                0x0040U /**< Option for Key Policy as GCM, CCM Encryption or Signing with internal Nonce. */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_HMAC                                               0x0080U /**< Option for Key Policy as HMAC (Hash based Message Authentication Code). */
#define PHAL_NTAGXDNA_SYM_KEY_POLICY_HKDF                                               0x0100U /**< Option for Key Policy as HKDF (). */
/**
 * end of group phalNtagXDna_Symm_KeyManagement_Defines_KeyPolicy
 * @}
 */

/**
 * end of group phalNtagXDna_Symm_KeyManagement_Defines
 * @}
 */

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Depending on the currently selected AID, this command update a key of the tag or
 * of an application AKS (Active Key Set).
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffer is null.
 *                                      - For invalid card key numbers (\b bKeyNoCard) at tag or Application level.
 *                                      - If Diversification is higher than \ref PHAL_NTAGXDNA_DIV_INPUT_LEN_MAX
 *                                        "Max Diversification Length".
 * \retval #PH_ERR_KEY                  KeyType not supported.
 * \retval #PH_ERR_AUTH_ERROR           If command is called without prior authentication.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ChangeKey(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                                       /**< [In] Option to be used for diversifying the Current and New key.
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_NO_DIVERSIFICATION
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_ENCR | #PHAL_NTAGXDNA_CHGKEY_DIV_OLD_KEY
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_ENCR | #PHAL_NTAGXDNA_CHGKEY_DIV_NEW_KEY
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_ENCR | #PHAL_NTAGXDNA_CHGKEY_DIV_OLD_KEY |
                                                                                                 *            #PHAL_NTAGXDNA_CHGKEY_DIV_NEW_KEY
                                                                                                 *
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_CMAC | #PHAL_NTAGXDNA_CHGKEY_DIV_OLD_KEY
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_CMAC | #PHAL_NTAGXDNA_CHGKEY_DIV_NEW_KEY
                                                                                                 *          - #PHAL_NTAGXDNA_CHGKEY_DIV_METHOD_CMAC | #PHAL_NTAGXDNA_CHGKEY_DIV_OLD_KEY |
                                                                                                 *            #PHAL_NTAGXDNA_CHGKEY_DIV_NEW_KEY
                                                                                                 */
        uint16_t wCurrKeyNo,                                                                    /**< [In] Current key number in KeyStore of software. */
        uint16_t wCurrKeyVer,                                                                   /**< [In] Current key version in KeyStore of software. */
        uint16_t wNewKeyNo,                                                                     /**< [In] New key number in KeyStore of software. */
        uint16_t wNewKeyVer,                                                                    /**< [In] New key version in KeyStore of software. */
        uint8_t bKeyNoCard,                                                                     /**< [In] Key number of the key to be changed. To be ORed with
                                                                                                 *          - Bit[7 - 6]:
                                                                                                 *              - With the type of the key if targeting AppMasterKey or CryptoRequestKey,
                                                                                                 *                  - 10     : KeyType.AES128
                                                                                                 *                  - 11     : KeyType.AES256
                                                                                                 *              - RFU otherwise
                                                                                                 *          - Bit[5 - 0]: Application Master or CryptoRequest Key.
                                                                                                 *              - 0x00 - 0x04: Application Keys
                                                                                                 *              - 0x10 - 0x17: CryptoRequest Keys
                                                                                                 */
        uint8_t * pKeyPolicy,                                                                   /**< [In] Defines the allowed crypto operations with the targeted key.
                                                                                                 *        Optional present if targeting CryptoRequest Key.
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_DISABLED "Crypto Operation Disabled"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_MAC_VERIFY "CMAC Verification"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_MAC_SIGN "CMAC Generation"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_ECB_CBC_DECRYPT "ECB / CBC Decryption"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_ECB_CBC_ENCRYPT "ECB / CBC Encryption"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_CCM_GCM_DECRYPT_VERIFY
                                                                                                 *            "CCM, GCM Decryption / Verification"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_CCM_GCM_ENCRYPT_SIGN
                                                                                                 *            "CCM, GCM Encryption / Signing"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_CCM_GCM_ENCRYPT_SIGN_INTERNAL_NONCE
                                                                                                 *            "CCM, GCM Encryption / Signing with Internal Nonce"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_HMAC "HMAC"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SYM_KEY_POLICY_HKDF "HKDF"
                                                                                                 */
        uint8_t * pDivInput,                                                                    /**< [In] Diversification input to be used for diversifying the key. Can be NULL. */
        uint8_t bDivLen                                                                         /**< [In] Length of bytes available in \b pDivInput buffer. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * \brief Gets Application Key Setting for the application. In addition it returns
 * the number of keys which are configured for the selected application and if applicable the AppKeySetSettings.
 *
 * Note:
 *      The Option information will not be exchanged to tag in case of \ref PHAL_NTAGXDNA_KEY_SETTING_EXCLUDE_OPTION
 *      "Tag / Application Key Settings".
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetKeySettings(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Option to be used for information retrieval. One of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_KEY_SETTING_EXCLUDE_OPTION
                                                                                                 *            "Exclude exchange of Option information"
                                                                                                 *          - \ref PHAL_NTAGXDNA_KEY_SETTING_CRYPTOREQUEST_KEY_METADATA
                                                                                                 *            "CryptoRequest Keys meta-data"
                                                                                                 *          - \ref PHAL_NTAGXDNA_KEY_SETTING_ECC_PRIVATE_KEY_METADATA
                                                                                                 *            "DUOX Private Key Meta-Data"
                                                                                                 *          - \ref PHAL_NTAGXDNA_KEY_SETTING_CA_ROOT_KEY_METADATA
                                                                                                 *            "CA Root Key Meta-Data"
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Returns the key settings. */
        uint16_t * pRspLen                                                                      /**< [Out] Length bytes available in \b ppKeySettings buffer. */
    );

/**
 * \brief Reads out the current key version of any key stored on the tag
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetKeyVersion(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bKeyNo,                                                                         /**< [In] Key number of the targeted key. */
        uint8_t ** ppResponse,                                                                  /**< [Out] Key set versions of the selected application ordered
                                                                                                 *         by ascending key set number, i.e. starting with the AKS.
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b ppResponse buffer. */
    );
/**
 * end of group phalNtagXDna_Symm_KeyManagement
 * @}
 */

/* NTAG X DNA ASymmetric Key management commands --------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_ASymm_KeyManagement Commands_ASymmetricKeyManagement
 * \brief Describes about the NTAG X DNA ASymmetric Key Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_ASymm_KeyManagement_Defines Defines
 * \brief Macro Definitions for ASymmetric Key Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_ASymm_KeyManagement_Defines_TargetAction TargetAction
 * \brief Target Action options to be used with \ref phalNtagXDna_ManageKeyPair "Manage Key-Pair" interface.
 * @{
 */
#define PHAL_NTAGXDNA_TARGET_ACTION_GENERATE_KEY_PAIR                                   0x00U   /**< Option for Key Pair generation. */
#define PHAL_NTAGXDNA_TARGET_ACTION_IMPORT_PRIVATE_KEY                                  0x01U   /**< Option for Private Key Import. */
#define PHAL_NTAGXDNA_TARGET_ACTION_UPDATE_META_DATA                                    0x02U   /**< Option for Meta-Data update. */
/**
 * end of group phalNtagXDna_ASymm_KeyManagement_Defines_TargetAction
 * @}
 */

/**
 * \defgroup phalNtagXDna_ASymm_KeyManagement_Defines_KeyPolicy KeyPolicy
 * \brief KeyPolicy Options to be used with \ref phalNtagXDna_ManageKeyPair "Manage Key-Pair" interface.
 * @{
 */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_DISABLED                                          0x0000U /**< Option for Key Policy as disabled. */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_FREEZE_KUCLIMIT                                   0x8000U /**< Option for Key Policy as Freeze Key Usage Counter Limit. */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_ECC_CARD_UNILATERAL_AUTH                          0x0100U /**< Option for Key Policy as ECC Based Card-Unilateral with ISOInternalAuthenticate. */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_ECC_SECURE_DYNAMINC_MESSAGING                     0x0020U /**< Option for Key Policy as ECC Based Secure Dynamic Messaging. */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_CRYPTO_REQUEST_ECC_SIGN                           0x0010U /**< Option for Key Policy as ECC Based CryptoRequest ECC Sign. */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_CRYPTO_REQUEST_ECC_DH                             0x0008U /**< Option for Key Policy as ECC Based CryptoRequest ECC DH. */
#define PHAL_NTAGXDNA_ASYM_KEY_POLICY_ECC_SIGMA_I_MUTUAL_AUTH                           0x0004U /**< Option for Key Policy as ECC Based SIGMA-I Mutual Authentication. */
/**
 * end of group phalNtagXDna_ASymm_KeyManagement_Defines_KeyPolicy
 * @}
 */

/**
 * end of group phalNtagXDna_ASymm_KeyManagement_Defines
 * @}
 */

/**
 * \brief Creates or updates a private key entry by generating a key pair or importing a private key.
 *
 * \note
 *      - \b bComOption will be
 *          - Communication mode of the targeted key
 */
#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 *          - If targeting not yet existing key, default CommMode defined by \ref phalNtagXDna_SetConfiguration
 *            "SetConfiguration" for option \b ECC Key Management should be used. Default is
 *            \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Fully Enciphered".
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
/**
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval #PH_ERR_KEY
 *                                      - If Key type is not ECC.
 *                                      - The Key format is not Binary (Uncompressed Point Representation).
 *                                      - The Key pair is not Private type.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ManageKeyPair(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bComOption,                                                                     /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data to tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t bKeyNo,                                                                         /**< [In] Key number of the key to be managed.
                                                                                                 *          - At tag level, two keys are supported.
                                                                                                 *          - At application level, up to five keys are supported.
                                                                                                 */
        uint8_t bOption,                                                                        /**< [In] Target action to perform. Should be one of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_ACTION_GENERATE_KEY_PAIR "Generate KeyPair"
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_ACTION_IMPORT_PRIVATE_KEY "Import Private Key"
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_ACTION_UPDATE_META_DATA "Update Meta-Data"
                                                                                                 */
        uint8_t bCurveID,                                                                       /**< [In] The targeted curve. Should be one of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_NIST_P256 "P-256"
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_BRAINPOOL_P256R1 "BP-256"
                                                                                                 */
        uint8_t * pKeyPolicy,                                                                   /**< [In] Defines the allowed crypto operations with the targeted key.
                                                                                                 *           - Should be two bytes as follows,
                                                                                                 *               - Byte 0 => Bit 7 - 0
                                                                                                 *               - Byte 1 => Bit 15 - 8
                                                                                                 *
                                                                                                 *           - Supported values are, should be ORed
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_DISABLED "Disabled"
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_FREEZE_KUCLIMIT "Freeze Key Usage Counter Limit"
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_ECC_CARD_UNILATERAL_AUTH "UniLateral Authentication"
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_ECC_SECURE_DYNAMINC_MESSAGING "Secure Dynamic Messaging"
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_CRYPTO_REQUEST_ECC_SIGN "Request ECC Signature"
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_CRYPTO_REQUEST_ECC_DH "Request ECC DH"
                                                                                                 *               - \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_ECC_SIGMA_I_MUTUAL_AUTH "SIGMA-I Mutual Authentication"
                                                                                                 */
        uint8_t bWriteAccess,                                                                   /**< [In] Defines the CommMode and access right required to update
                                                                                                 *        the key with Cmd.ManageKeyPair. Should contain below information.
                                                                                                 *          - Bits[7 - 6]: RFU
                                                                                                 *          - Bits[5 - 4]: Communication Modes, One of the below values.
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
                                                                                                /**<
                                                                                                 *          - Bits[3 - 0]: Access Rights, One of the below values.
                                                                                                 *              - 0x00 - 0x0B: Authentication Required
                                                                                                 *              - 0x0C       : Free Access over NFC, Authentication required over I2C
                                                                                                 *              - 0x0D       : Free Access over I2C, Authentication required over NFC
                                                                                                 *              - 0x0E       : Free Access
                                                                                                 *              - 0x0F       : No Access or RFU
                                                                                                 */
        uint32_t dwKUCLimit,                                                                    /**< [In] Defines the key usage limit of the targeted key.
                                                                                                 *          - 0x00000000: Key Usage Counter Limit is disabled
                                                                                                 *          - Any other value: Key Usage Counter Limit enabled with the given value (LSB first).
                                                                                                 */
        uint16_t wPrivKey_No,                                                                   /**< [In] Key number in KeyStore of Private Key. */
        uint16_t wPrivKey_Pos,                                                                  /**< [In] Key position in KeyStore of Private Key. */
        uint8_t ** ppResponse,                                                                  /**< [Out] The Public Key in uncompressed point representation format.
                                                                                                 *          - Present if \b bOption = \ref PHAL_NTAGXDNA_TARGET_ACTION_GENERATE_KEY_PAIR
                                                                                                 *            "Generate KeyPair"
                                                                                                 *          - NULL otherwise
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b ppResponse buffer.
                                                                                                 *          - Actual length if \b bOption = \ref PHAL_NTAGXDNA_TARGET_ACTION_GENERATE_KEY_PAIR
                                                                                                 *            "Generate KeyPair"
                                                                                                 *          - Zero otherwise
                                                                                                 */
    );

/**
 * \brief Creates or updates a public key entry for storing a CARootKey.
 *
 * \note
 *      - \b bComOption will be
 *          - Communication mode of the targeted key
 */
#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 *          - If targeting not yet existing key, default CommMode defined by \ref phalNtagXDna_SetConfiguration
 *            "SetConfiguration" for option \b DUOX Key Management should be used. Default is
 *            \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Fully Enciphered".
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
/**
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval #PH_ERR_KEY
 *                                      - If Key type is not DUOX.
 *                                      - The Key format is not Binary (Uncompressed Point Representation).
 *                                      - The Key pair is not Public type.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ManageCARootKey(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bComOption,                                                                     /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data to tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t bKeyNo,                                                                         /**< [In] Key number of the key to be managed.
                                                                                                 *          - At tag level, two keys are supported.
                                                                                                 *          - At application level, up to five keys are supported.
                                                                                                 */
        uint8_t bCurveID,                                                                       /**< [In] The targeted curve. Should be one of the below values.
                                                                                                 *           - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_NIST_P256 "P-256"
                                                                                                 *           - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_BRAINPOOL_P256R1 "BP-256"
                                                                                                 */
        uint8_t * pAccessRights,                                                                /**< [In] Access rights associated with the CARootKey. Should be 2 byte. */
        uint8_t bWriteAccess,                                                                   /**< [In] Defines the CommMode and access right required to update
                                                                                                 *        the key with Cmd.ManageCARootKey. Should contain below information.
                                                                                                 *          - Bits[7 - 6]: RFU
                                                                                                 *          - Bits[5 - 4]: Communication Modes, One of the below values.
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
                                                                                                /**<
                                                                                                 *          - Bits[3 - 0]: Access Rights, One of the below values.
                                                                                                 *              - 0x00 - 0x0B: Authentication Required
                                                                                                 *              - 0x0C       : Free Access over NFC, Authentication required over I2C
                                                                                                 *              - 0x0D       : Free Access over I2C, Authentication required over NFC
                                                                                                 *              - 0x0E       : Free Access
                                                                                                 *              - 0x0F       : No Access or RFU
                                                                                                 */
        uint16_t wPubKey_No,                                                                    /**< [In] Key number in KeyStore of Public Key. */
        uint16_t wPubKey_Pos,                                                                   /**< [In] Key position in KeyStore of Public Key. */
        uint8_t * pIssuer,                                                                      /**< [In] The Trusted issuer name. Should be one of the following.
                                                                                                 *          - NULL in case if No trusted issuer name check required.
                                                                                                 *          - The Trusted issuer information otherwise. Ranging from 1 - 255 bytes
                                                                                                 */
        uint8_t bIssuerLen                                                                      /**< [In] Length of bytes available in \b pIssuer buffer. */
    );
/**
 * end of group phalNtagXDna_ASymm_KeyManagement
 * @}
 */

/* NTAG X DNA Certificate management commands ------------------------------------------------------------------------------------------ */
/**
 * \defgroup phalNtagXDna_CertManagement Commands_CertificateKeyManagement
 * \brief Describes about the NTAG X DNA Certificate Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_CertManagement_Defines Defines
 * \brief Macro Definitions for Certificate Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_CertManagement_Defines_Action Action
 * \brief Action options to be used with \ref phalNtagXDna_ManageCertRepo "Manage Certificate Repository" interface.
 * @{
 */
#define PHAL_NTAGXDNA_ACTION_CREATE_CERTIFICATE_REPOSITORY                              0x00U   /**< Option for Certificate Repository creation. */
#define PHAL_NTAGXDNA_ACTION_LOAD_CERTIFICATE                                           0x01U   /**< Option for Loading Certificate. */
#define PHAL_NTAGXDNA_ACTION_LOAD_CERTIFICATE_MAPPING_INFO                              0x03U   /**< Option for Loading Certificate Mapping information. */
#define PHAL_NTAGXDNA_ACTION_ACTIVATE_REPOSITORY                                        0x04U   /**< Option for Activating Repository. */
#define PHAL_NTAGXDNA_ACTION_RESET_CERTIFICATE_REPOSITORY                               0x05U   /**< Option for Resetting Certificate Repository. */
/**
 * end of group phalNtagXDna_CertManagement_Defines_Action
 * @}
 */

/**
 * \defgroup phalNtagXDna_CertManagement_Defines_DataItem DataItem
 * \brief DataItem options to be used with \ref phalNtagXDna_ReadCertRepo "Manage Certificate Repository" interface.
 * @{
 */
#define PHAL_NTAGXDNA_DATA_ITEM_END_LEAF                                                0x00U   /**< Option for Data Item as End Leaf. */
#define PHAL_NTAGXDNA_DATA_ITEM_PARENT                                                  0x01U   /**< Option for Data Item as Parent. */
#define PHAL_NTAGXDNA_DATA_ITEM_GRAND_PARENT                                            0x02U   /**< Option for Data Item as Grand Parent. */
#define PHAL_NTAGXDNA_DATA_ITEM_REPOSITORY_METADATA                                     0xFFU   /**< Option for Repository Meta-data. */
/**
 * end of group phalNtagXDna_CertManagement_Defines_DataItem
 * @}
 */

/**
 * end of group phalNtagXDna_CertManagement_Defines
 * @}
 */

/**
 * \brief Manages Certificate Repositories.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ManageCertRepo(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bComOption,                                                                     /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data to tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t bAction,                                                                        /**< [In] Target action to perform. Should be one of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_ACTION_CREATE_CERTIFICATE_REPOSITORY
                                                                                                 *            "Create Certificate Repository"
                                                                                                 *          - \ref PHAL_NTAGXDNA_ACTION_LOAD_CERTIFICATE "Load Certificate"
                                                                                                 *          - \ref PHAL_NTAGXDNA_ACTION_LOAD_CERTIFICATE_MAPPING_INFO
                                                                                                 *            "Load Certificate Mapping Information"
                                                                                                 *          - \ref PHAL_NTAGXDNA_ACTION_ACTIVATE_REPOSITORY "Activate Repository"
                                                                                                 *          - \ref PHAL_NTAGXDNA_ACTION_RESET_CERTIFICATE_REPOSITORY
                                                                                                 *            "Reset Certificate Repository"
                                                                                                 */
        uint8_t bRepoID,                                                                        /**< [In] ID used to identify certificate repository for algorithm execution and
                                                                                                 *        repository modification. Note - the certificate Id shall be used to
                                                                                                 *        reference a private key/certificate chain when performing SIGMA-I
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Data to be exchanged to tag based on \b bAction information. */
        uint16_t wDataLen                                                                       /**< [In] Length of bytes available in \b pData buffer. */
    );

/**
 * \brief Perform Certificate Repository Read.
 * Returns information related to certificate repositories
 *
 * \note
 *      - Chaining upto the size of below mentioned buffers are handled within this interface. If more data is to be read,
 *        the user has to call this function again with bOption = #PH_EXCHANGE_RXCHAINING | one of the communication options.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bOption is not supported.
 *                                      - For Invalid Exchange option value (\b bOption).
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ReadCertRepo(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Options for processing of Secure Messaging and reading of data.
                                                                                                 *          - Reading data.
                                                                                                 *              - #PH_EXCHANGE_DEFAULT   : Exchanges the command and received the Data.
                                                                                                 *              - #PH_EXCHANGE_RXCHAINING: To Receive remaining Data.
                                                                                                 *
                                                                                                 *          - Communication modes. To be ORed with above values
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bRepoID,                                                                        /**< [In] ID used to identify certificate repository. */
        uint8_t bDataItem,                                                                      /**< [In] Data Item to be used for receiving the information.
                                                                                                 *        Should be one of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_DATA_ITEM_END_LEAF "End Leaf"
                                                                                                 *          - \ref PHAL_NTAGXDNA_DATA_ITEM_PARENT "Parent"
                                                                                                 *          - \ref PHAL_NTAGXDNA_DATA_ITEM_GRAND_PARENT "Grand Parent"
                                                                                                 *          - \ref PHAL_NTAGXDNA_DATA_ITEM_REPOSITORY_METADATA "Repository Meta-data"
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Response based on \b bDataItem information . */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b pRspLen buffer. */
    );
/**
 * end of group phalNtagXDna_CertManagement
 * @}
 */

/* NTAG X DNA File management commands ------------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_FileManagement Commands_FileManagement
 * \brief Describes about the NTAG X DNA File Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_FileManagement_Defines Defines
 * \brief Macro Definitions for File Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_FileManagement_Defines_FileOptions FileOptions
 * \brief The File Options to be used for all the File management commands.
 * @{
 */
#define PHAL_NTAGXDNA_FILE_OPTION_PLAIN                                                 0x00U   /**< Option for File communication mode as Plain. */
#define PHAL_NTAGXDNA_FILE_OPTION_PLAIN_1                                               0x02U   /**< Option for File communication mode as Plain. */
#define PHAL_NTAGXDNA_FILE_OPTION_MAC                                                   0x01U   /**< Option for File communication mode as Mac. */
#define PHAL_NTAGXDNA_FILE_OPTION_FULL                                                  0x03U   /**< Option for File communication mode as Full. */

#define PHAL_NTAGXDNA_FILE_OPTION_SDM_MIRRORING_ENABLED                                 0x40U   /**< Option to Enable Secure Dynamic Messaging and Mirroring support. */
#define PHAL_NTAGXDNA_FILE_OPTION_DEFERRED_CONFIGURATION_ENABLED                        0x08U   /**< Option to Enable Deferred Configuration support. */
/**
 * end of group phalNtagXDna_FileManagement_Defines_FileOptions
 * @}
 */

/**
 * \defgroup phalNtagXDna_Defines_File_Options File_Options
 * \brief Options for file creation interfaces.
 * @{
 */
#define PHAL_NTAGXDNA_ISO_FILE_INFO_NOT_AVAILABLE                                       0x00U   /**< Option to indicate no ISO File ID or ISODFName are present. */
#define PHAL_NTAGXDNA_ISO_FILE_ID_AVAILABLE                                             0x01U   /**< Option to indicate the presence of ISO FileID. */
/**
 * end of group phalNtagXDna_Defines_Appln_File_Options
 * @}
 */

/**
 * end of group phalNtagXDna_FileManagement_Defines
 * @}
 */

/**
 * \brief Creates files for the storage of plain unformatted user data within an existing application
 * on the tag.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid Option (\b bOption) information.
 *                                      - For Invalid File numbers (\b bFileNo).
 *                                      - For Invalid File communication mode (\b bFileOption).
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CreateStdDataFile(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Option to represent the presence of ISO information.
                                                                                                 *          - \ref PHAL_NTAGXDNA_ISO_FILE_INFO_NOT_AVAILABLE
                                                                                                 *            "ISO File Info not available"
                                                                                                 *          - \ref PHAL_NTAGXDNA_ISO_FILE_ID_AVAILABLE "ISO File ID Available"
                                                                                                 */
        uint8_t bFileNo,                                                                        /**< [In] The file number to be created. */
        uint8_t * pISOFileId,                                                                   /**< [In] ISO File ID to be used. Should be two bytes. */
        uint8_t bFileOption,                                                                    /**< [In] Option for the targeted file.
                                                                                                 *          - Communication settings for the file.
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_PLAIN "Plain Mode"
																								 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
                                                                                                /**<
                                                                                                 *          - ORed with one of the above options.
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_SDM_MIRRORING_ENABLED
                                                                                                 *                "SDM Mirroring Enabled"
                                                                                                 */
        uint8_t * pAccessRights,                                                                /**< [In] The new access right to be applied for the file. Should be 2 byte.
                                                                                                 *          - Bit[15 - 12]: Read
                                                                                                 *          - Bit[11 - 8] : Write
                                                                                                 *          - Bit[7 - 4]  : ReadWrite
                                                                                                 *          - Bit[3 - 0]  : Change or RFU. Change for the 1st mandatory set of access
                                                                                                 *                          condition else RFU (i.e. 0xF)
                                                                                                 *
                                                                                                 *          - Below are the values for the above bits.
                                                                                                 *              - 0x00 - 0x0B: Authentication Required
                                                                                                 *              - 0x0C       : Free Access over NFC, Authentication required over I2C
                                                                                                 *              - 0x0D       : Free Access over I2C, Authentication required over NFC
                                                                                                 *              - 0x0E       : Free Access
                                                                                                 *              - 0x0F       : No Access or RFU
                                                                                                 */
        uint8_t * pFileSize                                                                     /**< [In] The size of the file. Will be of 3 bytes with LSB first.
                                                                                                 *        If size 0x10 need to be created, then the FileSize will be 10 00 00.
                                                                                                 */
    );

/**
 * \brief Creates files for the storage of plain unformatted Counter information.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid Option (\b bOption) information.
 *                                      - For Invalid File numbers (\b bFileNo).
 *                                      - For Invalid File communication mode (\b bFileOption).
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CreateCounterFile(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo,                                                                        /**< [In] The file number to be created. */
        uint8_t bFileOption,                                                                    /**< [In] Option for the targeted file.
                                                                                                 *          - Communication settings for the file.
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t * pAccessRights,                                                                /**< [In] The new access right to be applied for the file. Should be 2 byte.
                                                                                                 *          - Bit[15 - 12]: Read
                                                                                                 *          - Bit[11 - 8] : Write
                                                                                                 *          - Bit[7 - 4]  : ReadWrite
                                                                                                 *          - Bit[3 - 0]  : Change or RFU. Change for the 1st mandatory set of access
                                                                                                 *                          condition else RFU (i.e. 0xF)
                                                                                                 *
                                                                                                 *          - Below are the values for the above bits.
                                                                                                 *              - 0x00 - 0x0B: Authentication Required
                                                                                                 *              - 0x0C       : Free Access over NFC, Authentication required over I2C
                                                                                                 *              - 0x0D       : Free Access over I2C, Authentication required over NFC
                                                                                                 *              - 0x0E       : Free Access
                                                                                                 *              - 0x0F       : No Access or RFU
                                                                                                 */
        uint32_t dwValue                                                                        /**< [In] Counter Value to be stored.
                                                                                                 *        Provide the counter value in regular format, like if 0x10 needs to be updated
                                                                                                 *        then \b dwValue = 0x00000010
                                                                                                 */
    );

/**
 * \brief Returns the file IDs of all active files within the currently selected application.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetFileIDs(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t ** ppFileId,                                                                    /**< [Out] The buffer containing the available File ID(s). */
        uint16_t * pFileIdLen                                                                   /**< [Out] Length of bytes available in \b ppFid buffer. */
    );

/**
 * \brief Get the ISO File IDs.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetISOFileIDs(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t ** ppISOFileId,                                                                 /**< [Out] The buffer containing the available ISO File ID(s). */
        uint16_t * pISOFileIdLen                                                                /**< [Out] Length of bytes available in \b ppFid buffer. */
    );

/**
 * \brief Get information on the properties of a specific file
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetFileSettings(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bFileNo,                                                                        /**< [In] The file number for which the setting to be retrieved. */
        uint8_t ** ppFSBuffer,                                                                  /**< [Out] The buffer containing the settings. */
        uint16_t * pFSBufLen                                                                    /**< [Out] Length of bytes available in \b ppFSBuffer buffer. */
    );

/**
 * \brief Get file related counters used for Secure Dynamic Messaging.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffer is null.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_GetFileCounters(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data to tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t bFileNo,                                                                        /**< [In] File number for which the Counter information need to be received. */
        uint8_t ** ppFileCounters,                                                              /**< [Out] The SDMReadCounter information returned by the tag. */
        uint16_t * pFileCounterLen                                                              /**< [Out] Length of bytes available in \b ppFileCounters buffer. */
    );

/**
 * \brief Changes the access parameters of an existing file.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid File numbers (\b bFileNo).
 *                                      - For Invalid File communication mode (\b bFileOption).
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ChangeFileSettings(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data to tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t bFileNo,                                                                        /**< [In] File number for which the setting need to be updated. */
        uint8_t bFileOption,                                                                    /**< [In] Option for the targeted file.
                                                                                                 *          - Communication settings for the file.
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
                                                                                                /**<
                                                                                                 *          - ORed with one of the above options.
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_SDM_MIRRORING_ENABLED
                                                                                                 *                "SDM Mirroring Enabled"
                                                                                                 *              - \ref PHAL_NTAGXDNA_FILE_OPTION_DEFERRED_CONFIGURATION_ENABLED
                                                                                                 *                "Deferred Configuration Enabled"
                                                                                                 */
        uint8_t * pAccessRights,                                                                /**< [In] Set of access conditions for the 1st set in the file. Should be 2 byte. */
        uint8_t * pAddInfo,                                                                     /**< [In] Buffer should contain the following information. \n
                                                                                                 *          - [SDMOption || SDM AccessRights || UIDOffset || SDMReadCtrOffset ||
                                                                                                 *            PICCDataOffset || GPIOStatusOffset || SDMMACInputOffset || SDMENCOffset ||
                                                                                                 *            SDMENCLength || SDMMACOffset || SDMReadCtrLimit] ||
                                                                                                 *          - [DeferOption] || [DeferMethod]
                                                                                                 */
        uint8_t bAddInfoLen                                                                     /**< [In] Length of bytes available in \b pAddInfo buffer. */
    );

/**
 * end of group phalNtagXDna_FileManagement
 * @}
 */

/* NTAG X DNA Data management commands ------------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_DataManagement Commands_DataManagement
 * \brief Describes about the NTAG X DNA Data Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_DataManagement_Defines Defines
 * \brief Macro Definitions for Data Management commands.
 * @{
 */

/**
 * end of group phalNtagXDna_DataManagement_Defines
 * @}
 */

/**
 * \brief Reads data from Standard data files.
 *
 * \note
 *      - Chaining upto the size of below mentioned buffers are handled within this interface. If more data is to be read,
 *        the user has to call this function again with bOption = #PH_EXCHANGE_RXCHAINING | one of the communication options.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_SUCCESS_CHAINING     Indicating more data to be read.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid File Number (\b bFileNo).
 *                                      - For Invalid Communication option value (\b bOption).
 *                                      - For Invalid Exchange option value (\b bOption).
 * \retval XXXX
 *                                      - Depending on status codes return by PICC.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ReadData(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Options for processing of Secure Messaging and reading of data.
                                                                                                 *          - Reading data.
                                                                                                 *              - #PH_EXCHANGE_DEFAULT   : Exchanges the command and received the Data.
                                                                                                 *              - #PH_EXCHANGE_RXCHAINING: To Receive remaining Data.
                                                                                                 *
                                                                                                 *          - Communication modes. To be ORed with above values
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *              - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bFileNo,                                                                        /**< [In] The file number from where the data to be read. */
        uint8_t * pOffset,                                                                      /**< [In] The offset from where the data should be read. Will be of 3 bytes with LSB first.
                                                                                                 *        If 0x10 need to be offset, then it will be 10 00 00.
                                                                                                 *          - 0 to (FixeSize - 1): Starting position of Read operation.
                                                                                                 *          - 0xFFFFFFFF         : Return CRLFile Meta-Data
                                                                                                 */
        uint8_t * pLength,                                                                      /**< [In] The number of bytes to be read. Will be of 3 bytes with LSB first.
                                                                                                 *          - If 0x10 bytes need to be read, then it will be 10 00 00.
                                                                                                 *          - If complete file need to be read, then it will be 00 00 00.
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] The data returned by the PICC. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Writes data to standard data files.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid File Number (\b bFileNo).
 *                                      - For Invalid Communication option value (\b bOption).
 * \retval XXXX
 *                                      - Depending on status codes return by PICC.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_WriteData(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Options for processing of Secure Messaging and writing of data.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bFileNo,                                                                        /**< [In] The file number to which the data to be written. */
        uint8_t * pOffset,                                                                      /**< [In] The offset from where the data should be written. Will be of 3 bytes with LSB first.
                                                                                                 *        If 0x10 need to be offset, then it will be 10 00 00.
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] The data to be written to the PICC. */
        uint8_t * pLength                                                                       /**< [In] The number of bytes to be written. Will be of 3 bytes with LSB first.
                                                                                                 *          - If 0x10 bytes need to be written, then it will be 10 00 00.
                                                                                                 */
    );

/**
 * \brief Increments the counter file.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - For Invalid File Number (\b bFileNo).
 *                                      - For Invalid Communication option value (\b bOption).
 * \retval XXXX
 *                                      - Depending on status codes return by PICC.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_IncrementCounterFile(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Options for processing of Secure Messaging and writing of data.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bFileNo,                                                                        /**< [In] The file number to which the value to be incremented. */
        uint32_t dwIncrValue                                                                    /**< [In] The value to be incremented.
                                                                                                 *        Provide the counter value in regular format, like if 0x10 needs to be incremented
                                                                                                 *        then \b dwIncrValue = 0x00000010
                                                                                                 */
    );
/**
 * end of group phalNtagXDna_DataManagement
 * @}
 */

/* NTAG X DNA CryptoAPI commands ------------------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_CryptoAPI Commands_CryptoAPI
 * \brief Describes about the NTAG X DNA Crypto API commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_CryptoAPI_Defines Defines
 * \brief Macro Definitions for Crypto API commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_CryptoAPI_Defines_Operation Operation
 * \brief The Options for representing Crypto Operation. To be used with
 *  - \ref phalNtagXDna_CryptoRequestSHA "CryptoRequest SHA" interface
 *  - \ref phalNtagXDna_CryptoRequestECCSign "CryptoRequest ECC_Sign" interface
 *  - \ref phalNtagXDna_CryptoRequestECCVerify "CryptoRequest ECC_Verify" interface
 *  - \ref phalNtagXDna_CryptoRequestECCDH "CryptoRequest ECC_DH" interface
 *  - \ref phalNtagXDna_CryptoRequestAES "CryptoRequest AES" interface
 *  - \ref phalNtagXDna_CryptoRequestAESCMAC "CryptoRequest AES CMAC" interface
 *  - \ref phalNtagXDna_CryptoRequestHMAC "CryptoRequest HMAC" interface
 *  - \ref phalNtagXDna_CryptoRequestHKDF "CryptoRequest HKDF" interface
 * @{
 */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT                                          0x01U   /**< Option to indicate initialization of Crypto Operation. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE                                        0x02U   /**< Option to indicate update of Crypto Operation. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE                                      0x03U   /**< Option to indicate finalization of Crypto Operation. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT                                      0x04U   /**< Option to indicate Crypto Operation in one shot. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT_PRE_COMPUTED_HASH                    0x05U   /**< Option to indicate Crypto Operation in one shot with Precomputed Hash. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_STEP                                      0x01U   /**< Option to indicate ECC DH Crypto Operation in One Step. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_TWO_STEP_FIRST                                0x02U   /**< Option to indicate ECC DH Crypto Operation in Two Step first step. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_TWO_STEP_FINAL                                0x03U   /**< Option to indicate ECC DH Crypto Operation in Two Step final step. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_EXTRACT_EXPAND                                0x00U   /**< Option to indicate Expand and Extract HKDF Operation. */
#define PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_EXPAND                                        0x01U   /**< Option to indicate Expand HKDF Operation. */
/**
 * end of group phalNtagXDna_CryptoAPI_Defines_Operation
 * @}
 */

/**
 * \defgroup phalNtagXDna_CryptoAPI_Defines_Algorithm Algorithm
 * \brief The Options for representing Crypto Algorithm's. To be used with
 *  - \ref phalNtagXDna_CryptoRequestSHA "CryptoRequest SHA" interface
 *  - \ref phalNtagXDna_CryptoRequestHMAC "CryptoRequest HMAC" interface
 * @{
 */
#define PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_256                                       0x01U   /**< Option to indicate Crypto Algorithm as SHA-256. */
#define PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_384                                       0x02U   /**< Option to indicate Crypto Algorithm as SHA-384. */
/**
 * end of group phalNtagXDna_CryptoAPI_Defines_Algorithm
 * @}
 */

/**
 * \defgroup phalNtagXDna_CryptoAPI_Defines_Primitive Primitive
 * \brief The Options for representing Primitive. To be used with
 *  - \ref phalNtagXDna_CryptoRequestAES "CryptoRequest AES" interface
 *  - \ref phalNtagXDna_CryptoRequestAESCMAC "CryptoRequest AES CMAC" interface
 *  - \ref phalNtagXDna_CryptoRequestHMAC "CryptoRequest HMAC" interface
 * @{
 */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_SIGN                                          0x01U   /**< Option to indicate Primitive as Sign (AES-CMAC or HMAC Generation). */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_VERIFY                                        0x02U   /**< Option to indicate Primitive as Verify (AES-CMAC or HMAC Verification). */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CBC_ENCRYPT                                   0x03U   /**< Option to indicate Primitive as AES-CBC Encryption. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CBC_DECRYPT                                   0x04U   /**< Option to indicate Primitive as AES-CBC Decryption. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_ECB_ENCRYPT                                   0x05U   /**< Option to indicate Primitive as AES-ECB Encryption. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_ECB_DECRYPT                                   0x06U   /**< Option to indicate Primitive as AES-ECB Decryption. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_ENCRYPT_SIGN                              0x07U   /**< Option to indicate Primitive as CCM Encryption / Signing. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_ENCRYPT_SIGN_INTERNAL_NONCE               0x08U   /**< Option to indicate Primitive as CCM Encryption / Signing with
                                                                                                 *   internally generate nonce.
                                                                                                 */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_DECRYPT_VERIFY                            0x09U   /**< Option to indicate Primitive as CCM Decryption / Verification. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_ENCRYPT_SIGN                              0x0AU   /**< Option to indicate Primitive as GCM Encryption / Signing. */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_ENCRYPT_SIGN_INTERNAL_NONCE               0x0BU   /**< Option to indicate Primitive as GCM Encryption / Signing with
                                                                                                 *   internally generate nonce.
                                                                                                 */
#define PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_DECRYPT_VERIFY                            0x0CU   /**< Option to indicate AES Primitive as GCM Decryption / Verification. */
/**
 * end of group phalNtagXDna_CryptoAPI_Defines_Primitive
 * @}
 */

/**
 * end of group phalNtagXDna_CryptoAPI_Defines
 * @}
 */

/**
 * \brief Perform CryptoRequest SHA.
 * It is possible to execute an SHA calculation using a single command or as a series of commands.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestSHA(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 */
        uint8_t bAlgorithm,                                                                     /**< [In] Indicates the Algorithm to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_256 "SHA-256"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_384 "SHA-384"
                                                                                                 */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t bResultDst,                                                                     /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Input data to be hashed. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be hashed if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen,                                                                  /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the Hashed information. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest RNG.
 * It is possible to generate random data, which is compliant with NIST SP800-90B using a 256-bit key.
 * The Maximum number of generated bytes is 128.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestRNG(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bNumBytes,                                                                      /**< [In] The number of bytes to generate random bytes. */
        uint8_t bResultDst,                                                                     /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the Random data. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest ECC Signature generation.
 * The ECC signature generation API supports signing of a data stream or a pre-computed hash.
 *
 * \note: Before using this interface, a PrivateKey should be generated or Imported using
 * \ref phalNtagXDna_ManageKeyPair "Manage Key-Pair" interface with Key-Policy set to
 * \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_CRYPTO_REQUEST_ECC_SIGN "Request ECC Signature".
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestECCSign(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT_PRE_COMPUTED_HASH
                                                                                                 *            "One-Shot with Pre-Computed Hash"
                                                                                                 */
        uint8_t bPrivateKeyID,                                                                  /**< [In] Id of the ECC key pair containing the private key to use.
                                                                                                 *        Note a key pair must be marked as 'Crypto API Signature'.
                                                                                                 */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Input data to be Signed. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be Signed if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen,                                                                  /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the Signature information. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest ECC Signature verification.
 * The ECC signature verification API supports verification of a data stream or data, which has
 * already been hashed.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval #PHAL_NTAGXDNA_ERR_SIGNATURE_VERIFICATION_FAILURE  Verification of Message / Signature combination failed.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestECCVerify(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT_PRE_COMPUTED_HASH
                                                                                                 *            "One-Shot with Pre-Computed Hash"
                                                                                                 */
        uint8_t bCurveID,                                                                       /**< [In] The targeted curve. Should be one of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_NIST_P256 "P-256"
                                                                                                 *          - \ref PHAL_NTAGXDNA_TARGET_CURVE_ID_BRAINPOOL_P256R1 "BP-256"
                                                                                                 */
        uint8_t * pHostPubKey,                                                                  /**< [In] The public key to use for signature verification provided in
                                                                                                 *        uncompressed format. i.e. leading 0x04 byte
                                                                                                 */
        uint8_t bHostPubKeyLen,                                                                 /**< [In] Length of Bytes available in \b pHostPubKey buffer. */
        uint8_t * pSignature,                                                                   /**< [In] Signature to verify. */
        uint8_t bSigLen,                                                                        /**< [In] Length of Bytes available in \b pSignature buffer. */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Input data to be verified. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be verified if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen                                                                   /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
    );

/**
 * \brief Perform CryptoRequest ECC Diffie-Hellman.
 * The ECC Diffie-Hellman API supports the use of static keys or ephemeral keys. In addition, it allows the shared
 * secret to be generated using a single or two-step approach.
 *
 * \note: Before using this interface, a PrivateKey should be generated or Imported using
 * \ref phalNtagXDna_ManageKeyPair "Manage Key-Pair" interface with Key-Policy set to
 * \ref PHAL_NTAGXDNA_ASYM_KEY_POLICY_CRYPTO_REQUEST_ECC_DH "Request ECC DH".
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestECCDH(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_STEP "One-Step Operation"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_TWO_STEP_FIRST "Two Step - First"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_TWO_STEP_FINAL "Two Step - Final"
                                                                                                 */
        uint8_t bKeyPairID,                                                                     /**< [In] Should be one of the below values.
                                                                                                 *          - 0x00 - 0x04: Static key pair - the key pair must be marked
                                                                                                 *                         as Crypto API ECDH
                                                                                                 *          - 0xFE       : Use NIST 256 ephemeral key pair
                                                                                                 *          - 0xFF       : Use Brain-pool 256 ephemeral key pair
                                                                                                 */
        uint8_t bSSDestination,                                                                 /**< [In] Crypto API Shared Secret destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pHostPubKey,                                                                  /**< [In] The host's public key to use for shared secret generation,
                                                                                                 *        provided in uncompressed format I.e leading 0x04 byte
                                                                                                 */
        uint8_t bHostPubKeyLen,                                                                 /**< [In] Length of Bytes available in \b pHostPubKey buffer. */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing,
                                                                                                 *      - [Card's ephemeral PublicKey]: If key pair Id indicates an
                                                                                                 *        ephemeral key and single step or two-step step 1
                                                                                                 *      - [Shared Secret]: If single step or two-step step 2 and output
                                                                                                 *        destination is the command buffer
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest AES ECB / CBC Encryption or Decryption.
 * \note: The AES primitives supported by a static key are defined by the KeyPolicy set via the ChangeKey command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestAES(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 */
        uint8_t bPrimitive,                                                                     /**< [In] One of the following Primitive.
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CBC_ENCRYPT
                                                                                                 *            "AES Primitive for CBC Encryption"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CBC_DECRYPT
                                                                                                 *            "AES Primitive for CBC Decryption"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_ECB_ENCRYPT
                                                                                                 *            "AES Primitive for ECB Encryption"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_ECB_DECRYPT
                                                                                                 *            "AES Primitive for ECB Decryption"
                                                                                                 */
        uint8_t bKeyID,                                                                         /**< [In] Id of the AES key.
                                                                                                 *          - 0x10 - 0x17: Id of AES Key (must be in crypto API range: '10' - '17'),
                                                                                                 *                         the key length from the static key.
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number containing the AES key, the
                                                                                                 *                         key length shall be in the following field
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number containing the AES key, the key
                                                                                                 *                         length shall be in the following field
                                                                                                 */
        uint8_t bKeyLen,                                                                        /**< [In] Length of AES key, only present when the key source is an
                                                                                                 *        internal buffer.
                                                                                                 */
        uint8_t bICVSource,                                                                     /**< [In] Crypto API ICV Source Selection. Only present for CBC operations.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pICV,                                                                         /**< [In] Only present for CBC operations and the ICV is in the
                                                                                                 *        command buffer.
                                                                                                 */
        uint8_t bICVLen,                                                                        /**< [In] Length of Bytes available in \b pICV buffer. */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t bResultDst,                                                                     /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Data to be Encrypted or Decrypted. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be encrypted or decrypted if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen,                                                                  /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the Encrypted or Decrypted data. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest AES CMAC Signing or Verification.
 * \note: The AES API supports the use of static crypto API keys or keys stored in an internal buffer. The AES primitives
 * supported by a static key are defined by the KeyPolicy set via the ChangeKey command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval #PHAL_NTAGXDNA_ERR_SIGNATURE_VERIFICATION_FAILURE  Verification of Message / Signature combination failed.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestAESCMAC(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 */
        uint8_t bPrimitive,                                                                     /**< [In] One of the following Primitive.
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_SIGN
                                                                                                 *            "CMAC Signing"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_VERIFY
                                                                                                 *            "CMAC Verification"
                                                                                                 */
        uint8_t bKeyID,                                                                         /**< [In] Id of the AES key.
                                                                                                 *          - 0x10 - 0x17: Id of AES Key (must be in crypto API range: '10' - '17'),
                                                                                                 *                         the key length from the static key.
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number containing the AES key, the
                                                                                                 *                         key length shall be in the following field
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number containing the AES key, the key
                                                                                                 *                         length shall be in the following field
                                                                                                 */
        uint8_t bKeyLen,                                                                        /**< [In] Length of AES key, only present when the key source is an
                                                                                                 *        internal buffer.
                                                                                                 */
        uint8_t * pCMACSignature,                                                               /**< [In] CMAC Signature to be verified.
                                                                                                 *          - NULL if Primitive is \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_SIGN
                                                                                                 *            "CMAC Signing"
                                                                                                 *          - 8 byte or 16 byte CMAC Signature if Primitive is
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_VERIFY "CMAC Verification"
                                                                                                 */
        uint8_t bCMACSignLen,                                                                   /**< [In] Length of Bytes available in \b pCMACSignature buffer. */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Data to be Signed or Verified. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be Signed or Verified if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen,                                                                  /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the following,
                                                                                                 *          - 16 byte CMAC Signature if Primitive is
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_VERIFY "CMAC Verification"
                                                                                                 *          - NULL if Primitive is \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_SIGN
                                                                                                 *            "CMAC Signing"
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest AES AEAD Encryption / Signing or Decryption / Verification.
 *  - The AES API supports the use of static crypto API keys or keys stored in an internal buffer. The AES primitives
 *    supported by a static key are defined by the KeyPolicy set via the ChangeKey command.
 *  - The output destination for multi-part AEAD shall always be the command buffer. For a one-shot operation, the
 *    result destination can be an internal static or transient buffer
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval #PHAL_NTAGXDNA_ERR_SIGNATURE_VERIFICATION_FAILURE  Verification of Message / Signature combination failed.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestAESAEAD(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 */
        uint8_t bPrimitive,                                                                     /**< [In] One of the following Primitive.
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_ENCRYPT_SIGN
                                                                                                 *            "AES Primitive for CCM Encryption / Signing"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_ENCRYPT_SIGN_INTERNAL_NONCE
                                                                                                 *            "AES Primitive for CCM Encryption / Signing with Internal Nonce"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_DECRYPT_VERIFY
                                                                                                 *            "AES Primitive for CCM Decryption / Verification"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_ENCRYPT_SIGN
                                                                                                 *            "AES Primitive for GCM Encryption / Signing"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_ENCRYPT_SIGN_INTERNAL_NONCE
                                                                                                 *            "AES Primitive for GCM Encryption / Signing with Internal Nonce"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_DECRYPT_VERIFY
                                                                                                 *            "AES Primitive for GCM Decryption / Verification"
                                                                                                 */
        uint8_t bKeyID,                                                                         /**< [In] Id of the AES key.
                                                                                                 *          - 0x10 - 0x17: Id of AES Key (must be in crypto API range: '10' - '17'),
                                                                                                 *                         the key length from the static key.
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number containing the AES key, the
                                                                                                 *                         key length shall be in the following field
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number containing the AES key, the key
                                                                                                 *                         length shall be in the following field
                                                                                                 */
        uint8_t bKeyLen,                                                                        /**< [In] Length of AES key, only present when the key source is an
                                                                                                 *        internal buffer.
                                                                                                 */
        uint8_t bNonceSource,                                                                   /**< [In] Crypto API Nonce Source Selection. Not present when \b bPrimitive is
                                                                                                 *        \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_CCM_ENCRYPT_SIGN_INTERNAL_NONCE
                                                                                                 *        "AES Primitive for CCM Encryption / Signing with Internal Nonce" or
                                                                                                 *        \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_GCM_ENCRYPT_SIGN_INTERNAL_NONCE
                                                                                                 *        "AES Primitive for GCM Encryption / Signing with Internal Nonce"
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pNonce,                                                                       /**< [In] Nonce to be used. One of the following,
                                                                                                 *          - Present for \b bPrimitive = Internally generated
                                                                                                 *          - NULL Otherwise
                                                                                                 */
        uint8_t bNonceLen,                                                                      /**< [In] Length of Nonce.One of the following,
                                                                                                 *      - 0x0D       : AES CCM
                                                                                                 *      - 0x0C - 0x3C: AES GCM
                                                                                                 */
        uint16_t wTotal_AAdLen,                                                                 /**< Total length of AAD information that will be exchanged. This will be application
                                                                                                 *   for below primitives,
                                                                                                 *      - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *      - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *      - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 */
        uint16_t wTotal_InputDataLen,                                                           /**< Total length of Input data that will be exchanged. This will be application
                                                                                                 *   for below primitives,
                                                                                                 *      - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *      - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *      - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 */
        uint8_t bAADSource,                                                                     /**< [In] Crypto API AAD Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pAAD,                                                                         /**< [In] AAD data. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be encrypted or decrypted if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bAADLen,                                                                        /**< [In] Length AAD as mentioned below
                                                                                                 *          - Actual data length if Input AAD source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pAAD buffer.
                                                                                                 */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Data to be Encrypted / Signed or Decrypted / Verified. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be Encrypted / Signed or Decrypted / Verified if Input
                                                                                                 *            data source is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen,                                                                  /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
        uint8_t * pTagData,                                                                     /**< [In] Tag Data to be used. One of the following
                                                                                                 *          - Tag information to be used. Only present when \b bPrimitive is
                                                                                                 *            Decrypt or Verify.
                                                                                                 *          - NULL otherwise.
                                                                                                 */
        uint8_t bTagDataLen,                                                                    /**< [In] Tag length as mentioned below
                                                                                                 *          - 0x08 or 0x10: CCM
                                                                                                 *          - 0x0C or 0x10: GCM
                                                                                                 */
        uint8_t bResultDst,                                                                     /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the following
                                                                                                 *          - Nonce if
                                                                                                 *              - \b bOperation is \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT
                                                                                                 *                "One-Shot" or \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *              - \b bPrimitive is Encrypt / Sign with internally generated Nonce.
                                                                                                 *          - Encrypted / Decrypted data for any of the operation.
                                                                                                 *          - Tag Data \b bPrimitive is Encryption / Sign and \b bOperation is
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot" or
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest Write Internal Buffer.
 * It is possible to write a specific value to an internal buffer using this command option. This allows data to be
 * loaded for use within other crypto API operations
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestWriteInternalBuffer(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bDestination,                                                                   /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Data to write to the internal buffer. */
        uint8_t bDataLen                                                                        /**< [In] Length of Bytes available in \b pData buffer. */
    );

/**
 * \brief Perform CryptoRequest HMAC (Hash-Based Message Authentication Code) Signature generation and verification.
 * \note: It is possible to execute an HMAC calculation using a single command or as a series of commands.
 * Using multiple steps allows the input data to be taken from different sources. The API uses a secure SHA
 * implementation.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval #PHAL_NTAGXDNA_ERR_SIGNATURE_VERIFICATION_FAILURE  Verification of Message / Signature combination failed.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestHMAC(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_INIT "Initialize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_UPDATE "Update"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot"
                                                                                                 */
        uint8_t bPrimitive,                                                                     /**< [In] One of the following Primitive.
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_SIGN "HMAC Signing"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_VERIFY "HMAC Verification"
                                                                                                 */
        uint8_t bDigestAlgo,                                                                    /**< [In] Indicates the Algorithm to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_256 "SHA-256"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_384 "SHA-384"
                                                                                                 */
        uint8_t bKeyID,                                                                         /**< [In] Id of the HMAC key.
                                                                                                 *          - 0x10 - 0x17: Id of AES Key (must be in crypto API range: '10' - '17'),
                                                                                                 *                         the key length from the static key.
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number containing the AES key, the
                                                                                                 *                         key length shall be in the following field
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number containing the AES key, the key
                                                                                                 *                         length shall be in the following field
                                                                                                 */
        uint8_t bKeyLen,                                                                        /**< [In] Length of HMAC key, only present when the key source is an
                                                                                                 *        internal buffer.
                                                                                                 */
        uint8_t * pHMac,                                                                        /**< [In] Hash MAC Signature to be verified.
                                                                                                 *          - 32 byte or 48 byte HMac signature if \b bOperation are
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_FINALIZE "Finalize" or
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_ONE_SHOT "One-Shot" and \b bPrimitive
                                                                                                 *            is \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_VERIFY "HMAC Verification"
                                                                                                 *          - NULL otherwise
                                                                                                 */
        uint8_t bHMacLen,                                                                       /**< [In] Length of Bytes available in \b pHMac buffer. */
        uint8_t bInputDataSource,                                                               /**< [In] Crypto API Data Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInputData,                                                                   /**< [In] Data to be Signed or Verified. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be Signed or Verified if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInputDataLen,                                                                  /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInputData buffer.
                                                                                                 */
        uint8_t bResultDst,                                                                     /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the following,
                                                                                                 *          - 32 byte or 48 byte HMac signature if \b bPrimitive is
                                                                                                 *            \ref PHAL_NTAGXDNA_CRYPTOAPI_PRIMITIVE_SIGN "HMAC Sign"
                                                                                                 *          - NULL otherwise
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest HKDF (HMAC Key Derivation function) Extract and Expand.
 * HKDF, as defined in RFC5869, requires execution of the extract operation followed by the expand operation.
 * The API uses a secure SHA implementation
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestHKDF(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Indicates the Operation to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_EXTRACT_EXPAND "Extract and Expand"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_OPERATION_EXPAND "Expand"
                                                                                                 */
        uint8_t bDigestAlgo,                                                                    /**< [In] Indicates the Algorithm to be used. One of the following,
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_256 "SHA-256"
                                                                                                 *          - \ref PHAL_NTAGXDNA_CRYPTOAPI_ALGORITHM_SHA_384 "SHA-384"
                                                                                                 */
        uint8_t bKeyID,                                                                         /**< [In] Id of the HMAC key.
                                                                                                 *          - 0x10 - 0x17: Id of AES Key (must be in crypto API range: '10' - '17'),
                                                                                                 *                         the key length from the static key.
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number containing the AES key, the
                                                                                                 *                         key length shall be in the following field
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number containing the AES key, the key
                                                                                                 *                         length shall be in the following field
                                                                                                 */
        uint8_t bKeyLen,                                                                        /**< [In] Length of HMAC key, only present when the key source is an
                                                                                                 *        internal buffer.
                                                                                                 */
        uint8_t bSaltSource,                                                                    /**< [In] Crypto API Salt Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pSaltData,                                                                    /**< [In] Salt data. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual data to be Signed or Verified if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bSaltDataLen,                                                                   /**< [In] Length of Salt data as mentioned below
                                                                                                 *          - Actual data length if Salt data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInfoData buffer.
                                                                                                 */
        uint8_t bInfoSource,                                                                    /**< [In] Crypto API Info Source Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t * pInfoData,                                                                    /**< [In] Context data. One of the following
                                                                                                 *          - NULL if Input data source is not Command Buffer
                                                                                                 *          - Actual Context data if Input data source
                                                                                                 *            is Command Buffer
                                                                                                 */
        uint8_t bInfoDataLen,                                                                   /**< [In] Length Input Data as mentioned below
                                                                                                 *          - Actual data length if Input data source is not Command Buffer
                                                                                                 *          - Length of bytes available in \b pInfoData buffer.
                                                                                                 */
        uint8_t bResultDst,                                                                     /**< [In] Crypto API Data Destination Selection.
                                                                                                 *          - 0x00       : Command Buffer
                                                                                                 *          - 0x80 - 0x87: Transient buffer slot number (0 to 7)
                                                                                                 *          - 0xC0 - 0xCF: Static buffer slot number (0 to 15)
                                                                                                 */
        uint8_t bResultLen,                                                                     /**< [In] Number of bytes to output */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing HKDF result. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * \brief Perform CryptoRequest ECHO.
 * It is possible to have the device echo the command data provided to it.
 * This may be useful to verify system setup.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CryptoRequestECHO(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Data to be Echoed back. */
        uint8_t bDataLen,                                                                       /**< [In] Length of Bytes available in \b pData buffer. */
        uint8_t ** ppResponse,                                                                  /**< [Out] Buffer containing the transmitted data. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of Bytes available in \b ppResponse buffer. */
    );

/**
 * end of group phalNtagXDna_CryptoAPI
 * @}
 */

/* NTAG X DNA GPIO Management commands ------------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_GPIOManagement Commands_GPIOManagement
 * \brief Describes about the NTAG X DNA GPIO management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines Defines
 * \brief Macro Definitions for GPIO Management commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_GPIONumber GPIONumber
 * \brief The Options for representing GPIO number. To be used with \ref phalNtagXDna_ManageGPIO "ManageGPIO" interface.
 * @{
 */
#define PHAL_NTAGXDNA_GPIO_NUMBER_1                                                     0x00U   /**< Option to indicate GPIO number as 1. */
#define PHAL_NTAGXDNA_GPIO_NUMBER_2                                                     0x01U   /**< Option to indicate GPIO number as 2. */
/**
 * end of group phalNtagXDna_GPIOManagement_Defines_GPIONumber
 * @}
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_Operation Operation
 * \brief The Options for representing operation to perform for the respective GPIO.
 * To be used with \ref phalNtagXDna_ManageGPIO "ManageGPIO" interface.
 * @{
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_Operation_Output Mode_Output
 * \brief The Options for representing operation to perform for the GPIO configured as output.
 * @{
 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_GPIO_CONTROL_CLEAR                                 0x00U   /**< Option to indicate GPIO Control as CLEAR.
                                                                                                 *      Clear the GPIO state to LOW(not driven) or stop power
                                                                                                 *      harvesting depending on the mode.
                                                                                                 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_GPIO_CONTROL_SET                                   0x01U   /**< Option to indicate GPIO Control as SET.
                                                                                                 *      Set the GPIO State to HIGH (driven) or start power harvesting
                                                                                                 *      depending on the mode.
                                                                                                 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_GPIO_CONTROL_TOGGLE                                0x02U   /**< Option to indicate GPIO Control as TOGGLE.
                                                                                                 *      Toggle the GPIO State or power harvesting state depending
                                                                                                 *      on the mode.
                                                                                                 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_NO_NFC_ACTION                                      0x00U   /**< Option to indicate No NFC action. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_PAUSE_NFC                                          0x80U   /**< Option to indicate Pause NFC (only accepted over NFC). */
#define PHAL_NTAGXDNA_GPIO_OPERATION_RELEASE_NFC_PAUSE                                  0x80U   /**< Option to indicate Release NFC Pause (only accepted over I2C) */
/**
 * end of group phalNtagXDna_GPIOManagement_Defines_Operation_Output
 * @}
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut Mode_DownStreamPowerOut
 * \brief The Options for representing operation to perform for the GPIO configured as
 * Down-Stream Power Out.
 * @{
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut_TargetVI Target Voltage and Current
 * \brief The Options for representing target Voltage and Current operation to perform for the GPIO
 * configured as Down-Stream Power Out. Applicable to Bits 7 - 2.
 * @{
 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_V_I_DEFAULT                                        0x00U   /**< Option to indicate Default level as configured with Cmd.SetConfiguration. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_100UA                                         0x04U   /**< Option to indicate Power downstream voltage of 1.8V and current of 100uA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_300UA                                         0x08U   /**< Option to indicate Power downstream voltage of 1.8V and current of 300uA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_500UA                                         0x0CU   /**< Option to indicate Power downstream voltage of 1.8V and current of 500uA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_1MA                                           0x10U   /**< Option to indicate Power downstream voltage of 1.8V and current of 1mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_2MA                                           0x14U   /**< Option to indicate Power downstream voltage of 1.8V and current of 2mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_3MA                                           0x18U   /**< Option to indicate Power downstream voltage of 1.8V and current of 3mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_5MA                                           0x1CU   /**< Option to indicate Power downstream voltage of 1.8V and current of 5mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_7MA                                           0x20U   /**< Option to indicate Power downstream voltage of 1.8V and current of 7mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_10MA                                          0x24U   /**< Option to indicate Power downstream voltage of 1.8V and current of 10mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_MAX_CURRENT                                   0x3CU   /**< Option to indicate Power downstream voltage of 1.8V and maximum available current. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_100UA                                           0x44U   /**< Option to indicate Power downstream voltage of 2V and current of 100uA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_300UA                                           0x48U   /**< Option to indicate Power downstream voltage of 2V and current of 300uA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_500UA                                           0x4CU   /**< Option to indicate Power downstream voltage of 2V and current of 500uA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_1MA                                             0x50U   /**< Option to indicate Power downstream voltage of 2V and current of 1mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_2MA                                             0x54U   /**< Option to indicate Power downstream voltage of 2V and current of 2mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_3MA                                             0x58U   /**< Option to indicate Power downstream voltage of 2V and current of 3mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_5MA                                             0x5CU   /**< Option to indicate Power downstream voltage of 2V and current of 5mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_7MA                                             0x60U   /**< Option to indicate Power downstream voltage of 2V and current of 7mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_10MA                                            0x64U   /**< Option to indicate Power downstream voltage of 2V and current of 10mA. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_2V_MAX_CURRENT                                     0x7CU   /**< Option to indicate Power downstream voltage of 2V and maximum available current. */
/**
 * end of group phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut_TargetVI
 * @}
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut_Control GPIO Measurement Control
 * \brief The Options for representing GPIO Measurement Control operation to perform for the GPIO
 * configured as Down-Stream Power Out. Applicable to Bit 1.
 * @{
 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_NO_MEASURE                                         0x00U   /**< Option to indicate GPIO Measurement control to perform no Measurement. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_EXECUTE_MEASURE                                    0x02U   /**< Option to indicate GPIO Measurement Control to Execute Measurement. */
/**
 * end of group phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut_Control
 * @}
 */

/**
 * \defgroup phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut_Harvest Harvest
 * \brief The Options for representing GPIO Power Harvesting operation to perform for the GPIO
 * configured as Down-Stream Power Out. Applicable to Bit 0.
 * @{
 */
#define PHAL_NTAGXDNA_GPIO_OPERATION_STOP_POWER_HARVEST                                 0x00U   /**< Option to indicate GPIO Control to Stop Power Harvest. */
#define PHAL_NTAGXDNA_GPIO_OPERATION_ENABLE_POWER_HARVEST                               0x01U   /**< Option to indicate GPIO Control to Enable Power Harvesting. */
/**
 * end of group phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut_Control
 * @}
 */

/**
 * end of group phalNtagXDna_GPIOManagement_Defines_Operation_PowerOut
 * @}
 */

/**
 * end of group phalNtagXDna_GPIOManagement_Defines_Operation
 * @}
 */

/**
 * end of group phalNtagXDna_GPIOManagement_Defines
 * @}
 */

/**
 * \brief Perform GPIO Management
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ManageGPIO(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                                       /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *        data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t bGPIONo,                                                                        /**< [In] GPIO Number to use for management. One of the below values.
                                                                                                 *          - \ref PHAL_NTAGXDNA_GPIO_NUMBER_1 "GPIO 1"
                                                                                                 *          - \ref PHAL_NTAGXDNA_GPIO_NUMBER_2 "GPIO 2"
                                                                                                 */
        uint8_t bOperation,                                                                     /**< [In] Targeted Operation perform on the selected GPIO. One of the below values.
                                                                                                 *          - GPIOxMode as Output
                                                                                                 *              - \ref PHAL_NTAGXDNA_GPIO_OPERATION_GPIO_CONTROL_CLEAR "Clear GPIO Control"
                                                                                                 *              - \ref PHAL_NTAGXDNA_GPIO_OPERATION_GPIO_CONTROL_SET "Set GPIO Control"
                                                                                                 *              - \ref PHAL_NTAGXDNA_GPIO_OPERATION_GPIO_CONTROL_TOGGLE "Toggle GPIO Control"
                                                                                                 *              - \ref PHAL_NTAGXDNA_GPIO_OPERATION_NO_NFC_ACTION "No NFC Action"
                                                                                                 *              - \ref PHAL_NTAGXDNA_GPIO_OPERATION_PAUSE_NFC "Pause NFC"
                                                                                                 *              - \ref PHAL_NTAGXDNA_GPIO_OPERATION_RELEASE_NFC_PAUSE "Release NFC Pause"
                                                                                                 *
                                                                                                 *          - GPIOxMode as Down-Stream Power Out
                                                                                                 *              - GPIO Control
                                                                                                 *                  - \ref PHAL_NTAGXDNA_GPIO_OPERATION_STOP_POWER_HARVEST "CLEAR: Stop Power Harvesting"
                                                                                                 *                  - \ref PHAL_NTAGXDNA_GPIO_OPERATION_ENABLE_POWER_HARVEST "SET: Enable Power Harvesting"
                                                                                                 *
                                                                                                 *              - GPIO Measurement Control
                                                                                                 *                  - \ref PHAL_NTAGXDNA_GPIO_OPERATION_NO_MEASURE "No Measurement"
                                                                                                 *                  - \ref PHAL_NTAGXDNA_GPIO_OPERATION_EXECUTE_MEASURE "MEASURE: Execute Measurement"
                                                                                                 *
                                                                                                 *              - Target Voltage / Current Level
                                                                                                 *                  - \ref PHAL_NTAGXDNA_GPIO_OPERATION_V_I_DEFAULT
                                                                                                 *                    "Default level as configured with Cmd.SetConfiguration"
                                                                                                 *                  - Power downstream voltage of 1.8V
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_100UA "Current of 100uA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_300UA"Current of 300uA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_500UA "Current of 500uA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_1MA "Current of 1mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_2MA "Current of 2mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_3MA "Current of 3mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_5MA "Current of 5mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_7MA "Current of 7mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_10MA "Current of 10mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_1_8V_MAX_CURRENT "Maximum Available Current"
                                                                                                 *                  - Power downstream voltage of 2V
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_100UA "Current of 100uA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_300UA"Current of 300uA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_500UA "Current of 500uA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_1MA "Current of 1mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_2MA "Current of 2mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_3MA "Current of 3mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_5MA "Current of 5mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_7MA "Current of 7mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_10MA "Current of 10mA"
                                                                                                 *                      - \ref PHAL_NTAGXDNA_GPIO_OPERATION_2V_MAX_CURRENT "Maximum Available Current"
                                                                                                 */
        uint8_t * pNFCPauseRspData,                                                             /**< [In] NFC Pause Response Data: Data to be returned to NFC host
                                                                                                 *        in the case of 'Release NFC Pause'
                                                                                                 */
        uint16_t wNFCPauseRspDataLen,                                                           /**< [In] Length of bytes available in \b pNFCPauseRspData buffer. */
        uint8_t ** ppResponse,                                                                  /**< [Out] Response from tag as follows.
                                                                                                 *          - If \b bOperation = \ref PHAL_NTAGXDNA_GPIO_OPERATION_PAUSE_NFC "Pause NFC"
                                                                                                 *             then, NFC Pause Response Data: Data received from the I2C interface
                                                                                                 *          - If \b bOperation = \ref PHAL_NTAGXDNA_GPIO_OPERATION_EXECUTE_MEASURE
                                                                                                 *            "Execute Measure" then, Measurement result will be received.
                                                                                                 *          - NULL for others.
                                                                                                 */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b pRspLen buffer. */
    );

/**
 * \brief Perform GPIO read.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - The values provided in \b bComOption is not supported.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ReadGPIO(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                                       /**< [In] Indicates the mode of communication to be used while exchanging the
                                                                                                 *         data from tag.
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_PLAIN "Plain Mode"
                                                                                                 */
#ifdef NXPBUILD__PH_CRYPTOSYM
                                                                                                /**<
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_MAC "MAC Mode"
                                                                                                 *          - \ref PHAL_NTAGXDNA_COMMUNICATION_FULL "Full Mode"
                                                                                                 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        uint8_t ** ppResponse,                                                                  /**< [Out] Cryptographic output for the provided input. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b pRspLen buffer. */
    );

/**
 * end of group phalNtagXDna_GPIOManagement
 * @}
 */

/* NTAG X DNA ISO7816-4 commands ------------------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_ISO7816 Commands_ISO7816
 * \brief Describes about the NTAG X DNA ISO/IEC 7816-4 Standard commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_ISO7816_Defines Defines
 * \brief Macro Definitions for ISO/IEC 7816-4 Standard commands.
 * @{
 */

/**
 * \defgroup phalNtagXDna_ISO7816_Defines_FCI FileControlIdentifier
 * \brief Macro Definitions for ISO/IEC 7816-4 FCI modes. To be used with \ref phalNtagXDna_IsoSelectFile "ISOSelect File" interface.
 * @{
 */
#define PHAL_NTAGXDNA_FCI_RETURNED                                                        0x00U   /**< Option to indicate return of FCI. */
#define PHAL_NTAGXDNA_FCI_NOT_RETURNED                                                    0x0CU   /**< Option to indicate no return of FCI. */
/**
 * end of group phalNtagXDna_ISO7816_Defines_FCI
 * @}
 */

/**
 * \defgroup phalNtagXDna_ISO7816_Defines_Selector Selection Control
 * \brief Macro Definitions for ISO/IEC 7816-4 Selection Controls. To be used with \ref phalNtagXDna_IsoSelectFile "ISOSelect File" interface.
 * @{
 */
#define PHAL_NTAGXDNA_SELECTOR_0                                                          0x00U   /**< Option to indicate Selection by 2 byte file Id. */
#define PHAL_NTAGXDNA_SELECTOR_1                                                          0x01U   /**< Option to indicate Selection by child DF. */
#define PHAL_NTAGXDNA_SELECTOR_2                                                          0x02U   /**< Option to indicate Select EF under current DF. FID = EF id. */
#define PHAL_NTAGXDNA_SELECTOR_3                                                          0x03U   /**< Option to indicate Select parent DF of the current DF. */
#define PHAL_NTAGXDNA_SELECTOR_4                                                          0x04U   /**< Option to indicate Selection by DF Name. DFName and length is then valid. */
/**
 * end of group phalNtagXDna_ISO7816_Defines_Selector
 * @}
 */

/**
 * \defgroup phalNtagXDna_ISO7816_Defines_APDU APDUFormat
 * \brief Macro Definitions for ISO/IEC 7816-4 APDU format. To be used with all ISO7816 - 4 interfaces.
 * @{
 */
#define PHAL_NTAGXDNA_APDU_FORMAT_SHORT_LEN                                               0x00U   /**< Option to indicate ISO7816-4 APDU format is Short Length format where LC
                                                                                                 *   and LE are of 1 byte.
                                                                                                 */
#define PHAL_NTAGXDNA_APDU_FORMAT_EXTENDED_LEN                                            0x01U   /**< Option to indicate ISO7816-4 APDU format is Extended Length format where LC
                                                                                                 *   is 3 bytes and LE is either 2 or 3 bytes.
                                                                                                 */
/**
 * end of group phalNtagXDna_ISO7816_Defines_APDU
 * @}
 */

/**
 * \defgroup phalNtagXDna_ISO7816_Defines_SFID SFID
 * \brief Macro Definitions for ISO/IEC 7816-4 P1 Encoding of ShortFile identifier or Offset.
 * To be used with \ref phalNtagXDna_IsoReadBinary "ISORead Binary" and \ref phalNtagXDna_IsoUpdateBinary "ISOUpdate Binary" interface.
 * @{
 */
#define PHAL_NTAGXDNA_SFID_DISABLED                                                       0x00U   /**< Option to indicate Encoding as offset. */
#define PHAL_NTAGXDNA_SFID_ENABLED                                                        0x80U   /**< Option to indicate Encoding as Short File Identifier. */
/**
 * end of group phalNtagXDna_ISO7816_Defines_SFID
 * @}
 */

/**
 * end of group phalNtagXDna_ISO7816_Defines
 * @}
 */

/**
 * \brief Perform File or Application selection. This command is implemented in compliance with ISO/IEC 7816-4.
 *
 * \note
 *      For all ISO7816 errors, library returns a command error code \ref PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR "ISO7816 General Errors".
 *      To know the exact error returned by tag call \ref phalNtagXDna_GetConfig "Get Config" with \ref PHAL_NTAGXDNA_ADDITIONAL_INFO
 *      "Additional Information" as Configuration Identifier.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - DFName Length is greater than 16 (\b bDFnameLen).
 *                                      - Invalid FCI (File Control Identifier) (\b bOption)
 *                                      - Invalid Selector option (\b bSelector).
 * \retval #PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR Any ISO7816 Error.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_IsoSelectFile(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOption,                                                                        /**< [In] Option for return / no return of FCI.
                                                                                                 *          - \ref PHAL_NTAGXDNA_FCI_RETURNED "FCI Returned"
                                                                                                 *          - \ref PHAL_NTAGXDNA_FCI_NOT_RETURNED "FCI Not Returned"
                                                                                                 */
        uint8_t bSelector,                                                                      /**< [In] The selector to be used.
                                                                                                 *          - \ref PHAL_NTAGXDNA_SELECTOR_0 "File Identifier"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SELECTOR_1 "Child DF"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SELECTOR_2 "EF under current DF"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SELECTOR_3 "DF of the Current DF"
                                                                                                 *          - \ref PHAL_NTAGXDNA_SELECTOR_4 "DF Name"
                                                                                                 */
        uint8_t * pFid,                                                                         /**< [In] The ISO File number to be selected.
                                                                                                 *          - Valid only if \b bSelector is one of the following.
                                                                                                 *              - \ref PHAL_NTAGXDNA_SELECTOR_0 "File Identifier"
                                                                                                 *              - \ref PHAL_NTAGXDNA_SELECTOR_1 "Child DF"
                                                                                                 *              - \ref PHAL_NTAGXDNA_SELECTOR_2 "EF under current DF"
                                                                                                 *          - NULL for other \b bSelector options.
                                                                                                 */
        uint8_t * pDFname,                                                                      /**< [In] The ISO DFName to be selected.
                                                                                                 *          - Valid only when \b bSelector = \ref PHAL_NTAGXDNA_SELECTOR_4
                                                                                                 *            "DF Name".
                                                                                                 *          - NULL for other \b bSelector options.
                                                                                                 */
        uint8_t bDFnameLen,                                                                     /**< [In] Length of bytes available in \b pDFname buffer. */
        uint8_t bExtendedLenApdu,                                                               /**< [In] Flag for Extended Length APDU.
                                                                                                 *          - \ref PHAL_NTAGXDNA_APDU_FORMAT_SHORT_LEN "Short Length APDU"
                                                                                                 *          - \ref PHAL_NTAGXDNA_APDU_FORMAT_EXTENDED_LEN "Extended Length APDU"
                                                                                                 */
        uint8_t ** ppFCI,                                                                       /**< [Out] The FCI information returned by the tag. */
        uint16_t * pFCILen                                                                      /**< [Out] Length of bytes available in \b ppFCI buffer. */
    );

/**
 * \brief Perform ISO Read Binary. This command is implemented in compliance with ISO/IEC 7816-4.
 *
 * \note
 *      For all ISO7816 errors, library returns a command error code \ref PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR "ISO7816 General Errors".
 *      To know the exact error returned by tag call \ref phalNtagXDna_GetConfig "Get Config" with \ref PHAL_NTAGXDNA_ADDITIONAL_INFO
 *      "Additional Information" as Configuration Identifier.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffer is null.
 *                                      - For invalid Short File identifier (\b bSfid).
 *                                      - For Invalid Buffering Options (\b wOption).
 * \retval #PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR Any ISO7816 Error.
 * \retval XXXX
 *                                      - Depending on status codes return by tag.
 *                                      - Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_IsoReadBinary(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wOption,                                                                       /**< [In] One of the below options.
                                                                                                 *          - #PH_EXCHANGE_DEFAULT   : To exchange command to the tag and
                                                                                                 *                                     receive the response.
                                                                                                 *          - #PH_EXCHANGE_RXCHAINING: To Receive pending response from tag.
                                                                                                 */
        uint8_t bOffset,                                                                        /**< [In] The offset from where the data should be read. \n
                                                                                                 *          Regardless of \b bSfid value, the encoding of offset will be
                                                                                                 *          from 0 - 255. This will be part of P2 information.
                                                                                                 */
        uint8_t bSfid,                                                                          /**< [In] Indication to use either Short ISO File Id or Offset.
                                                                                                 *          - If \ref PHAL_NTAGXDNA_SFID_ENABLED "Short File Identifier", then bit
                                                                                                 *            7 is set and bits 0-4indicates short file identifier.
                                                                                                 *          - If \ref PHAL_NTAGXDNA_SFID_DISABLED "Short File Identifier", then bits
                                                                                                 *            0-6 indicates MSB of offset information.
                                                                                                 *          - This will be part of P1 information.
                                                                                                 *          - Ex. If actual Offset = 8063 (1F7F), then \b bSfid will be 1F and
                                                                                                 *            \b bOffset will be 7F.
                                                                                                 */
        uint32_t dwBytesToRead,                                                                 /**< [In] The number of bytes to be read from the file.
                                                                                                 *          - If zero is provided, then entire file data is returned by tag.
                                                                                                 *          - If non-zero is provided, then data starting from offset is returned.
                                                                                                 */
        uint8_t bExtendedLenApdu,                                                               /**< [In] Flag for Extended Length APDU.
                                                                                                 *          - \ref PHAL_NTAGXDNA_APDU_FORMAT_SHORT_LEN "Short Length APDU"
                                                                                                 *          - \ref PHAL_NTAGXDNA_APDU_FORMAT_EXTENDED_LEN "Extended Length APDU"
                                                                                                 */
        uint8_t ** ppResponse,                                                                  /**< [Out] The data returned by the tag. */
        uint16_t * pRspLen                                                                      /**< [Out] Length of bytes available in \b ppResponse buffer. */
    );

/**
* \brief Perform ISO Update Binary. This command is implemented in compliance with ISO/IEC 7816-4.
*
* \note
*      For all ISO7816 errors, library returns a command error code \ref PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR "ISO7816 General Errors".
*      To know the exact error returned by tag call \ref phalNtagXDna_GetConfig "Get Config" with \ref PHAL_NTAGXDNA_ADDITIONAL_INFO
*      "Additional Information" as Configuration Identifier.
*
* \return Status code
* \retval #PH_ERR_SUCCESS              Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS  If DataParams is null.
* \retval #PH_ERR_INVALID_PARAMETER
*                                      - If the buffer is null.
*                                      - For invalid Short File identifier (\b bSfid).
* \retval #PHAL_NTAGXDNA_ERR_DF_7816_GEN_ERROR Any ISO7816 Error.
* \retval XXXX
*                                      - Depending on status codes return by tag.
*                                      - Other Depending on implementation and underlying component.
*/
phStatus_t phalNtagXDna_IsoUpdateBinary(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bOffset,                                                                        /**< [In] The offset from where the data should be updated. \n
                                                                                                 *          Regardless of \b bSfid value, the encoding of offset will be
                                                                                                 *          from 0 - 255. This will be part of P2 information.
                                                                                                 */
        uint8_t bSfid,                                                                          /**< [In] Indication to use either Short ISO File Id or Offset.
                                                                                                 *          - If \ref PHAL_NTAGXDNA_SFID_ENABLED "Short File Identifier", then bit
                                                                                                 *            7 is set and bits 0-4indicates short file identifier.
                                                                                                 *          - If \ref PHAL_NTAGXDNA_SFID_DISABLED "Short File Identifier", then bits
                                                                                                 *            0-6 indicates MSB of offset information.
                                                                                                 *          - This will be part of P1 information.
                                                                                                 *          - Ex. If actual Offset = 8063 (1F7F), then \b bSfid will be 1F and
                                                                                                 *            \b bOffset will be 7F.
                                                                                                 */
        uint8_t bExtendedLenApdu,                                                               /**< [In] Flag for Extended Length APDU.
                                                                                                 *          - \ref PHAL_NTAGXDNA_APDU_FORMAT_SHORT_LEN "Short Length APDU"
                                                                                                 *          - \ref PHAL_NTAGXDNA_APDU_FORMAT_EXTENDED_LEN "Extended Length APDU"
                                                                                                 */
        uint8_t * pData,                                                                        /**< [In] Data to be updated. */
        uint16_t wDataLen                                                                       /**< [In] Length of bytes available in \b pData buffer. */
    );

/**
 * end of group phalNtagXDna_ISO7816
 * @}
 */

/* NTAG X DNA Utility functions -------------------------------------------------------------------------------------------------------- */
/**
 * \defgroup phalNtagXDna_Utility Commands_Utility
 * \brief Describes about the NTAG X DNA Utility functions. These are not part of actual NTAG X DNA data sheet
 * rather its for internal purpose.
 * @{
 */

/**
 * \defgroup phalNtagXDna_Utility_Defines Defines
 * \brief Macro Definitions for Utility interface support.
 * @{
 */

/**
 * \defgroup phalNtagXDna_Utility_Defines_Configuration Configuration
 * \brief Macro Definitions for \ref phalNtagXDna_GetConfig "Get Config" and \ref phalNtagXDna_SetConfig "Set Config" interfaces.
 * @{
 */
#define PHAL_NTAGXDNA_ADDITIONAL_INFO                                                   0x00A1U /**< Option for \ref phalNtagXDna_GetConfig "Get Config" / \ref phalNtagXDna_SetConfig
                                                                                                 *   "Set Config" to get/set additional info of a generic error or some length
                                                                                                 *   exposed by interfaces.
                                                                                                 */
#define PHAL_NTAGXDNA_SHORT_LENGTH_APDU                                                 0x00A2U /**< Option for \ref phalNtagXDna_GetConfig "Get Config" / \ref phalNtagXDna_SetConfig
                                                                                                 *   "Set Config" to get/set current Status of Short Length APDU wrapping in ISO
                                                                                                 *   7816-4 APDUs.
                                                                                                 *      - #PH_ON : The commands will follow ISO7816 wrapped format with LC and LE as 1 byte.
                                                                                                 *      - #PH_OFF: The commands will follow ISO7816 wrapped format with LC as 3 bytes and LE
                                                                                                 *                 as 2 or 3 bytes.
                                                                                                 */
/**
 * end of group phalNtagXDna_Utility_Defines_Configuration
 * @}
 */

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \defgroup phalNtagXDna_Utility_Defines_SDMOptions SDMOptions
 * \brief Macro Definitions for below mentioned utility interfaces.
 *  - \ref phalNtagXDna_CalculateMACSDM "CalculateMACSDM"
 *  - \ref phalNtagXDna_DecryptSDMENCFileData "DecryptSDMENCFileData"
 * @{
 */
#define PHAL_NTAGXDNA_VCUID_RDCTR_PRESENT                                               0xC0U   /**< Both VCUID and SDMReadCounter is considered for SDM Session key calculation. */
#define PHAL_NTAGXDNA_VCUID_PRESENT                                                     0x80U   /**< Only VCUID is considered for SDM Session key calculation. */
#define PHAL_NTAGXDNA_RDCTR_PRESENT                                                     0x40U   /**< Only RDCTR  is considered for SDM Session key calculation. */
/**
 * end of group phalNtagXDna_Utility_Defines_SDMOptions
 * @}
 */
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * end of group phalNtagXDna_Utility_Defines
 * @}
 */

/**
 * \brief Perform a GetConfig command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS                  Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS      If any of the DataParams are null.
 * \retval #PH_ERR_INVALID_PARAMETER        If the return buffer is null
 * \retval #PH_ERR_UNSUPPORTED_PARAMETER    If configuration (\b wConfig) option provided is not supported.
 */
phStatus_t phalNtagXDna_GetConfig(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,                                                                       /**< [In] Configuration to read. Will be one of the below values.
                                                                                                 *        for list of supported configurations refer
                                                                                                 *        \ref phalNtagXDna_Utility_Defines_Configuration "Configuration Identifier"
                                                                                                 */
         uint16_t * pValue                                                                      /**< [Out] The value for the mentioned configuration. */
    );

/**
 * \brief Perform a SetConfig command.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS                  Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS      If any of the DataParams are null.
 * \retval #PH_ERR_UNSUPPORTED_PARAMETER    If configuration (\b wConfig) option provided is not supported.
 */
phStatus_t phalNtagXDna_SetConfig(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,                                                                       /**< [In] Configuration to set. Will be one of the below values.
                                                                                                 *        for list of supported configurations refer
                                                                                                 *        \ref phalNtagXDna_Utility_Defines_Configuration "Configuration Identifier"
                                                                                                 */
        uint16_t wValue                                                                         /**< [In] The value for the mentioned configuration. */
    );

/**
 * \brief Reset the authentication
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_ResetAuthentication(
        void * pDataParams                                                                      /**< [In] Pointer to this layer's parameter structure. */
    );

#ifdef NXPBUILD__PH_CRYPTOSYM
/**
 * \brief Performs computation of SDM MAC information. Supports MAC Calculation using AES128 and AES 256 key type.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If any of the DataParams are null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - If SDMOptions (\b bSdmOption) is not supported.
 * \retval #PH_ERR_KEY                  If key number passed is not of a support key type.
 * \retval #PH_ERR_PARAMETER_OVERFLOW   If SDM Read Counter value reached maximum.
 * \retval XXXX                         Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_CalculateMACSDM(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bSdmOption,                                                                     /**< [In] SDM Option to indicate which parameters to be considered.
                                                                                                 *          - \ref PHAL_NTAGXDNA_VCUID_RDCTR_PRESENT "VCUID and Read Counter Present"
                                                                                                 *          - \ref PHAL_NTAGXDNA_VCUID_PRESENT "VCUID Present"
                                                                                                 *          - \ref PHAL_NTAGXDNA_RDCTR_PRESENT "Read Counter Present"
                                                                                                 */
        uint16_t wKeyNo_SDMMac,                                                                 /**< [In] Key number to be used from KeyStore. */
        uint16_t wKeyVer_SDMMac,                                                                /**< [In] Key version to be used from KeyStore. */
        uint8_t * pUid,                                                                         /**< [In] UID of the card. */
        uint8_t bUidLen,                                                                        /**< [In] Length of bytes available in \b pUid buffer. */
        uint8_t * pSDMReadCtr,                                                                  /**< [In] SDM Read Counter Input. Should be 4 bytes in Length. */
        uint8_t * pInData,                                                                      /**< [In] Data read out from PICC that is between SDMMacInputoffset until SDMMACOffset. */
        uint16_t wInDataLen,                                                                    /**< [In] Length of bytes available in \b pInData buffer. */
        uint8_t ** ppSDMMAC,                                                                    /**< [Out] Secure Dynamic MAC information. */
        uint16_t * pSDMMACLen                                                                   /**< [Out] Length of bytes available in \b ppRespMac buffer. */
    );

/**
 * \brief Performs verification of SDM Signature information.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If any of the DataParams are null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffers are null.
 * \retval #PH_ERR_VERIFICATION_FAILED  Verification of Message / Signature combination failed.
 * \retval XXXX                         Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_VerifySDMSignature(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wPubKeyNo_SDMSig,                                                              /**< [In] Key number to be used from KeyStore. */
        uint16_t wPubKeyPos_SDMSig,                                                             /**< [In] Key position to be used from KeyStore. */
        uint8_t * pInData,                                                                      /**< [In] Data read out from PICC that is between SDMMacInputoffset until SDMMACOffset. */
        uint16_t wInDataLen,                                                                    /**< [In] Length of bytes available in \b pInData buffer. */
        uint8_t * pSignature,                                                                   /**< [In] Secure Dynamic Signature information. */
        uint16_t wSigLen                                                                        /**< [In] Length of bytes available in \b pSignature buffer. */
    );

/**
 * \brief Performs decryption of SDM File Data. Supports decryption using AES128 and AES 256 key type.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If any of the DataParams are null.
 * \retval #PH_ERR_INVALID_PARAMETER
 *                                      - If the buffers are null.
 *                                      - If SDMOptions (\b bSdmOption) is not supported.
 * \retval #PH_ERR_KEY                  If key number passed is not of a support key type.
 * \retval #PH_ERR_PARAMETER_OVERFLOW   If SDM Read Counter value reached maximum.
 * \retval XXXX                         Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_DecryptSDMENCFileData(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bSdmOption,                                                                     /**< [In] SDM Option to indicate which parameters to be considered.
                                                                                                 *          - \ref PHAL_NTAGXDNA_VCUID_RDCTR_PRESENT "VCUID and Read Counter Present"
                                                                                                 *          - \ref PHAL_NTAGXDNA_VCUID_PRESENT "VCUID Present"
                                                                                                 *          - \ref PHAL_NTAGXDNA_RDCTR_PRESENT "Read Counter Present"
                                                                                                 */
        uint16_t wKeyNo_SDMEnc,                                                                 /**< [In] Key number to be used from KeyStore. */
        uint16_t wKeyVer_SDMEnc,                                                                /**< [In] Key version to be used from KeyStore. */
        uint8_t * pUid,                                                                         /**< [In] UID of the card. */
        uint8_t bUidLen,                                                                        /**< [In] Length of bytes available in \b pUid buffer. */
        uint8_t * pSDMReadCtr,                                                                  /**< [In] SDM Read Counter Input. */
        uint8_t * pEncData,                                                                     /**< [In] Encrypted NonASCII SDM Encrypted File data. */
        uint16_t wEncDataLen,                                                                   /**< [In] Length of bytes available in \b pEncData buffer. */
        uint8_t ** ppPlainData,                                                                 /**< [Out] The decrypted SDMFile data. */
        uint16_t * pPlainDataLen                                                                /**< [Out] Length of bytes available in \b ppPlainData buffer. */
    );

/**
 * \brief Performs decryption of SDM PICC Data. Supports decryption using AES128 and AES 256 key type.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS              Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS  If any of the DataParams are null.
 * \retval #PH_ERR_INVALID_PARAMETER    If the buffers are null.
 * \retval #PH_ERR_KEY                  If key number passed is not of a support key type.
 * \retval #PH_ERR_PARAMETER_OVERFLOW   If SDM Read Counter value reached maximum.
 * \retval XXXX                         Other Depending on implementation and underlying component.
 */
phStatus_t phalNtagXDna_DecryptSDMPICCData(
        void * pDataParams,                                                                     /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wKeyNo,                                                                        /**< [In] Key number to be used from KeyStore. */
        uint16_t wKeyVer,                                                                       /**< [In] Key version to be used from KeyStore. */
        uint8_t * pEncData,                                                                     /**< [In] Encrypted NonASCII SDM PICC data. */
        uint16_t wEncDataLen,                                                                   /**< [In] Length of bytes available in \b pEncData buffer. */
        uint8_t ** ppPlainData,                                                                 /**< [Out] The decrypted SDMPICC data. */
        uint16_t * pPlainDataLen                                                                /**< [Out] Length of bytes available in \b ppPlainData buffer. */
    );
#endif /* NXPBUILD__PH_CRYPTOSYM */

/**
 * end of group phalNtagXDna_Utility
 * @}
 */

/**
 * end of group phalNtagXDna
 * @}
 */

#endif /* NXPBUILD__PHAL_NTAGXDNA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALNTAGXDNA_H */
