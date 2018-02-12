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

/* Local variables and structures */
static Hwi_Struct hwiStructs[OSAL_NONOS_CONFIGNUM_HWI];
static bool hwiUsed[OSAL_NONOS_CONFIGNUM_HWI] = {FALSE};
static bool gFirstTime = FALSE;
static CSL_IntcContext               gContext;
static CSL_IntcEventHandlerRecord    gEventRecord[OSAL_NONOS_CONFIGNUM_HWI];

/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    CSL_intcInterruptEnable((CSL_IntcVectId)intNum);
    return;
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    CSL_intcInterruptDisable((CSL_IntcVectId)intNum);
    return;
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    CSL_intcInterruptClear((CSL_IntcVectId)intNum);
    return;
}
/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
  return (uintptr_t)(_disable_interrupts());
}

/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
    _restore_interrupts(restoreValue);
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(int32_t interruptNum, HwiP_Fxn hwiFxn,
                          HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle=NULL;
    CSL_IntcParam                 vectId;

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

    if (gFirstTime == FALSE) {
      /* record the index in the handle */
      gContext.numEvtEntries      = OSAL_NONOS_CONFIGNUM_HWI;
      gContext.eventhandlerRecord = gEventRecord;
      CSL_intcInit(&gContext);
      gFirstTime = TRUE;
    }

    CSL_intcGlobalNmiEnable();
    CSL_intcGlobalEnable(NULL);

    vectId = (CSL_IntcParam)interruptNum;
    hwi_handle->handle = CSL_intcOpen (&hwi_handle->intcObj, params->evtId, &vectId, NULL);

    if(hwi_handle->handle != NULL)
    {
        CSL_IntcEventHandlerRecord  evtHandler;
        evtHandler.handler = (CSL_IntcEventHandler)hwiFxn;
        evtHandler.arg     = (void *) params->arg;

        CSL_intcPlugEventHandler(hwi_handle->handle, &evtHandler);
        CSL_intcHwControl(hwi_handle->handle,CSL_INTC_CMD_EVTCLEAR,NULL);
        CSL_intcHwControl(hwi_handle->handle,CSL_INTC_CMD_EVTENABLE,NULL);

        return ( (HwiP_Handle) (hwi_handle) );  /* _TMS320C6X */
    }
    else
    {
        return (NULL);
    }

}

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
    status = CSL_intcClose(hwi_hnd->handle);
    
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

/*
 *  ======== HwiP_getHandle ========
 *  Returns the HwiP handle associated with an interrupt number 
  */
HwiP_Handle OsalArch_getHandle(int32_t interruptNum)
{
   uint32_t i;
   Hwi_Struct *handle=NULL,*handle_temp;
   /* Fetch it if it is already allocated, else return NULL */
     /* Go through the list and find out if there is an HWI whose interrupt number matches */
     for(i=0;i<OSAL_NONOS_CONFIGNUM_HWI;i++) {
          if(hwiUsed[i]) {
              handle_temp=&hwiStructs[i];
              if(handle_temp->intcObj.vectId==interruptNum) {
                 handle=handle_temp;
                 break;
              }
          }
     }
  
    return((HwiP_Handle)handle);
}
/*
 *  ======== HwiP_getEventId ========
 *  Returns the Event ID associated with an interrupt
  */
int32_t OsalArch_getEventId(int32_t interruptNum)
{
   Hwi_Struct *handle;
   /* Find the handle registered to this interrupt number */
   handle=(Hwi_Struct *)OsalArch_getHandle(interruptNum);
   if(handle!=NULL) {
      return(handle->intcObj.eventId);
   } else {
    return(-1);
   }
   
}
/* Nothing past this point */
