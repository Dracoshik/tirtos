/*
 * Copyright (C) 2015-2017 Texas Instruments Incorporated - http://www.ti.com/
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

#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <soc.h>

#include "board.h"
#include "board_cfg.h"

#ifdef SOC_K2G
#include "diag_common_cfg.h"
#endif

#define GPIO_PIN_VAL_LOW        (0U)
#define GPIO_PIN_VAL_HIGH       (1U)

#define NUMBER_OF_CYCLES        (3)

/* Delay function */
void AppDelay(uint32_t delayVal);

#if defined (iceK2G)
#include <ti/drv/gpio/soc/GPIO_soc.h>
#define GPIO_LED0_PIN_NUM        (11U) /* by default PADCONFIG139 set to GPIO0_108 */
#define GPIO_LED0_PORT_NUM       (0U)   /* GPIO port 0 */
#define DELAY_VALUE              (5000000)

/* ON Board LED pins which are connected to GPIO pins. */
typedef enum GPIO_LED {
    USER_LED0 = 0,
    USER_LED1
}GPIO_LED;

uint32_t gLedNum;
#ifdef _TMS320C6X
#define GPIO_MUX_SEL             (16U) /* Event slect number for DSP GPIOMUX int */
#else
#define GPIO_MUX_SEL             (0U)  /* Event slect number for ARM GPIOMUX int */
#endif

/* Application callback function while running the LED test in interrupt mode */
void AppGpioCallbackFxn(void)
{
    if(gLedNum != USER_LED0)
    {
        GPIO_write(USER_LED0, GPIO_PIN_VAL_LOW);
        GPIO_write(gLedNum, GPIO_PIN_VAL_HIGH);
    }
}

#endif /* iceK2G */

int led_test()
{
    char p = 'r';
    int i, j, k;

#if defined (iceK2G) /*  Test is executed with GPIO interrupts enabled on iceK2G */
	/* Setup GPIO interrupt configurations */
	GPIO_socSetIntMux(GPIO_LED0_PORT_NUM, GPIO_LED0_PIN_NUM, NULL, GPIO_MUX_SEL);

	/* GPIO initialization */
	GPIO_init();

	/* Set the callback function */
	GPIO_setCallback(USER_LED0, AppGpioCallbackFxn);

	/* Enable GPIO interrupt on the specific gpio pin */
	GPIO_enableInt(USER_LED0);
#else
    GPIO_init();

#ifdef iceAMIC110
	GPIO_write(BOARD_GPIO_LED_NUM, GPIO_PIN_VAL_HIGH);
#endif
#endif	/* #if defined (iceK2G) */

	UART_printf("\n*********************************************\n");
	UART_printf  ("*                 LED Test                  *\n");
	UART_printf  ("*********************************************\n");

	UART_printf("\nTesting LED\n");
    UART_printf("Blinking LEDs...\n");

    for (i=0; i<BOARD_GPIO_LED_NUM; i++) {
        GPIO_write(i, GPIO_PIN_VAL_LOW);
    }

    do {
        UART_printf("Press 'y' to verify pass, 'r' to blink again,\n");
        UART_printf("or any other character to indicate failure: ");
        for (i=0; i<NUMBER_OF_CYCLES; i++) {
            for (j=0; j<BOARD_GPIO_LED_NUM; j++) {
                for (k=0; k<BOARD_GPIO_LED_NUM; k++) {
                    if (j==k) {
#if defined (iceK2G)
                        gLedNum = k;
                        GPIO_toggle(USER_LED0);
#else
                        GPIO_write(k, GPIO_PIN_VAL_HIGH);
#endif
                    }
                    else {
                        GPIO_write(k, GPIO_PIN_VAL_LOW);
                    }
                }
                AppDelay(5000000);
            }
    	}
        UART_scanFmt("%c", &p);
        if (p == 'r') UART_printf("\nBlinking again\n");
    } while (p == 'r');

    for (i=0; i<BOARD_GPIO_LED_NUM; i++) {
        GPIO_write(i, GPIO_PIN_VAL_LOW);
    }

	UART_printf("Received: %c\n", p);
	if ( (p == 'y') || (p == 'Y') ) {
		UART_printf("\nTest PASSED!\n");
		return 0;
	}
	else {
		UART_printf("\nTest FAILED!\n");
		return -1;
	}
}

#ifndef SPI_BOOT_FRAMEWORK
int main(void)
{
    Board_initCfg boardCfg;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_PINMUX_CONFIG | 
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);

#if defined (iceK2G)
    CSL_A15_INIT_copyVectorTable();
#endif
    return led_test();
}
#endif /* #ifndef SPI_BOOT_FRAMEWORK */

void AppDelay(uint32_t delayVal)
{
    uint32_t cnt = 0;
    while(cnt < delayVal)
    {
        asm("");
        cnt++;
    }
}
