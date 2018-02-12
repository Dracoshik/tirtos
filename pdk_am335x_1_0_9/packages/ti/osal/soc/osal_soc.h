/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
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
 */
/** ============================================================================
 *  @file       osal_soc.h
 *
 *  @brief      SOC specific includes for Osal
 *
 *
 *  ============================================================================
 */
#ifndef ti_osal_soc_top_include
#define ti_osal_soc_top_include
   
#ifdef __cplusplus
   extern "C" {
#endif

#if defined (SOC_AM572x)
#include <ti/osal/soc/am572x/osal_soc.h>
#elif defined (SOC_AM571x)
#include <ti/osal/soc/am571x/osal_soc.h>
#elif defined (SOC_AM335x)
#include <ti/osal/soc/am335x/osal_soc.h>
#elif defined (SOC_AM437x)
#include <ti/osal/soc/am437x/osal_soc.h>
#elif defined (SOC_DRA72x)
#include <ti/osal/soc/dra72x/osal_soc.h>
#elif defined (SOC_DRA75x)
#include <ti/osal/soc/dra75x/osal_soc.h>
#elif defined (SOC_DRA78x)
#include <ti/osal/soc/dra78x/osal_soc.h>
#elif defined (SOC_C6657)
#include <ti/osal/soc/c6657/osal_soc.h>
#elif defined (SOC_C6678)
#include <ti/osal/soc/c6678/osal_soc.h>
#elif defined (SOC_K2E)
#include <ti/osal/soc/k2e/osal_soc.h>
#elif defined (SOC_K2G)
#include <ti/osal/soc/k2g/osal_soc.h>
#elif defined (SOC_K2H)
#include <ti/osal/soc/k2h/osal_soc.h>
#elif defined (SOC_K2K)
#include <ti/osal/soc/k2k/osal_soc.h>
#elif defined (SOC_K2L)
#include <ti/osal/soc/k2l/osal_soc.h>
#elif defined (SOC_OMAPL137)
#include <ti/osal/soc/omapl137/osal_soc.h>
#elif defined (SOC_OMAPL138)
#include <ti/osal/soc/omapl138/osal_soc.h>
#elif defined (SOC_AM574x)
#include <ti/osal/soc/am574x/osal_soc.h>
#else
/* No known Soc is defined */
#endif

#ifdef __cplusplus
}
#endif

#endif

/* Nothing past this point */
