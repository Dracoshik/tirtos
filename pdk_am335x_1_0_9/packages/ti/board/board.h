/*
 * Copyright (c) 2010-2017, Texas Instruments Incorporated
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
 *  @file   board.h
 *
 *  @brief  The Board Library is a thin utility
 *  layer on top of  CSL and other board utilities. It provides uniform APIs
 *  for all supported boards. It aims to assist user to quickly write portable
 *  applications for its supported boards by hiding board level details
 *  from the user.
 *
 *
 */

#ifndef BOARD_UTILS_H_
#define BOARD_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @mainpage Board Utility APIs
 *  Defines a set of APIs for accessing and working with the various board peripherals.
 */


/** @defgroup  Board_standard_data_types  Standard Data Types */
/*@{*/
/**
 *  @brief 	Board Library uses basic C99 data types. The basic types used are
 *  		uint32_t, int32_t, uint8_t, int8_t, uint16_t and int16_t. The standard
 *  		C char is used for strings. Complex types (or typdefs) if used are defined
 *  		within this header file.
 */
#include <stdint.h>

#include <ti/csl/hw_types.h>
#include <ti/board/board_cfg.h>

/** @defgroup  Board_cache Board Cache */
/*@{*/
/**
  * @brief  The following definitions are for handling cache alignment on the board.
  *
  * 		MAX_CACHE_LINE must be set to the cache line size of the board.
  *
  * 		When allocating memory that must be cache aligned, it must be a multiple of
  * 		the cache line size. Use board_roundup to get the appropriate size.
  *
  * 		As an example to allocate a cache aligned block of memory you would do
  * 		something like:
  *
  * 			buffer_len_aligned = board_roundup (buffer_len, MAX_CACHE_LINE)
  * 			Malloc (buffer_len_aligned)
  *
  */
/* w should be power of 2 */
#define BOARD_CACHE_LINE_SIZE (128)
#define board_roundup(n,w) (((n) + (w) - 1) & ~((w) - 1))

/*************************************************************
 * Return/Error values                                       *
 *************************************************************/

/**
 * @brief   The return type for board library API calls
 *
 * @details Board library function calls will return this value, which contains
 *          information as to whether the function succeeded or encountered an
 *          error.
 */
typedef int32_t Board_STATUS;

/** Board status OK */
#define BOARD_SOK                           0
/** Error code for invalid parameters */
#define BOARD_INVALID_PARAM                 -1
/** Error code for unsupported feature */
#define BOARD_UNSUPPORTED_FEATURE           -2
/** I2C handle open failed */
#define BOARD_I2C_OPEN_FAIL                 -3
/** I2C transfer call failed */
#define BOARD_I2C_TRANSFER_FAIL             -4
/** IO Delay region not in internal memory */
#define BOARD_PINMUX_BAD_MEM_REGION         -5
/** Error code for init clock module */
#define BOARD_INIT_CLOCK_FAIL               -6
/** Error code for init DDR */
#define BOARD_INIT_DDR_FAIL                 -7
/** Runtime pinmux configuration mode */
#define BOARD_PINMUX_INVALID_MODE           -8

/*************************************************************
 * Init/Info Definitons                                      *
 *************************************************************/

/* Max number of characters per ID entry field */
#define BOARD_MAX_ID_STR_LEN  20

/*************************************************************
 * SoC Info Definitons                                      *
 *************************************************************/

/* Default system clock value */
#define BOARD_SYS_CLK_DEFAULT  (0xFFFFFFFFU)

/**
 *  @brief This structure contains board specific information
 */
typedef struct {

    /* Header ID of the SOC */
    char header[BOARD_MAX_ID_STR_LEN];

    /* Name of the board */
    char boardName[BOARD_MAX_ID_STR_LEN];

    /* Board version/revision info */
    char version[BOARD_MAX_ID_STR_LEN];

    /* Board serial number */
    char serialNum[BOARD_MAX_ID_STR_LEN];

    /* Board configuration codes */
    char configCodes[2*BOARD_MAX_ID_STR_LEN];

    /* Industrial Ethernet MAC address 0 */
    char indEthMacID0[BOARD_MAX_ID_STR_LEN];

    /* Industrial Ethernet MAC address 3 */
    char indEthMacID3[BOARD_MAX_ID_STR_LEN];

} Board_IDInfo;

/**
 *  @brief This structure contains SoC specific information
 */
typedef struct {

    /* System clock value in Hz */
    uint32_t sysClock;

} Board_SoCInfo;

typedef uint32_t Board_initCfg;
#define BOARD_INIT_ALL                  (0xFFFFFFFFU)
#define BOARD_INIT_UNLOCK_MMR           (1 << 1U)
#define BOARD_INIT_PLL                  (1 << 2U)
#define BOARD_INIT_MODULE_CLOCK         (1 << 3U)
#define BOARD_INIT_DDR                  (1 << 4U)
#define BOARD_INIT_WATCHDOG_DISABLE     (1 << 5U)
#define BOARD_INIT_PINMUX_CONFIG        (1 << 6U)
#define BOARD_INIT_UART_STDIO           (1 << 7U)
#define BOARD_INIT_ICSS_PINMUX          (1 << 8U)
#define BOARD_INIT_ETH_PHY              (1 << 9U)
#define BOARD_INIT_ECC                  (1 << 10U)
#define BOARD_INIT_PLL_OPP_HIGH         (1 << 11U)
#define BOARD_INIT_PLL_OPP_OD           (1 << 12U)
#define BOARD_INIT_PLL_OPP_NOM          (1 << 13U)
/* iceK2G requires board specific Ethernet configurations for
   proper operation of Ethernet interfaces. Applications
   using NSS(Gigabit) or ICSS(10/100mbps) Ethernet inteface
   should call Board_init() with config control macro
   'BOARD_INIT_ETH_PHY' for initializing Gigabit Ethernet PHY and
   'BOARD_INIT_ICSS_ETH_PHY' to invoke the PRU_ICSS Ethernet PHY
   initialization */
/* Config control bit definition for 10/100 Ethernet PHY intialization */
#define BOARD_INIT_ICSS_ETH_PHY         (1 << 14U)
/* Flag to enable EMIF pin mux configuration during Board_init.
   EMIF pin mux is done as part of default pin mux as configuring EMIF pinmux
   while system is running from SDRAM/DDR can corrupt the memory.
   Use this flag only from the applications which are not running from SDRAM/DDR
*/
#define BOARD_INIT_EMIF_PINMUX          (1 << 15U)
/* Applies only to devices with DDR ECC support */
#define BOARD_INIT_DDR_ECC              (1 << 16U) 

#define BOARD_INIT_DEFAULT              BOARD_INIT_ALL

/*************************************************************
 * APIs                                                      *
 *************************************************************/

/**
 *  @brief      Get board information.
 *
 *  @param[out] Board_STATUS
 *    Returns status on API call
 *  @param[out] info
 *    This structure will have board information on return
 *
 */
Board_STATUS Board_getIDInfo(Board_IDInfo *info);

/**
 *  @brief      Get SoC information.
 *
 *  @param[out] Board_STATUS  
 *    Returns status on API call 
 *  @param[out] info  
 *    This structure will have SoC information on return
 *
 */
Board_STATUS Board_getSoCInfo(Board_SoCInfo *socInfo);

/**
 *  @brief      Initialization functions for a specific board.
 *
 *  @param[out] Board_STATUS  
 *    Returns status on API call 
 *  @param[in] cfg  
 *    Param to indicate what to initialize when calling Board_init()
 *
 */
Board_STATUS Board_init(Board_initCfg cfg);

#ifdef __cplusplus
}
#endif

#endif
