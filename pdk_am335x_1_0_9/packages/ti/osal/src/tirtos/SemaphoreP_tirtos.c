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
 *  ======== SemaphoreP_tirtos.c ========
 */
#include <ti/osal/SemaphoreP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/osal/src/tirtos/tirtos_config.h>

extern void _DebugP_assert(int32_t expression, const char *file, int32_t line);
#define SEMOSAL_Assert(expression) (_DebugP_assert((expression),      \
                                                  __FILE__, __LINE__))

extern uint32_t  gOsalSemAllocCnt, gOsalSemPeak;

/*!
 *  @brief    Semaphore structure
 */
typedef struct SemaphoreP_tiRtos_s {
    Bool              used;
    Semaphore_Struct  sem;
} SemaphoreP_tiRtos;

/* global pool of statically allocated semaphore pools */
static SemaphoreP_tiRtos gOsalSemPTiRtosPool[OSAL_TIRTOS_CONFIGNUM_SEMAPHORE] = {{0}};

/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create(uint32_t count,
                                    SemaphoreP_Params *params)
{
    SemaphoreP_Handle ret_handle;
    SemaphoreP_tiRtos *handle = (SemaphoreP_tiRtos *) NULL;
    Semaphore_Params     semaphoreParams;
    uint32_t          i;
    uintptr_t         key;

    key = HwiP_disable();

     for (i = 0; i < OSAL_TIRTOS_CONFIGNUM_SEMAPHORE; i++)
     {
         if (gOsalSemPTiRtosPool[i].used == FALSE)
         {
             gOsalSemPTiRtosPool[i].used = TRUE;
             /* Update statistics */
             gOsalSemAllocCnt++;
             if (gOsalSemAllocCnt > gOsalSemPeak)
             {
                 gOsalSemPeak = gOsalSemAllocCnt;
             }
             break;
         }
     }
     HwiP_restore(key);

    if (i < OSAL_TIRTOS_CONFIGNUM_SEMAPHORE)
    {
        /* Grab the memory */
        handle = (SemaphoreP_tiRtos *) &gOsalSemPTiRtosPool[i];
    }

    if (handle == (SemaphoreP_tiRtos *)NULL) {
        ret_handle = (SemaphoreP_Handle) NULL;
    }
    else
    {
        if (params == NULL) {
            Semaphore_construct(&handle->sem, (int32_t)count, NULL);
        }
        else {
            Semaphore_Params_init(&(semaphoreParams));
            /*
             * The default mode for TI-RTOS is counting, so only change if a binary
             * semaphore is requested.
             */
            if (params->mode == SemaphoreP_Mode_BINARY) {
                (semaphoreParams).mode = Semaphore_Mode_BINARY;
            }
            (semaphoreParams).instance->name = params->name;
            Semaphore_construct(&handle->sem, (int32_t)count, &(semaphoreParams));
        }

        ret_handle = ((SemaphoreP_Handle)handle);
    }
    return ret_handle;
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle)
{
    SEMOSAL_Assert((handle == NULL));

    uintptr_t   key;

    SemaphoreP_tiRtos *semaphore = (SemaphoreP_tiRtos *)handle;

    Semaphore_destruct(&semaphore->sem);

    key = HwiP_disable();
    semaphore->used = FALSE;
    /* Found the osal semaphore object to delete */
    if (gOsalSemAllocCnt > 0U)
    {
        gOsalSemAllocCnt--;
    }
    HwiP_restore(key);

    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_Params_init ========
 */
void SemaphoreP_Params_init(SemaphoreP_Params *params)
{
    SEMOSAL_Assert((params == NULL));

    params->mode = SemaphoreP_Mode_COUNTING;
    params->name = NULL;
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle, uint32_t timeout)
{
    Bool flag;
    SemaphoreP_Status ret_val;
    SemaphoreP_tiRtos *semaphore = (SemaphoreP_tiRtos *)handle;

    SEMOSAL_Assert((handle == NULL));

    flag = Semaphore_pend((Semaphore_Handle)&semaphore->sem, timeout);
    if (FALSE == flag) {
        ret_val = SemaphoreP_TIMEOUT;
    }
    else
    {
        ret_val = SemaphoreP_OK;
    }

    return (ret_val);
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle)
{
    SEMOSAL_Assert((handle == NULL));
    SemaphoreP_tiRtos *semaphore = (SemaphoreP_tiRtos *)handle;

    Semaphore_post((Semaphore_Handle)&semaphore->sem);
    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_postFromClock ========
 */
SemaphoreP_Status SemaphoreP_postFromClock(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}

/*
 *  ======== SemaphoreP_postFromISR ========
 */
SemaphoreP_Status SemaphoreP_postFromISR(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}

/* Nothing past this point */
