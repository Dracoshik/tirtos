#
# Copyright (c) 2013-2017, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#Default build environment, windows or linux
ifeq ($(OS),)
  #if nothing is defined, default to linux
  OS ?= linux
endif

#Default BUILD_OS_TYPE (tirtos/baremetal)
BUILD_OS_TYPE ?= tirtos

# Default board
# Supported values: tda2xx-evm, am572x-evm, evmDRA72x, evmDRA75x, tda3xx-evm, tda3xx-rvp, evmDRA78x,
#                   tda2px-evm,
#                   tda3xx-evm-radar, tda2xx-evm-radar, tda3xx-ar12-booster,
#                   tda3xx-ar12-alps, tda3xx-ar12-rvp,
#                   am572x-idk am571x-evm,  evmAM572x, idkAM571x, idkAM572x idkAM574x
#                   evmK2H, evmK2K, evmK2E, evmK2L, evmK2G, evmC6678, evmC6657,
#                   evmAM335x, icev2AM335x, iceAMIC110, skAM335x, bbbAM335x,
#                   evmAM437x idkAM437x skAM437x evmOMAPL137 lcdkOMAPL138
#
BOARD ?= bbbAM335x myBBB
SOC ?= am335x

#Below applicable only for TDA devices
mmwavelink_version := mmwave_dfp_00_09_01_05

ifeq ($(OS),Windows_NT)
#Paths for windows machine
  TOOLCHAIN_PATH_GCC ?= E:/uC-Soft/TI/TIRTOS/gcc-arm-none-eabi-6-2017-q1-update
  TOOLCHAIN_PATH_TI_CGT ?= c:/ti/ti-cgt-arm_16.9.1.LTS
  C6X_GEN_INSTALL_PATH ?= c:/ti/C6000_7.4.2
  TOOLCHAIN_PATH_EVE ?= c:/ti/arp32_1.0.7
  PDK_INSTALL_PATH ?= E:/uC-Soft/TI/TIRTOS/pdk_am335x_1_0_9/packages
  BIOS_INSTALL_PATH ?= E:/uC-Soft/TI/TIRTOS/bios_6_53_02_00
  XDC_INSTALL_PATH ?= E:/uC-Soft/TI/TIRTOS/xdctools_3_50_04_43_core
  EDMA3LLD_BIOS6_INSTALLDIR ?= E:/uC-Soft/TI/TIRTOS/edma3_lld_2_12_05_30B
  #Required only for TDA RADAR build
  radarLink_PATH := c:/ti/$(mmwavelink_version)
  #Required only for TDA Secure build
  MSHIELD_DK_DIR ?= c:/ti/mshield-dk_std_4_5_3
  #Required only for library size output generation
  CG_XML_BIN_INSTALL_PATH ?= E:/uC-Soft/TI/TIRTOS/cg_xml/bin
  #Not used for TDA device build
  CL_PRU_INSTALL_PATH ?= E:/uC-Soft/TI/TIRTOS/ti-cgt-pru_2.1.5
  NDK_INSTALL_PATH ?=  E:/uC-Soft/TI/TIRTOS/ndk_2_26_00_08
# Utilities directory. This is required only if the build machine is Windows.
#   - specify the installation directory of utility which supports POSIX commands
#     (eg: Cygwin installation or MSYS installation).
# This could be in CCS install directory as in c:/ti/ccsv7/utils/cygwin or
# the XDC install bin folder $(XDC_INSTALL_PATH)/bin
  utils_PATH ?= E:/uC-Soft/TI/ccsv7/utils/cygwin
  export utils_PATH
else
#Paths for linux machine
  TOOLCHAIN_PATH_GCC ?= /opt/tools/gcc-arm-none-eabi-4_9-2015q3
  TOOLCHAIN_PATH_TI_CGT ?= /opt/tools/cgtools/ti-cgt-arm_16.9.1.LTS
  C6X_GEN_INSTALL_PATH ?= /opt/tools/cgtools/C6000_7.4.2
  TOOLCHAIN_PATH_EVE ?= /opt/tools/cgtools/arp32_1.0.7
  PDK_INSTALL_PATH ?= /opt/ti/pdk/packages
  BIOS_INSTALL_PATH ?= /opt/tools/bios6/bios_6_46_04_53
  XDC_INSTALL_PATH ?= /opt/tools/xdc/xdctools_3_32_01_22_core
  EDMA3LLD_BIOS6_INSTALLDIR ?= /opt/tools/edma3/edma3_lld_02_12_00_20
  #Required only for TDA RADAR build
  radarLink_PATH := /opt/ti/$(mmwavelink_version)
  #Required only for TDA Secure build
  MSHIELD_DK_DIR ?= /opt/tools/mshield-dk_std_4_5_3
  #Required only for library size output generation
  CG_XML_BIN_INSTALL_PATH ?= /opt/ti/cg_xml_2_41_00/bin
  #Not used for TDA device build
  CL_PRU_INSTALL_PATH ?= /opt/ti-cgt-pru_2.1.4
  NDK_INSTALL_PATH ?= /opt/tools/ndk/ndk_2_24_03_35
endif

TOOLCHAIN_PATH_A8 ?= $(TOOLCHAIN_PATH_GCC)
TOOLCHAIN_PATH_A9 ?= $(TOOLCHAIN_PATH_GCC)
TOOLCHAIN_PATH_ARM9 ?= $(TOOLCHAIN_PATH_TI_CGT)
TOOLCHAIN_PATH_Arm9 ?= $(TOOLCHAIN_PATH_ARM9)
TOOLCHAIN_PATH_A15 ?= $(TOOLCHAIN_PATH_GCC)
TOOLCHAIN_PATH_M4 ?= $(TOOLCHAIN_PATH_TI_CGT)


# GCC versioning information
GCC_VERSION_HARDLIB=6.3.1

ifeq ($(SOC),$(filter $(SOC), am335x))
  HARDLIB_PATH ?= $(TOOLCHAIN_PATH_A8)/lib/gcc/arm-none-eabi/$(GCC_VERSION_HARDLIB)/hard
else ifeq  ($(SOC),$(filter $(SOC), am437x))
  HARDLIB_PATH ?= $(TOOLCHAIN_PATH_A9)/lib/gcc/arm-none-eabi/$(GCC_VERSION_HARDLIB)/hard
else
  HARDLIB_PATH ?= $(TOOLCHAIN_PATH_A15)/lib/gcc/arm-none-eabi/$(GCC_VERSION_HARDLIB)/hard
endif

# Default to m4 build depending on BOARD selected!!
ifeq ($(BOARD),$(filter $(BOARD), evmAM572x idkAM572x idkAM571x idkAM574x ))
  CORE ?= a15_0
endif
CORE ?= ipu1_0

# Default Build Profile
# Supported Values: debug | release
BUILD_PROFILE ?= release

# Set Core Build Profile depending on BUILD_PROFILE value
BUILD_PROFILE_$(CORE) ?= $(BUILD_PROFILE)

# Default PACKAGE_SELECT build flag
# Supported values: all, vps-hal-only, vps-vip-only, vps-vpe-only, vps-dss-only, vps-vip-dss, vps-vip-vpe
PACKAGE_SELECT ?= all

TREAT_WARNINGS_AS_ERROR ?= yes

# Disable recursive building of example dependencies
DISABLE_RECURSE_DEPS ?= no

# Default C++ build flag, yes or no
CPLUSPLUS_BUILD ?= no

#use <module>_PATH variable as makefile internally expects PATH variable this way for external component path
pdk_PATH := $(PDK_INSTALL_PATH)
bios_PATH := $(BIOS_INSTALL_PATH)
xdc_PATH := $(XDC_INSTALL_PATH)
edma3_lld_PATH := $(EDMA3LLD_BIOS6_INSTALLDIR)
ndk_PATH := $(NDK_INSTALL_PATH)
TI_SECURE_DEV_PKG := $(MSHIELD_DK_DIR)

ROOTDIR := $(pdk_PATH)
XDCPATH =
ifeq ($(BUILD_OS_TYPE),tirtos)
  XDCPATH = $(bios_PATH)/packages;$(xdc_PATH)/packages;$(edma3_lld_PATH)/packages;$(ndk_PATH)/packages;$(pdk_PATH);
endif

#Default SECTTI SIZE INFORMATION
SECTTI_SIZE_INFO ?= no

#Default SECTTI tool
SECTTI ?= $(CG_XML_BIN_INSTALL_PATH)/sectti

# Individual component path, needed to build a component in a different location
# as that of PDK
PDK_CSL_ROOT_PATH ?= $(pdk_PATH)
PDK_CSL_COMP_PATH = $(PDK_CSL_ROOT_PATH)/ti/csl
export PDK_CSL_COMP_PATH
PDK_PM_ROOT_PATH ?= $(pdk_PATH)
PDK_PM_COMP_PATH = $(PDK_PM_ROOT_PATH)/ti/drv/pm
export PDK_PM_COMP_PATH
PDK_OSAL_ROOT_PATH ?= $(pdk_PATH)
PDK_OSAL_COMP_PATH = $(PDK_OSAL_ROOT_PATH)/ti/osal
export PDK_OSAL_COMP_PATH
PDK_I2C_ROOT_PATH ?= $(pdk_PATH)
PDK_I2C_COMP_PATH = $(PDK_I2C_ROOT_PATH)/ti/drv/i2c
export PDK_I2C_COMP_PATH
PDK_VPS_ROOT_PATH ?= $(pdk_PATH)
PDK_VPS_COMP_PATH = $(PDK_VPS_ROOT_PATH)/ti/drv/vps
export PDK_VPS_COMP_PATH
PDK_GPIO_ROOT_PATH ?= $(pdk_PATH)
PDK_GPIO_COMP_PATH = $(PDK_GPIO_ROOT_PATH)/ti/drv/gpio
export PDK_GPIO_COMP_PATH
PDK_FATFS_ROOT_PATH ?= $(pdk_PATH)
PDK_FATFS_COMP_PATH = $(PDK_FATFS_ROOT_PATH)/ti/fs/fatfs
export PDK_FATFS_COMP_PATH
PDK_MMCSD_ROOT_PATH ?= $(pdk_PATH)
PDK_MMCSD_COMP_PATH = $(PDK_MMCSD_ROOT_PATH)/ti/drv/mmcsd
export PDK_MMCSD_COMP_PATH
PDK_PCIE_ROOT_PATH ?= $(pdk_PATH)
PDK_PCIE_COMP_PATH = $(PDK_PCIE_ROOT_PATH)/ti/drv/pcie
export PDK_PCIE_COMP_PATH
PDK_USB_ROOT_PATH ?= $(pdk_PATH)
PDK_USB_COMP_PATH = $(PDK_USB_ROOT_PATH)/ti/drv/usb
export PDK_USB_COMP_PATH
PDK_MCASP_ROOT_PATH ?= $(pdk_PATH)
PDK_MCASP_COMP_PATH = $(PDK_MCASP_ROOT_PATH)/ti/drv/mcasp
export PDK_MCASP_COMP_PATH
PDK_PROFILING_ROOT_PATH ?= $(pdk_PATH)
PDK_PROFILING_COMP_PATH = $(PDK_PROFILING_ROOT_PATH)/ti/utils/profiling
export PDK_PROFILING_COMP_PATH
PDK_PRUSS_ROOT_PATH ?= $(pdk_PATH)
PDK_PRUSS_COMP_PATH = $(PDK_PRUSS_ROOT_PATH)/ti/drv/pruss
export PDK_PRUSS_COMP_PATH
PDK_NIMU_ROOT_PATH ?= $(pdk_PATH)
PDK_NIMU_COMP_PATH = $(PDK_NIMU_ROOT_PATH)/ti/transport/ndk/nimu
export PDK_NIMU_COMP_PATH
PDK_NIMU_ICSS_ROOT_PATH ?= $(pdk_PATH)
PDK_NIMU_ICSS_COMP_PATH = $(PDK_NIMU_ICSS_ROOT_PATH)/ti/transport/ndk/nimu_icss
export PDK_NIMU_ICSS_COMP_PATH
PDK_ICSS_EMAC_ROOT_PATH ?= $(pdk_PATH)
PDK_ICSS_EMAC_COMP_PATH = $(PDK_ICSS_EMAC_ROOT_PATH)/ti/drv/icss_emac
export PDK_ICSS_EMAC_COMP_PATH
PDK_EMAC_ROOT_PATH ?= $(pdk_PATH)
PDK_EMAC_COMP_PATH = $(PDK_EMAC_ROOT_PATH)/ti/drv/emac
export PDK_EMAC_COMP_PATH
PDK_UART_ROOT_PATH ?= $(pdk_PATH)
PDK_UART_COMP_PATH = $(PDK_UART_ROOT_PATH)/ti/drv/uart
export PDK_UART_COMP_PATH
PDK_SPI_ROOT_PATH ?= $(pdk_PATH)
PDK_SPI_COMP_PATH = $(PDK_SPI_ROOT_PATH)/ti/drv/spi
export PDK_SPI_COMP_PATH
PDK_BOARD_ROOT_PATH ?= $(pdk_PATH)
PDK_BOARD_COMP_PATH = $(PDK_BOARD_ROOT_PATH)/ti/board
export PDK_BOARD_COMP_PATH
PDK_GPMC_ROOT_PATH ?= $(pdk_PATH)
PDK_GPMC_COMP_PATH = $(PDK_GPMC_ROOT_PATH)/ti/drv/gpmc
export PDK_GPMC_COMP_PATH
PDK_AUD_ROOT_PATH ?= $(pdk_PATH)
PDK_AUD_COMP_PATH = $(PDK_AUD_ROOT_PATH)/ti/addon/aud
export PDK_AUD_COMP_PATH
PDK_SBL_ROOT_PATH ?= $(pdk_PATH)
PDK_SBL_COMP_PATH = $(PDK_SBL_ROOT_PATH)/ti/boot/sbl
export PDK_SBL_COMP_PATH
PDK_SBL_AUTO_ROOT_PATH ?= $(pdk_PATH)
PDK_SBL_AUTO_COMP_PATH = $(PDK_SBL_AUTO_ROOT_PATH)/ti/boot/sbl_auto
export PDK_SBL_AUTO_COMP_PATH
PDK_CMB_ROOT_PATH ?= $(pdk_PATH)
PDK_CMB_COMP_PATH = $(PDK_CMB_ROOT_PATH)/ti/addon/cmb
export PDK_CMB_COMP_PATH
PDK_BOARD_IND_ROOT_PATH ?= $(pdk_PATH)
PDK_BOARD_IND_COMP_PATH = $(PDK_BOARD_IND_ROOT_PATH)/ti/addon/board_ind
export PDK_BOARD_IND_COMP_PATH

#Below applicable only for TDA devices
PDK_DIAG_ROOT_PATH ?= $(pdk_PATH)
PDK_DIAG_COMP_PATH = $(PDK_DIAG_ROOT_PATH)/ti/diag
export PDK_DIAG_COMP_PATH
PDK_FIREWALL_L3L4_ROOT_PATH ?= $(pdk_PATH)
PDK_FIREWALL_L3L4_COMP_PATH = $(PDK_FIREWALL_L3L4_ROOT_PATH)/ti/drv/fw_l3l4
export PDK_FIREWALL_L3L4_COMP_PATH
PDK_IPCLITE_ROOT_PATH ?= $(pdk_PATH)
PDK_IPCLITE_COMP_PATH = $(PDK_IPCLITE_ROOT_PATH)/ti/drv/ipc_lite
export PDK_IPCLITE_COMP_PATH
PDK_TDA3X_SECURITY_ROOT_PATH ?= $(pdk_PATH)
PDK_TDA3X_SECURITY_COMP_PATH = $(PDK_TDA3X_SECURITY_ROOT_PATH)/ti/boot/sbl_auto/security
export PDK_TDA3X_SECURITY_COMP_PATH

#Below applicable only for TDA devices for backward compatibility;not applicable for PRSDK
PDK_NORFLASH_ROOT_PATH ?= $(pdk_PATH)
PDK_NORFLASH_COMP_PATH = $(PDK_NORFLASH_ROOT_PATH)/ti/boot/sbl_auto/norflash
export PDK_NORFLASH_COMP_PATH
PDK_QSPIFLASH_ROOT_PATH ?= $(pdk_PATH)
PDK_QSPIFLASH_COMP_PATH = $(PDK_QSPIFLASH_ROOT_PATH)/ti/boot/sbl_auto/qspiflash
export PDK_QSPIFLASH_COMP_PATH
PDK_STW_LLD_ROOT_PATH ?= $(pdk_PATH)
PDK_STW_LLD_COMP_PATH = $(PDK_STW_LLD_ROOT_PATH)/ti/drv/stw_lld
export PDK_STW_LLD_COMP_PATH
PDK_BSP_LLD_ROOT_PATH ?= $(pdk_PATH)
PDK_BSP_LLD_COMP_PATH = $(PDK_BSP_LLD_ROOT_PATH)/ti/drv/bsp_lld
export PDK_BSP_LLD_COMP_PATH

export OS
export BUILD_OS_TYPE
export BOARD
export SOC
export CORE
export BUILD_PROFILE
export BUILD_PROFILE_$(CORE)
export TOOLCHAIN_PATH_A15
export TOOLCHAIN_PATH_A8
export TOOLCHAIN_PATH_A9
export TOOLCHAIN_PATH_ARM9
export TOOLCHAIN_PATH_Arm9
export TOOLCHAIN_PATH_M4
export TOOLCHAIN_PATH_EVE
export HARDLIB_PATH
export C6X_GEN_INSTALL_PATH
export CL_PRU_INSTALL_PATH
export PDK_INSTALL_PATH
export bios_PATH
export xdc_PATH
export pdk_PATH
export edma3_lld_PATH
export ndk_PATH
export ROOTDIR
export XDCPATH
export PACKAGE_SELECT
export TREAT_WARNINGS_AS_ERROR
export DISABLE_RECURSE_DEPS
export CPLUSPLUS_BUILD
export mmwavelink_version
export radarLink_PATH
export MSHIELD_DK_DIR
export TI_SECURE_DEV_PKG
export SECTTI_SIZE_INFO
export SECTTI

ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif

include $(MAKERULEDIR)/build_config.mk
include $(MAKERULEDIR)/platform.mk
include $(MAKERULEDIR)/env.mk
