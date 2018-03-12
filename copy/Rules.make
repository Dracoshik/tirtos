#
# Copyright (c) 2017, Texas Instruments Incorporated
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

ifeq ($(OS),Windows_NT)
    SDK_INSTALL_PATH ?= C:/ti
else
    #Paths for linux machine
    SDK_INSTALL_PATH ?= $(HOME)/ti
endif

TOOLS_INSTALL_PATH ?= $(SDK_INSTALL_PATH)

################################################################################
# Configure toolchain paths
################################################################################

# Cross compiler prefix
GCC_CROSS_TOOL_PREFIX=arm-none-eabi-
GCC_CROSS_TOOL_TAG=6-2017-q1-update

# Version of CG-Tools
CGT_VERSION=8.2.2

# Version of CG-PRU-Tools for PRUICSS
CGT_PRU_VERSION=2.1.4

# Version of HARDLIB for ARM
GCC_VERSION_HARDLIB=6.3.1

# Version of GCC for ARM A8
GCC_VERSION_ARM_A8=$(GCC_CROSS_TOOL_PREFIX)$(GCC_CROSS_TOOL_TAG)

# Version of GCC for ARM A9
GCC_VERSION_ARM_A9=$(GCC_CROSS_TOOL_PREFIX)$(GCC_CROSS_TOOL_TAG)

# Version of GCC for ARM A15
GCC_VERSION_ARM_A15=$(GCC_CROSS_TOOL_PREFIX)$(GCC_CROSS_TOOL_TAG)

# Version of CG-ARM-Tools
CGT_ARM_VERSION=16.9.7.LTS

# Version of EVE CG-Tools
CGT_ARP32_VERSION=1.0.8

CROSS_TOOL_PRFX      ?= $(GCC_CROSS_TOOL_PREFIX)
C6X_GEN_INSTALL_PATH ?= $(TOOLS_INSTALL_PATH)/ti-cgt-c6000_$(CGT_VERSION)
CL_PRU_INSTALL_PATH  ?= $(TOOLS_INSTALL_PATH)/ti-cgt-pru_$(CGT_PRU_VERSION)
HARDLIB_PATH         ?= $(TOOLCHAIN_PATH_A9)/lib/gcc/arm-none-eabi/$(GCC_VERSION_HARDLIB)/hard
TOOLCHAIN_PATH_A8    ?= $(TOOLS_INSTALL_PATH)/gcc-$(GCC_VERSION_ARM_A8)
TOOLCHAIN_PATH_A9    ?= $(TOOLS_INSTALL_PATH)/gcc-$(GCC_VERSION_ARM_A9)
TOOLCHAIN_PATH_Arm9  ?= $(TOOLS_INSTALL_PATH)/ti-cgt-arm_$(CGT_ARM_VERSION)
TOOLCHAIN_PATH_A15   ?= $(TOOLS_INSTALL_PATH)/gcc-$(GCC_VERSION_ARM_A15)
TOOLCHAIN_PATH_EVE   ?= $(TOOLS_INSTALL_PATH)/ccsv7/tools/compiler/arp32_$(CGT_ARP32_VERSION)
TOOLCHAIN_PATH_M4    ?= $(TOOLS_INSTALL_PATH)/ti-cgt-arm_$(CGT_ARM_VERSION)

export CROSS_TOOL_PRFX
export C6X_GEN_INSTALL_PATH
export CL_PRU_INSTALL_PATH
export HARDLIB_PATH
export TOOLCHAIN_PATH_A8
export TOOLCHAIN_PATH_A9
export TOOLCHAIN_PATH_Arm9
export TOOLCHAIN_PATH_A15
export TOOLCHAIN_PATH_EVE
export TOOLCHAIN_PATH_M4

################################################################################
# Configure SDK component paths
################################################################################

# Version of BIOS
BIOS_VERSION=6_53_02_00

# Version of the DSPLIB
DSPLIB_VERSION=c66x_3_4_0_0

# EDMA3 Version
EDMA_VERSION=2_12_05_30B

# Version of the IMGLIB
IMGLIB_VERSION=c66x_3_1_1_0

# Version of IPC
IPC_VERSION=3_47_00_00

# Version of the MATHLIB
MATHLIB_VERSION=c66x_3_1_1_0

# Version of the NDK
NDK_VERSION=2_26_00_08

# SoC & Version of PDK
PDK_SOC=am335x
PDK_VERSION=1_0_9

# Version of the UIA
UIA_VERSION=2_20_00_02

# Version of XDC
XDC_VERSION=3_50_04_43_core

BIOS_INSTALL_PATH         ?= $(SDK_INSTALL_PATH)/bios_$(BIOS_VERSION)
DSPLIB_INSTALL_PATH       ?= $(SDK_INSTALL_PATH)/dsplib_$(DSPLIB_VERSION)
EDMA3LLD_BIOS6_INSTALLDIR ?= $(SDK_INSTALL_PATH)/edma3_lld_$(EDMA_VERSION)
IMGLIB_INSTALL_PATH       ?= $(SDK_INSTALL_PATH)/imglib_$(IMGLIB_VERSION)
IPC_INSTALL_PATH          ?= $(SDK_INSTALL_PATH)/ipc_$(IPC_VERSION)
MATHLIB_INSTALL_PATH      ?= $(SDK_INSTALL_PATH)/mathlib_$(IMGLIB_VERSION)
NDK_INSTALL_PATH          ?= $(SDK_INSTALL_PATH)/ndk_$(NDK_VERSION)
PDK_INSTALL_PATH          ?= $(SDK_INSTALL_PATH)/pdk_$(PDK_SOC)_$(PDK_VERSION)/packages
UIA_INSTALL_PATH          ?= $(SDK_INSTALL_PATH)/uia_$(UIA_VERSION)
XDC_INSTALL_PATH          ?= $(SDK_INSTALL_PATH)/xdctools_$(XDC_VERSION)

export BIOS_INSTALL_PATH
export EDMA3LLD_BIOS6_INSTALLDIR
export DSPLIB_INSTALL_PATH
export IMGLIB_INSTALL_PATH
export IPC_INSTALL_PATH
export MATHLIB_INSTALL_PATH
export NDK_INSTALL_PATH
export PDK_INSTALL_PATH
export UIA_INSTALL_PATH
export XDC_INSTALL_PATH

################################################################################
# Remaining Environment Setup
################################################################################

LIBDIR ?= ./lib
export LIBDIR

export LIMIT_SOCS ?= am335x
export LIMIT_BOARDS ?= evmAM335x bbbAM335x

export CGTOOLS=$(C6X_GEN_INSTALL_PATH)

export XDCCGROOT=$(C6X_GEN_INSTALL_PATH)

################################################################################
# XDC PATH Configuration
################################################################################

export XDCPATH := ../../..;$(XDC_INSTALL_PATH)/packages;$(PDK_INSTALL_PATH);$(EDMA3LLD_BIOS6_INSTALLDIR)/packages;$(BIOS_INSTALL_PATH)/packages;$(IPC_INSTALL_PATH)/packages;$(NDK_INSTALL_PATH)/packages;$(UIA_INSTALL_PATH)/packages

################################################################################
# Build Tools Configuration
################################################################################

# Compiler Tools:
export PATH := $(PATH):$(C6X_GEN_INSTALL_PATH)/bin

# XDC Tools location:
export PATH := $(PATH):$(XDC_INSTALL_PATH):$(XDC_INSTALL_PATH)\bin:$(XDC_INSTALL_PATH)/packages/xdc/services/io/release

.PHONY: .env
.env:
	@echo "**************************************************************************"
	@echo "Tools Configuration:"
	@echo "    CROSS_TOOL_PRFX           : $(CROSS_TOOL_PRFX)"
	@echo "    C6X_GEN_INSTALL_PATH      : $(C6X_GEN_INSTALL_PATH)"
	@echo "    CL_PRU_INSTALL_PATH       : $(CL_PRU_INSTALL_PATH)"
	@echo "    HARDLIB_PATH              : $(HARDLIB_PATH)"
	@echo "    TOOLCHAIN_PATH_A8         : $(TOOLCHAIN_PATH_A8)"
	@echo "    TOOLCHAIN_PATH_A9         : $(TOOLCHAIN_PATH_A9)"
	@echo "    TOOLCHAIN_PATH_Arm9       : $(TOOLCHAIN_PATH_Arm9)"
	@echo "    TOOLCHAIN_PATH_A15        : $(TOOLCHAIN_PATH_A15)"
	@echo "    TOOLCHAIN_PATH_EVE        : $(TOOLCHAIN_PATH_EVE)"
	@echo "    TOOLCHAIN_PATH_M4         : $(TOOLCHAIN_PATH_M4)"
	@echo "Component Configuration:"
	@echo "    BIOS_INSTALL_PATH         : $(BIOS_INSTALL_PATH)"
	@echo "    DSPLIB_INSTALL_PATH       : $(DSPLIB_INSTALL_PATH)"
	@echo "    EDMA3LLD_BIOS6_INSTALLDIR : $(EDMA3LLD_BIOS6_INSTALLDIR)"
	@echo "    IMGLIB_INSTALL_PATH       : $(IMGLIB_INSTALL_PATH)"
	@echo "    IPC_INSTALL_PATH          : $(IPC_INSTALL_PATH)"
	@echo "    MATHLIB_INSTALL_PATH      : $(MATHLIB_INSTALL_PATH)"
	@echo "    NDK_INSTALL_PATH          : $(NDK_INSTALL_PATH)"
	@echo "    PDK_INSTALL_PATH          : $(PDK_INSTALL_PATH)"
	@echo "    PROC_SDK_INSTALL_PATH     : $(PROC_SDK_INSTALL_PATH)"
	@echo "    UIA_INSTALL_PATH          : $(UIA_INSTALL_PATH)"
	@echo "    XDC_INSTALL_PATH          : $(XDC_INSTALL_PATH)"
	@echo "Remaining SDK Configuration:"
	@echo "    LIMIT_BOARDS              : $(LIMIT_BOARDS)"
	@echo "    LIMIT_SOCS                : $(LIMIT_SOCS)"
	@echo "    IPC_PLATFORM              : $(IPC_PLATFORM)"
	@echo "    IPC_ALT_PLATFORM          : $(IPC_ALT_PLATFORM)"
	@echo "**************************************************************************"

