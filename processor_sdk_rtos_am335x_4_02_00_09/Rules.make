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
# Configure SDK component paths
################################################################################

# SoC of SDK
SDK_SOC=am335x

# Version of PDK
PDK_VERSION=1_0_9

# Version of PROC SDK
PROC_SDK_VERSION=4_02_00_09

PDK_INSTALL_PATH      ?= $(SDK_INSTALL_PATH)/pdk_$(SDK_SOC)_$(PDK_VERSION)/packages
PROC_SDK_INSTALL_PATH ?= ${SDK_INSTALL_PATH}/processor_sdk_rtos_$(SDK_SOC)_$(PROC_SDK_VERSION)

export PDK_INSTALL_PATH      
export PROC_SDK_INSTALL_PATH 

################################################################################
# Remaining Environment Setup
################################################################################

# Configure rest of environment via PDK's Rules.make
include $(PDK_INSTALL_PATH)/Rules.make

# Configure IPC platform
export IPC_PLATFORM=UNKNOWN
export IPC_ALT_PLATFORM=
