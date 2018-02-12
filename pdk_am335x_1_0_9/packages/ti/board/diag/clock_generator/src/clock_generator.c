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
 *
 */

/**
 *
 * \file  clock_generator.c
 *
 * \brief This file contains clock generator test functions.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/drv/uart/UART_stdio.h>

#ifdef SOC_K2G
#include "diag_common_cfg.h"
#endif

#include "board.h"
#include "board_cfg.h"

extern const I2C_Config I2C_config[];
extern void BOARD_delay(uint32_t usecs);

/**
 *  \brief    This function executes clock generator detection test
 *
 *  \param    void   
 *
 * \return
 * \n         TEST_PASS  - Test Passed
 * \n         TEST_FAIL  - Test Failed
 */
static TEST_STATUS clock_generator_detect_test(void)
{
	uint32_t index;
	I2C_Params i2cParams;
    I2C_Handle handle = NULL;
	uint8_t tx;
	uint8_t rx;
	I2C_Transaction i2cTransaction;
	
	for (index=0; I2C_config[index].fxnTablePtr != NULL; index++)
	{
		((I2C_HwAttrs *)I2C_config[index].hwAttrs)->enableIntr = false;
	}
	
	I2C_init();

    I2C_Params_init(&i2cParams);

    handle = I2C_open(BOARD_CLOCK_GENERATOR_INSTANCE, &i2cParams);
	if(handle == NULL)
	{
		UART_printf("I2C Handle open failed");
		return(TEST_FAIL);
	}

	I2C_transactionInit(&i2cTransaction);
	i2cTransaction.slaveAddress = BOARD_I2C_CLOCK_GENERATOR;
	i2cTransaction.writeBuf = &tx;
	i2cTransaction.writeCount = 1;
	i2cTransaction.readBuf = &rx;
	i2cTransaction.readCount = 0;
	tx = 0x0;
	rx = 0x0;
	I2C_transfer(handle, &i2cTransaction);
	BOARD_delay(1000);
	i2cTransaction.writeCount = 0;
	i2cTransaction.readCount = 1;
	
	I2C_transfer(handle, &i2cTransaction);
	if(rx == 0x0)
    {
        UART_printf ("Clock generator Detection Failed!\n");
        return(TEST_FAIL);
    }

    UART_printf ("\nClock generator Detection Successful!\n");

    return (TEST_PASS);
}

/**
 *  \brief    This function executes clock generator probe test
 *
 *  \param    void
 *
 * \return
 * \n         TEST_PASS  - Test Passed
 * \n         TEST_FAIL  - Test Failed
 */
static TEST_STATUS clock_generator_probe_test(void)
{
	TEST_STATUS testStatus;
	int8_t op;

	UART_printf("Probe the clock generator clock outputs\n");
	UART_printf("Are the signals generated properly ? \n");
	UART_printf("Press 'Y' to confirm, any other key to deny \n");
	UART_scanFmt("%c",&op);
	if(op == 'Y' || op == 'y')
	{
		testStatus = TEST_PASS;
	}
	else
	{
		testStatus = TEST_FAIL;
	}
	return(testStatus);
}

/**
 *  \brief    This function is used to perform Clock generator detection
 *            test and Clock generator probe test
 *
 *  \param    void
 *
 * \return
 * \n         TEST_PASS  - Test Passed
 * \n         TEST_FAIL  - Test Failed
 */
static TEST_STATUS run_clock_generator_test(void)
{
	TEST_STATUS testStatus;

    UART_printf("\nRunning Clock generator Detect Test\n");

    testStatus = clock_generator_detect_test();
    if(testStatus != TEST_PASS)
	{
		UART_printf("\nClock generator Detect Test Failed!!\n");
		return (testStatus);
	}
	else
	{
		UART_printf("\nClock generator Detect Test Passed!\n");
	}
	
    UART_printf("\n\nRunning Clock generator probe Test\n\n");

    testStatus = clock_generator_probe_test();
    if(testStatus != TEST_PASS)
    {
   	    UART_printf("\nClock generator probe Test Failed!!\n");
   		return (testStatus);
   	}
   	else
   	{
		UART_printf("\nClock generator probe Test Passed!\n");
	}

    return (testStatus);
}

/**
 * \brief This function performs clock generatorTest test
 *
 * \param void
 *
 * \return
 * \n      TEST_PASS  - Test Passed
 * \n      TEST_FAIL  - Test Failed
 *
 */
static TEST_STATUS clockgeneratorTest(void)
{
	TEST_STATUS testStatus = 0;

	UART_printf("\n**********************************\n");
	UART_printf(  "       CLOCK GENERATOR Test       \n");
	UART_printf(  "**********************************\n");

	testStatus = run_clock_generator_test();
	if(testStatus != 0)
	{
		UART_printf("\nClock generator Test Failed!\n");
	}
	else
	{
		UART_printf("\nClock generator Test Passed!\n");
	}

	UART_printf("\nClock generator Tests Completed!!\n");
	UART_printf("\n-----------------X-----------------\n\n\n");

	return (testStatus);

} // clockgeneratorTest

/**
 * \brief Invokes clock generator test functions
 *
 */
int main(void)
{
    TEST_STATUS      testStatus;
    Board_initCfg    boardCfg;

#ifdef SOC_K2G
    DIAG_IntrInit();
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);

    /* Invoke Clock generator Test */
    testStatus = clockgeneratorTest();
    if(testStatus != TEST_PASS)
    {
    	return (-1);
    }

    return (0);
}
/* Nothing past this point */
