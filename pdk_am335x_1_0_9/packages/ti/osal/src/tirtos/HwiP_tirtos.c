/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
/*
 *  ======== HwiP_tirtos.c ========
 */

#include <ti/osal/HwiP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#if  defined(__ARM_ARCH_7A__)
#if defined(gnu_targets_arm_A15F) || defined(gnu_targets_arm_A9F)
#include <ti/sysbios/family/arm/gic/Hwi.h>
#elif defined(ti_targets_arm_elf_A8F) || \
      defined(ti_targets_arm_elf_A8Fnv) || \
      defined(gnu_targets_arm_A8F)
#include  <ti/sysbios/family/arm/a8/intcps/Hwi.h>
#endif
#elif ((defined (SOC_OMAPL137) || defined (SOC_OMAPL138)) && (!defined(_TMS320C6X)))
	#include <ti/sysbios/family/arm/da830/Hwi.h>
#else
#ifdef _TMS320C6X
#include <ti/sysbios/family/c64p/Hwi.h>
#else
#include <ti/sysbios/hal/Hwi.h>
#endif
#endif /* __ARM_ARCH_7A__ */

#include <ti/osal/src/tirtos/tirtos_config.h>

extern void _DebugP_assert(int32_t expression, const char *file, int32_t line);
#define HWIPOSAL_Assert(expression) (_DebugP_assert((expression),      \
                                                  __FILE__, __LINE__))

extern uint32_t  gOsalHwiAllocCnt, gOsalHwiPeak;

/*!
 *  @brief    Hwi structure
 */
typedef struct HwiP_tiRtos_s {
    Bool              used;
    Hwi_Struct        hwi;
} HwiP_tiRtos;

/* global pool of statically allocated semaphore pools */
static HwiP_tiRtos gOsalHwiPTiRtosPool[OSAL_TIRTOS_CONFIGNUM_HWI] = {{0}};

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int32_t interruptNum)
{
    Hwi_clearInterrupt((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int32_t interruptNum, HwiP_Fxn hwiFxn,
                        HwiP_Params *params)
{
    HwiP_tiRtos *handle = (HwiP_tiRtos *) NULL;
    Hwi_Params  hwiParams;
    Error_Block eb;
    uint32_t          i;
    uintptr_t         key;

    /* Grab the memory */
    key = HwiP_disable();

    for (i = 0; i < OSAL_TIRTOS_CONFIGNUM_HWI; i++)
    {
        if (gOsalHwiPTiRtosPool[i].used == FALSE)
        {
            gOsalHwiPTiRtosPool[i].used = TRUE;
            /* Update statistics */
            gOsalHwiAllocCnt++;
            if (gOsalHwiAllocCnt > gOsalHwiPeak)
            {
                gOsalHwiPeak = gOsalHwiAllocCnt;
            }
            break;
        }
    }
    HwiP_restore(key);

    if (i < OSAL_TIRTOS_CONFIGNUM_HWI)
    {
        /* Grab the memory */
        handle = (HwiP_tiRtos *) &gOsalHwiPTiRtosPool[i];
    }

    if (handle != NULL)
    {
        /* Initialize the error */
        Error_init(&eb);

        if (params == NULL)
        {
            Hwi_construct(&handle->hwi, interruptNum, (Hwi_FuncPtr)hwiFxn, NULL, &eb);
        }
        else
        {
            Hwi_Params_init(&hwiParams);
            hwiParams.instance->name = params->name;
            hwiParams.arg            = (xdc_UArg)(params->arg);

            if (params->priority==0) 
            {
               /* A priority of 0 is invalid for many targets. -1 forces 
                  sysbios to assign a default priority */
               hwiParams.priority       = -1;
            }
            else
            {
               hwiParams.priority       = (int)params->priority;
            }

            hwiParams.eventId        = (int)params->evtId;
            hwiParams.enableInt      = TRUE;
            hwiParams.maskSetting    = Hwi_MaskingOption_SELF;
#if defined (__ARM_ARCH_7A__)
#if defined(gnu_targets_arm_A15F) || defined(gnu_targets_arm_A9F)
            hwiParams.triggerSensitivity = (UInt)params->triggerSensitivity;
#endif
#endif
            Hwi_construct(&handle->hwi, interruptNum, (Hwi_FuncPtr)hwiFxn,
                          &hwiParams, &eb);

            if (Error_check(&eb))
            {
                /* Free the allocated memory and return null */
                handle->used = FALSE;
                handle = (HwiP_tiRtos *) NULL;
            }
        }
    }

    return ((HwiP_Handle)handle);
}

/*
 *  ======== HwiP_delete ========
 */
HwiP_Status HwiP_delete(HwiP_Handle handle)
{
    HWIPOSAL_Assert((handle == NULL));

    uintptr_t   key;
    HwiP_tiRtos *hwi = (HwiP_tiRtos *)handle;

    Hwi_destruct(&hwi->hwi);
    key = HwiP_disable();
    hwi->used = FALSE;
    /* Found the osal hwi object to delete */
    if (gOsalHwiAllocCnt > 0U)
    {
        gOsalHwiAllocCnt--;
    }

    HwiP_restore(key);
    return (HwiP_OK);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uintptr_t key;

    key = Hwi_disable();

    return (key);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int32_t interruptNum)
{
    Hwi_disableInterrupt((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int32_t interruptNum)
{
    Hwi_enableInterrupt((uint32_t)interruptNum);
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    params->name = NULL;
    params->arg = 0;
    params->priority = ~((uint32_t)0);
    params->evtId    = 0;    
#if defined (__ARM_ARCH_7A__) 
#if defined(gnu_targets_arm_A15F) || defined(gnu_targets_arm_A9F)
    params->triggerSensitivity = OSAL_ARM_GIC_TRIG_TYPE_LEVEL;
#endif
#endif
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    Hwi_restore(key);
}
