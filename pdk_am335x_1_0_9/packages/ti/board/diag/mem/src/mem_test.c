/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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

#include <ti/drv/uart/UART_stdio.h>
#include <stdlib.h>

#include "board.h"
#include "board_cfg.h"

#ifdef SOC_K2G
#include "diag_common_cfg.h"
#endif

static inline int32_t board_memory_test (uint32_t start_address, uint32_t end_address)
{
	uint32_t index, value;

	UART_printf("First test started\n");
	UART_printf("Writing to test area...\n");
	/* Write a pattern */
	for (index = start_address; (index >= start_address) && (index < end_address); index += 4) {
		*(volatile uint32_t *) index = (uint32_t)index;
		if (!(index & 0xFFFFFFc)) UART_printf("\tWrite up to 0x%08x done\n", index);
	}

	UART_printf("Write finished!\n");
	UART_printf("Checking values...\n");

	/* Read and check the pattern */
	for (index = start_address;  (index >= start_address) && (index < end_address); index += 4) {

		value = *(uint32_t *) index;

		if (value  != index) {
			UART_printf("board_memory_test: Failed at address index = 0x%x value = 0x%x *(index) = 0x%x\n",
					index, value, *(volatile uint32_t *) index);
			return (-1);
		}
		if (!(index & 0xFFFFFFc))UART_printf("\tRead up to 0x%08x okay\n", index);
	}

	UART_printf("Second test started\n");
	UART_printf("Writing complementary values to test area...\n");
	/* Write a pattern for complementary values */
	for (index = start_address; (index >= start_address) && (index < end_address); index += 4) {
		*(volatile uint32_t *) index = (uint32_t)~index;
		if (!(index & 0xFFFFFFc)) UART_printf("\tWrite up to 0x%08x done\n", index);
	}

	UART_printf("Write finished!\n");
	UART_printf("Checking values...\n");
	/* Read and check the pattern */
	for (index = start_address;  (index >= start_address) && (index < end_address); index += 4) {

		value = *(uint32_t *) index;

		if (value  != ~index) {
			UART_printf("board_memory_test: Failed at address index = 0x%x value = 0x%x *(index) = 0x%x\n",
					index, value, *(volatile uint32_t *) index);
			return (-1);
		}
		if (!(index & 0xFFFFFFc)) UART_printf("\tRead up to 0x%08x okay\n", index);
	}

	return 0;
}

int32_t board_external_memory_test()
{
	UART_printf("board_external_memory_test: Start address (0x%08x), end address (0x%08x)\n",
		BOARD_DDR3_START_ADDR, BOARD_DDR3_END_ADDR);
	return board_memory_test(BOARD_DDR3_START_ADDR, BOARD_DDR3_END_ADDR);
}

int mem_test()
{
	int32_t status;
	int ret;

	UART_printf("\n*********************************************\n"); 
	UART_printf  ("*              DDR Memory Test              *\n");
	UART_printf  ("*********************************************\n");

	UART_printf("\nTesting writing and reading memory\n");
	status = board_external_memory_test();
	if (status < 0)
	{
		UART_printf("Memory test failed!\n");
		ret = -1;
	}
	else
	{
		UART_printf("Memory test passed!\n");
		ret = 0;
	}
	return ret;
}

/*
 *  ======== main ========
 */
#ifndef SPI_BOOT_FRAMEWORK
int main(void)
{
    Board_initCfg boardCfg;

#ifdef SOC_K2G
    DIAG_IntrInit();
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_DEFAULT;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);
    return mem_test();
}
#endif
