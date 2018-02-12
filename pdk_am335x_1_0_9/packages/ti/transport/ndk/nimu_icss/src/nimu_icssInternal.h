/**
 * \file nimu_icssInternal.h
 * \brief Internal Include file for NIMU Packet Architecture
 * 
 *
 * \copyright Copyright (c) 2012 Texas Instruments Incorporated ALL RIGHTS RESERVED
 *
*/
#ifndef _NIMU_ICSS_INTERNAL_H_
#define _NIMU_ICSS_INTERNAL_H_

#include <stdint.h>
#include "ti/ndk/inc/stkmain.h"
#include "ti/ndk/inc/os/osif.h"
#include "ti/drv/icss_emac/icss_emacDrv.h"


//#define printf(...) DbgPrintf(1, __VA_ARGS__) /* level 1 = DBG_INFO */
//#define printf   printf
//#define printf(...) DbgPrintf(1, __VA_ARGS__) /* level 1 = DBG_INFO */

/**
 * Pre-Pad Packet Data Offset
 *
 *   The TCP/IP stack library requires that every packet device
 *   include enough L2 header room for all supported headers. In
 *   order to support PPPoE, this requires a 22 byte L2 header.
 *   Thus, since standard Ethernet is only 14 bytes, we must add
 *   on an additional 8 byte offset, or PPPoE can not function
 *   with our driver.
*/
#define     PKT_PREPAD                      0

/** Indicates whether RAM based multicast lists are suported for this
 * EMAC peripheral.
 */
#define     RAM_MCAST                       0

/** Indicates whether HASH based multicasting is suported for this
 * EMAC peripheral.
 */
#define     HASH_MCAST                      0

/** Multicast Address List Size */
#define     PKT_MAX_MCAST                   31

/** Rx queue (one for all PKT devices) */
#ifndef _INCLUDE_NIMU_CODE
extern PBMQ    PBMQ_rx;
#endif

/** 
 * @brief 
 *  Packet device information
 *
 * @details
 *  This structure caches the device info.
 */
typedef struct _pdinfo 
{
    /**
     * @brief       Physical index of this device (0 to n-1).
     */        
    unsigned int           PhysIdx;
    /**
     * @brief       Handle to logical driver.
     */
    HANDLE          hEther;
    /**
     * @brief       Semaphore handle used by NDK stack and driver
     *              to communicate any pending Rx events that need 
     *              to be serviced by NDK ethernet stack.
     */            
    STKEVENT_Handle hEvent;
    /**
     * @brief       MAC Address
     */
    uint8_t         bMacAddr[6];
    /**
     * @brief       Current RX filter
     */
    unsigned int    Filter;
    /**
     * @brief       Previous MCast Address Counter
     */
    uint32_t        OldMCastCnt;
    /**
     * @brief       Previous Multicast list configured by the Application.
     */
    uint8_t         bOldMCast[6*PKT_MAX_MCAST];
    /**
     * @brief       Current MCast Address Countr
     */
    unsigned int    MCastCnt;
    /**
     * @brief       Multicast list configured by the Application.
     */
    uint8_t         bMCast[6*PKT_MAX_MCAST];
    /**
     * @brief       Transmitter "free" flag
     */
    unsigned int    TxFree;
    /**
     * @brief       Tx queue (one for each PKT device)
     */
    PBMQ            PBMQ_tx;
    /**
     * @brief       Rx queue (one for each PKT device)
     */            
    PBMQ            PBMQ_rx;
    ICSS_EmacSubSysHandle_T ICSS_EmacSubSysHandle;
} PDINFO;

/*
 * Functions Provided by Hw specific code
 */
/**
 *  \name HwPktInit
 *  \brief  Initializes the device MAC address to use.Not used Currently
 *  \param none
 *
 *  \return  Whether mac address has been initialized 1 - true 0 - false
 */
extern uint HwPktInit();
/**
 *  \name HwPktShutdown
 *  \brief none
 *
 *  \param none
 *
 *  \return none
 *
 */
extern void HwPktShutdown();
/**
 *  \name HwPktOpen
 *  \brief  Opens and configures EMAC. Configures Interrupts
 *
 *  \param  pi  PDINFO structure pointer.
 *
 *  \return Success(0) or failure(Error Codes defined)
 */
extern uint HwPktOpen( PDINFO *pi );
/**
 *  \name HwPktClose
 *  \brief  Closes EMAC and disables interrupts.
 *
 *  \param  pi  PDINFO structure pointer.
 *  \return none
 */
extern void HwPktClose( PDINFO *pi );
extern void HwPktSetRx( PDINFO *pi );
/**
 *  \name HwPktTxNext
 *  \brief  Routine to send out a packet.
 *
 *  \param pi PDINFO structure pointer.
 *
 *  \return none
 */
extern void HwPktTxNext( PDINFO *pi );
/**
 *  \name _HwPktPoll
 *  \brief This function is called at least every 100ms, faster in a
 *      polling environment. The fTimerTick flag is set only when
 *      called on a 100ms event.
 *
 *  \param  pi PDINFO structure pointer.
 *  \param  fTimerTick Flag for timer, set when called on a 100ms event.
 *
 *  \return none
 */
extern void _HwPktPoll( PDINFO *pi, uint fTimerTick );
/**
 *  \name HwPktIoctl
 *  \brief
 *  Low level driver Ioctl interface. This interface can be used for
 *  ALE configuration,control,statistics
 *
 *  \param  pi PDINFO structure pointer.
 *  \param cmd Ioctl command.
 *  \param pBuf Ioctl buffer with commands and params to set/get  configuration from hardware.
 *  \param size Size of Ioctl buffer
 *
 *  \return none
 */
extern uint32_t HwPktIoctl(PDINFO *pi, uint cmd, void* param, uint size);


/**number of Driver Devices*/
#define NIMU_DEVICE_NUM 3

#endif /* _NIMU_ICSS_INTERNAL_H_ */

