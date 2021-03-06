# Filename: Makefile
#
# This file is the makefile for building this module.
#
# Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
#
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#******************************************************************************


MODULE_NAME = device

include $(ROOTDIR)/build/makerules/platform.mk


SRCDIR = $(SOC)
INCDIR = . $(SOC)

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_INTERNAL_INTERFACES = device soc utils

MODULE_SRC_BASE_PATH := $(starterware_PATH)/$(MODULE_NAME)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += qspi_flash.c lcd_device.c pixcir_tsc.c camera_device.c enet_phy_device.c nand_device.c pmic_device.c pmic_device_tps65910.c pmic_device_tps65217.c pmic_device_tps65218.c clock_synthesizer.c

# Get the file list from filelist.mk files in sub-folders
# None

PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON =

# Core/SoC/Toolchain/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/Toolchain/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/Toolchain/platform-name> =
# SRCS_a8host +=
# SRCS_ASM_COMMON +=
# SRCS_ASM_a8host +=

# Include common make files
include $(ROOTDIR)/build/makerules/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
