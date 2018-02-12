/**
 * \file   button_test.c
 *
 * \brief  This application demonstrates the working of GPIO based button keypad.
 *         It waits for a key press event from the button keypad.
 *         On a key press the diagnostic test sequentially moves to the other
 *         buttons in the keypad.
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

#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>

#include <ti/drv/uart/UART_stdio.h>
#include <soc.h>
#include <ti/starterware/include/interrupt.h>

#include "board.h"
#include "board_cfg.h"

#define GPIO_PIN_VAL_LOW        (0U)
#define GPIO_PIN_VAL_HIGH       (1U)

#define BOARD_NAME_LENGTH                  (8)

/** \brief Successful completion of the functionality.  */
#define S_PASS                  (0)
/** \brief Generic failure code.                        */
#define E_FAIL                  (-1)

/** \brief structure holding the GPIO related information for buttons.  */
typedef struct gpioInfo
{
    uint32_t instance;
    uint32_t pin;
}gpioInfo_t;

/** \brief structure holding the Keypad information of the buttons.  */
typedef struct keyPadInfo
{
    uint32_t buttonSet;
    uint32_t scnKeyNum;
    uint32_t pwrKeyNum;
    uint32_t pwrKeyIdx;
}keyPadInfo_t;

uint32_t buttonOffset[2]={0, 0}, powerOffset[2]={0, 0}, buttonStart[2]={0, 0};

/* Variable to hold the GPIO key info. */
gpioInfo_t KeyScn[4];

/**
 * \brief   Get the Board keypad information as per the board Id.
 *
 * \param   pBoardName    Pointer to the board name.
 *
 * \retval  S_PASS  if the board id matches.
 * \retval  E_FAIL  if the board id fails.
 *
 */
int32_t BoardDiag_GetKeyPadInfo(char *pBoardName, keyPadInfo_t *pBoardKeyPad);

/**
 * \brief    Function to demonstrate the GPIO Button test.
 *
 * \retval   S_PASS    on success.
 * \retval   E_FAIL    on failure.
 */
int32_t BoardDiag_ButtonTest();

/**
 * \brief    Function Waits for a GPIO button key press and returns the status.
 *
 * \retval   S_PASS    on success.
 * \retval   E_FAIL    on failure.
 */
int32_t BoardDiag_KeyPressCheck(keyPadInfo_t *pBoardKeyPad,
                                uint32_t button,
                                uint32_t scnKey,
                                uint32_t pwrKey);

/**
 * \brief    Function to power up the GPIO keypad.
 *
 */
void BoardDiag_BtnPwr(keyPadInfo_t *pBoardKeyPad, uint32_t num);

int main(void)
{
    int status = S_PASS;
    Board_initCfg boardCfg;
    Board_IDInfo boardInfo;
    keyPadInfo_t boardKeyPad;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_PINMUX_CONFIG | 
        BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    Board_init(boardCfg);

    UART_printf("\n*********************************************\n"); 
    UART_printf  ("*                 Button Test               *\n");
    UART_printf  ("*********************************************\n");

    Board_getIDInfo(&boardInfo);

    /* Update the KeyPad information. */
    status = BoardDiag_GetKeyPadInfo(boardInfo.boardName, &boardKeyPad);

    if(S_PASS == status)
    {
        status = BoardDiag_ButtonTest(&boardKeyPad);

        if(S_PASS == status)
        {
            UART_printf("Test PASSED! \n");
        }
        else
        {
            UART_printf("Test Failed!\n");
        }
    }

    return status;
}

void BoardDiag_BtnPwr(keyPadInfo_t *pBoardKeyPad, uint32_t num)
{
    uint32_t i = 0U;

    for(i = 0; i < pBoardKeyPad->pwrKeyNum; i++)
    {
        if(i==num)
        {
            GPIO_write(pBoardKeyPad->pwrKeyIdx + i, GPIO_PIN_VAL_HIGH);
        }
        else
        {
            GPIO_write(pBoardKeyPad->pwrKeyIdx + i, GPIO_PIN_VAL_LOW);
        }
    }
}

int32_t BoardDiag_ButtonTest(keyPadInfo_t *pBoardKeyPad)
{
    uint32_t retVal = S_PASS;
    uint32_t i, j, k;

    /* Initialize the GPIO driver. */
    GPIO_init();

    for(k=0; k < pBoardKeyPad->buttonSet; k++)
    {
        for(i=0; i< pBoardKeyPad->pwrKeyNum; i++)
        {
            for(j=0; j < pBoardKeyPad->scnKeyNum; j++)
            {
                retVal = BoardDiag_KeyPressCheck(pBoardKeyPad,
                            buttonStart[k]+(i*powerOffset[k])+(j*buttonOffset[k]),
                            j, i);
            }
        }
    }

    return retVal;
}

int32_t BoardDiag_KeyPressCheck(keyPadInfo_t *pBoardKeyPad,
                                uint32_t button,
                                uint32_t scnKey,
                                uint32_t pwrKey)
{
    int32_t ret_val=0;
    uint32_t level = 0U;

    UART_printf(  "Button SW %2d            ", button);
    UART_printf("WAIT      Waiting for button press");

    BoardDiag_BtnPwr(pBoardKeyPad, pwrKey);
    do
    {
        level = (GPIO_read(scnKey)>>KeyScn[scnKey].pin);
    }while(level != 1);

    UART_printf("Button SW %2d            ", button);

    if(level != 1)
    {
        UART_printf("FAIL                                            \n");
        ret_val = E_FAIL;
    }
    else
    {
        UART_printf("PASS                                            \n");
        ret_val = S_PASS;
    }

    return ret_val;
}

int32_t BoardDiag_GetKeyPadInfo(char *pBoardName, keyPadInfo_t *pBoardKeyPad)
{
    int32_t status = S_PASS;

    /* Check if the board is AM437x GPEVM by comparing the string read from EEPROM. */
    if (strncmp("AM43__GP", pBoardName, BOARD_NAME_LENGTH) == 0U)
    {
        pBoardKeyPad->buttonSet = 1;
        pBoardKeyPad->scnKeyNum = 3;
        pBoardKeyPad->pwrKeyNum = 2;
        pBoardKeyPad->pwrKeyIdx = 3;
        buttonStart[0]=4;
        buttonOffset[0]=1;
        powerOffset[0]=3;
        /* Update the GPIO data for keypad inputs. */
        KeyScn[0].instance=3;
        KeyScn[0].pin=21;

        KeyScn[1].instance=4;
        KeyScn[1].pin=3;

        KeyScn[2].instance=4;
        KeyScn[2].pin=2;
    }
    /* Check if the board is SKAM437x by comparing the string read from EEPROM. */
    else if (strncmp("AM43__SK", pBoardName, BOARD_NAME_LENGTH) == 0U)
    {
        pBoardKeyPad->buttonSet = 1;
        pBoardKeyPad->scnKeyNum = 2;
        pBoardKeyPad->pwrKeyNum = 2;
        pBoardKeyPad->pwrKeyIdx = 2;
        buttonStart[0]=6;
        buttonOffset[0]=1;
        powerOffset[0]=2;

        /* Update the GPIO data for keypad inputs. */
        KeyScn[0].instance=5;
        KeyScn[0].pin=5;

        KeyScn[1].instance=5;
        KeyScn[1].pin=6;
    }
    /* Check if the board is SKAM335x by comparing the string read from EEPROM. */
    else if (strncmp("A335X_SK", pBoardName, BOARD_NAME_LENGTH) == 0U)
    {
        pBoardKeyPad->buttonSet = 1;
        pBoardKeyPad->scnKeyNum = 4;
        pBoardKeyPad->pwrKeyNum = 1;
        pBoardKeyPad->pwrKeyIdx = 0;
        buttonStart[0]=1;
        buttonOffset[0]=1;
        powerOffset[0]=1;

        /*
        ** Note: For skAM335x board the order of key press on the keypad should
                 be in the following order
        1: Key1: SW3 corresponding to SW3 switch on board.
        2: Key2: SW1 corresponding to SW1 switch on board.
        3: Key3: SW2 corresponding to SW2 switch on board.
        4: Key4: SW4 corresponding to SW4 switch on board.
        */
        KeyScn[0].instance=0;
        KeyScn[0].pin=30;

        KeyScn[1].instance=2;
        KeyScn[1].pin=3;

        KeyScn[2].instance=2;
        KeyScn[2].pin=2;

        KeyScn[3].instance=2;
        KeyScn[3].pin=5;
    }
    else
    {
        status = E_FAIL;
        UART_printf("The test is not supported on this board\n");
    }

    return status;
}
