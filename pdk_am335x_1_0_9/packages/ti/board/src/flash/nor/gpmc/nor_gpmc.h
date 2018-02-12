/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *
 * \file  nor_gpmc.h
 *
 * \brief This file contains structure, typedefs, functions and
 *        prototypes used for GPMC interface for NOR flash.
 *
 *****************************************************************************/
#ifndef NOR_GPMC_H_
#define NOR_GPMC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include <ti/board/src/flash/nor/nor.h>
#include <ti/drv/gpmc/GPMC.h>

#if defined (icev2AM335x)
#include <ti/board/src/flash/nor/device/m29w160eb.h>
#include <ti/drv/gpmc/src/V1/GPMC_v1.h>
#endif

#define BIT5          (0x20U)
#define BIT7          (0x80U)
#define BIT15         (0x8000U)

typedef union
{
    uint8_t  c;
    uint16_t w;
    uint32_t l;
}
NOR_Data;

typedef union
{
    volatile uint8_t  *cp;
    volatile uint16_t *wp;
    volatile uint32_t *lp;
}
NOR_Ptr;


/**************************************************************************
 **                       Macro Definitions
 **************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* NOR_GPMC_H_ */

/* Nothing past this point */
