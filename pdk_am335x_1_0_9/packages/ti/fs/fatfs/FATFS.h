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
/** ============================================================================
 *  @file       FATFS.h
 *
 *  @brief      FATFS interface for driver
 *
 *  The SD header file should be included in an application as follows:
 *  @code
 *  #include <ti/fs/fatfs/FATFS.h>
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef FATFS_H
#define FATFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ti/fs/fatfs/ff.h>
#include <ti/fs/fatfs/diskio.h>

/*@{*/
/**
 *  @def  FATFS_OK
 *        FATFS return code -- Function executed successfully
 */
#define FATFS_OK	                                 0

/**
 *  @def  FATFS_ERR
 *        FATFS return code -- Function executed failed
 */
#define FATFS_ERR                                   (-(1))

/*@}*/
/** @} */

/*!
 *  @brief      Return status of FATFS.
 */
typedef int32_t                   FATFS_Error;

/*!
 *  @brief      A handle that is returned from a FATFS_open() call.
 */
typedef void                     *FATFS_Handle;

/*!
 *  @brief      Parameters that is required for FATFS_open() call.
 */
typedef void                     *FATFS_Params;

/*!
 *  @brief      A handle that is returned to open driver.
 */
typedef void                     *FATFS_DrvHandle;

/*!
 *  @brief      Parameters that is required to open driver.
 */
typedef void                     *FATFS_DrvParams;

/*!
 *  @brief      The definition of a SD function table that contains the
 *              required set of functions to control a specific SD driver
 *              implementation.
 */
typedef struct FATFS_DrvFxnTable_s  FATFS_DrvFxnTable;

/*!
 *  @brief  SD Parameters
 *
 *  SD parameters are used to with the FATFS_open() call. Default values for
 *  these parameters are set using FATFS_Params_init().
 *
 *  @sa     FATFS_Params_init()
 */
typedef struct FATFS_ConfigParams_s {
    FATFS_DrvHandle    drvHandle;       /*!< Driver Handle. */
    FATFS_DrvParams    drvParams;       /*!< Driver Parameters. */
    FATFS_DrvFxnTable *drvFxnTablePtr;  /*!< FATFS functional table to be
                                             register on open. */
    void              *custom;          /*!< Custom argument used by driver
                                             implementation */
} FATFS_ConfigParams;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              FATFS_close().
 */
typedef FATFS_Error (*FATFS_CloseDrvFxn) (FATFS_DrvHandle drvHandle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              FATFS_control().
 */
typedef FATFS_Error (*FATFS_ControlDrvFxn) (FATFS_DrvHandle drvHandle,
                                            uint32_t cmd,
                                            void *arg);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              FATFS_init().
 */
typedef FATFS_Error (*FATFS_InitDrvFxn) (void);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              FATFS_open().
 */
typedef FATFS_Error (*FATFS_OpenDrvFxn) (uint32_t index,
                                         FATFS_DrvParams  drvParams,
                                         FATFS_DrvHandle *drvHandle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              FATFS_transfer().
 */
typedef FATFS_Error (*FATFS_WriteDrvFxn) (FATFS_DrvHandle drvHandle,
                                          uint8_t *buf,
                                          uint32_t block,
                                          uint32_t numBlks);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              FATFS_transfer().
 */
typedef FATFS_Error (*FATFS_ReadDrvFxn) (FATFS_DrvHandle drvHandle,
                                         uint8_t *buf,
                                         uint32_t block,
                                         uint32_t numBlks);

/*!
 *  @brief      The definition of a SD function table that contains the
 *              required set of functions to control a specific SD driver
 *              implementation.
 */
struct FATFS_DrvFxnTable_s {
    /*! Function to close the specified peripheral */
    FATFS_CloseDrvFxn             closeDrvFxn;

    /*! Function to implementation specific control function */
    FATFS_ControlDrvFxn           controlDrvFxn;

    /*! Function to initialize the given data object */
    FATFS_InitDrvFxn              initDrvFxn;

    /*! Function to open the specified peripheral */
    FATFS_OpenDrvFxn              openDrvFxn;

    /*! Function to initiate a driver data write */
    FATFS_WriteDrvFxn             writeDrvFxn;

    /*! Function to initiate a driver data read */
    FATFS_ReadDrvFxn              readDrvFxn;
};

/* SD function table pointer */
extern FATFS_DrvFxnTable FATFS_drvFxnTable;

/*!
 *  @brief  SDAm57x Hardware attributes
 */
typedef struct FATFS_HwAttrs_s {
    uint32_t        drvInst; /*!< Driver instance if driver is not opened. */
} FATFS_HwAttrs;

/*!
 *  @brief  SDAm572x Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct FATFS_Object_s {
    FATFS_DrvHandle     drvHandle;     /*!< Driver Handle. */
    uint32_t            drvInst;       /*!< SD driver instance if SD driver is not
                                            opened. */
    uint32_t            driveNumber;   /*!< Drive number used by FatFs */
    DSTATUS             diskState;     /*!< Disk status */
    FATFS               filesystem;    /*!< FATFS data object */

    FATFS_ConfigParams  FATFSConfigParams;   /*! Stores SD parameters */

    uint32_t            isOpen;        /*! flag to indicate module is open */
} FATFS_Object;

/*!
 *  @brief  SD Global configuration
 *
 *  The FATFS_Config structure contains a set of pointers used to characterize
 *  the SD driver implementation.
 *
 *  This structure needs to be defined before calling FATFS_init() and it must
 *  not be changed thereafter.
 *
 *  @sa     FATFS_init()
 */
typedef struct FATFS_Config_s {
    /*! Pointer to a table of driver-specific implementations of SD APIs */
    FATFS_DrvFxnTable    *drvFxnTablePtr;

    /*! Pointer to a driver specific data object */
    FATFS_Object         *object;

    /*! Pointer to a driver specific hardware attributes structure */
    FATFS_HwAttrs        *hwAttrs;
} FATFS_Config;


/* Avoid Misra warning "MISRA.DECL.ARRAY_SIZE" by pairing config array type 
 * with its array size to avoid externs with [] (no size) */
typedef FATFS_Config FATFSConfigList[((uint32_t)_VOLUMES) + (uint32_t)1U];


/*!
 *  @brief  Function to close a SD peripheral specified by the SD handle
 *
 *  @pre    FATFS_open() had to be called first.
 *
 *  @param  handle  A FATFS_Handle returned from FATFS_open
 *
 *  @sa     FATFS_open()
 */
extern FATFS_Error FATFS_close(FATFS_Handle handle);

/*!
 *  @brief  Function to initialize a given SD peripheral specified by the
 *          particular index value. The parameter specifies which mode the SD
 *          will operate.
 *
 *  @pre    SD controller has been initialized
 *
 *  @param  index         Logical peripheral number for the disk indexed into
 *                        the FATFS_config table
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values. All the fields in this structure are
 *                        RO (read-only).
 *
 *  @return A FATFS_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *
 *  @sa     FATFS_init()
 *  @sa     FATFS_close()
 */
extern FATFS_Error FATFS_open(uint32_t index,
                              FATFS_Params  params,
                              FATFS_Handle *handle);

/*!
 *  @brief  Function to initialize the FATFS_Params struct to its defaults
 *
 *  @param  params      An pointer to FATFS_Params structure for
 *                      initialization
 *
 */
extern FATFS_Error FATFS_Params_init(FATFS_Params params);

/*!
 *  @brief  Function to initializes the FATFS module
 *
 *  @pre    The FATFS_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other FATFS APIs. This function call does not modify any
 *          peripheral registers.
 */
extern FATFS_Error FATFS_init(void);

#ifdef __cplusplus
}
#endif

#endif /* FATFS_H */
