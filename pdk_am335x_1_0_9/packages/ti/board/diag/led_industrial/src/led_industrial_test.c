/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
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
void AppDelay(uint32_t delayVal);

void led_write(I2C_Handle h, uint8_t val)
{
	uint8_t tx[2];
	I2C_Transaction i2cTransaction;

	memset(&i2cTransaction, 0, sizeof(i2cTransaction));

	I2C_transactionInit(&i2cTransaction);
	tx[0] = 0x44;
	tx[1] = val;
	i2cTransaction.slaveAddress = BOARD_I2C_LED_ADDR;
	i2cTransaction.writeBuf = tx;
	i2cTransaction.writeCount = 2;
	i2cTransaction.readCount = 0;
	I2C_transfer(h, &i2cTransaction);
}


/*
 *  ======== test function ========
 */
int industrial_led_test()
{
    char p = 'r';
	int cnt = 0;
	int ret = 0;
	int i;
    I2C_Params i2cParams;
    I2C_Handle handle = NULL;;

	for (i=0; I2C_config[i].fxnTablePtr != NULL; i++)
	{
		((I2C_HwAttrs *)I2C_config[i].hwAttrs)->enableIntr = false;
	}

    I2C_init();

    I2C_Params_init(&i2cParams);

    handle = I2C_open(BOARD_I2C_LED_INSTANCE, &i2cParams);

	UART_printf("\n*********************************************\n"); 
	UART_printf  ("*          Industrial LEDs Test             *\n");
	UART_printf  ("*********************************************\n");

	UART_printf("\nTesting Industrial LEDs\n");
    UART_printf("Cycling LEDs 3 times\n");
	cnt = 0x80;

    do {
		UART_printf("Press 'y' to verify pass, 'r' to cycle leds again,\n");
	    UART_printf("or any other character to indicate failure: ");
        for (i=0; i<24; i++) {
			if ( (cnt & 0xff) == 0) cnt = 0x80;
			led_write(handle, cnt);
			cnt >>= 1;
#ifdef SOC_K2G
		    BOARD_delay(200000);
#else
		    AppDelay(0x1FFFFFF);
#endif
    	}
    	UART_scanFmt("%c", &p);
        if (p == 'r') UART_printf("\nCycling LEDs 3 times again\n");
    } while (p == 'r');

	UART_printf("Received: %c\n", p);
	if ( (p == 'y') || (p == 'Y') ) {
		UART_printf("\nTest PASSED!\n");
		ret = 0;
	}
	else {
		UART_printf("\nTest FAILED!\n");
		ret = -1;
	}

	I2C_close(handle);

	return ret;
}

/*
 *  ======== main ========
 */
int main(void)
{
    Board_initCfg boardCfg;

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
    return industrial_led_test();
}

void AppDelay(uint32_t delayVal)
{
    uint32_t cnt = 0;
    while(cnt < delayVal)
    {
        asm("");
        cnt++;
    }
}

