set SDK_INSTALL_PATH=E:\uC-Soft\TI\TIRTOS
REM TOOLS_INSTALL_PATH = ;

set CG_XML_BIN_INSTALL_PATH=E:/uC-Soft/TI/TIRTOS/cg_xml/bin

call .\processor_sdk_rtos_am335x_4_02_00_09\setupenv.bat 

set BUILD_PROFILE=debug

set BIOS_VERSION=6_52_00_12
set CGT_PRU_VERSION=2.1.5
set EDMA_VERSION=2_12_05_30B
set NDK_VERSION=2_26_00_08
set UIA_VERSION=2_20_00_02
set XDC_VERSION=3_50_04_43_core

set PROC_SDK_VERSION=${SOC}_4_02_00_09
set PDK_VERSION=${SOC}_1_0_9

set CGT_ARM_VERSION=16.9.6.LTS
set CGT_VERSION=8.2.2
set GCC_VERSION_HARDLIB=6.3.1

set IPC_VERSION=3_47_00_00
set IPC_PLATFORM=UNKNOWN

set LIMIT_BOARDS=bbbAM335x
set LIMIT_CORES=a8host
set LIMIT_SOCS=am335x
set SOC=am335x
set SOC_FAMILY=AM
set PARTNO=AM

set UTILS_PATH=E:/uC-Soft/TI/ccsv7/utils/cygwin

call .\pdk_am335x_1_0_9\packages\pdksetupenv.bat "E:\uC-Soft\TI\TIRTOS\pdk_am335x_1_0_9\packages"

gmake clean

REM pause

gmake all
