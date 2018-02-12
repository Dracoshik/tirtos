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

/**
 *   @file  emac_test.c
 *
 *   @brief
 *      This is the Loopback test code for the EMAC Driver. The test code 
 *      showcases usage of the EMAC Driver exported API for sending/receiving
 *      Ethernet packets.
 *
 */
 
#include <string.h>

#include <ti/csl/cslr_emac.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/soc/k2g/src/cslr_soc_baseaddress.h>
#include <ti/csl/cslr_msmc.h>
#include <ti/csl/csl_cpsw.h>
#include <ti/csl/csl_mdioAux.h>


/* EMAC Driver Header File. */
#include <ti/drv/emac/emac_drv.h>
#include <ti/drv/emac/soc/emac_soc_v1.h>

/* Test application local header file */
#include "test_loc.h"

#ifdef SOC_K2G
#include "diag_common_cfg.h"
#endif

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/board/src/iceK2G/include/iceK2G_ethernet_config.h>



/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

EMAC_LINK_INFO_T link_info;
uint32_t linkUp = 0;
extern void BOARD_delay(uint32_t usecs);

uint8_t  app_pkt_buffer[APP_TOTAL_PKTBUF_SIZE] __attribute__ ((aligned (128)));

APP_EMAC_MCB_T   app_mcb;


#define APP_NUM_HOST_DESC 128

uint8_t  gHostDesc[EMAC_SIZE_HOST_DESC * APP_NUM_HOST_DESC] __attribute__ ((aligned (128)));

uint8_t test_mac_addr[6] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
static uint32_t pkt_rcv_count = 0;
uint32_t linkCheckTime = 0;


/**********************************************************************
 ****************** Test Configuration Variables **********************
 **********************************************************************/

#define PKT_SEND_COUNT  10

/* DO NOT CHANGE test_pkt UNLESS TEST_PKT_SIZE IS UPDATED */

#define TEST_PKT_SIZE       64

static const uint8_t test_pkt[64] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* broadcast mac */
    0x01, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00,0x01,
    0x01, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0xc0, 0xa8, 0x01, 0x16,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xc0, 0xa8,0x01, 0x02,
    0x01,0x02,0x03,0x04,
    0x01,0x02,0x03,0x04,
    0x01,0x02,0x03,0x04,
    0x01,0x02,0x03,0x04,
    0x01,0x02,0x03,0x04,
    0xfe,0xfe
};


/**********************************************************************
 ************************ EMAC TEST FUNCTIONS *************************
 **********************************************************************/

/**
 *  @b app_queue_pop
 *  @n
 *      Dequeues a packet descriptor from an app queue.
 *
 *  @param[in]  
 *   pq       Packet queue of type APP_PKT_QUEUE_T .
 *   port_num EMAC port number
 *
 *  @retval
 *      EMAC_Pkt popped from the queue.
 */
static EMAC_PKT_DESC_T*
app_queue_pop
(
    Uint32              port_num,
    APP_PKT_QUEUE_T*    pq
)
{
    EMAC_PKT_DESC_T*    pPktHdr;

    if (!pq->Count)
    {
        return 0;
    }

    pPktHdr = pq->pHead;
    if( pPktHdr )
    {
        pq->pHead = pPktHdr->pNext;
        pq->Count--;
        pPktHdr->pPrev = pPktHdr->pNext = 0;
    }

    return( pPktHdr );
}

/**
 *  @b app_queue_push
 *  @n
 *      Enqueues a packet in EMAC_Pkt queue.
 *
 *  @param[in]  
 *   pq       Packet queue of type EMAC_PKT_QUEUE_T .
 *   pPktHdr  EMAC_PKT_DESC_T type packet to push.
 *   port_num EMAC port number
 *
 *  @retval
 *      void
 */
static void
app_queue_push
(
    Uint32              port_num,
    APP_PKT_QUEUE_T*    pq,
    EMAC_PKT_DESC_T*    pPktHdr
)
{
    pPktHdr->pNext = 0;

    if( !pq->pHead )
    {
        /* Queue is empty - Initialize it with this one packet */
        pq->pHead = pPktHdr;
        pq->pTail = pPktHdr;
    }
    else
    {
        /* Queue is not empty - Push onto end */
        pq->pTail->pNext = pPktHdr;
        pq->pTail        = pPktHdr;
    }
    pq->Count++;
}



/**
 *  @b Description
 *  @n
 *      Call back function provided by application for EMAC driver
 *      to allocate a packet descriptor.
 *
 *  @param[in]
 *   port_num EMAC port number 
 *   pkt_size size of the packet
 *
 *  @retval
 *      pointer to the allocated packet descriptor.
 */
static EMAC_PKT_DESC_T*
app_alloc_pkt
(
    Uint32              port_num,
    Uint32              pkt_size
)
{
    EMAC_PKT_DESC_T*    p_pkt_desc = NULL;

    if (pkt_size < APP_EMAC_MAX_PKT_SIZE)
    {
        /* Get a packet descriptor from the free queue */
        p_pkt_desc              = app_queue_pop(port_num,
		                                        &app_mcb.emac_pcb[port_num].freeQueue);
        p_pkt_desc->AppPrivate  = (Uint32)p_pkt_desc;
        p_pkt_desc->BufferLen   = pkt_size;
        p_pkt_desc->DataOffset  = 0;
    }
    else
    {
        UART_printf ("app_alloc_pkt on port %d failed, packet size %d is big\n"
		              , port_num, pkt_size);
        return NULL;
    }
    return p_pkt_desc;
}

/**
 *  @b Description
 *  @n
 *      Call back function provided by application for EMAC driver
 *      to free a packet descriptor.
 *
 *  @param[in]
 *   port_num   EMAC port number 
 *   p_pkt_desc Packet descriptor of type EMAC_PKT_DESC_T .
 *
 *  @retval
 *      None.
 */
static void
app_free_pkt
(
    Uint32              port_num,
    EMAC_PKT_DESC_T*    p_pkt_desc
)
{
    /* Free a packet descriptor to the free queue */
    app_queue_push(port_num, &app_mcb.emac_pcb[port_num].freeQueue,
                   (EMAC_PKT_DESC_T *)p_pkt_desc->AppPrivate);
}

/**
 *  @b app_display_linkstatus
 *  @n
 *      This function is called whenever there is a change in link state on
 *      master core.
 *
 *  @param[in]  port_num
 *      EMAC port number.
 *  @param[in]  link_status
 *      Status of the link.
 *
 *  @retval
 *      void
 */
void
app_display_linkstatus
(
    Uint32              port_num,
    Uint32              link_status
)
{
    /* This string array corresponds to link state as defined in csl_mdio.h */
    char *LinkStr[] = { "No Link",
                        "10Mb/s Half Duplex",
                        "10Mb/s Full Duplex",
                        "100Mb/s Half Duplex",
                        "100Mb/s Full Duplex",
                        "1000Mb/s Full Duplex" };

    if (app_mcb.core_num == APP_MASTER_CORE_NUM)
    {
        UART_printf("Port %d Link Status: %s on PHY %d\n",
                      port_num, LinkStr[link_status],
                      app_mcb.emac_pcb[port_num].phy_addr);
    }
}


/**
*  @b Description
*  @n
*      Intialize the application control block, free/rx packet queue.
*
*  @retval
*      None.
*/
void
app_init(void)
{
    Uint32              i, j;
    EMAC_PKT_DESC_T*    p_pkt_desc;
    Uint8*              pktbuf_ptr;

    UART_printf ("EMAC loopback test application initialization\n");

    /* Reset application control block */
    memset(&app_mcb, 0, sizeof (APP_EMAC_MCB_T));

    app_mcb.core_num = 0;
    pktbuf_ptr = (Uint8 *) ((Uint32) app_pkt_buffer | 0x10000000);

    /* Initialize the free packet queue */
    for (i=0; i<MAX_NUM_EMAC_PORTS; i++)
    {
        if (i == 0)
        {
            app_mcb.emac_pcb[0].phy_addr   = APP_PORT0_PHY_ADDR;
        }

        for (j=0; j<APP_MAX_PKTS; j++)
        {
            p_pkt_desc               = &app_mcb.emac_pcb[i].pkt_desc[j];
            p_pkt_desc->pDataBuffer  = pktbuf_ptr;
            p_pkt_desc->BufferLen    = APP_EMAC_MAX_PKT_SIZE;
            app_queue_push( i, &app_mcb.emac_pcb[i].freeQueue, p_pkt_desc );
            pktbuf_ptr += APP_EMAC_MAX_PKT_SIZE;
        }
    }
}

/**
 *  @brief  This function is used to call back the network application when a
 *          packet is received.
 * 
 *  @param[in]
 *   port_num  EMAC port number
 *   p_desc    packet descriptor of type EMAC_PKT_DESC_T
 *
 *  @retval
 *      None
 */
void app_test_rx_pkt_cb(Uint32 port_num, EMAC_PKT_DESC_T* p_desc)
{
	/*Change buffer length to not compare checksum data*/
	p_desc->BufferLen = 64;
    if (memcmp(p_desc->pDataBuffer, test_pkt, p_desc->BufferLen) == 0)
    {
        pkt_rcv_count++;
        UART_printf("Received Packet: %d\n", pkt_rcv_count);
    }
    else
    {
        UART_printf("packet match failed\n");
    }

    app_free_pkt(port_num,  (EMAC_PKT_DESC_T*) p_desc->AppPrivate);
}

/**
 *  @brief  This function is used to send packets to the specified emac port         
 * 
 *  @retval
 *      None
 */
static void app_test_task_send_pkts(void)
{
    uint32_t index;
    uint32_t port_num = 0;
    static uint32_t pkt_send_count =0;

    for (index = 0; index < PKT_SEND_COUNT; index++)
    {
        UART_printf("Sending Packet: %d\n", (index+1));
        EMAC_PKT_DESC_T *p_pkt_desc = app_alloc_pkt(0, TEST_PKT_SIZE);

        memcpy (p_pkt_desc->pDataBuffer, &test_pkt[0], TEST_PKT_SIZE);
        p_pkt_desc->AppPrivate = (uint32_t)p_pkt_desc;
        p_pkt_desc->Flags        = 0;
        p_pkt_desc->ValidLen     = TEST_PKT_SIZE;
        p_pkt_desc->DataOffset   = 0;
        p_pkt_desc->PktChannel   = 0;
        p_pkt_desc->PktLength    = TEST_PKT_SIZE;
        p_pkt_desc->BufferLen = TEST_PKT_SIZE;
        p_pkt_desc->PktFrags     = 1;
        p_pkt_desc->pNext = NULL;
        p_pkt_desc->pPrev = NULL;


        emac_send(port_num, p_pkt_desc);
        pkt_send_count++;
    }
}

/** ============================================================================
 *   @n@b Init_MDIO
 *
 *   @b Description
 *   @n Initialises the phy o enter external loopback mode
 *
 *   @param[in]
 *   @n None
 *
 *   @return
 *   @n None
 * =============================================================================
 */
int Init_MDIO (void)
{
	uint16_t regData;
	uint32_t baseAddr = (CSL_NSS_0_CFG_REGS + 0x00200F00);

	linkCheckTime = 0;

	/*
     * Configure Ethernet PHY to enter External Loopback mode
     * reg 0: 0x2100
     * reg 9: 0x1000
     */
	UART_printf("Configuring Phy\n");
	/* MDIO is not reinitialized here as it is done by Board_gigEthConfig */
    CSL_MDIO_phyRegWrite(baseAddr, BOARD_GIGABIT_EMAC_PHY_ADDR, 0, 0x2100);
    CSL_MDIO_phyRegWrite(baseAddr, BOARD_GIGABIT_EMAC_PHY_ADDR, 9, 0x1000);

	UART_printf("Waiting for Link Status\n");
	regData = 0;
	while ((regData & 0x400) == 0)
	{
		CSL_MDIO_phyRegRead(baseAddr, BOARD_GIGABIT_EMAC_PHY_ADDR, 0x11, &regData);
		BOARD_delay(10000);
		linkCheckTime++;
		if(linkCheckTime > 1000)
		{
			return 1;
		}
	}

	UART_printf("Link is UP!!\n\n");
	return 0;
}

/*****************************************************************
 * FUNCTION PURPOSE: Power up PA subsystem
 *****************************************************************
 * DESCRIPTION: this function powers up the PA subsystem domains
 *****************************************************************/
void nssPowerUp (void)
{
    /* PASS power domain is turned OFF by default. It needs to be turned
     * on before doing any PASS device register access. 
	 * This not required for the simulator. */

    /* Set NSS Power domain to ON */
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_NSS);

    /* Enable the clocks for NSS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_NSS, PSC_MODSTATE_ENABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_NSS);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_NSS));
}

/*****************************************************************
 * FUNCTION PURPOSE: Power down PA subsystem
 *****************************************************************
 * DESCRIPTION: this function powers down the PA subsystem domains
 *****************************************************************/
void nssPowerDown (void)
{
    /* PASS power domain is turned OFF by default. It needs to be turned
     * on before doing any PASS device register access.
	 * This not required for the simulator. */

    /* Set NSS Power domain to ON */
    CSL_PSC_disablePowerDomain (CSL_PSC_PD_NSS);

    /* Enable the clocks for NSS modules */
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_NSS, PSC_MODSTATE_DISABLE);

    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_NSS);

    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_NSS));
}

/* Function to read Ethernet PHY status and strapping registers
   for debug purpose */
void getPhyRegDump(uint32_t baseAddr, uint32_t phyAddr)
{
    uint16_t regAddr;
    uint16_t regVal = 0;

    UART_printf("\n******************************************************\n");
    UART_printf("Register Dump for PHY Addr - 0x%04X\n", phyAddr);

    for (regAddr = 0; regAddr < BOARD_GIGABIT_EMAC_REG_DUMP_MAX; regAddr++)
    {
        CSL_MDIO_phyRegRead(baseAddr, phyAddr, regAddr, &regVal);
        UART_printf("PHY Register 0x%04X - 0x%04X\n", regAddr, regVal);
    }

    Mdio_PhyExtendedRegRead(baseAddr, phyAddr, BOARD_GIGABIT_EMAC_STRAP_STS1_ADDR, &regVal);
    UART_printf("PHY Register(STRAP1) 0x%04X - 0x%04X\n", BOARD_GIGABIT_EMAC_STRAP_STS1_ADDR, regVal);
    Mdio_PhyExtendedRegRead(baseAddr, phyAddr, BOARD_GIGABIT_EMAC_STRAP_STS2_ADDR, &regVal);
    UART_printf("PHY Register(STRAP2) 0x%04X - 0x%04X\n", BOARD_GIGABIT_EMAC_STRAP_STS2_ADDR, regVal);
    UART_printf("\n******************************************************\n\n");
}

EMAC_MAC_ADDR_T macTest;

/**
*  @b Description
*  @n
*      Entry point for the application test code.
*
*  @retval
*      None.
*/
int main(void)
{
    Board_initCfg boardCfg;
    EMAC_HwAttrs_V1 emac_cfg;
    EMAC_OPEN_CONFIG_INFO_T open_cfg;
    EMAC_CONFIG_INFO_T cfg_info;
    EMAC_DRV_ERR_E open_ret;
    EMAC_CHAN_MAC_ADDR_T        chan_cfg[APP_EMAC_NUM_CHANS_PER_CORE];
    uint32_t mac1 = 0;
    uint32_t mac2 = 0;
	uint32_t port_num = 0;
    uint32_t baseAddr;

#ifdef SOC_K2G
    DIAG_IntrInit();
#endif

    nssPowerUp();

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO |
               BOARD_INIT_ETH_PHY;
#else
    boardCfg = BOARD_INIT_UART_STDIO |
               BOARD_INIT_ETH_PHY;
#endif

    Board_init(boardCfg);

	UART_printf  ("************************************************\n"); 
	UART_printf  ("*             ETHERNET LOOPBACK Test           *\n");
	UART_printf  ("************************************************\n");

    UART_printf("\n\nReading Ethernet PHY Register Dump...\n");
    baseAddr = (CSL_NSS_0_CFG_REGS + 0x00200F00);
    getPhyRegDump(baseAddr, BOARD_GIGABIT_EMAC_PHY_ADDR);

    EMAC_socGetInitCfg(0, &emac_cfg);
    /* Need to update the descriptor base address */
    emac_cfg.p_desc_base = &gHostDesc;

    /* Now set the config after updating desc base address */
    EMAC_socSetInitCfg(0, &emac_cfg);

    app_init();

    open_cfg.hwAttrs = (void*)&emac_cfg;
    open_cfg.alloc_pkt_cb = app_alloc_pkt;
    open_cfg.free_pkt_cb = app_free_pkt;
    open_cfg.loop_back = 0;
    open_cfg.master_core_flag = 1;
    open_cfg.max_pkt_size = 1514;
    open_cfg.mdio_flag = 1;
    open_cfg.num_of_chans = 1;
    open_cfg.num_of_rx_pkt_desc = 64;
    open_cfg.num_of_tx_pkt_desc = 64;
    open_cfg.phy_addr = 1;

    open_cfg.p_chan_mac_addr = &chan_cfg[0];

    open_cfg.rx_pkt_cb = app_test_rx_pkt_cb;


    /* Set the channel configuration */
    chan_cfg[0].chan_num = 0;
    chan_cfg[0].num_of_mac_addrs = 1;
    CSL_BootCfgGetMacIdentifier(&mac1, &mac2);

    macTest.addr[0] = ((mac2 & 0x0000ff00) >> 8);
    macTest.addr[1] =  (mac2 & 0x000000ff);
    macTest.addr[2] = ((mac1 & 0xff000000) >> 24);
    macTest.addr[3] = ((mac1 & 0x00ff0000) >> 16);
    macTest.addr[4] = ((mac1 & 0x0000ff00) >> 8);
    macTest.addr[5] =  (mac1 & 0x000000ff);

     chan_cfg[0].p_mac_addr = & macTest;

    if ((open_ret = emac_open(0, &open_cfg)) == EMAC_DRV_RESULT_OPEN_PORT_ERR)
    {
        UART_printf("main: emac_open failure: %d\n", open_ret);
    }
    else
        UART_printf("main: emac_open sucess\n");

    cfg_info.mcast_cnt = 0;
    cfg_info.p_mcast_list = NULL;
    cfg_info.rx_filter = EMAC_PKTFLT_MULTICAST;;
    emac_config(port_num, &cfg_info);

    CSL_CPGMAC_SL_disableGigabit(0);
    if(Init_MDIO())
    {
		UART_printf("LINK Timeout!\n");
		UART_printf("EMAC Test Failed!!\n");
		emac_close(port_num);
		nssPowerDown();
		return (1);
	}

    app_test_task_send_pkts();
	
	/*Adjust Delay according to number of packets*/
	BOARD_delay(4000000);  
	emac_close(port_num);
	if (pkt_rcv_count ==  PKT_SEND_COUNT)
    {
    	UART_printf("\npackets sent: %d, packets received: %d\n", 
		             PKT_SEND_COUNT, pkt_rcv_count);
        UART_printf("\nEthernet Loopback test passed\n");
    }
	else
	{
		UART_printf("\nEthernet Loopback test failed!!\n");
	}

	/* Powerdown the NSS so that re-configuration will be clean while 
	   running the test multiple times */
	nssPowerDown();

	UART_printf("All tests completed\n");
    return (0);
}
