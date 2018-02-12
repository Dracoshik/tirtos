/**
 *  \file   main.c
 *
 *  \brief  Example application main file. This application will toggle the led.
 *          The led toggling will be done inside an callback function, which
 *          will be called by Interrupt Service Routine. Interrupts are
 *          triggered manually and no external source is used to trigger
 *          interrupts.
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/i2c/I2C.h>
#if defined (SOC_K2G)
#include <ti/drv/i2c/soc/I2C_soc.h>
#else
#include <ti/drv/i2c/soc/I2C_soc.h>
#endif
#include <ti/drv/uart/UART_stdio.h>

#include "board.h"
#include "board_cfg.h"

#ifdef SOC_K2G
#include "diag_common_cfg.h"
#endif

/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/

extern const I2C_Config I2C_config[];
void AppDelay(uint32_t delayVal);

/* Board specific definitions */
#if defined (EVM_K2G) || defined (EVM_AM335x) || defined (EVM_AM437x)
#define I2C_INSTANCE        (1U)
#else
#define I2C_INSTANCE        (0U)
#endif

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

uint16_t readTemp(I2C_Handle h)
{
    uint8_t tx[1];
    uint8_t rx[2];
    uint16_t val;
    I2C_Transaction t;

    I2C_transactionInit(&t);

    t.slaveAddress = 0x48;
    t.writeBuf = tx;
    t.writeCount = 1;
    t.readBuf = rx;
    t.readCount = 0;
    tx[0] = 0x0;
    I2C_transfer(h, &t);
    AppDelay(1000);
    t.writeCount = 0;
    t.readCount = 2;
    I2C_transfer(h, &t);
    val = (rx[0] << 4) | (rx[1] >> 4);
    UART_printf("got rx[0] = %d, rx[1] = %d, val = %d\n", rx[0], rx[1], val);
    return val;
}

int convertTemp(uint16_t temp)
{
    return (temp * 625) / 10000;
}

/*
 *  ======== test function ========
 */
int temperature_test()
{
    int ret = 0;
    int i;
    uint16_t temp;
    I2C_Params i2cParams;
    I2C_Handle handle = NULL;

    for (i=0; I2C_config[i].fxnTablePtr != NULL; i++)
    {
        ((I2C_HwAttrs *)I2C_config[i].hwAttrs)->enableIntr = false;
    }

    I2C_init();

    I2C_Params_init(&i2cParams);

    handle = I2C_open(I2C_INSTANCE, &i2cParams);

    UART_printf("\n*********************************************\n");
    UART_printf  ("*         Temperature Sensor Test           *\n");
    UART_printf  ("*********************************************\n");
    UART_printf("Testing temperature sensor... \n");
    temp = readTemp(handle);
    UART_printf("Temperature = %d C\n", convertTemp(temp));
    UART_printf("Test PASSED!\n");
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
    boardCfg = BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);
    return temperature_test();
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

