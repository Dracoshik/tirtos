/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *  ======== SemaphoreP_nonos.c ========
 */
#include <ti/osal/SemaphoreP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ti/osal/osal.h>
#include <ti/osal/src/nonos/Nonos_config.h>

extern uint32_t  gOsalSemAllocCnt, gOsalSemPeak;

/*!
 *  @brief    Semaphore structure
 */
typedef struct Sem_Struct_s {
    bool              used;
    uint32_t          sem;
    uint32_t          count;
    SemaphoreP_Mode   mode;
} Sem_Struct;

Sem_Struct semStructs[OSAL_NONOS_CONFIGNUM_SEMAPHORE] =
{
    {false, 0, 0, SemaphoreP_Mode_COUNTING},
    {false, 1, 0, SemaphoreP_Mode_COUNTING},
    {false, 2, 0, SemaphoreP_Mode_COUNTING},
    {false, 3, 0, SemaphoreP_Mode_COUNTING},
    {false, 4, 0, SemaphoreP_Mode_COUNTING},
    {false, 5, 0, SemaphoreP_Mode_COUNTING},
    {false, 6, 0, SemaphoreP_Mode_COUNTING},
    {false, 7, 0, SemaphoreP_Mode_COUNTING}
};

/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create(uint32_t count,
                                    SemaphoreP_Params *params)
{
    uint32_t          i;
    uintptr_t         key;
    SemaphoreP_Handle ret_val;
    key = HwiP_disable();

    for (i = 0; i < OSAL_NONOS_CONFIGNUM_SEMAPHORE; i++)
    {
        if (semStructs[i].used == false)
        {
            semStructs[i].used = true;

            /* Update statistics */
            gOsalSemAllocCnt++;
            if (gOsalSemAllocCnt > gOsalSemPeak)
            {
                gOsalSemPeak = gOsalSemAllocCnt;
            }

            semStructs[i].sem = i;
            semStructs[i].count = count;
            if (params)
            {
                semStructs[i].mode = params->mode;
            }

            break;
        }
    }
    HwiP_restore(key);

    if (i == OSAL_NONOS_CONFIGNUM_SEMAPHORE)
    {
        ret_val = NULL;
    }
    else
    {
        ret_val = ((SemaphoreP_Handle)&semStructs[i]);
    }
    return (ret_val);
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle)
{
    OSAL_Assert((handle == NULL));

    uintptr_t   key;
    Sem_Struct *semS = (Sem_Struct *)handle;

    key = HwiP_disable();
    semS->used = false;
    /* Found the bsp osal semaphore object to delete */
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
    OSAL_Assert((params == NULL));
    params->mode = SemaphoreP_Mode_COUNTING;
    params->name = NULL;
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle, uint32_t timeout)
{
    OSAL_Assert((handle == NULL));

    uintptr_t           key;
    Sem_Struct         *semS        = (Sem_Struct *)handle;
    uint32_t            semTimeout  = timeout;
    SemaphoreP_Status   ret_val     = SemaphoreP_OK;

    while (ret_val == SemaphoreP_OK)
    {
        key = HwiP_disable();
        if (semS->count > 0)
        {
            if (semS->mode == SemaphoreP_Mode_BINARY)
            {
				semS->count = 0;
            }
            else
            {
                semS->count--;
            }
            HwiP_restore(key);
            break;
        }
	else
        {
            HwiP_restore(key);
            if (semTimeout)
            {
                /* Time out not supported, will use timer delay function
                   when timer functions are implemented */
                semTimeout--;
                if (semTimeout == 0)
                {
                    ret_val = (SemaphoreP_TIMEOUT);
                }
            }
        }
    }

    return (ret_val);
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle)
{
    OSAL_Assert((handle == NULL));

    uintptr_t   key;
    Sem_Struct *semS = (Sem_Struct *)handle;

    key = HwiP_disable();
    if (semS->mode == SemaphoreP_Mode_BINARY)
    {
        semS->count = 1;
    }
    else
    {
        semS->count++;
    }
    HwiP_restore(key);

    return (SemaphoreP_OK);
}

/* Nothing past this point */

