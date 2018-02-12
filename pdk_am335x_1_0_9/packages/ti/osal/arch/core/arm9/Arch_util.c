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

/* Local hwi structures */
static Hwi_Struct hwiStructs[OSAL_NONOS_CONFIGNUM_HWI];
static bool hwiUsed[OSAL_NONOS_CONFIGNUM_HWI] = {FALSE};

/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    Intc_SystemEnable((unsigned int)intNum);
    return;
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    Intc_SystemDisable((uint32_t)intNum);
    return;
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    IntSystemStatusClear((uint32_t)intNum);
    return;
}
/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
    uintptr_t status = Intc_IntDisable();

    return (status);
}

/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
   Intc_IntEnable((uint32_t) restoreValue);
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(int32_t interruptNum, HwiP_Fxn hwiFxn,
                          HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle=NULL;

    int32_t i;
	uint32_t channelNo;
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

	Intc_Init();

	/* Enable the interrupt */
	OsalArch_globalRestoreInterrupt(0);

	/* Registering the Interrupt Service Routine(ISR). */
	Intc_IntRegister((uint32_t)interruptNum, (IntrFuncPtr) hwiFxn);

    /*Set the channel number for a system interrupt. Channel numbers 0-1
      are mapped to FIQ and Channel numbers 2-31 are mapped to IRQ of ARM.*/
    channelNo = ((params->priority > 31) || (params->priority < 2)) ? 0x12U : params->priority;
    IntChannelSet((uint32_t)interruptNum, channelNo);

	/* Enabling the UART interrupt in INTC. */
	OsalArch_enableInterrupt(interruptNum);

    /* Enable the Globle interrupt AINTC */
	IntMasterIRQEnable();
    IntGlobalEnable();
    IntIRQEnable();

	return ( (HwiP_Handle) (hwi_handle) );  /* __TI_ARM_V5__ */

}

/* Below function deletes the hwi created */
HwiP_Status OsalArch_HwiPDelete(HwiP_Handle handle)
{
    CSL_Status  status = CSL_SOK;
    Hwi_Struct *hwi_hnd = (Hwi_Struct*) handle;
    uintptr_t   key;
    HwiP_Status ret_val;

    int index = (hwi_hnd - hwiStructs); /* struct subtraction */

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
