/**
 * Note: This file was auto-generated by TI PinMux on 5/12/2014 at 11:47:50 AM.
 *
 * \file   am43xx_pinmux.h
 *
 * \brief  This file contains pad configure register offsets and bit-field 
 *         value macros for different configurations,
 *
 *           6    SLEWCTRL       Select between faster or slower slew rate
 *           5    RXACTIVE       Input enable value for the Pad
 *           4    PULLTYPESEL    Pad pullup/pulldown type selection
 *           3    PULLUDEN       Pad Pullup/pulldown enable
 *           2-0  MUXMODE        Pad functional signal mux select
 *
 *  \copyright Copyright (CU) 2014 Texas Instruments Incorporated - 
 *             http://www.ti.com/
 */

/**
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDINGU, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef AM43XX_PIN_MUX_H_
#define AM43XX_PIN_MUX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "pinmux.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define PIN_MODE(mode)	                (mode)

/** \brief Active mode configurations */
/** \brief Pull up/down disabled */
#define PIN_PULL_UD_DIS	                (0x1U << 16U)  
/** \brief Pull UP enable */
#define	PIN_PULL_UP_EN	                (0x1U << 17U)
/** \brief Receiver enable */  
#define	PIN_RX_ACTIVE	                (0x1U << 18U)

/** \brief Sleep mode configurations */
/** \brief In DS mode, override the pad config with DS values  */
#define PIN_DS_VALUE_OVERRIDE_EN	(0x1U << 24U)  
/** \brief Disable output in DS mode */
#define PIN_DS_OP_DIS            	(0x1U << 25U)  
/** \brief Drive output to '1' DS mode */
#define PIN_DS_OP_VAL_1          	(0x1U << 26U)  
/** \brief Pull up/down disable in DS mode */
#define PIN_DS_PULL_UD_EN          	(0x1U << 27U)  
/** \brief Pull up disable in DS mode */
#define PIN_DS_PULL_UP_EN          	(0x1U << 28U)  
/** \brief Wakeup enable on the pin */
#define PIN_WAKE_UP_EN          	(0x1U << 29U)  
/** \brief Wakeup event occurred on the pin */
#define PIN_WAKE_EVENT_OCCURED     	(0x1U << 30U)  

/** \brief Pinmux register mask */
#define PINMUX_REG_MASK                 (0x007FU)

/** \brief Pad config register offset in control module */
enum pinOffsets
{
	PIN_OFFSET_MIN             = 0x0800U,
	PIN_GPMC_AD0               = 0x0800U,
	PIN_GPMC_AD1               = 0x0804U,
	PIN_GPMC_AD2               = 0x0808U,
	PIN_GPMC_AD3               = 0x080cU,
	PIN_GPMC_AD4               = 0x0810U,
	PIN_GPMC_AD5               = 0x0814U,
	PIN_GPMC_AD6               = 0x0818U,
	PIN_GPMC_AD7               = 0x081cU,
	PIN_GPMC_AD8               = 0x0820U,
	PIN_GPMC_AD9               = 0x0824U,
	PIN_GPMC_AD10              = 0x0828U,
	PIN_GPMC_AD11              = 0x082cU,
	PIN_GPMC_AD12              = 0x0830U,
	PIN_GPMC_AD13              = 0x0834U,
	PIN_GPMC_AD14              = 0x0838U,
	PIN_GPMC_AD15              = 0x083cU,
	PIN_GPMC_A0                = 0x0840U,
	PIN_GPMC_A1                = 0x0844U,
	PIN_GPMC_A2                = 0x0848U,
	PIN_GPMC_A3                = 0x084cU,
	PIN_GPMC_A4                = 0x0850U,
	PIN_GPMC_A5                = 0x0854U,
	PIN_GPMC_A6                = 0x0858U,
	PIN_GPMC_A7                = 0x085cU,
	PIN_GPMC_A8                = 0x0860U,
	PIN_GPMC_A9                = 0x0864U,
	PIN_GPMC_A10               = 0x0868U,
	PIN_GPMC_A11               = 0x086cU,
	PIN_GPMC_WAIT0             = 0x0870U,
	PIN_GPMC_WPN               = 0x0874U,
	PIN_GPMC_BE1N              = 0x0878U,
	PIN_GPMC_CSN0              = 0x087cU,
	PIN_GPMC_CSN1              = 0x0880U,
	PIN_GPMC_CSN2              = 0x0884U,
	PIN_GPMC_CSN3              = 0x0888U,
	PIN_GPMC_CLK               = 0x088cU,
	PIN_GPMC_ADVN_ALE          = 0x0890U,
	PIN_GPMC_OEN_REN           = 0x0894U,
	PIN_GPMC_WEN               = 0x0898U,
	PIN_GPMC_BE0N_CLE          = 0x089cU,
	PIN_DSS_DATA0              = 0x08a0U,
	PIN_DSS_DATA1              = 0x08a4U,
	PIN_DSS_DATA2              = 0x08a8U,
	PIN_DSS_DATA3              = 0x08acU,
	PIN_DSS_DATA4              = 0x08b0U,
	PIN_DSS_DATA5              = 0x08b4U,
	PIN_DSS_DATA6              = 0x08b8U,
	PIN_DSS_DATA7              = 0x08bcU,
	PIN_DSS_DATA8              = 0x08c0U,
	PIN_DSS_DATA9              = 0x08c4U,
	PIN_DSS_DATA10             = 0x08c8U,
	PIN_DSS_DATA11             = 0x08ccU,
	PIN_DSS_DATA12             = 0x08d0U,
	PIN_DSS_DATA13             = 0x08d4U,
	PIN_DSS_DATA14             = 0x08d8U,
	PIN_DSS_DATA15             = 0x08dcU,
	PIN_DSS_VSYNC              = 0x08e0U,
	PIN_DSS_HSYNC              = 0x08e4U,
	PIN_DSS_PCLK               = 0x08e8U,
	PIN_DSS_AC_BIAS_EN         = 0x08ecU,
	PIN_MMC0_DAT3              = 0x08f0U,
	PIN_MMC0_DAT2              = 0x08f4U,
	PIN_MMC0_DAT1              = 0x08f8U,
	PIN_MMC0_DAT0              = 0x08fcU,
	PIN_MMC0_CLK               = 0x0900U,
	PIN_MMC0_CMD               = 0x0904U,
	PIN_MII1_COL               = 0x0908U,
	PIN_MII1_CRS               = 0x090cU,
	PIN_MII1_RX_ER             = 0x0910U,
	PIN_MII1_TX_EN             = 0x0914U,
	PIN_MII1_RX_DV             = 0x0918U,
	PIN_MII1_TXD3              = 0x091cU,
	PIN_MII1_TXD2              = 0x0920U,
	PIN_MII1_TXD1              = 0x0924U,
	PIN_MII1_TXD0              = 0x0928U,
	PIN_MII1_TX_CLK            = 0x092cU,
	PIN_MII1_RX_CLK            = 0x0930U,
	PIN_MII1_RXD3              = 0x0934U,
	PIN_MII1_RXD2              = 0x0938U,
	PIN_MII1_RXD1              = 0x093cU,
	PIN_MII1_RXD0              = 0x0940U,
	PIN_RMII1_REF_CLK          = 0x0944U,
	PIN_MDIO_DATA              = 0x0948U,
	PIN_MDIO_CLK               = 0x094cU,
	PIN_SPI0_SCLK              = 0x0950U,
	PIN_SPI0_D0                = 0x0954U,
	PIN_SPI0_D1                = 0x0958U,
	PIN_SPI0_CS0               = 0x095cU,
	PIN_SPI0_CS1               = 0x0960U,
	PIN_ECAP0_IN_PWM0_OUT      = 0x0964U,
	PIN_UART0_CTSN             = 0x0968U,
	PIN_UART0_RTSN             = 0x096cU,
	PIN_UART0_RXD              = 0x0970U,
	PIN_UART0_TXD              = 0x0974U,
	PIN_UART1_CTSN             = 0x0978U,
	PIN_UART1_RTSN             = 0x097cU,
	PIN_UART1_RXD              = 0x0980U,
	PIN_UART1_TXD              = 0x0984U,
	PIN_I2C0_SDA               = 0x0988U,
	PIN_I2C0_SCL               = 0x098cU,
	PIN_MCASP0_ACLKX           = 0x0990U,
	PIN_MCASP0_FSX             = 0x0994U,
	PIN_MCASP0_AXR0            = 0x0998U,
	PIN_MCASP0_AHCLKR          = 0x099cU,
	PIN_MCASP0_ACLKR           = 0x09a0U,
	PIN_MCASP0_FSR             = 0x09a4U,
	PIN_MCASP0_AXR1            = 0x09a8U,
	PIN_MCASP0_AHCLKX          = 0x09acU,
	PIN_CAM0_HD                = 0x09b0U,
	PIN_CAM0_VD                = 0x09b4U,
	PIN_CAM0_FIELD             = 0x09b8U,
	PIN_CAM0_WEN               = 0x09bcU,
	PIN_CAM0_PCLK              = 0x09c0U,
	PIN_CAM0_DATA8             = 0x09c4U,
	PIN_CAM0_DATA9             = 0x09c8U,
	PIN_CAM1_DATA9             = 0x09ccU,
	PIN_CAM1_DATA8             = 0x09d0U,
	PIN_CAM1_HD                = 0x09d4U,
	PIN_CAM1_VD                = 0x09d8U,
	PIN_CAM1_PCLK              = 0x09dcU,
	PIN_CAM1_FIELD             = 0x09e0U,
	PIN_CAM1_WEN               = 0x09e4U,
	PIN_CAM1_DATA0             = 0x09e8U,
	PIN_CAM1_DATA1             = 0x09ecU,
	PIN_CAM1_DATA2             = 0x09f0U,
	PIN_CAM1_DATA3             = 0x09f4U,
	PIN_CAM1_DATA4             = 0x09f8U,
	PIN_CAM1_DATA5             = 0x09fcU,
	PIN_CAM1_DATA6             = 0x0a00U,
	PIN_CAM1_DATA7             = 0x0a04U,
	PIN_CAM0_DATA0             = 0x0a08U,
	PIN_CAM0_DATA1             = 0x0a0cU,
	PIN_CAM0_DATA2             = 0x0a10U,
	PIN_CAM0_DATA3             = 0x0a14U,
	PIN_CAM0_DATA4             = 0x0a18U,
	PIN_CAM0_DATA5             = 0x0a1cU,
	PIN_CAM0_DATA6             = 0x0a20U,
	PIN_CAM0_DATA7             = 0x0a24U,
	PIN_UART3_RXD              = 0x0a28U,
	PIN_UART3_TXD              = 0x0a2cU,
	PIN_UART3_CTSN             = 0x0a30U,
	PIN_UART3_RTSN             = 0x0a34U,
	PIN_GPIO5_8                = 0x0a38U,
	PIN_GPIO5_9                = 0x0a3cU,
	PIN_GPIO5_10               = 0x0a40U,
	PIN_GPIO5_11               = 0x0a44U,
	PIN_GPIO5_12               = 0x0a48U,
	PIN_GPIO5_13               = 0x0a4cU,
	PIN_SPI4_SCLK              = 0x0a50U,
	PIN_SPI4_D0                = 0x0a54U,
	PIN_SPI4_D1                = 0x0a58U,
	PIN_SPI4_CS0               = 0x0a5cU,
	PIN_SPI2_SCLK              = 0x0a60U,
	PIN_SPI2_D0                = 0x0a64U,
	PIN_SPI2_D1                = 0x0a68U,
	PIN_SPI2_CS0               = 0x0a6cU,
	PIN_XDMA_EVENT_INTR0       = 0x0a70U,
	PIN_XDMA_EVENT_INTR1       = 0x0a74U,
	PIN_CLKREQ                 = 0x0a78U,
	PIN_WARMRSTN               = 0x0a7cU,
	PIN_PWRONRSTN              = 0x0a80U,
	PIN_EXTINTN                = 0x0a84U,
	PIN_XTALIN                 = 0x0a88U,
	PIN_XTALOUT                = 0x0a8cU,
	PIN_TMS                    = 0x0a90U,
	PIN_TDI                    = 0x0a94U,
	PIN_TDO                    = 0x0a98U,
	PIN_TCK                    = 0x0a9cU,
	PIN_NTRST                  = 0x0aa0U,
	PIN_EMU0                   = 0x0aa4U,
	PIN_EMU1                   = 0x0aa8U,
	PIN_RTC_XTALIN             = 0x0aacU,
	PIN_RTC_XTALOUT            = 0x0ab0U,
	PIN_RTC_PWRONRSTN          = 0x0ab4U,
	PIN_RTC_WAKEUP             = 0x0ab8U,
	PIN_RTC_PMIC_EN            = 0x0abcU,
	PIN_USB0_DRVVBUS           = 0x0ac0U,
	PIN_USB1_DRVVBUS           = 0x0ac4U,
	PIN_OFFSET_MAX             = 0x0ac4U
};


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/** \brief Pinmux configuration data for the board. Auto-generated from 
           Pinmux tool. */
extern pinmuxBoardCfg_t gGpevmPinmuxData[];
extern pinmuxBoardCfg_t gEvmskPinmuxData[];
extern pinmuxBoardCfg_t gIdkevmPinmuxData[];
extern pinmuxBoardCfg_t gEposevmPinmuxData[];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* AM43XX_PIN_MUX_H_ */
