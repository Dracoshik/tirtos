/**
 *  \file   FATFS_drv.c
 *
 *  \brief  SD Driver high level APIs implementation.
 *
 *   This file contains the driver APIs for SD controller.
 */

/*
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <ti/fs/fatfs/src/FATFS_drv_log.h>
#include <ti/fs/fatfs/FATFS.h>
#include <ti/fs/fatfs/src/FATFS_osal.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DRIVE_NOT_MOUNTED           (~((uint32_t)0U))

/* Function prototypes */

/* FatFs disk I/O functions */
static DSTATUS  FATFS_diskInitialize(BYTE drv);
static DRESULT  FATFS_diskIOctrl(BYTE drv, BYTE ctrl, void *buf);
static DRESULT  FATFS_diskRead(BYTE drv, BYTE *buf, DWORD sector, BYTE count);
static DSTATUS  FATFS_diskStatus(BYTE drv);
static DRESULT  FATFS_diskWrite(BYTE drv, const BYTE *buf, DWORD sector,
                                BYTE count);

/* Externs */
extern const FATFSConfigList FATFS_config;

/* Used to check status and initialization */
static int32_t FATFS_count = -1;

/* Default SD parameters structure */
const FATFS_ConfigParams FATFS_defaultParams = {
    NULL,
    NULL,
    NULL,                /* Custom argument used by driver implementation */
    NULL
};

/*
 *  ======== FATFS_close ========
 */
FATFS_Error FATFS_close(FATFS_Handle handle)
{
    DRESULT                dresult;
    FRESULT                fresult;
    FATFS_Object          *object = NULL;
#ifdef LOG_EN
    FATFS_HwAttrs const   *hwAttrs = NULL;
#endif
    TCHAR drive_path[5];
    /* Input parameter validation */
    FATFS_osalAssert(handle == NULL);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *) handle)->object;
#ifdef LOG_EN
    hwAttrs = ((FATFS_Config *) handle)->hwAttrs;
#endif

    /* Unmount the FatFs drive */
	snprintf(drive_path,sizeof(drive_path),"%d:",(int)object->driveNumber);
    fresult = f_mount(NULL,drive_path,0);
    if (fresult != FR_OK) {
#ifdef LOG_EN
        Log_print2(Diags_USER1,
                   "SDFATFS:(%p) Could not unmount FatFs volume @ drive number %d",
                   hwAttrs->baseAddr,
                   object->driveNumber);
#endif
    }

    /* Unregister the disk_*() functions */
    dresult = disk_unregister(object->driveNumber);
    if (dresult != RES_OK) {
#ifdef LOG_EN
        Log_print2(Diags_USER1,
                   "SDFATFS:(%p) Error unregistering disk functions @ drive number %d",
                   hwAttrs->baseAddr,
                   object->driveNumber);
#endif
    }

#ifdef LOG_EN
    Log_print1(Diags_USER1, "SDFATFS:(%p) closed", hwAttrs->baseAddr);
#endif

    object->driveNumber = DRIVE_NOT_MOUNTED;

    ((FATFS_Config *) handle)->object->isOpen = 0;

    return (FATFS_OK);
}

/*
 *  ======== FATFS_control ========
 */
FATFS_Error FATFS_control(FATFS_Handle handle, uint32_t cmd, void *arg); /*for misra warnings*/
FATFS_Error FATFS_control(FATFS_Handle handle, uint32_t cmd, void *arg)
{
    /* Input parameter validation */
    FATFS_osalAssert(handle == NULL);

	/* No implementation yet */
	return (FATFS_ERR);
}

/*
 *  ======== FATFS_init ========
 */
FATFS_Error FATFS_init(void)
{
    if (FATFS_count == -1) {
        /* Call each driver's init function */
        for (FATFS_count = 0; FATFS_config[FATFS_count].object != NULL; FATFS_count++) {
            /* Mark the object as available */
            FATFS_config[FATFS_count].object->isOpen = 0;
            FATFS_config[FATFS_count].object->driveNumber = DRIVE_NOT_MOUNTED;
            FATFS_config[FATFS_count].object->diskState = STA_NOINIT;
        }
    }

    return (FATFS_OK);
}

/*
 *  ======== FATFS_open ========
 */
FATFS_Error FATFS_open(uint32_t index,
                       FATFS_Params  params,
                       FATFS_Handle *handle)
{
    FATFS_Error                 ret = FATFS_ERR;
    DRESULT                     dresult;
    FRESULT                     fresult;
    FATFS_Object               *object = NULL;
    FATFS_HwAttrs const        *hwAttrs = NULL;
    TCHAR drive_path[5];
	
    /* Input parameter validation */
    FATFS_osalAssert((FATFS_Handle)&(FATFS_config[index]) == NULL);

    /* Get handle for this driver instance */
    *handle = (FATFS_Config *)&(FATFS_config[index]);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *)&(FATFS_config[index]))->object;
    hwAttrs = ((FATFS_Config *)&(FATFS_config[index]))->hwAttrs;

    /* Store the FATFS parameters */
    if (params == NULL) {
        /* No params passed in, so use the defaults */
        FATFS_Params_init(&(object->FATFSConfigParams));
    }
    else {
        /* Copy the params contents */
        object->FATFSConfigParams.drvHandle = ((FATFS_ConfigParams *) params)->drvHandle;
        object->FATFSConfigParams.drvParams = ((FATFS_ConfigParams *) params)->drvParams;
        object->FATFSConfigParams.drvFxnTablePtr = ((FATFS_ConfigParams *) params)->drvFxnTablePtr;
        object->FATFSConfigParams.custom = ((FATFS_ConfigParams *) params)->custom;
    }

    if(0 != object->isOpen)
    {
        ret = FATFS_ERR;
    }
    else
    {
        object->drvInst = hwAttrs->drvInst;
        object->driveNumber = index;
        ret = FATFS_OK;
    }

    if(FATFS_OK == ret)
    {
        /* Register the new disk_*() functions */
        dresult = disk_register(object->driveNumber,
                                FATFS_diskInitialize,
                                FATFS_diskStatus,
                                FATFS_diskRead,
                                FATFS_diskWrite,
                                FATFS_diskIOctrl);

        /* Check for drive errors */
        if (dresult != RES_OK) {
#ifdef LOG_EN
            FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk functions not registered",
                        hwAttrs->baseAddr);
#endif
            FATFS_close(handle);
            ret = FATFS_ERR;
        }
    }

    if(FATFS_OK == ret)
    {
        /*
         * Register the filesystem with FatFs. This operation does not access the
         * SDCard yet.
         */
         snprintf(drive_path,sizeof(drive_path),"%d:",(int)object->driveNumber);
		 fresult = f_mount(&(object->filesystem),drive_path,0);
        if (fresult != FR_OK) {
#ifdef LOG_EN
            FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) drive %d not mounted",
                        hwAttrs->baseAddr, object->driveNumber);
#endif

            FATFS_close(handle);
            ret = FATFS_ERR;
        }
    }

    if(FATFS_OK == ret)
    {
        object->isOpen = 1;

#ifdef LOG_EN
        FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) opened", hwAttrs->baseAddr);
#endif
    }

    if((FATFS_OK != ret) && (0 != object->isOpen))
    {
        ret = FATFS_OK;
    }

    return (ret);
}

/*
 *  ======== FATFS_Params_init =======
 */
FATFS_Error FATFS_Params_init(FATFS_Params params)
{
    /* Input parameter validation */
    FATFS_osalAssert(params == NULL);

    ((FATFS_ConfigParams *) params)->drvHandle = FATFS_defaultParams.drvHandle;
    ((FATFS_ConfigParams *) params)->drvParams = FATFS_defaultParams.drvParams;
    ((FATFS_ConfigParams *) params)->drvFxnTablePtr = FATFS_defaultParams.drvFxnTablePtr;
    ((FATFS_ConfigParams *) params)->custom = FATFS_defaultParams.custom;

    return (FATFS_OK);
}

/*
 *  ======== FATFS_diskInitialize ========
 *  Function to initialize the SD Card.  This function is called by the FatFs
 *  module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 */
static DSTATUS FATFS_diskInitialize(BYTE drv)
{
    uint32_t              res = STA_NOINIT;
    FATFS_Object         *object = NULL;
#ifdef LOG_EN
    FATFS_HwAttrs const  *hwAttrs = NULL;
#endif
    FATFS_Handle          handle;
    FATFS_DrvHandle       drvHandle = NULL;

    /* Get handle for this driver instance */
    handle = (FATFS_Handle)&(FATFS_config[drv]);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *) handle)->object;
#ifdef LOG_EN
    hwAttrs = ((FATFS_Config *) handle)->hwAttrs;
#endif

    ((FATFS_Config *) handle)->drvFxnTablePtr->initDrvFxn();

    if (FATFS_OK != ((FATFS_Config *) handle)->drvFxnTablePtr->openDrvFxn(object->drvInst, object->FATFSConfigParams.drvParams, &drvHandle))
    {
#ifdef LOG_EN
        FATFS_drv_log("\r\nCard Init Failed \r\n");
#endif
    }
    else
    {
        object->drvHandle = drvHandle;
        object->diskState &= ~STA_NOINIT;
        res = RES_OK;
    }

    return ((DSTATUS)res);
}

/*
 *  ======== FATFS_diskIOctrl ========
 *  Function to perform specifed disk operations. This function is called by the
 *  FatFs module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 *
 *  @param  ctrl        Control code
 *
 *  @param  buf         Buffer to send/receive control data
 */
static DRESULT FATFS_diskIOctrl(BYTE drv, BYTE ctrl, void *buf)
{
    DRESULT               res = RES_ERROR;
    FATFS_Handle          handle;
    FATFS_Object         *object = NULL;
#ifdef LOG_EN
    FATFS_HwAttrs const  *hwAttrs = NULL;
#endif

    /* Get handle for this driver instance */
    handle = (FATFS_Handle)&(FATFS_config[drv]);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *) handle)->object;
#ifdef LOG_EN
    hwAttrs = ((FATFS_Config *) handle)->hwAttrs;
#endif

    if (object->diskState & STA_NOINIT) {
#ifdef LOG_EN
        FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk IO control: disk not initialized",
                    hwAttrs->baseAddr);
#endif
        res = RES_NOTRDY;
    }

    res = RES_OK;

    return (res);
}

/*
 *  ======== FATFS_diskRead ========
 *  Function to perform a disk read from the SDCard. This function is called by
 *  the FatFs module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 *
 *  @param  buf         Pointer to a buffer on which to store data
 *
 *  @param  sector      Starting sector number (LBA)
 *
 *  @param  count       Sector count (1...255)
 */
static DRESULT FATFS_diskRead(BYTE drv, BYTE *buf, DWORD sector, BYTE count)
{
    DRESULT               res = RES_ERROR;
    FATFS_Handle          handle;
    FATFS_Object          *object = NULL;
#ifdef LOG_EN
    FATFS_HwAttrs const   *hwAttrs = NULL;
#endif

    /* Get handle for this driver instance */
    handle = (FATFS_Handle)&(FATFS_config[drv]);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *) handle)->object;
#ifdef LOG_EN
    hwAttrs = ((FATFS_Config *) handle)->hwAttrs;
#endif

    if (count == 0U) {
#ifdef LOG_EN
        FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk read: 0 sectors to read",
                                 hwAttrs->baseAddr);
#endif
        res= RES_PARERR;
    }
    else if (object->diskState & STA_NOINIT) {
#ifdef LOG_EN
        FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk read: disk not initialized",
                    hwAttrs->baseAddr);
#endif
        res = RES_NOTRDY;
    }
    /* READ BLOCK */
    else 
    {
        if (FATFS_OK == ((FATFS_Config *) handle)->drvFxnTablePtr->readDrvFxn(object->drvHandle, buf, sector, count))
        {
            res = RES_OK;
        }
    }

    return res;
}

/*
 *  ======== FATFS_diskStatus ========
 *  Function to return the current disk status. This function is called by
 *  the FatFs module and must not be called by the application!
 *
 *  @param(drv)         Drive Number
 */
static DSTATUS FATFS_diskStatus(BYTE drv)
{
    DRESULT               res = RES_ERROR;
    FATFS_Handle          handle;
    FATFS_Object         *object = NULL;
#ifdef LOG_EN
    FATFS_HwAttrs const  *hwAttrs = NULL;
#endif

    /* Get handle for this driver instance */
    handle = (FATFS_Handle)&(FATFS_config[drv]);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *) handle)->object;
#ifdef LOG_EN
    hwAttrs = ((FATFS_Config *) handle)->hwAttrs;
#endif

    /* Use Diags_USER1 to reduce noise in the logs */
#ifdef LOG_EN
    FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk status: diskState: %d",
               (hwAttrs->baseAddr, object->diskState);
#endif
    res = (DRESULT) object->diskState;

    return res;
}

/*
 *  ======== FATFS_diskWrite ========
 *  Function to perform a disk write from the SDCard. This function is called by
 *  the FatFs module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 *
 *  @param  buf         Pointer to a buffer from which to read data
 *
 *  @param  sector      Starting sector number (LBA)
 *
 *  @param  count       Sector count (1...255)
 */
static DRESULT FATFS_diskWrite(BYTE drv, const BYTE *buf,
                            DWORD sector, BYTE count)
{
    DRESULT                res = RES_ERROR;
    FATFS_Handle           handle;
    FATFS_Object          *object = NULL;
#ifdef LOG_EN
    FATFS_HwAttrs const   *hwAttrs = NULL;
#endif

    /* Get handle for this driver instance */
    handle = (FATFS_Handle)&(FATFS_config[drv]);

    /* Get the pointer to the object and hwAttrs */
    object = ((FATFS_Config *) handle)->object;
#ifdef LOG_EN
    hwAttrs = ((FATFS_Config *) handle)->hwAttrs;
#endif

    if (count == 0U) {
#ifdef LOG_EN
        FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk write: 0 sectors to write",
                                 hwAttrs->baseAddr);
#endif
        res = RES_PARERR;
    }
    else
    {
        if (object->diskState & STA_NOINIT) {
    #ifdef LOG_EN
            FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk write: disk not initialized",
                        hwAttrs->baseAddr);
    #endif
            res = RES_NOTRDY;
        }
        else if (object->diskState & STA_PROTECT) {
    #ifdef LOG_EN
            FATFS_drv_log(Diags_USER1, "SDFATFS:(%p) disk write: disk protected",
                        hwAttrs->baseAddr);
    #endif
            res = RES_WRPRT;
        }
        /* WRITE BLOCK */
        else
        {
            if(FATFS_OK == ((FATFS_Config *) handle)->drvFxnTablePtr->writeDrvFxn(object->drvHandle,(BYTE*) buf, sector, count))
            {
                res = RES_OK;
            }
        }
    }
    return res;
}

