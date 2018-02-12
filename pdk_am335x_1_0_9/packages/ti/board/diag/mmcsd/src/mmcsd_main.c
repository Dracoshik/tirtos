/**
 *  \file   mmcsd_main.c
 *
 *  \brief  Test application main file. The test application demonstrates
 *          raw data write & read on a MMCSD device.
 *
 */

/*
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
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


/* TI-RTOS Header files */
#include <ti/csl/cslr_device.h>
#include <ti/drv/mmcsd/MMCSD.h>

/* UART Header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/board/board.h>

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/

/* Global data pointers */
#define HSMMCSD_DATA_SIZE               512

#define PAGE_OFFSET   0x0
#define MMCSTARTSECTOR  0x300000 //@1.5GB //100
#define MMCTESTSECTORS  0x20 //0x800 //0x5000  //10MB //512

#define DATA_PATTERN_00     0
#define DATA_PATTERN_FF     1
#define SDMMC_DATA_PATTERN_AA 2
#define SDMMC_DATA_PATTERN_55 3
#define SDMMC_DATA_PATTERN_INC  8

#define SECTORSIZE    (512)
#define TESTSECTORS    16 //16 //1

/* Enable the below macro to have prints on the IO Console */
//#define IO_CONSOLE

#ifndef IO_CONSOLE
#define MMCSD_log                UART_printf
#else
#define MMCSD_log                printf
#endif

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/

static int32_t HSMMCSDReadWriteTest(MMCSD_Handle handle);

static int32_t fillMmcPageData(uint8_t *buf, int32_t length, uint8_t flag);

/* Delay function */
static void delay(unsigned int delayValue);

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

MMCSD_Handle handle = NULL;
static uint8_t tx[SECTORSIZE*TESTSECTORS];
static uint8_t rx[SECTORSIZE*TESTSECTORS];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
    UART_printf("\n*********************************************\n"); 
    UART_printf  ("*                MMCSD Test                 *\n");
    UART_printf  ("*********************************************\n");

    MMCSD_init();

    MMCSD_open(0U, NULL, &handle);

    return HSMMCSDReadWriteTest(handle);
}

int32_t fillMmcPageData(uint8_t *buf, int32_t length, uint8_t flag)
{
    int32_t i;
    uint32_t data = 0x00;
    uint8_t incFlag = 0x00;

    switch(flag)
    {
        case DATA_PATTERN_00:
            data = 0;
            break;
        case DATA_PATTERN_FF:
            data = 0xFF;
            break;
        case SDMMC_DATA_PATTERN_AA:
            data = 0xAA;
            //MMCSD_log("\nSD Read/Write pattern 0xAA ");
            break;
        case SDMMC_DATA_PATTERN_55:
            data = 0x55;
            //MMCSD_log("\nSD Read/Write pattern 0x55 ");
            break;
        case 4:
             data = 0x00;
             incFlag = 0x01;
             //MMCSD_log("\nSD Read/Write incremental data");
             break;
		default:
            return -1;
    }

    for(i =0; i < length; i++)
    {
        *(buf+i) = data;
        if(incFlag)
            data++;
    }



    return 0;
}

int32_t HSMMCSDReadWriteTest(MMCSD_Handle handle)
{
  int32_t j, testIteration;
  unsigned long int blockNum;
  int32_t retVal = 0;

  for(blockNum = MMCSTARTSECTOR; blockNum < (MMCTESTSECTORS+MMCSTARTSECTOR); blockNum+=TESTSECTORS)
  {
    testIteration = SDMMC_DATA_PATTERN_INC;

    fillMmcPageData(&tx[0], (SECTORSIZE*TESTSECTORS), testIteration);

    retVal = MMCSD_write(handle, &tx[0], blockNum, TESTSECTORS);

    if(retVal != MMCSD_OK)
    {
		MMCSD_log("FAIL: Write failed on sector %d", blockNum);
        return -1;
    }

    delay(100);

    /* Clear receive buffer */
    fillMmcPageData(&rx[0], (SECTORSIZE*TESTSECTORS), DATA_PATTERN_00);

    /* Read a page */
    retVal = MMCSD_read(handle, &rx[0], blockNum, TESTSECTORS);

    if(retVal != MMCSD_OK)
    {
        MMCSD_log("FAIL: Read failed on sector %d", blockNum);
        return -1;
    }

    /* Check the pattern */
    for ( j = 0 ; j < (SECTORSIZE*TESTSECTORS) ; j++ )
    {
		/* Compare the data read against the expected */
		if ( tx[j] != rx[j])
		{
			MMCSD_log ("FAIL: Data comparison failed @ ");
			MMCSD_log("sector %d", blockNum);
			MMCSD_log(" ,data %d", j);
			MMCSD_log(" ,rx = %x", rx[j]);
			MMCSD_log(" ,tx = %x", tx[j]);
			return -1;
		}

	}
  }

  MMCSD_log ("\nPASS: Read/Write Success for this pattern\r\n");

  return 0;
}

/*
 *  ======== Delay function ========
 */
static void delay(unsigned int delayValue)
{
    volatile uint32_t delay1 = delayValue*10000;
    while (delay1--) ;
}
