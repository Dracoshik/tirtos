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
 */
/*
 *  ======== HwiP_tirtos.c ========
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include "interrupt.h"

/* Local hwi structures */
static Hwi_Struct hwiStructs[OSAL_NONOS_CONFIGNUM_HWI];
static bool hwiUsed[OSAL_NONOS_CONFIGNUM_HWI] = {FALSE};

/* Local function */

static void OsalArch_ISRStub(uint32_t intNum, uint32_t cpuId, void* pUserParam)
{
  Hwi_Struct *handle = (Hwi_Struct *)pUserParam;

  handle->fxn((uintptr_t)handle->arg);

}

/* Dummy function for starterware reference */
int32_t CONSOLEUtilsPuts(const char *pString)
{
  return (0);
}

/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    INTCEnableIntr(intNum);
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    INTCDisableIntr(intNum);
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    INTCClearIntr(intNum);
}
/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
    return ( (uintptr_t)INTCDisableCpuIntr());
}

/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
    INTCEnableCpuIntr((uint32_t)restoreValue);
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(int32_t interruptNum, HwiP_Fxn hwiFxn,
                          HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle=NULL;
    intcIntrParams_t             intrParams;
    int32_t i;
    uintptr_t key;

    if (params == NULL)
    {
        return (NULL);
    }
    
    key = OsalArch_globalDisableInterrupt();
    for (i = 0; i < OSAL_NONOS_CONFIGNUM_HWI; i++) {
        if (hwiUsed[i] == false) {
            hwiUsed[i] = true;
            break;
        }
    }
    OsalArch_globalRestoreInterrupt(key);

    if (i != OSAL_NONOS_CONFIGNUM_HWI)
    {
      hwi_handle = &(hwiStructs[i]);
    }
    else
    {
      return (NULL);
    }

    INTCInit(FALSE);

    switch (params->triggerSensitivity)
    {
        case OSAL_ARM_GIC_TRIG_TYPE_EDGE:
             intrParams.triggerType = INTC_TRIG_BOTH_EDGE;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_FALLING_EDGE:
             intrParams.triggerType = INTC_TRIG_FALLING_EDGE;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_RISING_EDGE:
             intrParams.triggerType = INTC_TRIG_RISING_EDGE;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_LOW_LEVEL:
             intrParams.triggerType = INTC_TRIG_LOW_LEVEL;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL:
        case OSAL_ARM_GIC_TRIG_TYPE_LEVEL:
        default:
             intrParams.triggerType = INTC_TRIG_HIGH_LEVEL;
             break;
    }

    /* Record the hwiFxn and argument */
    hwi_handle->arg           = (void*) params->arg;
    hwi_handle->fxn           = hwiFxn;
    hwi_handle->intNum        = interruptNum;

    /* set the priority */
    intrParams.priority       = params->priority;
    intrParams.pFnIntrHandler = &OsalArch_ISRStub;
    intrParams.pUserParam     = (void* )hwi_handle;
    intrParams.isIntrSecure   = FALSE;

    /* Configure the interrupt Controller */
    INTCConfigIntr(interruptNum,&intrParams, FALSE);

    /* Enabling the interrupt in INTC. */
    HwiP_enableInterrupt(interruptNum);

    return ( (HwiP_Handle) (hwi_handle) );

}

HwiP_Status OsalArch_HwiPDelete(HwiP_Handle handle)
  {
      Hwi_Struct *hwi_hnd = (Hwi_Struct*) handle;
      uintptr_t   key;
  
      int index = (hwi_hnd - hwiStructs); /* struct subtraction */
  
      /* mark that index as free */
      key = OsalArch_globalDisableInterrupt();
      hwiUsed[index] = FALSE;
      OsalArch_globalRestoreInterrupt(key);
      return (HwiP_OK);
  }


/* Nothing past this point */
