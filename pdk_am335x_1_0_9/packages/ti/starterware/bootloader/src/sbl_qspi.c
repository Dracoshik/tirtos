/**
 *  \file  sbl_qspi.c
 *
 * \brief  This file copy the application binary from QSPI nor flash to DDR.
 */

/**
 * \copyright  Copyright (C) 2013 Texas Instruments Incorporated -
 *             http://www.ti.com/
 */

/**
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "types.h"
#include "console_utils.h"
#include "sbl_copy.h"
#include "sbl_qspi.h"
#include "error.h"
#include "qspi_flash.h"
#include "qspi_lib.h"
#include "qspi_app.h"
#include "device.h"
#include "soc.h"
#include "board.h"
#include "pinmux.h"
#include "prcm.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief Instance number for QSPI nor flash
 */
#define QSPI_FLASH_DEV_INST_NUM 0U

/**
 * \brief Offset from where the application is read from the QSPI flash
 */
#define OFFSET_ADDR  0x80000U

/** \brief Config base address for QSPI module */
#define QSPI_CONFIG_BASE_ADDR 0x47900000U

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This function initializes the QSPI controller.
 *
 * \param instNum  QSPI Instance number.
 */
static void SblQspiInit(uint32_t instNum);

/**
 * \brief This function copies the application image to the DDR by reading the
 *       header info.
 *
 * \param  pEntryPoint  Entry address read from TI image header.
 *
 * \retval TRUE         Image copy successful.
 * \retval FALSE        Image copy failed.
 */
static uint32_t SblQspiImageCopy(uint32_t *pEntryPoint);

/**
 * \brief This API gets the board info like instance and chip select.
 *
 * \param pQspiObj Pointer to the flash device object
 *
 * \retval S_PASS    BOARD info successfully retrieved. QSPI is connected to
 *                   serial flash on this board.
 *
 * \retval E_FAIL    This board does not support this boot mode
 *                   - Serial Flash device may not be present on this board.
 *                   - Serial flash may not be connected to QSPI controller.
 *
 */
static int32_t SblQspiBoardInfoGet(qspiAppObj_t *pQspiObj);

/**
 * \brief  This API gots the soc info - base address of the QSPI instance
 *
 * \param  pQspiObj     Pointer to the flash device object.
 *
 * \retval S_PASS SOC info successfully returned.
 * \retval E_FAIL This SOC does not support this boot mode.
 */
static int32_t SblQspiSocInfoGet(qspiAppObj_t *pQspiObj);

/* ========================================================================== */
/*                            Global Variables Declarations                   */
/* ========================================================================== */

/** \brief  Global structure for QSPI instance related data */
static qspiAppObj_t gSblQspiObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

uint32_t SBLQspiCopy(uint32_t *pEntryPoint, uint32_t instNum)
{
    uint32_t status;

    /* Get Board Info for QSPI*/
    status = SblQspiBoardInfoGet(&gSblQspiObj);
    if(S_PASS == status)
    {
        /* Get SOC info for QSPI*/
        status = SblQspiSocInfoGet(&gSblQspiObj);
        if (S_PASS == status)
        {
            /* Clock configuration for QSPI controller*/
            status = PRCMModuleEnable(CHIPDB_MOD_ID_QSPI,
                                      gSblQspiObj.instNum,
                                      FALSE);
            if(S_PASS == status)
            {
                /** PinMux for QSPI controller */
                status = PINMUXModuleConfig(CHIPDB_MOD_ID_QSPI,
                                            gSblQspiObj.instNum,
                                            NULL);
                if(S_PASS == status)
                {
                    SblQspiInit(instNum);
                    status = SblQspiImageCopy(pEntryPoint);
                }
                else
                {
                    CONSOLEUtilsPrintf("\nFAILURE!!\nQSPI Pinmux failed\n");
                }
            }
            else
            {
                CONSOLEUtilsPrintf("\nFAILURE!! QSPI clock enable failed\n");
            }
        }
        else
        {
            CONSOLEUtilsPrintf("\nFAILURE!!\n QSPI boot not supported on this SOC ");
        }
    }
    else
    {
        CONSOLEUtilsPrintf("FAILURE!!!\n QSPI boot not supported on this board\n");
    }
    return status;
}


/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

static uint32_t SblQspiImageCopy(uint32_t *pEntryPoint)
{
    ti_header imageHdr;
    uint32_t *dstAddr;
    uint32_t retVal = TRUE;
    uint32_t srcOffsetAddr = OFFSET_ADDR;
    /* Read the ti header*/
    QSPILibRead(&gSblQspiObj.qspiLibInfo,
                srcOffsetAddr,
                (uint32_t) &imageHdr,
                8U);
    srcOffsetAddr += 8U;
    CONSOLEUtilsPrintf("\nCopying Header of the application image ");
    dstAddr = (uint32_t *)imageHdr.load_addr;
    *pEntryPoint = imageHdr.load_addr;

    CONSOLEUtilsPrintf("\nCopying image from flash to DDR\n\n");
    /* Read image file from QSPI nor flash memory to DDR */
    QSPILibRead(&gSblQspiObj.qspiLibInfo,
                srcOffsetAddr,
                (uint32_t) dstAddr,
                imageHdr.image_size);
    return retVal;
}

static void SblQspiInit(uint32_t instNum)
{
    qspiLibCtrlInfo_t qspiLibCtrlInfo;
    /* Set the QSPI config base address */
    qspiLibCtrlInfo.cfgBaseAddr = QSPI_CONFIG_BASE_ADDR;
    /* Set the QSPI memory mapped base Address from chibdb*/
    qspiLibCtrlInfo.memMapBaseAddr = gSblQspiObj.instAddr;
    qspiLibCtrlInfo.chipSelect = gSblQspiObj.chipSelect;
    qspiLibCtrlInfo.qspiLibReadType = QSPI_LIB_READ_TYPE_QUAD;
    qspiLibCtrlInfo.qspiLibTxMode = QSPI_LIB_TX_MODE_MEMORY_MAPPED;
    gSblQspiObj.qspiLibInfo.qspiLibCtrlInfo = qspiLibCtrlInfo;
    /* Initialise the QSPI library */
    QSPILibInit(&gSblQspiObj.qspiLibInfo);
}

static int32_t SblQspiBoardInfoGet(qspiAppObj_t *pQspiObj)
{
    int32_t status = E_FAIL;
    chipdbModuleID_t modId;
    uint32_t qspiInstNum;
    uint32_t chipSelect;
    uint32_t devIndex = 0U;
    uint32_t * qspiFlashDeviceIdList;

    /* Check which QSPI device is present on this board */
    qspiFlashDeviceIdList = QSPIFlashGetDeviceIdList();

    do {
        if (TRUE == BOARDIsDevicePresent(qspiFlashDeviceIdList[devIndex]))
        {
            /* QSPI Device Found */
            pQspiObj->devId = qspiFlashDeviceIdList[devIndex];
            break;
        }
        devIndex++;
    } while (DEVICE_ID_INVALID != qspiFlashDeviceIdList[devIndex]);

    if (DEVICE_ID_INVALID == qspiFlashDeviceIdList[devIndex])
    {
        CONSOLEUtilsPrintf("No QSPI device found!\n");
    }
    else
    {
        /* Set the QSPI data for Serial NOR flash from board data */
        modId = BOARDGetDeviceCtrlModId(pQspiObj->devId,
                                        QSPI_FLASH_DEV_INST_NUM);
        if (CHIPDB_MOD_ID_INVALID == modId)
        {
            CONSOLEUtilsPrintf("Invalid device information from board data\n");
        }
        else if (CHIPDB_MOD_ID_QSPI == modId)
        {
            qspiInstNum = BOARDGetDeviceCtrlInfo(pQspiObj->devId,
                QSPI_FLASH_DEV_INST_NUM);
            chipSelect = BOARDGetDeviceCtrlInfo(pQspiObj->devId,
                QSPI_FLASH_DEV_INST_NUM);
            if((INVALID_INST_NUM == qspiInstNum) || (INVALID_INFO == chipSelect))
            {
                CONSOLEUtilsPrintf("Invalid QSPI board data!\n");
            }
            else
            {
                pQspiObj->instNum = qspiInstNum;
                pQspiObj->chipSelect  = chipSelect;
                status = S_PASS;
            }
        }
        else
        {
            CONSOLEUtilsPrintf("This flash is not connected to QSPI\n");
        }
    }
    return (status);
}

static int32_t SblQspiSocInfoGet(qspiAppObj_t *pQspiObj)
{
    int32_t status = S_PASS;
    if(TRUE == CHIPDBIsResourcePresent(CHIPDB_MOD_ID_QSPI, pQspiObj->instNum))
    {
        pQspiObj->instAddr = CHIPDBBaseAddress(CHIPDB_MOD_ID_QSPI,
            pQspiObj->instNum);
    }
    else
    {
        /*QSPI instance number is not present . */
        status = E_FAIL;
    }
    return status;
}
