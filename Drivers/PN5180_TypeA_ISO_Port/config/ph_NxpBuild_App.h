#ifndef PH_NXPBUILD_APP_H_INC
#define PH_NXPBUILD_APP_H_INC

/* PN5180, ISO profile, NFC-A polling and ISO-DEP. */
#define NXPRDLIB_REM_GEN_INTFS
#define NXPBUILD__PHHAL_HW_PN5180
#define NXPBUILD__PHNFCLIB
#define NXPBUILD__PHNFCLIB_PROFILES
#define NXPBUILD__PH_NFCLIB_ISO
#define NXPBUILD__PHAC_DISCLOOP_SW
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
#define NXPBUILD__PHPAL_I14443P3A_SW
#define NXPBUILD__PHPAL_I14443P4A_SW
#define NXPBUILD__PHPAL_I14443P4_SW

/* MIFARE Classic application stack and development key storage. */
#define NXPBUILD__PH_KEYSTORE_SW
#define NXPBUILD__PHPAL_MIFARE_SW
#define NXPBUILD__PHAL_MFC_SW

/* Software crypto components reserved for later secure-card integration. */
#define NXPBUILD__PH_CRYPTOSYM_SW
#define NXPBUILD__PH_CRYPTORNG_SW

/* No DESFire EVx application layer, NFC-B/F/V or target mode. */
/* Select PH_OSAL_NULLOS or PH_OSAL_FREERTOS in compiler definitions. */
#endif
