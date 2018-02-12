/*
 * Copyright (C) 2016 Texas Instruments Incorporated - http://www.ti.com/
 *
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
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
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

/*******************************************************************************
* FILE PURPOSE: Test Application Header Local Definitions
*
********************************************************************************
* FILE NAME:    test_loc.h
*
* DESCRIPTION:  This file contains all Local definitions for EMAC test application.
*******************************************************************************/
#ifndef __TESTLOC_H__
#define __TESTLOC_H__

/**********************************************************************
 ************************** LOCAL Definitions *************************
 **********************************************************************/

#define MAX_NUM_EMAC_PORTS 1


/**
 * @brief   Applicaton external memory start address
 */
#if defined(C6474) || defined(C6657)
#define     APP_EXTMEM                          0x80000000
#define		APP_MSMCMEM                 0x0C000000
#else
#define     APP_EXTMEM                          0xe0000000
#endif

/**
 * @brief   Master core number
 */
#define     APP_MASTER_CORE_NUM                 0


#ifdef SOC_K2G
/**
 * @brief   Number of channels configured by a core on one port
 */
#define     APP_EMAC_NUM_CHANS_PER_CORE         1

/**
 * @brief   Nummber of EMAC MAC addresses configured per channel
 */
#define     APP_EMAC_NUM_MACADDRS_PER_CHAN      1
#else
/**
 * @brief   Number of channels configured by a core on one port
 */
#define     APP_EMAC_NUM_CHANS_PER_CORE         8

/**
 * @brief   Nummber of EMAC MAC addresses configured per channel
 */
#define     APP_EMAC_NUM_MACADDRS_PER_CHAN      4
/**
 * @brief   Nummber of EMAC MAC addresses configured per channel
 */
#endif

/**
 * @brief   Max EMAC packet size in bytes initialized for the driver
 */
#define     APP_EMAC_INIT_PKT_SIZE              1514

/**
 * @brief   EMAC loopback Test TX packet size in bytes: (APP_EMAC_INIT_PKT_SIZE - 4 bytes CRC)
 */
#define     APP_EMAC_MAX_LB_TX_PKT_SIZE         (APP_EMAC_INIT_PKT_SIZE-4)

/**
 * @brief
 *  Minimum Ethernet packet payload size
 */
#define     APP_EMAC_MIN_PL_SIZE                46

/**
 * @brief
 *  Number of packets to be tested for each channel
 */
#define     APP_EMAC_ETH_TYPE_SIZE              2

/**
 * @brief
 *  Number of packets to be tested for each channel
 */
#define     APP_EMAC_HEADER_SIZE                (2*EMAC_MAC_ADDR_LENTH+APP_EMAC_ETH_TYPE_SIZE)

/**
 * @brief   Max packet size in bytes used in the application,
 *          align to 128 byte cache line size
 */
#define     APP_EMAC_MAX_PKT_SIZE               1536

/**
 * @brief   Max number of packets in the application free packet queue
 *
 */
#define     APP_MAX_PKTS                        128

/**
 * @brief   Max number of packet descriptors per port initialized
 *          for driver managed RX queue
 */
#define     APP_EMAC_INIT_RX_PKTS               (8*APP_EMAC_NUM_CHANS_PER_CORE)

/**
 * @brief   Max number of packet descriptors per port initialized
 *          for driver managed TX queue
 */
#define     APP_EMAC_INIT_TX_PKTS               (APP_MAX_PKTS-APP_EMAC_INIT_RX_PKTS)

/**
 * @brief   Total packet buffer size in bytes per core
 *
 */
#define     APP_TOTAL_PKTBUF_SIZE               (MAX_NUM_EMAC_PORTS*APP_MAX_PKTS*APP_EMAC_MAX_PKT_SIZE)

/**
 * @brief   EMAC port 0 phy address
 */

#ifdef SOC_K2G
#define     APP_PORT0_PHY_ADDR                  1 /* FOR NOW SET THIS TO 0 */
#endif

#ifdef C6472
#define     APP_PORT0_PHY_ADDR                  24
#endif

#ifdef C6457
#define     APP_PORT0_PHY_ADDR                  1
#endif

#ifdef C6474
#define     APP_PORT0_PHY_ADDR                  1
#endif

#ifdef C6657
#define     APP_PORT0_PHY_ADDR                  24
#endif

/**
 * @brief   EMAC port 1 phy address
 */
#define     APP_PORT1_PHY_ADDR                  25

/**
 * @brief   EMAC TX/RX Interrupt Definitions
 */
#define     APP_EMAC0_RXINT_ID                  7
#define     APP_EMAC0_TXINT_ID                  8
#define     APP_EMAC0_INT_FLAG                  (3<<APP_EMAC0_RXINT_ID)

#ifdef TWO_PORT_DEV
#define     APP_EMAC1_RXINT_ID                  9
#define     APP_EMAC1_TXINT_ID                  10
#define     APP_EMAC1_INT_FLAG                  (3<<APP_EMAC1_RXINT_ID)
#endif

#ifdef C6472
#define     APP_EMAC0_RX_INT_EV                 5
#define     APP_EMAC0_TX_INT_EV                 6

#define     APP_EMAC1_RX_INT_EV                 7
#define     APP_EMAC1_TX_INT_EV                 8
#endif

#ifdef C6457
#define     APP_EMAC0_RX_INT_EV                 25
#define     APP_EMAC0_TX_INT_EV                 26
#endif

#ifdef C6474
#define     APP_EMAC0_RX_INT_EV                 6
#define     APP_EMAC0_TX_INT_EV                 7
#endif

#ifdef C6657
#define     APP_EMAC0_RX_INT_EV                 99
#define     APP_EMAC0_TX_INT_EV                 94
#endif

/**
 * @brief
 *  Number of packets to be tested for each channel
 */
#define     APP_NUM_TEST_PKTS                   10240

/**
 * @brief
 *  DDR2 definitions
 */
#define DDR2C   ((CSL_Ddr2Regs *) CSL_DDR2_0_REGS)

/**
 * @brief
 *  Phy definitions
 */
//#define MDIOR   ((CSL_MdioRegs *) CSL_MDIO_0_REGS)

enum APP_ETHERNET_MODES_E
{
    PHY_LOOPBACK            = 0,
    COPPER_LOOPBACK,
    AMC_MODE,
    NORMAL_PHY_MODE
};

/**
 * @brief
 *  PLLC definitions
 */
#define PLLCTL_1	0x029A0100  // PLL1 control register
#define PLLM_1		0x029A0110	// PLL1 multiplier control register
#define PLLCMD_1	0x029A0138	// PLL1 controller command register
#define PLLSTAT_1	0x029A013C	// PLL1 controller status register
#define ALNCTL_1	0x029A0140	// PLL1 controller clock align control register
#define DCHANGE_1	0x029A0144	// PLL1 PLLDIV ratio change status register
#define SYSTAT_1	0x029A0150	// PLL1 SYSCLK status register
#define PLLDIV7_1	0x029A016C	// PLL1 controller divider 7 register
#define PLLDIV8_1	0x029A0170	// PLL1 controller divider 8 register
#define PLLDIV9_1	0x029A0174	// PLL1 controller divider 9 register
#define PLLDIV10_1	0x029A0178	// PLL1 controller divider 10 register

#define PLLCTL_2	0x029C0100	// PLL2 control register
#define PLLM_2		0x029C0110	// PLL2 multiplier control register
#define PLLDIV1_2	0x029C0118	// PLL2 controller divider 1 register
#define PLLDIV2_2	0x029C011C	// PLL2 controller divider 2 register
#define PLLDIV3_2	0x029C0120	// PLL2 controller divider 3 register
#define PLLCMD_2	0x029C0138	// PLL2 controller command register
#define PLLSTAT_2	0x029C013C	// PLL2 controller status register
#define ALNCTL_2	0x029C0140	// PLL2 controller clock align control register
#define DCHANGE_2	0x029C0144	// PLL2 PLLDIV ratio change status register
#define SYSTAT_2	0x029C0150	// PLL2 SYSCLK status register
#define PLLDIV4_2	0x029A0160	// PLL2 controller divider 4 register
#define PLLDIV5_2	0x029A0164	// PLL2 controller divider 5 register
#define PLLDIV6_2	0x029A0168	// PLL2 controller divider 6 register

#define PLLCTL_3	0x029C0500	// PLL3 control register
#define PLLM_3		0x029C0510	// PLL3 multiplier control register
#define PLLCMD_3	0x029C0538	// PLL3 controller command register
#define PLLSTAT_3	0x029C053C	// PLL3 controller status register

 /**
 * @brief
 *  Application Queue Data Structure
*/
typedef struct APP_PKT_QUEUE_tag
{
    uint32_t            Count;
    /**< Number of packets in queue */
    EMAC_PKT_DESC_T*  pHead;
    /**< Pointer to the first packet */
    EMAC_PKT_DESC_T*  pTail;
    /**< Pointer to the last packet */
} APP_PKT_QUEUE_T;

/**
 * @brief
 *  EMAC port states
 */
#define APP_EMAC_PORT_STATE_CLOSE       0
#define APP_EMAC_PORT_STATE_OPEN        1
#define APP_EMAC_PORT_STATE_LINKUP      2
#define APP_EMAC_PORT_STATE_LINKDN      3

/**
 * @brief
 *  Core specific EMAC port control block
 *
 * @details
 *  Maintains the EMAC port control information of a core
 */
typedef struct APP_EMAC_PCB_tag
{
    Uint32                          emac_state;
    /**< EMAC Port state */
    Uint32                          phy_addr;
    /**< Physical layer transceiver address mapped to the EMAC port */
    EMAC_PKT_DESC_T                 pkt_desc[APP_MAX_PKTS];
    /**< Pre-allocated/initialized packet descriptiors for both free queue and RX queues */
    APP_PKT_QUEUE_T                 freeQueue;
    /**< Free packet descriptor queue, one queue per channel */
    APP_PKT_QUEUE_T                 rxQueue[APP_EMAC_NUM_CHANS_PER_CORE];
    /**< Received packet descriptor queue, one queue per channel */
    EMAC_MAC_ADDR_T                 mac_addr[APP_EMAC_NUM_CHANS_PER_CORE][APP_EMAC_NUM_MACADDRS_PER_CHAN];
    /**< MAC address for all the channels */

} APP_EMAC_PCB_T;


/**
 * @brief
 *  EMAC Master Control Block
 *
 * @details
 *  Maintains the EMAC control information and error statistics.
 */
typedef struct APP_EMAC_MCB_tag
{
    Uint32              core_num;
    /**< DSP core number */
    Uint32              timer_count;
    /**< 100 msec timer count */
    APP_EMAC_PCB_T      emac_pcb[MAX_NUM_EMAC_PORTS];
    /**< EMAC port control block */
} APP_EMAC_MCB_T;

#ifdef _TMS320C6X
extern cregister volatile unsigned int  IER;
#endif

#endif /* __TESTLOC_H__ */

