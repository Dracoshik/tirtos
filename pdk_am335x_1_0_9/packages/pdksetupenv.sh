#!/bin/bash
# ******************************************************************************
# * FILE PURPOSE: Environment Setup for building PDK
# ******************************************************************************
# * FILE NAME: pdksetupenv.sh
# *
# * DESCRIPTION: 
# *  Configures and sets up the Build Environment for PDK. 
# *
# *  The batch file expects an optional argument:PDK_INSTALL_PATH: Location
# *  of the PDK package.  If the argument is not specified the batch file
# *  assumes that the PDK is installed in the same location where the batch
# *  file is located and is being executed.
# *
# * USAGE:
# *  ./pdksetupenv.sh ~/ti/pdk_<device>_<version>/packages
# *     --- OR ---
# *  ./pdksetupenv.sh
# *
# * Copyright (C) 2012-2017, Texas Instruments, Inc.
# *****************************************************************************

# *******************************************************************************
# ********************** GET PARAMETERS PASSED THROUGH ARGUMENT   ***************
# *******************************************************************************
# Parameter Validation: Check if the argument was passed to the batch file and
# if so we use that else we default to the working directory where the batch 
# file was invoked from

tempVar=$1
if [ ! -z $tempVar ];then
    export PDK_INSTALL_PATH=$tempVar
else
    export PDK_INSTALL_PATH=${PWD}
fi

# TI SDK root directory
if [ -z $SDK_INSTALL_PATH ]; then
    export SDK_INSTALL_PATH=~/ti
fi

# Tools root directory
if [ -z $TOOLS_INSTALL_PATH ]; then
    export TOOLS_INSTALL_PATH=~/ti
fi

# Output directory for generated libraries
export LIBDIR=./lib

# *******************************************************************************
# ********************** CHECK REQUIRED ENVIRONMENT DEFINES BEGIN ***************
# *******************************************************************************

# Version of CG-Tools
CGT_VERSION=8.2.2

# Cross compiler prefix
GCC_CROSS_TOOL_PREFIX=arm-none-eabi-
GCC_CROSS_TOOL_TAG=6-2017-q1-update

# Version of GCC
GCC_VERSION=${GCC_CROSS_TOOL_PREFIX}${GCC_CROSS_TOOL_TAG}

# Version of GCC for ARM A15
GCC_VERSION_ARM_A15=${GCC_CROSS_TOOL_PREFIX}${GCC_CROSS_TOOL_TAG}

# Version of GCC for ARM A8
GCC_VERSION_ARM_A8=${GCC_CROSS_TOOL_PREFIX}${GCC_CROSS_TOOL_TAG}

# Version of GCC for ARM A9
GCC_VERSION_ARM_A9=${GCC_CROSS_TOOL_PREFIX}${GCC_CROSS_TOOL_TAG}

# Version of CG-ARM-Tools
CGT_ARM_VERSION=16.9.3.LTS

# Version of CG-PRU-Tools for PRUICSS
CGT_PRU_VERSION=2.1.5

# Version of EVE CG-Tools
CGT_ARP32_VERSION=1.0.8

# Version of HARDLIB for ARM
GCC_VERSION_HARDLIB=6.3.1

# Version of XDC
XDC_VERSION=3_50_03_33_core

# Version of BIOS
BIOS_VERSION=6_52_00_12

# Version of IPC
IPC_VERSION=3_47_00_00

# EDMA3 Version
EDMA_VERSION=2_12_05_30B

# Version of the NDK
NDK_VERSION=2_26_00_08

# Version of the IMGLIB
IMGLIB_VERSION=c66x_3_1_1_0

# Version of the DSPLIB
DSPLIB_VERSION=c66x_3_4_0_0

# Version of the MATHLIB
MATHLIB_VERSION=c66x_3_1_1_0

# Version of the UIA
UIA_VERSION=2_21_02_07

if [ -z $C6X_GEN_INSTALL_PATH ]; then
    export C6X_GEN_INSTALL_PATH="${SDK_INSTALL_PATH}/ti-cgt-c6000_$CGT_VERSION"
fi

if [ -z $TOOLCHAIN_PATH_GCC ]; then
    export TOOLCHAIN_PATH_GCC="${SDK_INSTALL_PATH}/gcc-$GCC_VERSION"
fi

if [ -z $TOOLCHAIN_PATH_A15 ]; then
    export TOOLCHAIN_PATH_A15="${SDK_INSTALL_PATH}/gcc-$GCC_VERSION_ARM_A15"
fi

if [ -z $TOOLCHAIN_PATH_A8 ]; then
    export TOOLCHAIN_PATH_A8="${SDK_INSTALL_PATH}/gcc-$GCC_VERSION_ARM_A8"
fi

if [ -z $TOOLCHAIN_PATH_A9 ]; then
    export TOOLCHAIN_PATH_A9="${SDK_INSTALL_PATH}/gcc-$GCC_VERSION_ARM_A9"
fi

if [ -z $TOOLCHAIN_PATH_M4 ]; then
    export TOOLCHAIN_PATH_M4="${SDK_INSTALL_PATH}/ti-cgt-arm_$CGT_ARM_VERSION"
fi

if [ -z $TOOLCHAIN_PATH_Arm9 ]; then
    export TOOLCHAIN_PATH_Arm9="${SDK_INSTALL_PATH}/ti-cgt-arm_$CGT_ARM_VERSION"
fi

if [ -z $TOOLCHAIN_PATH_EVE ]; then
    export TOOLCHAIN_PATH_EVE="${SDK_INSTALL_PATH}/arp32_$CGT_ARP32_VERSION"
fi

if [ -z $CL_PRU_INSTALL_PATH ]; then
    export CL_PRU_INSTALL_PATH="${SDK_INSTALL_PATH}/ti-cgt-pru_$CGT_PRU_VERSION"
fi

if [ -z $HARDLIB_PATH ]; then
    export HARDLIB_PATH="$TOOLCHAIN_PATH_A9/lib/gcc/arm-none-eabi/$GCC_VERSION_HARDLIB/hard"
fi

if [ -z $CROSS_TOOL_PRFX ]; then
    export CROSS_TOOL_PRFX="$GCC_CROSS_TOOL_PREFIX"
fi

if [ -z $XDC_INSTALL_PATH ]; then
    export XDC_INSTALL_PATH="${SDK_INSTALL_PATH}/xdctools_$XDC_VERSION"
fi

if [ -z $BIOS_INSTALL_PATH ]; then
    export BIOS_INSTALL_PATH="${SDK_INSTALL_PATH}/bios_$BIOS_VERSION"
fi

if [ -z $IPC_INSTALL_PATH ]; then
    export IPC_INSTALL_PATH="${SDK_INSTALL_PATH}/ipc_$IPC_VERSION"
fi

if [ -z $EDMA3LLD_BIOS6_INSTALLDIR ]; then
    export EDMA3LLD_BIOS6_INSTALLDIR="${SDK_INSTALL_PATH}/edma3_lld_$EDMA_VERSION"
fi

if [ -z $NDK_INSTALL_PATH ]; then
    export NDK_INSTALL_PATH="${SDK_INSTALL_PATH}/ndk_$NDK_VERSION"
fi

if [ -z $IMGLIB_INSTALL_PATH ]; then
    export IMGLIB_INSTALL_PATH="${SDK_INSTALL_PATH}/imglib_$IMGLIB_VERSION"
fi

if [ -z $DSPLIB_INSTALL_PATH ]; then
    export DSPLIB_INSTALL_PATH="${SDK_INSTALL_PATH}/dsplib_$DSPLIB_VERSION"
fi

if [ -z $MATHLIB_INSTALL_PATH ]; then
    export MATHLIB_INSTALL_PATH="${SDK_INSTALL_PATH}/mathlib_$MATHLIB_VERSION"
fi

if [ -z $UIA_INSTALL_PATH ]; then
    export UIA_INSTALL_PATH="${SDK_INSTALL_PATH}/uia_$UIA_VERSION"
fi

if [ -z $CG_XML_BIN_INSTALL_PATH ]; then
    echo "Optional parameter not configured : CG_XML_BIN_INSTALL_PATH"
    echo "REQUIRED for xdc release build"
    echo "Example: export CG_XML_BIN_INSTALL_PATH=\"~/ti/cg_xml/bin\""
fi

if [ -z $DOXYGEN_INSTALL_PATH ]; then
    echo "Optional parameter not configured : DOXYGEN_INSTALL_PATH"
    echo "REQUIRED for xdc release build"
    echo "Example: export DOXYGEN_INSTALL_PATH=\"~/ti/Doxygen/doxygen/1.5.1-p1/bin\""
fi

# *******************************************************************************
# ********************** CHECK REQUIRED ENVIRONMENT DEFINES END ***************
# *******************************************************************************


# *******************************************************************************
# *************************** OPTIONAL ENVIRONMENT DEFINES **********************
# *************************** NOT REQUIRED FOR BUILDING THE PDK *****************
# *******************************************************************************
# STATIC_ANALYZE_PATH: Environment Variable for tool to do STATIC Analysis 
#                      of the code
# 
# DOXYGEN_INSTALL_PATH: DOXYGEN Version 1.7.3 [Only needed for generating Doxygen]
# 
# HTML_HELP_WORKSHOP_INSTALL_PATH: HTML Help compiler.[Only needed for generating Doxygen]
# 
# PDSP_CGEN_INSTALL_PATH: PDSP CGEN TOOLS [Only needed for PA firmware]
# *******************************************************************************
# *******************************************************************************
# *******************************************************************************

# Keystone 2 AIF2 LLD 
export SOC_FAMILY=KeyStone2
# PDK PARTNO
export PARTNO=am57xx
# Platforms supported by the PDK
export LIMIT_SOCS="am335x"
export LIMIT_BOARDS="evmAM335x icev2AM335x iceAMIC110 bbbAM335x skAM335x"

export CGTOOLS=$C6X_GEN_INSTALL_PATH

# *******************************************************************************
# ** Convert C6X_GEN_INSTALL_PATH and PDK_INSTALL_PATH to short name and to UNIX STYLE PATH  for XDC build **
# *******************************************************************************
export PATH=$PATH:$XDC_INSTALL_PATH/packages/xdc/services/io/release

export XDCCGROOT=$C6X_GEN_INSTALL_PATH

echo "**************************************************************************"
echo "Environment Configuration:"
echo "    LIMIT_SOCS                : $LIMIT_SOCS"
echo "    LIMIT_BOARDS              : $LIMIT_BOARDS"
echo "    PDK_INSTALL_PATH          : $PDK_INSTALL_PATH"
echo "    C6X_GEN_INSTALL_PATH      : $C6X_GEN_INSTALL_PATH"
echo "    TOOLCHAIN_PATH_GCC        : $TOOLCHAIN_PATH_GCC"
echo "    TOOLCHAIN_PATH_A15        : $TOOLCHAIN_PATH_A15"
echo "    TOOLCHAIN_PATH_A8         : $TOOLCHAIN_PATH_A8"
echo "    TOOLCHAIN_PATH_A9         : $TOOLCHAIN_PATH_A9"
echo "    TOOLCHAIN_PATH_M4         : $TOOLCHAIN_PATH_M4"
echo "    TOOLCHAIN_PATH_Arm9       : $TOOLCHAIN_PATH_Arm9"
echo "    TOOLCHAIN_PATH_EVE        : $TOOLCHAIN_PATH_EVE"
echo "    CL_PRU_INSTALL_PATH       : $CL_PRU_INSTALL_PATH"
echo "    HARDLIB_PATH              : $HARDLIB_PATH"
echo "    CROSS_TOOL_PRFX           : $CROSS_TOOL_PRFX"
echo "    XDC_INSTALL_PATH          : $XDC_INSTALL_PATH"
echo "    BIOS_INSTALL_PATH         : $BIOS_INSTALL_PATH"
echo "    IPC_INSTALL_PATH          : $IPC_INSTALL_PATH"
echo "    EDMA3LLD_BIOS6_INSTALLDIR : $EDMA3LLD_BIOS6_INSTALLDIR"
echo "    NDK_INSTALL_PATH          : $NDK_INSTALL_PATH"
echo "    IMGLIB_INSTALL_PATH       : $IMGLIB_INSTALL_PATH"
echo "    DSPLIB_INSTALL_PATH       : $DSPLIB_INSTALL_PATH"
echo "    MATHLIB_INSTALL_PATH      : $MATHLIB_INSTALL_PATH"
echo "    UIA_INSTALL_PATH          : $UIA_INSTALL_PATH"
if [ ! -z $CG_XML_BIN_INSTALL_PATH ]; then
    echo "    CG_XML_BIN_INSTALL_PATH   : $CG_XML_BIN_INSTALL_PATH"
fi

# *******************************************************************************
# *************************** XDC PATH Configuration ****************************
# *******************************************************************************

# Ensure that all the components inside PDK are a part of the XDC Path; such that
# one component can use another.
export XDCPATH="../../..;$XDC_INSTALL_PATH/packages;$PDK_INSTALL_PATH"
export XDCPATH="$XDCPATH;$EDMA3LLD_BIOS6_INSTALLDIR/packages;$BIOS_INSTALL_PATH/packages;$IPC_INSTALL_PATH/packages;$NDK_INSTALL_PATH/packages;$UIA_INSTALL_PATH/packages"

# *******************************************************************************
# ************************** Build Tools Configuration **************************
# *******************************************************************************

# Compiler Tools:
export PATH=$PATH:$C6X_GEN_INSTALL_PATH/bin

# XDC Tools location:
export PATH=$PATH:$XDC_INSTALL_PATH:$XDC_INSTALL_PATH\bin

# CG-XML Package:
if [ ! -z $CG_XML_BIN_INSTALL_PATH ]; then
    export PATH=$PATH:$CG_XML_BIN_INSTALL_PATH
fi

# Third Party Tools: Doxygen
if [ ! -z $DOXYGEN_INSTALL_PATH ]; then
    export PATH=$PATH:$DOXYGEN_INSTALL_PATH
fi

# Third Party Tools: HTML Help compiler.
if [ ! -z $HTML_HELP_WORKSHOP_INSTALL_PATH ]; then
    export PATH=$PATH:$HTML_HELP_WORKSHOP_INSTALL_PATH
fi

# Third Party Tools: Install-Jammer (Not required by customers)
if [ ! -z $INSTALL_JAMMER_DIR ]; then
    export PATH=$PATH:$INSTALL_JAMMER_DIR
fi

# Third Party Tools: Coverity
if [ ! -z $STATIC_ANALYZE_PATH ]; then
    export PATH=$PATH:$STATIC_ANALYZE_PATH/bin
fi

if [ -z $PDK_PATH ]; then
    # SDK level makefile did not invoke PDK level makefile

    echo "PDK BUILD ENVIRONMENT CONFIGURED"
    echo "*******************************************************************************"
fi
