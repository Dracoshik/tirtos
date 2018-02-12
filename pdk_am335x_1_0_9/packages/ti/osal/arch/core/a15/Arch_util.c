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
 *  ======== HwiP_tirtos.c ========
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/csl/arch/a15/csl_a15_startup.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/* Local hwi structures */
static Hwi_Struct hwiStructs[OSAL_NONOS_CONFIGNUM_HWI];
static bool hwiUsed[OSAL_NONOS_CONFIGNUM_HWI] = {FALSE};
static CSL_ArmGicCpuIntrf  *gCpuIntrf;

void OsalArch_gicInit(void)
{
    uint32_t initStatus;

    initStatus = CSL_armGicIsInitialized();
    if (initStatus == FALSE)
    {
        /* If not initialized via startup, initialze now 
         * initStatus FALSE, indicates that the CSL startup
         * is not followed and possibly, some other schemes
         * such as SBL/diag has started up and has done other
         * required initializations, only pending initialization
         * is Arm GIC */
        gCpuIntrf = CSL_armGicGetCpuIfHandler();
        gCpuIntrf->gicDist = CSL_armGicGetDistHandler();
        gCpuIntrf->cpuIntfBasePtr = (void *)SOC_INTC_MPU_PHYS_CPU_IF_BASE;
        gCpuIntrf->gicDist->distBasePtr = (void *)SOC_INTC_MPU_DISTRIBUTOR_BASE;
        gCpuIntrf->initStatus = (Uint32)FALSE;
        gCpuIntrf->gicDist->initStatus = (Uint32)FALSE;
        gCpuIntrf->pDefaultIntrHandlers  = NULL;
        gCpuIntrf->pDefaultUserParameter = NULL;
        CSL_armGicInit(gCpuIntrf);
    }
    else {
        gCpuIntrf = CSL_armGicGetCpuIfHandler();
    }
    return;
}

/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    CSL_armGicEnableIntr(gCpuIntrf, (uint32_t)intNum);
    return;
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    CSL_armGicDisableIntr(gCpuIntrf, (uint32_t)intNum);
    return;
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    CSL_armGicClearIntr(gCpuIntrf, (uint32_t)intNum);
    return;
}
/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
    uintptr_t status = CSL_armGicGlobalDisableInterrupt();

    return (status);
}

/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
   CSL_armGicGlobalRestoreInterrupt((uint32_t) restoreValue);
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(int32_t interruptNum, HwiP_Fxn hwiFxn,
                          HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle=NULL;

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

    /* Initialize User parameters */
    hwi_handle->gicParams.pUserParam = (void *) params->arg;
    hwi_handle->gicParams.pFnIntrHandler = (void *) hwiFxn;
    hwi_handle->gicParams.priority = params->priority;

    switch (params->triggerSensitivity)
    {
        case OSAL_ARM_GIC_TRIG_TYPE_FALLING_EDGE:
             hwi_handle->gicParams.triggerType = CSL_ARM_GIC_TRIG_TYPE_FALLING_EDGE;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_RISING_EDGE:
        case OSAL_ARM_GIC_TRIG_TYPE_EDGE:
             hwi_handle->gicParams.triggerType = CSL_ARM_GIC_TRIG_TYPE_RISING_EDGE;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_LOW_LEVEL:
        case OSAL_ARM_GIC_TRIG_TYPE_LEVEL:
             hwi_handle->gicParams.triggerType = CSL_ARM_GIC_TRIG_TYPE_LOW_LEVEL;
             break;
        case OSAL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL:
        default:
             hwi_handle->gicParams.triggerType = CSL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL;
             break;
    }

    /* Configure the interrupt Controller */
    CSL_armGicConfigIntr(gCpuIntrf, (uint32_t)interruptNum, &hwi_handle->gicParams);
    
    /* Return Non Zero value for the handle for A15 target */
    return ((HwiP_Handle) (hwi_handle) );

}

/* Below function deletes the hwi created */
HwiP_Status OsalArch_HwiPDelete(HwiP_Handle handle)
{
    CSL_Status  status = CSL_SOK;
    Hwi_Struct *hwi_hnd = (Hwi_Struct*) handle;
    uintptr_t   key;
    HwiP_Status ret_val;

    int32_t index = (int32_t)(hwi_hnd - hwiStructs); /* struct subtraction */

    /* mark that index as free */
    key = OsalArch_globalDisableInterrupt();
    hwiUsed[index] = FALSE;
    OsalArch_globalRestoreInterrupt(key);

    if (status == CSL_SOK)
    {
      ret_val = (HwiP_OK);
    }
    else 
    {
      ret_val = (HwiP_FAILURE);
    }
    return (ret_val);
}

/* Nothing past this point */