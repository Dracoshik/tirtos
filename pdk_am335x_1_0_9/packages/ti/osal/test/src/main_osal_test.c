/**
 *  \file   main_osal_test.c
 *
 *  \brief  OSAL driver test application main file.
 *
 */

/*
 * Copyright (C) 2016 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BARE_METAL
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#endif

#include <stdio.h>
#include <string.h>

/* TI-RTOS Header files */
#include <ti/osal/osal.h>
#include "OSAL_log.h"
#include "OSAL_board.h"

/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/
#define TWO_TIMER_INTERRUPT_TEST 0

#ifdef BARE_METAL
#include <ti/csl/soc.h>
#include <ti/csl/csl_timer.h>
#include <ti/csl/arch/csl_arch.h>

#if   defined (SOC_AM571x) || defined (SOC_AM572x) || defined (SOC_K2G) || defined (SOC_AM574x)
#undef  TWO_TIMER_INTERRUPT_TEST
#define TWO_TIMER_INTERRUPT_TEST 1
#endif

#else
void ErrorHandler(Error_Block *eb)
{
    System_printf("ErrorHandler: ");
    Error_print(eb);
}
#endif


/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
#if defined (__TI_ARM_V5__)
TimerP_Handle handle;
#endif

/*
 *  ======== Board_initOSAL ========
 */
void Board_initOSAL(void)
{
    Board_initCfg boardCfg;

#if defined(EVM_K2E) || defined(EVM_C6678)
    boardCfg = BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);
}

/*
 *  ======== HWI test function ========
 */
bool OSAL_hwi_test()
{
    return true;
}

typedef enum UT_Timer_Type_s {
    UT_Timer_DMTIMER = 0,
    UT_Timer_TIMER64 = 1
} UT_Timer_Type_t;


#if defined(SOC_AM335x)
#define CM_PER_TIMER3_CLKCTRL             (0x44e00084)
#define CM_DPLL_CLKSEL_TIMER3_CLK         (0x44e0050c)
UT_Timer_Type_t  timer_type    =          UT_Timer_DMTIMER;
#define OSAL_TEST_TIMER_ID                (1U)
#define OSAL_TEST_TIMER_PERIOD            (5000U)
#elif defined(SOC_AM437x)
#define CM_PER_TIMER3_CLKCTRL             (0x44DF8800 + 0x538)
#define CM_DPLL_CLKSEL_TIMER3_CLK         (0x44DF4200 + 0x8)
UT_Timer_Type_t  timer_type  =            UT_Timer_DMTIMER;
#define OSAL_TEST_TIMER_ID                (3U)
#define OSAL_TEST_TIMER_PERIOD            (5000U)

#elif defined(SOC_K2H) || defined(SOC_K2K)
UT_Timer_Type_t  timer_type =             UT_Timer_TIMER64;
#define OSAL_TEST_TIMER_ID                (8U)
#define OSAL_TEST_TIMER_PERIOD            (5000U)
#elif defined(SOC_K2G)
UT_Timer_Type_t  timer_type =             UT_Timer_TIMER64;
#define OSAL_TEST_TIMER_ID                (1U)
#define OSAL_TEST_TIMER_ID2               (2U)
#define OSAL_TEST_TIMER_PERIOD            (5000U)
#elif defined(SOC_AM572x) || defined (SOC_AM574x)
UT_Timer_Type_t  timer_type   =           UT_Timer_DMTIMER;
  #if defined (__ARM_ARCH_7A__)
    #define OSAL_TEST_TIMER_ID                (1U)
    #define OSAL_TEST_TIMER_ID2               (9U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #elif   defined (_TMS320C6X)
    #define OSAL_TEST_TIMER_ID                (4U)
    #define OSAL_TEST_TIMER_ID2               (5U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #else
    #define OSAL_TEST_TIMER_ID                (8U)
    #define OSAL_TEST_TIMER_ID2               (10U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #endif
#elif defined(SOC_AM571x)
UT_Timer_Type_t  timer_type   =           UT_Timer_DMTIMER;
  #if defined (__ARM_ARCH_7A__)
    #define OSAL_TEST_TIMER_ID                (1U)
    #define OSAL_TEST_TIMER_ID2               (9U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #elif   defined (_TMS320C6X)
    #define OSAL_TEST_TIMER_ID                (4U)
    #define OSAL_TEST_TIMER_ID2               (5U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #else
    #define OSAL_TEST_TIMER_ID                (8U)
    #define OSAL_TEST_TIMER_ID2               (10U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #endif
#elif defined(SOC_OMAPL137) || defined (SOC_OMAPL138)
UT_Timer_Type_t  timer_type =             UT_Timer_TIMER64;
#define OSAL_TEST_TIMER_ID                (1U)
#define OSAL_TEST_TIMER_PERIOD            (5000U)

#else
UT_Timer_Type_t  timer_type   =           UT_Timer_DMTIMER;
  #if defined (__ARM_ARCH_7A__)
    #define OSAL_TEST_TIMER_ID                (1U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #elif   defined (_TMS320C6X)
    #define OSAL_TEST_TIMER_ID                (4U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #else
    #define OSAL_TEST_TIMER_ID                (8U)
    #define OSAL_TEST_TIMER_PERIOD            (5000U)
  #endif
#endif

volatile uint32_t timerIsrCount = 0;
volatile uint32_t timerIsr2Count = 0;

void timerIsr(void *arg)
{
#if defined(BARE_METAL) && defined(__TI_ARM_V5__)
    TimerP_ClearInterrupt(handle);
#endif
    timerIsrCount++;
}


#if    TWO_TIMER_INTERRUPT_TEST
void timerIsr2(void *arg)
{
    timerIsr2Count++;
}
#endif

/*
 * ========= Osal Delay Test function =========
 */
bool Osal_delay_test(void)
{
   int32_t i;
  /* This test assumes that Board init has been already
   * called outside this function
   */

   /* Notice a '.' on terminal for every 1 second */
   for (i=0; i<10;i++) {
      Osal_delay(1000);
      OSAL_log(".");
   }
   OSAL_log("\n");

  return(true);
}

/*
 *  ======== Timer test function ========
 */
bool OSAL_timer_test()
{
    TimerP_Params timerParams;
#if !defined (__TI_ARM_V5__)
    /* Timer handle is defined as global for OMAPL13x ARM9 core since
       it is required for clearing the interrupt from ISR */
    TimerP_Handle handle;
#endif

#if    TWO_TIMER_INTERRUPT_TEST
    TimerP_Handle handle2;
#endif
    TimerP_Status timerStatus;
    Osal_HwAttrs  hwAttrs;

#ifdef BARE_METAL
    int32_t       id    = OSAL_TEST_TIMER_ID;
#else
    int32_t       id    = TimerP_ANY;
#endif
    volatile      int32_t i;
    uint32_t      prevCount;
    bool          ret = true;
    int32_t       osal_ret;

#if (defined (SOC_AM437x) || defined (SOC_AM335x))
    *(unsigned int*)CM_DPLL_CLKSEL_TIMER3_CLK = 0x1; /* high frequency input clock */

    // enable the TIMER
    *(unsigned int*)CM_PER_TIMER3_CLKCTRL = 0x2; /* Module is explicitly enabled */
#endif

    /* Set the timer base for the osal delay */
    Osal_getHwAttrs(&hwAttrs);

#if defined (SOC_AM437x)
    hwAttrs.osalDelayTimerBaseAddr = (uintptr_t) 0x48040000U; /* DMTIMER2 */
#endif

    /* This API should return osal_OK for AM3/AM4 and return unsupported for other Socs */
    osal_ret = Osal_setHwAttrs(OSAL_HWATTR_SET_OSALDELAY_TIMER_BASE, &hwAttrs);

#if defined (SOC_AM437x) || defined (SOC_AM335x)
    if (osal_ret == osal_OK) {
      ret = true;
    }
    else {
      ret = false;
    }
#else
    if ((osal_ret == osal_UNSUPPORTED) ||
       (osal_ret == osal_OK)) {
      ret = true;
    }
    else {
      ret = false;
    }
#endif /* (SOC_AM437x) || defined (SOC_AM335x) */

    TimerP_Params_init(&timerParams);
    timerParams.runMode    = TimerP_RunMode_CONTINUOUS;
    timerParams.startMode  = TimerP_StartMode_USER;
    timerParams.periodType = TimerP_PeriodType_COUNTS;
    timerParams.period     = OSAL_TEST_TIMER_PERIOD;

#if defined (_TMS320C6X)
    timerParams.intNum     = 15;
#endif

    if (timer_type == UT_Timer_TIMER64)
    {
      timerParams.timerMode = TimerP_Timer64Mode_UNCHAINED;
      timerParams.timerHalf = TimerP_Timer64Half_LOWER;
    }
    handle = TimerP_create(id, (TimerP_Fxn)&timerIsr, &timerParams);

    /* don't expect the handle to be null */
    if (handle == NULL)
    {
      OSAL_log("\n Timer Create error \n");
      ret = false;
    }

#if    TWO_TIMER_INTERRUPT_TEST

#if defined (_TMS320C6X)
    timerParams.intNum     = 14;
#endif

    if (ret == true)  {
      handle2 = TimerP_create(OSAL_TEST_TIMER_ID2, (TimerP_Fxn)&timerIsr2, &timerParams);
      /* don't expect the handle to be null */
      if (handle2 == NULL)
      {
        OSAL_log("\n Timer Create error for handle2 \n");
        ret = false;
      }
    }
#endif

    if (ret == true)
    {
      timerIsrCount = 0U;
      timerStatus = TimerP_start(handle);

      if (timerStatus != TimerP_OK) {
        OSAL_log("Err: Coult not start the timer %d \n", id);
        ret = false;
      }

#if    TWO_TIMER_INTERRUPT_TEST
      timerIsr2Count = 0U;
      timerStatus = TimerP_start(handle2);

      if (timerStatus != TimerP_OK) {
        OSAL_log("Err: Coult not start the timer %d \n", id + 1);
        ret = false;
      }

      while (1)
      {
        if (timerIsr2Count >= 100U) {
          timerStatus = TimerP_stop(handle2);
          if (timerStatus != TimerP_OK) {
            OSAL_log("Err: Coult not stop the timer %d \n", id);
            ret = false;
          }
          break;
        }
      }
#endif

      while (1)
      {
        if (timerIsrCount >= 100U) {
          timerStatus = TimerP_stop(handle);
          if (timerStatus != TimerP_OK) {
            OSAL_log("Err: Coult not stop the timer %d \n", id);
            ret = false;
          }

          break;
        }
      }

      /* Latch the ISR count after the timer stops */
      prevCount = (timerIsrCount);

      /* Give some time for all pending interrupts to be processed */
      for (i = 0; i < 100000; i++);

      /* Now check if timer ISR gets kicked in
       * timer ISR should not be kicked in */
      if (ret == true)
      {
         for (i = 0; i < 50000; i++);

         if (prevCount != timerIsrCount)
         {
           OSAL_log(" Error: Timer appears to be not stopped via OSAL calls, val1 = %u and val2 = %u \n ", prevCount, timerIsrCount);
           ret = false;
         }
         else
         {
           OSAL_log("  TimerTestLog: got %u ticks from timer id = %d \n", timerIsrCount, id);
#if    TWO_TIMER_INTERRUPT_TEST
           OSAL_log("  TimerTestLog: got %u ticks from timer id = %d \n", timerIsr2Count, id+1);
#endif
         }
       }
    }

    if (handle != NULL)
    {
      if (TimerP_delete(handle) != TimerP_OK)
      {
        ret = false;
      }
    }
    return (ret);
}

/*
 *  ======== Clock test function ========
 */
bool OSAL_clock_test()
{
    return true;
}

#ifdef MANUAL_CACHE_TEST

#if defined (__ARM_ARCH_7A__)
#include <ti/csl/csl_a15.h>
#include <ti/csl/csl_armGicAux.h>
#elif defined (_TMS320C6X)
#include <ti/csl/csl_cacheAux.h>
#endif

#if defined (SOC_AM437x) || defined(SOC_AM335x)
#include <ti/starterware/include/chipdb.h>
#include <ti/starterware/include/armv7a/pl310.h>
#include <ti/starterware/include/armv7a/pl310_config.h>
#endif

#define BUF_LEN 256
#define LOCAL_CPU_WRITE_PATTERN_1 0xA1A1A1A1
#define LOCAL_CPU_WRITE_PATTERN_2 0xA2A2A2A2
#define LOCAL_CPU_WRITE_PATTERN_3 0xA3A3A3A3

#define ALTERNATE_CPU_WRITE_PATTERN_1 0xB1B1B1B1
#define ALTERNATE_CPU_WRITE_PATTERN_2 0xB2B2B2B2
#define ALTERNATE_CPU_WRITE_PATTERN_3 0xB3B3B3B3

#if defined (__ARM_ARCH_7A__)
uint32_t ddr_buf[BUF_LEN] __attribute__((aligned(256))); // GCC way of aligning
#elif defined (_TMS320C6X)
#pragma DATA_SECTION(ddr_buf,".cachetest_ddrbuf")
#pragma DATA_ALIGN(ddr_buf,256)
uint32_t ddr_buf[BUF_LEN];
#elif defined(__TI_ARM_V7M4__)
uint32_t ddr_buf[BUF_LEN];
#endif
void OSAL_cache_test_fill_buf(uint32_t *buf, int len,uint32_t pattern) {
  int i;
  for(i=0;i<len;i++) {
     buf[i]=pattern;
  }
}
/* Returns FALSE if all the buffer is not 'pattern'. This will read the contents though */
bool OSAL_cache_test_check_buf(uint32_t *buf, int len,uint32_t pattern) {
  int i;
  bool match=TRUE;
  for(i=0;i<len;i++) {
     if(buf[i]!=pattern) {
       match=FALSE;
     }
  }
  return(match);
}

#if defined (SOC_AM572x) || defined (SOC_AM574x)
bool OSAL_cache_test()
{

  bool match,res=TRUE;
#if defined (__ARM_ARCH_7A__)
  CSL_a15EnableCache();
#elif defined(_TMS320C6X)
    /* Enable for most of the DDR3 memory , especially the first two MAR regions where ddr_buf will reside */
    CACHE_enableCaching(128);
    CACHE_enableCaching(129);
    CACHE_setL1DSize(CACHE_L1_4KCACHE);
    CACHE_setL2Size(CACHE_1024KCACHE);
#endif
  OSAL_log("This manual cache test is a CCS based test. Please do the following\n");
  OSAL_log("1) On the local CPU open two memory windows for the address 0x%x\n: One CPU view and One Physical memory view\n",&ddr_buf[0]);
  OSAL_log("2) Connect an alternate CPU (for example if local cpu is A15, open a c66/m4 as the alternate CPU\n");
  OSAL_log("3) On the alternate CPU ( preferably a different window through (CCS->New Window)) Open the address 0x%x\n with a physical memory view\n",&ddr_buf[0]);

  OSAL_log("\n**** Forcing a read to get the buffer cached() ******\n");
  OSAL_cache_test_check_buf(ddr_buf,BUF_LEN,0); /* Checking for dummy pattern, just to force the cache read */

  OSAL_log("\n**** Testing Cache_Invalidate() ******\n");

  OSAL_log("\nWriting  0x%x local ddr memory\n",LOCAL_CPU_WRITE_PATTERN_1);
    /************************ TEST for CacheP_Inv() *********************/
    /* A15- Write LOCAL_CPU_WRITE_PATTERN_1 in to ddr_buf[0] */
    OSAL_cache_test_fill_buf(ddr_buf,BUF_LEN,LOCAL_CPU_WRITE_PATTERN_1);

    /* Check in the alternate cpu window (m4/c66x/a15) if it can see it (Physical memory). It shouldnt if cache is enabled. */
    OSAL_log("\nCheck in the alternate cpu window (m4/c66x/a15) if it can see it (Physical memory). It shouldn't appear as it is still in cache\n");

    OSAL_log("\nInvalidating cache.\n");
    /* Now OSAL_CacheInv() - Invalidates and writes back cache() */
    CacheP_Inv(ddr_buf,BUF_LEN*sizeof(uint32_t));

    OSAL_log("\nCheck in alternate cpu window (m4/c66x/a15) if it can see it (Physical memory). For A15 it should (Invalidate also writes back for A15), for others it should not \n");

    OSAL_log("\nNow write(%d 32-bit words) of pattern 0x%x on the alternate CPU physical memory window (a15/m4/c66x) \n",BUF_LEN,ALTERNATE_CPU_WRITE_PATTERN_1);
    OSAL_log("\nCheck if this pattern 0x%x has made it to the physical memory view of this cpu.It should. \n",ALTERNATE_CPU_WRITE_PATTERN_1);

#if defined(__ARM_ARCH_7A__)
    /* The below invalidate test is to account for ARM prefetching. On A15 Cache Inv writes back too. To see any changes AFTER the DDR was
    externally modified, a Cache Invalidate is required */
    CacheP_Inv(ddr_buf,BUF_LEN*sizeof(uint32_t));
#endif
    OSAL_log("\nChecking if local CPU read fetches the 0x%x value updated by the alternate cpu core\n",ALTERNATE_CPU_WRITE_PATTERN_1);
    /* Now read the value on A15 now */
    match=OSAL_cache_test_check_buf(ddr_buf,BUF_LEN,ALTERNATE_CPU_WRITE_PATTERN_1);
    if(match==TRUE) {
        OSAL_log("\nPattern matched. OSAL_CacheInv() Test passed\n");
    } else {
        OSAL_log("\n OSAL_CacheInv() Test failed\n");
    }

    /************************ TEST for CacheP_wb() *********************/
    OSAL_log("\n**** Testing Cache_WriteBack() ******\n");

    OSAL_log("\nWriting  0x%x local ddr memory\n",LOCAL_CPU_WRITE_PATTERN_2);
    /* local cpu- Write LOCAL_CPU_WRITE_PATTERN_2 in to ddr_buf[0] */
    OSAL_cache_test_fill_buf(ddr_buf,BUF_LEN,LOCAL_CPU_WRITE_PATTERN_2);

    OSAL_log("\nCheck in the alternate cpu window (m4/c66x/a15) if the pattern 0x%x can be seen it (Physical memory). It shouldn't appear as it is still in cache\n",LOCAL_CPU_WRITE_PATTERN_2);

    OSAL_log("\nWriting back cache.\n");
    /* Now OSAL_CacheWb() - Writes back cache() */
    CacheP_wb(ddr_buf,BUF_LEN*sizeof(uint32_t));

    /* Check in DSP window if it can see it (Physical memory). It should now. */
    OSAL_log("\nCheck in alternate cpu window (m4/c66x/a15) and verify that the 0x%x is seen in Physical memory\n",LOCAL_CPU_WRITE_PATTERN_2);

    /* If so, write back portion is tested. */
    OSAL_log("\nNow write the pattern 0x%x on the alternate CPU physical memory window (a15/m4/c66x) \n",ALTERNATE_CPU_WRITE_PATTERN_2);
    /* Since, on this local cpu, we just wrote back cache() only, and not invalidated, the ALTERNATE_CPU_WRITE_PATTERN_2 wont be seen in the local cpu's cached memory
       Read Variable on A15 to ensure CPU Read(). The value should be the old LOCAL_CPU_WRITE_PATTERN_2.. and NOT the ALTERNATE_CPU_WRITE_PATTERN_2.. which the alternate CPU wrote. */
    match=OSAL_cache_test_check_buf(ddr_buf,BUF_LEN,LOCAL_CPU_WRITE_PATTERN_2);
    if(match==TRUE) {
        OSAL_log("\nPattern matched. OSAL_Cache_Writeback() Test passed\n");
     } else {
        OSAL_log("\n OSAL_Cache_Writeback() Test failed\n");
     }

    /************************ TEST for CacheP_wbInv() *********************/
     OSAL_log("\n**** Testing Cache_WriteBack_Invalidate() ******\n");

     OSAL_log("\nWriting  0x%x local ddr memory\n",LOCAL_CPU_WRITE_PATTERN_3);

     /* local cpu- Write LOCAL_CPU_WRITE_PATTERN_3 in to ddr_buf[0] */
     OSAL_cache_test_fill_buf(ddr_buf,BUF_LEN,LOCAL_CPU_WRITE_PATTERN_3);

     OSAL_log("\nCheck in the alternate cpu window (m4/c66x/a15) if the pattern 0x%x can be seen it (Physical memory). It shouldn't appear as it is still in cache\n",LOCAL_CPU_WRITE_PATTERN_3);

     OSAL_log("\nWriting back cache and invalidating it.\n");

     /* Now OSAL_CacheWbInv() - Writes back cache and invalidate() */
      CacheP_wbInv(ddr_buf,BUF_LEN*sizeof(uint32_t));

      /* Check in DSP window if it can see it (Physical memory). It should now. */
      OSAL_log("\nCheck in alternate cpu window (m4/c66x/a15) and verify that the 0x%x is seen in Physical memory. If not it is a failure \n",LOCAL_CPU_WRITE_PATTERN_3);


      OSAL_log("\nNow write the pattern 0x%x on the alternate CPU physical memory window (a15/m4/c66x) \n",ALTERNATE_CPU_WRITE_PATTERN_3);

#if defined(__ARM_ARCH_7A__)
    /* This is due to ARM prefetching */
     CacheP_Inv(ddr_buf,BUF_LEN*sizeof(uint32_t));
#endif

    /* Since, on local cpu, we not only wrote back cache but also invalidated it,the local cpu should be able the alternate pattern now */
    match=OSAL_cache_test_check_buf(ddr_buf,BUF_LEN,ALTERNATE_CPU_WRITE_PATTERN_3);
    if(match==TRUE) {
        OSAL_log("\nPattern matched. OSAL_Cache_WritebackInvalidate() Test passed\n");
     } else {
        OSAL_log("\nOSAL_Cache_WritebackInvalidate() Test failed\n");
     }

     return(res);
}
#endif /* SOC_AM572x || SOC_AM574x */
#endif /* MANUAL_CACHE_TEST */
/*
 *  ======== Semaphore test function ========
 */
bool OSAL_semaphore_test()
{
    SemaphoreP_Params semParams;
    SemaphoreP_Handle handle1, handle2;

    SemaphoreP_Params_init(&semParams);

    /* Test 1: counting mode, no timeout */
    semParams.mode = SemaphoreP_Mode_COUNTING;
    handle1 = SemaphoreP_create(0, &semParams);
    if (handle1 == NULL)
    {
        return false;
    }
    if (SemaphoreP_post(handle1) != SemaphoreP_OK)
    {
        return false;
    }
    if (SemaphoreP_pend(handle1, 0) != SemaphoreP_OK)
    {
        return false;
    }

    /* Test 2: binary mode, with timeout */
    semParams.mode = SemaphoreP_Mode_BINARY;
    handle2 = SemaphoreP_create(1, &semParams);
    if (handle2 == NULL)
    {
        return false;
    }
    if (SemaphoreP_pend(handle2, 0) != SemaphoreP_OK)
    {
        return false;
    }
    if (SemaphoreP_pend(handle2, 10) != SemaphoreP_TIMEOUT)
    {
        return false;
    }

    if (SemaphoreP_delete(handle1) != SemaphoreP_OK)
    {
        return false;
    }

    if (SemaphoreP_delete(handle2) != SemaphoreP_OK)
    {
        return false;
    }

    return true;
}

/*
 *  ======== main test function ========
 */
#ifdef BARE_METAL
void osal_test()
#else
void osal_test(UArg arg0, UArg arg1)
#endif
{
    bool testFail = false;
    Osal_StaticMemStatus pMemStats;

    if(OSAL_hwi_test() == true)
    {
        OSAL_log("\n HWI tests have passed. \n");
    }
    else
    {
        OSAL_log("\n HWI tests have failed. \n");
        testFail = true;
    }

#ifdef ENABLE_TIMER_TEST
    if(OSAL_timer_test() == true)
    {
        OSAL_log("\n Timer tests have passed. \n");
    }
    else
    {
        OSAL_log("\n Timer tests have failed. \n");
        testFail = true;
    }
#endif

/* The test does not support below SoCs */
#if !defined(SOC_TDA2XX) && !defined(SOC_TDA2PX) && !defined(SOC_TDA2EX) && !defined(SOC_TDA3XX)
  OSAL_log("\n Running Osal_Delay test:");

  if(Osal_delay_test() == true)
  {
      OSAL_log("\n Osal_Delay  tests have passed. \n");
  }
  else
  {
      OSAL_log("\n Osal_Delay  tests have failed. \n");
      testFail = true;
  }
#endif

    if(OSAL_clock_test() == true)
    {
        OSAL_log("\n Clock tests have passed. \n");
    }
    else
    {
        OSAL_log("\n Clock tests have failed. \n");
        testFail = true;
    }

    if(OSAL_semaphore_test() == true)
    {
        OSAL_log("\n Semaphore tests have passed. \n");
    }
    else
    {
        OSAL_log("\n Semaphore tests have failed. \n");
        testFail = true;
    }
    #ifdef MANUAL_CACHE_TEST
    /* This test is valid only for MANUAL testing */
    OSAL_cache_test();
    #endif

    /* Now print the static memory statistics */
    if(Osal_getStaticMemStatus(&pMemStats) == osal_OK)
    {
       OSAL_log("\n Semaphore Statistics:     \
                 \n  PeakSemObjs    = %u,     \
                 \n  numMaxSemObjs  = %u,     \
                 \n  numFreeSemObjs = %u \n", \
                 pMemStats.peakSemObjs,       \
                 pMemStats.numMaxSemObjs,     \
                 pMemStats.numFreeSemObjs);

       OSAL_log("\n Hwi Statistics:           \
                 \n  PeakHwiObjs    = %u,     \
                 \n  numMaxHwiObjs  = %u,     \
                 \n  numFreeHwiObjs = %u \n", \
                 pMemStats.peakHwiObjs,       \
                 pMemStats.numMaxHwiObjs,     \
                 pMemStats.numFreeHwiObjs);

       OSAL_log("\n Timer Statistics:           \
                 \n  PeakTimerObjs    = %u,     \
                 \n  numMaxTimerObjs  = %u,     \
                 \n  numFreeTimerObjs = %u \n", \
                 pMemStats.peakTimerObjs,       \
                 pMemStats.numMaxTimerObjs,     \
                 pMemStats.numFreeTimerObjs);

    }
    else
    {
      testFail = true;
      OSAL_log("\n Memory Statistics query failed \n");
    }

    if(testFail == true)
    {
        OSAL_log("\n Some tests have failed. \n");
    }
    else
    {
        OSAL_log("\n All tests have passed. \n");
    }

#ifdef BARE_METAL
    while (1)
    {
    }
#endif

}

/*
 *  ======== main ========
 */
int main(void)
{
    Board_initOSAL();

#ifdef BARE_METAL
    osal_test();
#else
    /* Start BIOS */
    BIOS_start();
#endif
    return (0);
}

