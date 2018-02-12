/**
 *   @file  icss_emac_osal.h
 *
 *   @brief
 *      This is the sample OS Adaptation layer which is used by the ICSS_EMAC
 *      driver. The OSAL layer can be ported in either of the following
 *      manners to a native OS:
 *
 *      <b> Approach 1: </b>
 *      @n  Use Prebuilt Libraries
 *           - Ensure that the provide an implementation of all
 *             Osal_XXX API for their native OS.
 *           - Link the prebuilt libraries with their application.
 *           - Refer to the "example" directory for an example of this
 *       @n <b> Pros: </b>
 *           - Customers can reuse prebuilt TI provided libraries
 *       @n <b> Cons: </b>
 *           - Level of indirection in the API to get to the actual OS call
 *
 *      <b> Approach 2: </b>
 *      @n  Rebuilt Library
 *           - Create a copy of this file and modify it to directly
 *             inline the native OS calls
 *           - Rebuild the ICSS_EMAC Driver library; ensure that the Include
 *             path points to the directory where the copy of this file
 *             has been provided.
 *           - Please refer to the "test" directory for an example of this
 *       @n <b> Pros: </b>
 *           - Optimizations can be done to remove the level of indirection
 *       @n <b> Cons: </b>
 *           - ICSS_EMAC Libraries need to be rebuilt by the customer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2014 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/
#ifndef __ICSS_EMAC_OSAL_H__
#define __ICSS_EMAC_OSAL_H__

#ifndef _COMMON_OSAL_DATATYPES_
#define _COMMON_OSAL_DATATYPES_


#define OSAL_OK  0
#define OSAL_FAILURE -1
#define OSAL_TIMEOUT 2



typedef void (*TimerP_Fxn)(uintptr_t arg);

typedef  void *TimerP_Handle;


typedef struct TimerP_Params {
    char *name;
    uint32_t periodType;
    uint32_t freqLo;
    uint32_t freqHi;
    uint32_t startMode;
    uint32_t runMode;
    uint32_t period;
    void* arg;
} TimerP_Params;



/*!
 *  @brief    Status codes for SemaphoreP APIs
 */
typedef enum SemaphoreP_Status {
    /*! API completed successfully */
    SemaphoreP_OK = 0,
    /*! API failed */
    SemaphoreP_FAILURE = -1,
    /*! API failed because of a timeout */
    SemaphoreP_TIMEOUT = -2
} SemaphoreP_Status;

/*!
 *  @brief    Wait forever define
 */
#define SemaphoreP_WAIT_FOREVER  (~(0))

/*!
 *  @brief    No wait define
 */
#define SemaphoreP_NO_WAIT       (0)


/*!
 *  @brief    Opaque client reference to an instance of a SemaphoreP
 *
 *  A SemaphoreP_Handle returned from the SemaphoreP_create represents that
 *  instance and  is used in the other instance based functions (e.g.
 *  SemaphoreP_post or SemaphoreP_pend, etc.).
 */
typedef  void *SemaphoreP_Handle;

/*!
 *  @brief    Mode of the semaphore
 */
typedef enum SemaphoreP_Mode {
    SemaphoreP_Mode_COUNTING = 0x0,
    SemaphoreP_Mode_BINARY   = 0x1
} SemaphoreP_Mode;

/*!
 *  @brief    Basic SemaphoreP Parameters
 *
 *  Structure that contains the parameters are passed into SemaphoreP_create
 *  when creating a SemaphoreP instance. The SemaphoreP_Params_init function
 *  should be used to initialize the fields to default values before the
 *  application sets the fields manually. The SemaphoreP default parameters are
 *  noted in SemaphoreP_Params_init.
 */
typedef struct SemaphoreP_Params {
    char *name;           /*!< Name of the semaphore instance. Memory must
                               persist for the life of the semaphore instance */
    SemaphoreP_Mode mode; /*!< Mode for the semaphore */
    uint32_t maxCount;    /*!< The max count allowed for counting semaphore */
} SemaphoreP_Params;

/*!
 *  @brief    Opaque client reference to an instance of a HwiP
 *
 *  A HwiP_Handle returned from the HwiP_create represents that instance.
 */
typedef  void *HwiP_Handle;

/*!
 *  @brief    Status codes for HwiP APIs
 */
typedef enum HwiP_Status {
    HwiP_OK = 0,
    HwiP_FAILURE = -1
} HwiP_Status;


/*!
 *  @brief  Prototype for the entry function for a hardware interrupt
 */
typedef void (*HwiP_Fxn)(uintptr_t arg);

/*!
 *  @brief    Basic HwiP Parameters
 *
 *  Structure that contains the parameters passed into ::HwiP_create
 *  when creating a HwiP instance. The ::HwiP_Params_init function should
 *  be used to initialize the fields to default values before the application sets
 *  the fields manually. The HwiP default parameters are noted in
 *  HwiP_Params_init.
 */
typedef struct HwiP_Params {
    char      *name;      /*!< Name of the clock instance. Memory must
                               persist for the life of the clock instance.
                               This can be used for debugging purposes, or
                               set to NULL if not needed. */
    uintptr_t  arg;       /*!< Argument passed into the Hwi function. */
    uint32_t   priority;  /*!< Device specific priority. */
    uint32_t   evtId;     /*!< Event Id. */
} HwiP_Params;


#define OSAL_Assert(expr)  {                      \
                             if(expr)             \
                             {                    \
                                while(1);         \
                             }                    \
                           }

#endif /* _COMMON_OSAL_DATATYPES_ */
/**********************************************************************
 ************************* Extern Declarations ************************
 **********************************************************************/

/* #include <string.h> is here because there used to be
 * memcpy/memset prototypes here.  This #include prevents warnings in
 * other code that unintentionally worked because of these prototypes
 */
#include <string.h>

extern SemaphoreP_Handle SemaphoreP_create(unsigned int count,
                                           SemaphoreP_Params *params);
extern SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle);
extern void SemaphoreP_Params_init(SemaphoreP_Params *params);
extern SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle,
                                         uint32_t timeout);
extern SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle);

extern uintptr_t HwiP_disable(void);
extern void HwiP_restore(uint32_t key);

extern HwiP_Status HwiP_delete(HwiP_Handle handle);

extern void HwiP_disableInterrupt(int interruptNum);
extern void HwiP_enableInterrupt(int interruptNum);

extern HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn,
                               HwiP_Params *params);
extern void HwiP_Params_init(HwiP_Params *params);



#define ICSS_EMAC_osalCreateBlockingLock(x,y)        SemaphoreP_create(x,y)


#define ICSS_EMAC_osalDeleteBlockingLock(X)       SemaphoreP_delete(X)


#define ICSS_EMAC_osalSemParamsInit(x)  SemaphoreP_Params_init(x)


#define ICSS_EMAC_osalPendLock(X,Y)         SemaphoreP_pend(X,Y)


#define ICSS_EMAC_osalPostLock(X)     SemaphoreP_post(X)


#define ICSS_EMAC_osalHardwareIntDisable() HwiP_disable()


#define ICSS_EMAC_osalHardwareIntRestore(X) HwiP_restore(X)


#define ICSS_EMAC_osalHardwareIntDestruct(X)  HwiP_delete(X)


#define ICSS_EMAC_osalRegisterInterrupt(X,Y,Z)     HwiP_create(X,Y,Z)


#define ICSS_EMAC_osalHwiParamsInit(X)             HwiP_Params_init(X)

#endif /* __ICSS_EMAC_OSAL_H__ */


