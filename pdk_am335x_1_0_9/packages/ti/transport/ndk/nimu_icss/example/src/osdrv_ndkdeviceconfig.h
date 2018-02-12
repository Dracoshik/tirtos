/**
 * @file   osdrv_ndkdeviceconfig.h
 *
 * @brief 
 *
*/
/*
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 */

#ifndef _OSDRV_NDKDEVICECONFIG_H
#define _OSDRV_NDKDEVICECONFIG_H
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <xdc/std.h>
#include <ti/ndk/inc/stkmain.h>
#include <ti/ndk/inc/netmain.h>
#include "ti/drv/icss_emac/icss_emacDrv.h"
#include "ti/drv/pruss/pruicss.h"
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define MAX_ICSSNIMU_TBL_ENTRIES   3

typedef int32_t (*NIMUInitFn) (STKEVENT_Handle hEvent) ;

typedef void*       NimuHandle;

//#define info_printf(...) DbgPrintf(0, __VA_ARGS__) /* level 1 = DBG_INFO */
/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */



/* ========================================================================== */
/*                            Global Variables Declarations                   */
/* ========================================================================== */



/**
 * Desc
 * 		 This API adds initialization function to NDK device table,
 * 		  that will be executed by NDK on startup. This needs to be called before BIOS_Start
 * Params
 * 		 CPSW_NETIF_INIT - To initiate CPSW driver
 * 		 ICSS_NETIF_INIT - To initiate ICSS Netif driver
 * Return
 * 		0 - Failure
 * 		1 - Success
 *
 */
int32_t OSDRV_addNetifEntry(NIMUInitFn initFct,HANDLE emacHandle);

HANDLE OSDRV_getNetifHandle(uint32_t inst);

#endif  //_OSDRV_NDKDEVICECONFIG_H

