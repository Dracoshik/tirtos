@REM ******************************************************************************
@REM * FILE PURPOSE: Environment Setup for building PDK
@REM ******************************************************************************
@REM * FILE NAME: pdksetupenv.bat
@REM *
@REM * DESCRIPTION: 
@REM *  Configures and sets up the Build Environment for PDK.
@REM *
@REM *  The batch file expects an optional argument:PDK_INSTALL_PATH: Location
@REM *  of the PDK package.  If the argument is not specified the batch file
@REM *  assumes that the PDK is installed in the same location where the batch
@REM *  file is located and is being executed.
@REM *
@REM * USAGE:
@REM *  pdksetupenv.bat "C:\ti\pdk_<device>_<version>\packages"
@REM *   --- OR ---
@REM *  pdksetupenv.bat
@REM *
@REM * Copyright (C) 2012-2017, Texas Instruments, Inc.
@REM *****************************************************************************
@echo off
@REM *******************************************************************************
@REM ********************** GET PARAMETERS PASSED THROUGH ARGUMENT   ***************
@REM *******************************************************************************
@REM Parameter Validation: Check if the argument was passed to the batch file and
@REM if so we use that else we default to the working directory where the batch 
@REM file was invoked from

set tempVar=%1
if defined tempVar (
    set PDK_INSTALL_PATH=%~fs1
) else (
    set PDK_INSTALL_PATH=%~sdp0
)

set PDK_INSTALL_PATH=%PDK_INSTALL_PATH:\=/%

@rem Remove trailing space from PDK_INSTALL_PATH.  Otherwise, makefiles will append a
@rem second slash when creating object paths and will cause compilation error in case
@rem an application includes xdc/cfg/global.h
if %PDK_INSTALL_PATH:~-1% == / (
    set PDK_INSTALL_PATH=%PDK_INSTALL_PATH:~0,-1%
)

@rem PDK_INSTALL_PATH workaround for compilation of board library.
@rem A drive letter in PDK_INSTALL_PATH will cause make of board library to fail because
@rem make mis-interprets drive letters as rules.  Thus remove the drive letter which
@rem means the 'make' or 'xdc' shall run from same drive letter as PDK when building PDK
@rem or SDK components.
set PDK_INSTALL_PATH=%PDK_INSTALL_PATH:~2%

if not defined SDK_INSTALL_PATH (
    set SDK_INSTALL_PATH=C:/ti
)

if not defined TOOLS_INSTALL_PATH (
    set TOOLS_INSTALL_PATH=C:/ti
)

@REM *******************************************************************************
@REM ********************** CHECK REQUIRED ENVIRONMENT DEFINES BEGIN ***************
@REM *******************************************************************************

@REM Version of CG-Tools
set CGT_VERSION=8.2.2

@REM Cross compiler prefix
set GCC_CROSS_TOOL_PREFIX=arm-none-eabi-
set GCC_CROSS_TOOL_TAG=6-2017-q1-update

@REM Version of GCC
set GCC_VERSION=%GCC_CROSS_TOOL_PREFIX%%GCC_CROSS_TOOL_TAG%

@REM Version of GCC for ARM A15
set GCC_VERSION_ARM_A15=%GCC_CROSS_TOOL_PREFIX%%GCC_CROSS_TOOL_TAG%

@REM Version of GCC for ARM A8
set GCC_VERSION_ARM_A8=%GCC_CROSS_TOOL_PREFIX%%GCC_CROSS_TOOL_TAG%

@REM Version of GCC for ARM A9
set GCC_VERSION_ARM_A9=%GCC_CROSS_TOOL_PREFIX%%GCC_CROSS_TOOL_TAG%

@REM Version of CG-ARM-Tools
set CGT_ARM_VERSION=16.9.3.LTS

@REM Version of CG-PRU-Tools for PRUICSS
set CGT_PRU_VERSION=2.1.5

@REM Version of EVE CG-Tools
set CGT_ARP32_VERSION=1.0.8

@REM Version of HARDLIB for ARM
set GCC_VERSION_HARDLIB=6.3.1

@REM Version of XDC
set XDC_VERSION=3_50_03_33_core

@REM Version of BIOS
set BIOS_VERSION=6_52_00_12

@REM Version of IPC
set IPC_VERSION=3_47_00_00

@REM EDMA3 Version
set EDMA_VERSION=2_12_05_30B

@REM Version of the NDK
set NDK_VERSION=2_26_00_08

@REM Version of the IMGLIB
set IMGLIB_VERSION=c66x_3_1_1_0

@REM Version of the DSPLIB
set DSPLIB_VERSION=c66x_3_4_0_0

@REM Version of the MATHLIB
set MATHLIB_VERSION=c66x_3_1_1_0

@REM Version of the UIA
set UIA_VERSION=2_21_02_07

if not defined C6X_GEN_INSTALL_PATH (
    set C6X_GEN_INSTALL_PATH=%SDK_INSTALL_PATH%/ti-cgt-c6000_%CGT_VERSION%
)
set C6X_GEN_INSTALL_PATH=%C6X_GEN_INSTALL_PATH:\=/%

if not defined TOOLCHAIN_PATH_GCC (
    set TOOLCHAIN_PATH_GCC=%SDK_INSTALL_PATH%/gcc-%GCC_VERSION%
)
set TOOLCHAIN_PATH_GCC=%TOOLCHAIN_PATH_GCC:\=/%

if not defined TOOLCHAIN_PATH_A15 (
    set TOOLCHAIN_PATH_A15=%SDK_INSTALL_PATH%/gcc-%GCC_VERSION_ARM_A15%
)
set TOOLCHAIN_PATH_A15=%TOOLCHAIN_PATH_A15:\=/%

if not defined TOOLCHAIN_PATH_A8 (
    set TOOLCHAIN_PATH_A8=%SDK_INSTALL_PATH%/gcc-%GCC_VERSION_ARM_A8%
)
set TOOLCHAIN_PATH_A8=%TOOLCHAIN_PATH_A8:\=/%

if not defined TOOLCHAIN_PATH_A9 (
    set TOOLCHAIN_PATH_A9=%SDK_INSTALL_PATH%/gcc-%GCC_VERSION_ARM_A9%
)
set TOOLCHAIN_PATH_A9=%TOOLCHAIN_PATH_A9:\=/%

if not defined TOOLCHAIN_PATH_M4 (
    set TOOLCHAIN_PATH_M4=%SDK_INSTALL_PATH%/ti-cgt-arm_%CGT_ARM_VERSION%
)
set TOOLCHAIN_PATH_M4=%TOOLCHAIN_PATH_M4:\=/%

if not defined TOOLCHAIN_PATH_Arm9 (
    set TOOLCHAIN_PATH_Arm9=%SDK_INSTALL_PATH%/ti-cgt-arm_%CGT_ARM_VERSION%
)
set TOOLCHAIN_PATH_M4=%TOOLCHAIN_PATH_Arm9:\=/%

if not defined TOOLCHAIN_PATH_EVE (
    set TOOLCHAIN_PATH_EVE=%SDK_INSTALL_PATH%/arp32_%CGT_ARP32_VERSION%
)
set TOOLCHAIN_PATH_EVE=%TOOLCHAIN_PATH_EVE:\=/%

if not defined CL_PRU_INSTALL_PATH (
    set CL_PRU_INSTALL_PATH=%SDK_INSTALL_PATH%/ti-cgt-pru_%CGT_PRU_VERSION%
)
set CL_PRU_INSTALL_PATH=%CL_PRU_INSTALL_PATH:\=/%

if not defined HARDLIB_PATH (
    set HARDLIB_PATH=%TOOLCHAIN_PATH_A9%/lib/gcc/arm-none-eabi/%GCC_VERSION_HARDLIB%/hard
)
set HARDLIB_PATH=%HARDLIB_PATH:\=/%

if not defined CROSS_TOOL_PRFX (
    set CROSS_TOOL_PRFX=%GCC_CROSS_TOOL_PREFIX%
)
set CROSS_TOOL_PRFX=%CROSS_TOOL_PRFX:\=/%

if not defined XDC_INSTALL_PATH (
    set XDC_INSTALL_PATH=%SDK_INSTALL_PATH%/xdctools_%XDC_VERSION%
)
set XDC_INSTALL_PATH=%XDC_INSTALL_PATH:\=/%

if not defined UTILS_INSTALL_DIR (
    set UTILS_INSTALL_DIR=%XDC_INSTALL_PATH%/bin
)
set UTILS_INSTALL_DIR=%UTILS_INSTALL_DIR:\=/%

if not defined BIOS_INSTALL_PATH (
    set BIOS_INSTALL_PATH=%SDK_INSTALL_PATH%/bios_%BIOS_VERSION%
)
set BIOS_INSTALL_PATH=%BIOS_INSTALL_PATH:\=/%

if not defined IPC_INSTALL_PATH (
    set IPC_INSTALL_PATH=%SDK_INSTALL_PATH%/ipc_%IPC_VERSION%
)
set IPC_INSTALL_PATH=%IPC_INSTALL_PATH:\=/%

if not defined EDMA3LLD_BIOS6_INSTALLDIR (
    set EDMA3LLD_BIOS6_INSTALLDIR=%SDK_INSTALL_PATH%/edma3_lld_%EDMA_VERSION%
)
set EDMA3LLD_BIOS6_INSTALLDIR=%EDMA3LLD_BIOS6_INSTALLDIR:\=/%

if not defined NDK_INSTALL_PATH (
    set NDK_INSTALL_PATH=%SDK_INSTALL_PATH%/ndk_%NDK_VERSION%
)
set NDK_INSTALL_PATH=%NDK_INSTALL_PATH:\=/%

if not defined IMGLIB_INSTALL_PATH (
    set IMGLIB_INSTALL_PATH=%SDK_INSTALL_PATH%/imglib_%IMGLIB_VERSION%
)
set IMGLIB_INSTALL_PATH=%IMGLIB_INSTALL_PATH:\=/%

if not defined DSPLIB_INSTALL_PATH (
    set DSPLIB_INSTALL_PATH=%SDK_INSTALL_PATH%/dsplib_%DSPLIB_VERSION%
)
set DSPLIB_INSTALL_PATH=%DSPLIB_INSTALL_PATH:\=/%

if not defined MATHLIB_INSTALL_PATH (
    set MATHLIB_INSTALL_PATH=%SDK_INSTALL_PATH%/mathlib_%MATHLIB_VERSION%
)
set MATHLIB_INSTALL_PATH=%MATHLIB_INSTALL_PATH:\=/%

if not defined UIA_INSTALL_PATH (
    set UIA_INSTALL_PATH=%SDK_INSTALL_PATH%/uia_%UIA_VERSION%
)
set UIA_INSTALL_PATH=%UIA_INSTALL_PATH:\=/%

if not defined CG_XML_BIN_INSTALL_PATH (
    @echo Optional parameter not configured : CG_XML_BIN_INSTALL_PATH
    @echo REQUIRED for xdc release build
    @echo Example: set CG_XML_BIN_INSTALL_PATH=C:/ti/cg_xml/bin
)

if not defined DOXYGEN_INSTALL_PATH (
    @echo Optional parameter not configured : DOXYGEN_INSTALL_PATH
    @echo REQUIRED for xdc release build
    @echo Example: set DOXYGEN_INSTALL_PATH=C:/ti/Doxygen/doxygen/1.5.1-p1/bin
)

@REM *******************************************************************************
@REM ********************** CHECK REQUIRED ENVIRONMENT DEFINES END ***************
@REM *******************************************************************************

@REM *******************************************************************************
@REM *************************** OPTIONAL ENVIRONMENT DEFINES **********************
@REM *************************** NOT REQUIRED FOR BUILDING THE PDK *****************
@REM *******************************************************************************
@REM STATIC_ANALYZE_PATH: Environment Variable for tool to do STATIC Analysis 
@REM                      of the code
@REM 
@REM DOXYGEN_INSTALL_PATH: DOXYGEN Version 1.7.3 [Only needed for generating Doxygen]
@REM 
@REM HTML_HELP_WORKSHOP_INSTALL_PATH: HTML Help compiler.[Only needed for generating Doxygen]
@REM 
@REM PDSP_CGEN_INSTALL_PATH: PDSP CGEN TOOLS [Only needed for PA firmware]
@REM *******************************************************************************
@REM *******************************************************************************
@REM *******************************************************************************

@REM PDK PARTNO
if not defined PARTNO set PARTNO=AM
if not defined SOC_FAMILY set SOC_FAMILY=AM

@REM Platforms supported by the PDK
if not defined LIMIT_SOCS set LIMIT_SOCS=am335x
if not defined LIMIT_BOARDS set LIMIT_BOARDS=evmAM335x icev2AM335x iceAMIC110 bbbAM335x skAM335x

set CGTOOLS=%C6X_GEN_INSTALL_PATH%

@REM *******************************************************************************
@REM ** Convert paths to short name and to UNIX STYLE PATH for XDC build **
@REM *******************************************************************************
set PATH=%PATH%;%XDC_INSTALL_PATH%\packages\xdc\services\io\release

@REM *******************************************************************************
@REM This file requires path2dos.exe utility available from XDC tools. If path2dos.exe
@REM is not available in the build environment then short names for paths SHOULD be
@REM provided to environment variables e.g. C:\PROGRA~1\TEXASI~1\PDK_TC~4\packages.
@REM In Windows environment, "dir /x" and "dir /-n" displays the short names generated
@REM for non-8dot3 file and directory names.
@REM *******************************************************************************

@REM for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%PDK_INSTALL_PATH%"') do set PDK_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%C6X_GEN_INSTALL_PATH%"') do set C6X_GEN_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_GCC%"') do set TOOLCHAIN_PATH_GCC=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_A15%"') do set TOOLCHAIN_PATH_A15=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_A8%"') do set TOOLCHAIN_PATH_A8=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_A9%"') do set TOOLCHAIN_PATH_A9=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_M4%"') do set TOOLCHAIN_PATH_M4=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_Arm9%"') do set TOOLCHAIN_PATH_Arm9=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%TOOLCHAIN_PATH_EVE%"') do set TOOLCHAIN_PATH_EVE=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%CL_PRU_INSTALL_PATH%"') do set CL_PRU_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%UTILS_INSTALL_DIR%"') do set UTILS_INSTALL_DIR=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%HARDLIB_PATH%"') do set HARDLIB_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%XDC_INSTALL_PATH%"') do set XDC_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%BIOS_INSTALL_PATH%"') do set BIOS_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%IPC_INSTALL_PATH%"') do set IPC_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%EDMA3LLD_BIOS6_INSTALLDIR%"') do set EDMA3LLD_BIOS6_INSTALLDIR=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%NDK_INSTALL_PATH%"') do set NDK_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%IMGLIB_INSTALL_PATH%"') do set IMGLIB_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%DSPLIB_INSTALL_PATH%"') do set DSPLIB_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%MATHLIB_INSTALL_PATH%"') do set MATHLIB_INSTALL_PATH=%%a
for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%UIA_INSTALL_PATH%"') do set UIA_INSTALL_PATH=%%a
if defined CG_XML_BIN_INSTALL_PATH (
    for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos "%CG_XML_BIN_INSTALL_PATH%"') do set CG_XML_BIN_INSTALL_PATH=%%a
)

set XDCCGROOT=%C6X_GEN_INSTALL_PATH%

echo **************************************************************************
echo Environment Configuration:
echo     LIMIT_SOCS                : %LIMIT_SOCS%
echo     LIMIT_BOARDS              : %LIMIT_BOARDS%
echo     PDK_INSTALL_PATH          : %PDK_INSTALL_PATH%
echo     C6X_GEN_INSTALL_PATH      : %C6X_GEN_INSTALL_PATH%
echo     TOOLCHAIN_PATH_GCC        : %TOOLCHAIN_PATH_GCC%
echo     TOOLCHAIN_PATH_A15        : %TOOLCHAIN_PATH_A15%
echo     TOOLCHAIN_PATH_A8         : %TOOLCHAIN_PATH_A8%
echo     TOOLCHAIN_PATH_A9         : %TOOLCHAIN_PATH_A9%
echo     TOOLCHAIN_PATH_M4         : %TOOLCHAIN_PATH_M4%
echo     TOOLCHAIN_PATH_Arm9       : %TOOLCHAIN_PATH_Arm9%
echo     TOOLCHAIN_PATH_EVE        : %TOOLCHAIN_PATH_EVE%
echo     CL_PRU_INSTALL_PATH       : %CL_PRU_INSTALL_PATH%
echo     UTILS_INSTALL_DIR         : %UTILS_INSTALL_DIR%
echo     HARDLIB_PATH              : %HARDLIB_PATH%
echo     CROSS_TOOL_PRFX           : %CROSS_TOOL_PRFX%
echo     XDC_INSTALL_PATH          : %XDC_INSTALL_PATH%
echo     BIOS_INSTALL_PATH         : %BIOS_INSTALL_PATH%
echo     IPC_INSTALL_PATH          : %IPC_INSTALL_PATH%
echo     EDMA3LLD_BIOS6_INSTALLDIR : %EDMA3LLD_BIOS6_INSTALLDIR%
echo     NDK_INSTALL_PATH          : %NDK_INSTALL_PATH%
echo     IMGLIB_INSTALL_PATH       : %IMGLIB_INSTALL_PATH%
echo     DSPLIB_INSTALL_PATH       : %DSPLIB_INSTALL_PATH%
echo     MATHLIB_INSTALL_PATH      : %MATHLIB_INSTALL_PATH%
echo     UIA_INSTALL_PATH          : %UIA_INSTALL_PATH%
if defined CG_XML_BIN_INSTALL_PATH (
    echo     CG_XML_BIN_INSTALL_PATH   : %CG_XML_BIN_INSTALL_PATH%
)

@echo off
@REM *******************************************************************************
@REM *************************** XDC PATH Configuration ****************************
@REM *******************************************************************************

@REM Ensure that all the components inside PDK are a part of the XDC Path; such
@REM that one component can use another.
set XDCPATH=../../..;%XDC_INSTALL_PATH%/packages;%PDK_INSTALL_PATH%/.
set XDCPATH=%XDCPATH%;%EDMA3LLD_BIOS6_INSTALLDIR%/packages;%BIOS_INSTALL_PATH%/packages;%IPC_INSTALL_PATH%/packages;%NDK_INSTALL_PATH%/packages;%UIA_INSTALL_PATH%/packages

@REM *******************************************************************************
@REM ************************** Build Tools Configuration **************************
@REM *******************************************************************************

@REM Windows Path
set PATH=C:/Windows/System32

@REM Compiler Tools:
set PATH=%PATH%;%C6X_GEN_INSTALL_PATH%/bin

@REM XDC Tools location:
set PATH=%PATH%;%XDC_INSTALL_PATH%;%XDC_INSTALL_PATH%\bin;%XDC_INSTALL_PATH%\packages\xdc\services\io\release

@REM CG-XML Package:
if defined CG_XML_BIN_INSTALL_PATH  set PATH=%PATH%;%CG_XML_BIN_INSTALL_PATH%

@REM Third Party Tools: Doxygen
if defined DOXYGEN_INSTALL_PATH  set PATH=%PATH%;%DOXYGEN_INSTALL_PATH%

@REM Third Party Tools: HTML Help compiler.
if defined HTML_HELP_WORKSHOP_INSTALL_PATH  set PATH=%PATH%;%HTML_HELP_WORKSHOP_INSTALL_PATH%;

@REM Third Party Tools: Coverity
if defined STATIC_ANALYZE_PATH  set PATH=%PATH%;%STATIC_ANALYZE_PATH%\bin

@REM Set the Title Window appropiately.
Title Processor-SDK RTOS Build Environment

if not defined PDK_PATH (
    @REM SDK level makefile did not invoke PDK level makefile

    @echo **************************************************************************
    @echo Changing to short name to support directory names containing spaces
    @echo current directory: %PDK_INSTALL_PATH%

    cd %PDK_INSTALL_PATH%
    @echo PDK BUILD ENVIRONMENT CONFIGURED
    @echo **************************************************************************
)
