set SDK_INSTALL_PATH=E:/uC-Soft/TI/TIRTOS
set CG_XML_BIN_INSTALL_PATH=E:/uC-Soft/TI/TIRTOS/cg_xml/bin

call ..\..\processor_sdk_rtos_am335x_4_02_00_09\setupenv.bat 

REM call pdksetupenv.bat "E:\uC-Soft\TI\TIRTOS\pdk_am335x_1_0_9\packages"

gmake clean

gmake all

pause
