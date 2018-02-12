/**
 *  \file   current_monitor_test.c
 *
 *  \brief  Example application for current monitor
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
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
#define I2C_INSTANCE        (2U)
#define INA_DEV0_ADDR      (0x40U)
#define INA_DEV1_ADDR      (0x41U)
#define INA_DEV2_ADDR      (0x43U)
#define INA_DEV3_ADDR      (0x44U)

#define SHUNT_VOLTAGE_ADDR (1)
#define BUS_VOLTAGE_ADDR   (2)

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

/*
 * Function to read current monitor register
 */
uint16_t readRegister(I2C_Handle h, uint8_t slaveAddress, uint8_t regAddr)
{
	uint8_t tx[1];
	uint8_t rx[2];
	uint16_t val;

	I2C_Transaction t;
    I2C_transactionInit(&t);
	t.slaveAddress = slaveAddress;
	t.writeBuf = tx;
	t.writeCount = 1;
	t.readBuf = rx;
	t.readCount = 0;
	tx[0] = regAddr;
	I2C_transfer(h, &t);
	AppDelay(1000);
	t.writeCount = 0;
	t.readCount = 2;

	I2C_transfer(h, &t);
	val = (rx[0] << 8) | (rx[1] & 0xFF);

	return val;
}

/*
 * Function to write current monitor register
 */
uint8_t writeRegister(I2C_Handle h, uint8_t slaveAddress, uint8_t regAddr, uint8_t regData)
{
	uint8_t tx[2];
	uint8_t ret;

	I2C_Transaction t;
    I2C_transactionInit(&t);
	t.slaveAddress = slaveAddress;
	t.writeBuf = tx;
	t.writeCount = 2;
	t.readBuf = NULL;
	t.readCount = 0;
	tx[0] = regAddr;
	tx[1] = regData;

	ret = I2C_transfer(h, &t);

	return ret;
}

/*
 * Set the calibration to the current monitor
 */
uint8_t setCalibration(I2C_Handle h, uint8_t slaveAddr, uint8_t currentLSB, uint8_t maxRShunt)
{
    uint16_t calibration;
    uint8_t ret;

    calibration = (5120000/(currentLSB * maxRShunt));

    ret = writeRegister(h, slaveAddr, 0x05, calibration);

    return ret;
}

/*
 *  ======== test function ========
 */
int current_monitor_test(void)
{
	int ret = 0;
	int i;
	uint16_t value;
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
	UART_printf  ("*         Current Monitor Test           *\n");
	UART_printf  ("*********************************************\n");

	UART_printf("\n\nVerifying Device at Address - 0x%x\n",INA_DEV0_ADDR);
	setCalibration(handle, INA_DEV0_ADDR, 125, 25);
	value = readRegister(handle, INA_DEV0_ADDR, SHUNT_VOLTAGE_ADDR);
	UART_printf("Shunt Voltage = %dmV\n", value);
	value = readRegister(handle, INA_DEV0_ADDR, BUS_VOLTAGE_ADDR);
	UART_printf("Bus Voltage = %dmV\n", value);

	UART_printf("\n\nVerifying Device at Address - 0x%x\n",INA_DEV1_ADDR);
	setCalibration(handle, INA_DEV1_ADDR, 125, 25);
	value = readRegister(handle, INA_DEV1_ADDR, SHUNT_VOLTAGE_ADDR);
	UART_printf("Shunt Voltage = %dmV\n", value);
	value = readRegister(handle, INA_DEV1_ADDR, BUS_VOLTAGE_ADDR);
	UART_printf("Bus Voltage = %dmV\n", value);

	UART_printf("\n\nVerifying Device at Address - 0x%x\n",INA_DEV2_ADDR);
	value = readRegister(handle, INA_DEV2_ADDR, SHUNT_VOLTAGE_ADDR);
	setCalibration(handle, INA_DEV2_ADDR, 80, 25);
	UART_printf("Shunt Voltage = %dmV\n", value);
	value = readRegister(handle, INA_DEV2_ADDR, BUS_VOLTAGE_ADDR);
	UART_printf("Bus Voltage = %dmV\n", value);

	UART_printf("\n\nVerifying Device at Address - 0x%x\n",INA_DEV3_ADDR);
	setCalibration(handle, INA_DEV3_ADDR, 80, 25);
	value = readRegister(handle, INA_DEV3_ADDR, SHUNT_VOLTAGE_ADDR);
	UART_printf("Shunt Voltage = %dmV\n", value);
	value = readRegister(handle, INA_DEV3_ADDR, BUS_VOLTAGE_ADDR);
	UART_printf("Bus Voltage = %dmV\n", value);

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

#ifdef ICE_K2G

/* Current monitor is available only on Alpha ICE board */
#ifdef K2G_ICE_ALPHA
	UART_printf("Unsupported Test!!\n");
	return 1;
#else
	return current_monitor_test();
#endif

#else
	return current_monitor_test();
#endif
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

